# BEE-1000 Ground Station — Claude Code Context

This file is auto-loaded by Claude Code. Keep it concise — detailed analysis lives in [docs/](docs/).

## Project at a glance

- **What**: Satellite ground station GUI for BEE-1000 CubeSat (Yonsei University)
- **Language**: C/C++17, single Linux executable `bin/BEE-1000`
- **Stack**: GLFW + Dear ImGui + OpenGL (GUI), pthread (16 threads), CSP protocol (radio), libsgp4 (orbital), libssh (S-band remote), CURL (TLE fetch)
- **Entry point**: [src/app/main.cpp](src/app/main.cpp) — single 577-line `main()` runs everything
- **Build**: CMake → see [docs/BUILD_WSL.md](docs/BUILD_WSL.md)

## Critical context (read before editing)

1. **Linux-only**. Uses `pthread`, `termios`, `/dev/GS102`, `system("../amp/ampoff")`, `libssh`. Windows native build is not feasible — use **WSL2 Ubuntu**.
2. **Working directory matters**. The executable must be launched from `bin/` because main.cpp references `../bin/font/Inconsolata.ttf`, `../data/BEE_TC_Log/`, `../amp/ampoff` as relative paths.
3. **Hardware-coupled**. Without `/dev/GS102` (UHF transceiver), `/dev/GS232B` (rotator), `/dev/KTA223` (switch) the GUI still appears but device init logs `[ERROR]`. Do not "fix" those errors — they are expected on a dev machine.
4. **The header `src/YonseiGS_config.h` is the choke point**. 5,477 lines, 70+ includes, defines all packet/command types. Editing it triggers a full rebuild. See [docs/MIMAN_CONFIG_ANALYSIS.md](docs/MIMAN_CONFIG_ANALYSIS.md).
5. **Global state hub**: `StateCheckUnit State` (defined in YonseiGS_config.h, instantiated in main.cpp) carries ~35 boolean flags that drive every module's control flow. Threads communicate via these flags, not direct calls.

## Detailed docs

| Topic | File |
|---|---|
| Folder layout, module dependencies, data flow, 5-layer architecture | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |
| `YonseiGS_config.h` deep dive (Setup / SatelliteObject / StateCheckUnit / Command) | [docs/MIMAN_CONFIG_ANALYSIS.md](docs/MIMAN_CONFIG_ANALYSIS.md) |
| WSL2 + VSCode build & run guide | [docs/BUILD_WSL.md](docs/BUILD_WSL.md) |
| Honest code quality assessment ("is this spaghetti?") | [docs/CODE_REVIEW.md](docs/CODE_REVIEW.md) |

## Module quick reference (src/miman_*.cpp)

| Module | Role | Hardware/Protocol | Threads |
|---|---|---|---|
| miman_config | Global state, Setup, Console class | — | — |
| miman_imgui | All GUI windows + popups | GLFW/ImGui | main loop |
| miman_orbital | TLE tracking (SGP4), rotator command, Doppler | `/dev/GS232B`, CURL | T[2], T[3], T[7] |
| miman_radial | Radio param tables (Ptable_0/1/5) | `/dev/GS102` via libparam | T[12] |
| miman_csp | CSP stack init, KISS USART, watchdog | `/dev/GS100`, `/dev/GS101` | T[0] (csp route) |
| miman_coms | TX/RX scheduler, beacon/HK parse | CSP network | T[4], T[10], T[11] |
| miman_model | 3D satellite mesh (ASSIMP+OpenGL) | GPU | main loop |
| miman_s_control | S-band GS remote control | SSH 165.132.28.235 | — |
| miman_autopilot | Pass auto-scheduling | CSP | T[13] |
| miman_ftpfcd, miman_ftprdp | FTP up/download/list | libftp_client | T[8] |

## Conventions

- **Do not amend `YonseiGS_config.h` lightly** — full rebuild. Add new files under `src/` and include them only where needed.
- **Don't add new pthread slots without updating the comment block at the top of [src/app/main.cpp](src/app/main.cpp)**. The 16-slot array uses fixed indices.
- **Cleanup**: `temp.cpp` and `tmtctest.cpp` exist in `src/` but are NOT in CMakeLists — they are scratch files.

## When working on this project

- For build/run issues → consult [docs/BUILD_WSL.md](docs/BUILD_WSL.md)
- For "where is X" → check the module table above, then grep
- For architectural changes → read [docs/CODE_REVIEW.md](docs/CODE_REVIEW.md) refactoring priorities first
- Korean responses are fine; the user is Korean-speaking

## Refactoring methodology (MUST FOLLOW)

This codebase is being incrementally refactored toward MVVM. The user is doing
this as their first large refactor and acts as the reviewer. Follow these rules
strictly — they exist to keep the program working at every step.

1. **One small change at a time.** After every change the program MUST build and
   behave exactly as before. Never batch many changes "to test all at once."
2. **Never mix refactoring with feature changes.** If you spot a bug while
   refactoring, write it down in the "Known defects" list below — do NOT fix it
   in the same change. Behavior must stay identical so any difference is
   provably a refactoring mistake, not an intended change.
3. **Propose scope before editing.** State exactly what will change and — just as
   important — what will NOT change. Wait for the user to approve/narrow scope.
4. **Provide a verification checklist** with each change (build OK? layout
   unchanged? buttons/logs identical?). The user verifies by A/B comparison.
5. **Keep old code during transition.** Don't delete a replaced function until
   its replacement is verified — enables instant rollback and A/B comparison.
6. **Commit per verified unit.** One small commit each, message describing the
   single change (e.g. `style: switch theme teal->navy`). Conventional Commits.
7. **If the user can't tell whether behavior changed → the step was too big.**
   Split it smaller.

### Target directory layout (agreed — the refactor is "done" when the code looks like this)

```
src/
  app/        main.cpp, theme, layout (MainWorkspace)
  view/       one file per window/popup — ImGui calls + vm. calls only
    popups/   setup, tle, param tables, ...
    cmdgen/   command picker + per-FSW-app forms (forms_eps.cpp, forms_adcs.cpp, ...)
  viewmodel/  one class per window (RotatorVM, TrackVM, FrequencyVM, BeaconVM,
              ControlVM, CmdGeneratorVM, ...)
  model/
    service/     svc_* — thread lifecycle, start/stop sequences (done)
    telecommand/ command catalog, CCSDS/cFE encoder, command schema
    missions/    per-satellite definitions (bee1000/, cosmic/)
    device/      rotator, GS100 radio, switch box, amplifier (split out of
                 miman_orbital / radial / csp as classes)
    orbit/       tracking target, pass table / prediction, SGP4 (split out of
                 miman_orbital; added with the Track window)
    telemetry/   beacon / HK parsing (split out of miman_coms)
```
Large files are split **object-oriented, by purpose**: many short files/classes,
each with one responsibility, instead of a few multi-thousand-line files.
Legacy `src/miman_*.cpp` files move into this tree as they are split.

### Per-window MVVM migration order (agreed)

Work **one window at a time**. For each window, in one verified sequence of small commits:
1. Extract its **View** into `view/<window>_view.cpp` (ImGui calls only, no logic change).
2. Create its **ViewModel** class in `viewmodel/` and move orchestration there.
3. Move domain/hardware code it needs into `model/` (as classes where it has state).

Files are split **as each window is migrated** (changed 2026-10: previously
"split only after all windows"; the snapshot + golden harnesses now make
per-window splitting safe to verify).

Window sequence: Frequency → Rotator → Track → Beacon → Control → Command
(largest, last; its command layer is being prepared in `model/telecommand/`).
**Frequency is done** and is the reference example to copy for the next windows:
`view/frequency_view.cpp` (ImGui + `vm.` only) → `viewmodel/frequency_vm.*`
(`FrequencyVM`, one instance via `FrequencyViewModel()`) → `model/device/*`
(`Gs100Radio`, `SwitchBox`, `GsLink`, thin wrappers until miman_radial is split).
**Rotator is done** the same way: `view/rotator_view.cpp` → `viewmodel/rotator_vm.*`
(`RotatorVM`) → `model/device/rotator.*` (`Gs232bRotator`, wraps miman_orbital for now).
**Track is done**: `view/track_view.cpp` → `viewmodel/track_vm.*` (`TrackVM`; keeps the
`Requested` selection and search text) → `model/orbit/track_target.*` (`TrackTarget`, wraps
Get/SetNowTracking) + `model/orbit/pass_table.*` (`ExamplePassTable`: still the hardcoded
20-satellite example data, not real SGP4 passes — wiring real passes is a feature change).
**Beacon is done**: `view/beacon_view.cpp` (tab bar only) + one file per tab in `view/beacon/`
(`beacon_tab`, `mission_beacon_tab`, `report_tab`, `event_tab`, `response_tab`, `ax100_rparam_tab`)
→ `viewmodel/beacon_vm.*` (`BeaconVM`, telemetry display), `viewmodel/ax100_vm.*` (`Ax100VM`,
AX100 remote params, one method per button, logging here), `viewmodel/shell_vm.*` (`ShellVM`,
OBC direct shell) → `model/telemetry/telemetry_store.*` (`TelemetryStore`),
`model/device/ax100_radio.*` (`Ax100Radio`, gs_rparam/CSP calls), `model/telecommand/direct_shell.*`
(`DirectShell`). `report_tab.cpp` is still ~2,350 lines (one big per-report-type switch) — split
it by report type when miman_coms' report parser moves into `model/telemetry/`.
**Control is done**: `view/control_view.cpp` + popups in `view/control/` (`ping_node_popup`,
`switch_satellite_popup`, `custom_rparam_popup`) → `viewmodel/control_vm.*` (`ControlVM`,
`CustomRparamVM`) → `model/device/csp_remote_param.*` (`CspRemoteParam`),
`model/telecommand/obc_ping.*`, `model/telecommand/command_queue.*` (`CommandQueue`).
Note: the middle Pass Operation column (`ImGui_CommandWindow_Body`) is actually the **FTP
manager** (file list / upload / download), not telecommands. The telecommand generator
(`DrawCmdGeneratorBody`, ~8,900 lines) is the popup opened by Control's "Send Telecommand" /
"Initial Ops" — it is still in YonseiGS_imgui.cpp (non-static now) and is the last window.
**FTP is done**: `view/ftp_view.cpp` (functions keep the old `ImGui_CommandWindow*` names) →
`viewmodel/ftp_vm.*` (`FtpVM`; AMP button shares `ControlVM::Amp()`) →
`model/telecommand/ftp_client.*` (`FtpClient`), `model/telecommand/downlink_request.*`.
**Command generator**: `view/cmdgen/cmdgen_view.cpp` (category + command combos,
ImGui + `vm.` only) → `viewmodel/cmdgen_vm.*` (`CmdGeneratorVM`, catalog-driven picker).
Per-command input forms are split by (mission, FSW app) into `view/cmdgen/raw/forms_raw.cpp`,
`view/cmdgen/bee1000/forms_<app>.cpp`, `view/cmdgen/cosmic/forms_<app>.cpp` (verbatim cases,
dispatched by `cmdgen_forms.h`). **Popups** are extracted verbatim to `view/popups/*.cpp`
(now with ViewModels, see step 2 below). **app/** holds `main.cpp`, theme, `main_menu.cpp`,
`workspace.cpp`.

### Planned refactor steps 1–5 — done (2026-11-02, each commit verified with golden + snapshot)
1. **Command forms → encoder / single send path — done.** Every form's header/checksum/envelope copy was
   replaced by `mim::tc::EncodeCfeCommand` (`model/telecommand/ccsds_cmd`, plus `EncodeCfeCommandFcAt7` for the
   D13 Generic s8–float layout) and `mim::tc::SendTelecommand` (`model/telecommand/telecommand_sender`).
   COSMIC multi-packet procedures 207/208 moved verbatim to `model/missions/cosmic/cosmic_procedures`,
   Raw Schedule path (D2) to `model/telecommand/scheduled_command`. `view/` has no `SendUplink` call.
   **Not done (deferred):** forms still fill `command->...` payload fields inline (no per-app
   `<app>_commands` encoder functions yet) and there is no schema-driven generic form. Both are large
   and should be planned separately (forms.txt fingerprint + golden.txt are the safety net).
2. **Popup ViewModels — done.** `SetupVM`, `Gs100ParamVM`, `TleVM` (+ Load progress), `FdsVM`, `SatInfoVM`,
   `SbandVM`. `view/popups/` has no `State.` / `console.` / `setup->`; `popups_common.h` keeps only
   `mim_winflags` and the table freeze constants.
3. **YonseiGS_imgui.cpp leftovers — done.** Console → `model/service/console_log.cpp` (log storage/file) +
   `view/console_view.cpp` (Draw); AutoPilot window → `view/autopilot_view.cpp` + `AutoPilotVM`
   (window is still not called from main.cpp, as before); legacy `Templabels`/`Initialize_CMDLabels`/
   `FilterRules`/`INITIAL_CMD_INDICES` deleted from src, frozen copy in `tools/cmdgen_golden/legacy_tables.h`
   for the catalog check. `YonseiGS_imgui.cpp` is ~430 lines (globals + GL helpers).
4. **Backend split into model/ — done.** `model/telemetry/report_parser` + `telemetry_logger` (*Saver);
   `model/telecommand/cmd_generator_gs` + `packet_codec`; `view/beacon/report/report_{eps,adcs,uant}.cpp`
   (Report tab split by report type); `model/orbit/satellite_object`, `tle_loader`, `doppler`;
   `model/device/rotator_io`, `gs100_param_tables`, `csp_diagnostics`. What remains of the legacy files
   was `git mv`-ed: `model/device/miman_{coms,csp,radial}.*`, `model/orbit/miman_orbital.*`
   (link tasks, transceiver init, globals, tracking target).
5. **StateCheckUnit decomposition — done.** Members moved (same declarations/defaults, references
   rewritten mechanically) into `AppUi` (app/ui_state.h), `SatTabs` (app/sat_tabs.h), `FtpShared`
   (model/service/ftp_state.h), `AutoPilotCfg` (model/service/autopilot_state.h), `RotatorFlags`
   (model/device/rotator_state.h), `LinkFlags` (model/device/link_state.h), `OrbitData`
   (model/orbit/orbit_state.h). These headers are included at the end of YonseiGS_config.h. `State` keeps
   only app lifecycle flags (InitializeRequired, GUI, SbandUse, Debugmode, AllThread, Sleep).
   Still plain globals without locks (same as before) — turning them into classes with accessors /
   locking is a behavior-affecting follow-up, not done.

Verifying on a machine without sudo (WSL): download `xvfb xdotool imagemagick-6.q16` and their libs
with `apt-get download`, `dpkg -x` them into a local prefix, run Xvfb with `-nolisten unix -listen tcp`
(WSLg's /tmp/.X11-unix is not writable) and an LD_PRELOAD shim that rewrites Xvfb's hardcoded
`/usr/bin/xkbcomp` path; use `DISPLAY=127.0.0.1:N`.

### MVVM shape for ImGui (important)

ImGui is **immediate-mode**, so classic MVVM data binding does NOT apply. Take
the *separation of concerns*, not the binding mechanism:
- ViewModel exposes plain state via getters polled every frame (no observable
  properties / no PropertyChanged).
- View contains only `ImGui::` calls + `vm.` calls — no pthread, no `State.`,
  no hardware calls, no `console.AddLog`, no `setup->` access.
- ViewModel holds orchestration (when/conditionally to act, thread lifecycle,
  logging, state toggles). Model holds domain/hardware (SetRxFreq, CSP, SGP4).
- Decision so far: ViewModel = class + global refs (no DI yet); threads keep
  existing `p_thread[]` indices for now.

## UI theme (Navy Dark — current target)

- Global style is applied by `mim::ApplyTheme()` (in YonseiGS_theme.cpp), called
  from main.cpp. The old `ImGuiCustomStyle()` is dead code, kept for rollback;
  delete only after the navy theme is fully settled.
- Color tokens live in `YonseiGS_theme.h` as `mim::` constants (BG_MAIN, ACCENT,
  TEXT, OK, ERR, DANGER, ...). **Never hardcode `ImVec4(...)` colors in views —
  always use a token** so future theme changes propagate. (Two hardcoded teal
  values in sat tabs / status bar were already replaced with `BG_PANEL2`/`BG_MAIN`.)
- `mim_winflags` no longer includes `HorizontalScrollbar` (default = no scroll).
  Add it explicitly only where a window genuinely needs scrolling.
- Glyphs `●`/`○`/`①②` are NOT in Inconsolata.ttf and render as `?`. Use the
  `mim::Dot()` helper (draws a real circle via DrawList) instead of `●` text.
  Tab labels use ASCII (`1. Pre-Pass`) instead of `①`.

## Layout entry point

- Active path: `main.cpp` → `ImGui_MainMenu()` + `ImGui_MainWorkspace(Rw,Rh)`
  (the latter defined inside YonseiGS_imgui.cpp). It draws: satellite tabs host →
  status bar host → function tab bar (Pre-Pass / Pass Operation, each a 3-column
  workspace) → console.
- `miman_layout.cpp` is an OLDER duplicate implementation and is **excluded from
  the build** (it caused multiple-definition errors). Don't re-add it.
- Window `_Body` functions (e.g. `ImGui_TrackWindow_Body`) must NOT call their
  own `ImGui::Begin/End` — the caller wraps them in `BeginChild`. (Track and
  Frequency previously had a stray inner `Begin` that made their columns blank.)

## Known defects (write here, fix separately — do NOT fix mid-refactor)

- `GetVisibleSatColor()` called with satellite index instead of slot index in
  some places (status bar) — wrong color, not a crash.
- Missing `sec < 0` guard in AOS calculation (sat tab status).
- `use` filter removed from `Initialize_SatContext()` → all dummy satellites
  populate the tabs (sat tab list shows garbage "AOS in -17754..." entries).
- `StateCheckUnit` has a duplicate `Doppler` member (bool + float) — confirm
  scoping during the eventual StateCheckUnit decomposition.
- ~~Missing `return` in 35 non-void functions~~ **fixed** (`fix:` commit after Rotator MVVM).
  Was: -O0 build died with SIGILL after Start; Release looped `Load_Paramtable0` forever
  (Tables 1/5 never loaded), SIGINT shutdown never finished, popups' `BeginPopupModal`
  check was optimized away. Now `-Wreturn-type` is clean; keep it that way.
- Without TLE loaded, **Start Doppler Correction** (and Doppler → Start Rotator
  Engaging) crashes with SIGSEGV in `SetNowTracking_impl` → `Console::AddLog`
  (Release build, original code). Snapshot scenario avoids these buttons.
- `Save2FRAM` is started with `&id` of a block-local `int8_t id` that goes out of
  scope immediately (3 places in param popups) — dangling pointer race.
- `SwitchSatelliteTask` writes an empty HMAC key: `target_key` is never filled
  (`hmac_keys[]` is unused).
- TX slot `p_thread[4]` is shared by uplink/ping/shell; several call sites create
  without a preceding join, overwriting the previous handle (leaked/unjoined thread).
- AX100 GNDWDT reset calls `csp_transaction(..., 1000, ...)` on the UI thread
  (blocks rendering up to 1 s).
- ~~Rotator **IO Buffer Free** hangs the UI thread~~ **fixed** with the missing-`return` fix
  (`RotatorEnter()` fell off the end → -O3 jumped back into the body forever).
- ~~**Shutdown race (SIGINT)**~~ **fixed** (`fix:` commit after Track MVVM). The SIGINT branch in
  `main` also set `State.AllThread = false`, skipping `StopAllThreads()`; the downlink thread then
  read `setup->default_timeout` after `delete setup` → SIGSEGV → libcsp `init_switch()`'s
  `sig_handler` → `exit(1)` (about half of the runs). Now SIGINT only clears `State.GUI` and takes
  the same cleanup path as closing the window. Note libcsp's `sig_handler` still turns any
  SIGSEGV/SIGFPE/SIGTERM into a silent `exit(1)` — check the exit status, not just the log.
- Track window `SELECTED` shows `?` when nothing is tracked: the `—` (em dash) glyph is not
  in Inconsolata.ttf. Polar plot reads `dummies[GetNowTracking()]` without an upper bound
  check (out of bounds if the tracking index is >= 20, e.g. set by AutoPilot).
- Rotator **Set / Init** → `RotatorSetTo()` does `sleep(1)` on the UI thread (UI freezes 1 s
  per press).

## Service layer (refactor in progress)

See [docs/REFACTOR_SERVICE_LAYER.md](docs/REFACTOR_SERVICE_LAYER.md).
- Service code lives in `src/model/service/` (`mim::svc` namespace); command catalog
  in `src/model/telecommand/`, per-satellite data in `src/model/missions/` (`mim::tc`).
- UI must not call `pthread_create/join` on `p_thread[]` directly — add or use a
  `mim::svc` function instead. `svc_thread.h` is the only place that knows slot numbers.
- Verify each step with `tools/ui_snapshot.sh <out>` against the previous commit and
  compare with `tools/ui_compare.sh <before> <after>`
  (screens pixel-identical above the console, sorted console log identical).
- Command generator (`DrawCmdGeneratorBody`) defects D1–D11 are listed in
  [docs/CMDGEN_ANALYSIS.md](docs/CMDGEN_ANALYSIS.md) §4. D1 (checksum on re-send) is
  fixed, D12 (u16 stack overflow) is fixed. Still open: **D2** — Schedule path
  passes `sch_cmd_t*` as `packetsign*`; **D13** — generic s8..float (case 5–9) swap
  function-code/checksum bytes (6/7) vs every other command; needs FSW confirmation.
  **D14** — Initial Operations list has 205 ("Beacon Mukbang") commented as UANT burn;
  real UANT Burn Channel (161) is missing from it.
- Verify any change touching command generation with `tools/cmdgen_golden.sh <out>`
  and `diff` both `golden.txt` (telecommand bytes) and `forms.txt` (picker lists per
  filter/mode + each command's widget list) against the previous commit.
