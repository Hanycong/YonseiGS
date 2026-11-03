# BEE-1000 Ground Station — Claude Code Context

This file is auto-loaded by Claude Code. Keep it concise — detailed analysis lives in [docs/](docs/).

## Project at a glance

- **What**: Satellite ground station GUI for BEE-1000 CubeSat (Yonsei University)
- **Language**: C/C++17, single Linux executable `bin/BEE-1000`
- **Stack**: GLFW + Dear ImGui + OpenGL (GUI), pthread (16 threads), CSP protocol (radio), libsgp4 (orbital), libssh (S-band remote), CURL (TLE fetch)
- **libsgp4** is compiled from `lib/libsgp4/*.cc` (target `sgp4_src`); the checked-in `lib/libsgp4/libsgp4.a` is stale and no longer linked. `Tle` accepts Alpha-5 catalog numbers (`A0965` = 100965, letters A–Z without I/O = 10–33).
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

## Module quick reference (YonseiGS_* files, formerly miman_*)

| Module | Role | Hardware/Protocol | Threads |
|---|---|---|---|
| common/YonseiGS_config | Shared defs (packet structs, Setup, Console decl), state structs | — | — |
| view/YonseiGS_imgui | 3D model window, View function declarations | GLFW/ImGui | main loop |
| view/YonseiGS_model | OpenGL satellite/globe/path drawing (a View, despite the name) | GPU | main loop |
| model/orbit/YonseiGS_orbital | TLE tracking (SGP4), rotator command, Doppler | `/dev/GS232B`, CURL | T[2], T[3], T[7] |
| model/device/YonseiGS_radial | Radio param tables (Ptable_0/1/5) | `/dev/GS102` via libparam | T[12] |
| model/device/YonseiGS_csp | CSP stack init, KISS USART, watchdog | `/dev/GS100`, `/dev/GS101` | T[0] (csp route) |
| model/device/YonseiGS_coms | TX/RX scheduler, beacon/HK parse | CSP network | T[4], T[10], T[11] |
| model/device/YonseiGS_s_control | S-band GS remote control | SSH | — |
| model/service/YonseiGS_autopilot | Pass auto-scheduling | CSP | T[13] |
| model/service/YonseiGS_ftprdp | FTP up/download/list | libftp_client | T[8] |
| app/YonseiGS_satcontext, app/YonseiGS_theme | Active/visible satellite tabs; theme tokens + fonts | — | — |

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
Get/SetNowTracking). Rows = satellites with `use == true` (chosen with "Select" in TLE Manager),
indexed by `OrbitData.Satellites` index (not row number); AOS/LOS/max El = `_nextaos[0]` etc.
(UTC+9); search is case-insensitive; Sky View az/el = tracked satellite's `topo` (updated by
the TrackingSatellites thread). The old hardcoded 20-satellite `pass_table` was removed.
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
  from main.cpp. The old `ImGuiCustomStyle()` is dead code, kept for rollback.
- Color tokens live in `YonseiGS_theme.h` as `mim::` constants (BG_MAIN, ACCENT,
  TEXT, OK, ERR, DANGER, ...). **Never hardcode `ImVec4(...)` colors in views —
  always use a token** so future theme changes propagate.
- **Fonts (2026-11-02):** body = DroidSans 20 (font [0], used everywhere by default),
  `mim::font_title` DroidSans-Bold 36, `mim::font_subtitle` DroidSans 18,
  `mim::font_section` DroidSans-Bold 15 (section headers), `mim::font_mono` Inconsolata 18
  (console only — log lines are aligned with dots). Hangul is merged in (MergeMode) from
  NanumGothic 20 into font [0] and NanumGothicCoding 18 into font_mono (OFL, `bin/font`);
  other fonts have no Hangul. Do not scale windows with
  `SetWindowFontScale(Rh * k)` any more; workspace bodies are called with `fontscale = 1.0`.
  Do not align labels with spaces (`"Name     : "`) — the body font is proportional; use
  `mim::ui::RowLabel()`.
- **`view/ui_kit.h` (mim::ui)** is the shared widget set every window uses: `SectionHeader`
  (small bold caps + separator), `RowLabel` (label column), `GridButton` (2-column button grid),
  `WideButton`, `ToggleWideButton` (red while active = the click stops something),
  `StatusLamp` (green ok / red not), `ValueRow`. New UI should be built from these so all
  windows keep the same spacing and button height (`ButtonHeight()` = frame height x 1.3).
- **`view/file_dialog.h` (mim::ui::FileDialog)** is the Windows-Explorer-style file picker
  (back/forward/up, breadcrumb, Quick access incl. WSL Windows Downloads/Desktop, sortable
  Name/Date/Type/Size, search, extension filter). Keep one static instance per caller; call
  `Open(start, ".txt;.tle")` on the button and `Draw(title, buf, size)` every frame inside the
  parent popup (returns true with an absolute path). Used by TLE Manager "Load from local...".
- Status bar: devices (GS100/Rotator/Switch/S-band) and modes (Engage/Doppler/AutoPilot/AMP)
  all use `StatusLamp`: green = connected / running, red = disconnected / stopped.
- `mim_winflags` no longer includes `HorizontalScrollbar` (default = no scroll).
- Glyphs `●`/`○`/`①②`/`—` are not in the loaded fonts. Use `mim::Dot()` / `StatusLamp`
  (DrawList circles) and ASCII.
- Snapshot harness click coordinates (tools/ui_snapshot.sh) are tied to this layout; after any
  layout change update them and confirm the console/stdout logs still match the baseline.

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
- ~~Track window `SELECTED` shows `?`~~ (now "Tracking -", ASCII). Polar plot reads `dummies[GetNowTracking()]` without an upper bound
  check (out of bounds if the tracking index is >= 20, e.g. set by AutoPilot).
- `Console::Draw` (view/console_view.cpp): when `ImGui::Begin` returns false (window collapsed)
  it returns without `PopStyleColor(3)` — style stack leak. Fix: pop before that return.
- S-band Manager popup shows garbage text (e.g. a window ID path) in its output box before any
  command was sent: `SbandVM::Output()` returns `sgs->buffer`, which is never cleared.
- Rotator **Set / Init** → `RotatorSetTo()` does `sleep(1)` on the UI thread (UI freezes 1 s
  per press).
- `SatelliteObject::Initializer` propagates all 64 `_nextaos` slots, including empty ones (default
  `DateTime`, year 1) when there are fewer than 64 passes in 7 days → SGP4 throws `(e <= -0.001)`,
  caught and logged as `[DEBUG]`, for every satellite on TLE Manager **Select**. Harmless today
  (the remaining slots are empty anyway) but it hides real SGP4 errors in that try block.

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
