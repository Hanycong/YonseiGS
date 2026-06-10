# BEE-1000 Ground Station — Claude Code Context

This file is auto-loaded by Claude Code. Keep it concise — detailed analysis lives in [docs/](docs/).

## Project at a glance

- **What**: Satellite ground station GUI for BEE-1000 CubeSat (Yonsei University)
- **Language**: C/C++17, single Linux executable `bin/BEE-1000`
- **Stack**: GLFW + Dear ImGui + OpenGL (GUI), pthread (16 threads), CSP protocol (radio), libsgp4 (orbital), libssh (S-band remote), CURL (TLE fetch)
- **Entry point**: [src/main.cpp](src/main.cpp) — single 577-line `main()` runs everything
- **Build**: CMake → see [docs/BUILD_WSL.md](docs/BUILD_WSL.md)

## Critical context (read before editing)

1. **Linux-only**. Uses `pthread`, `termios`, `/dev/GS102`, `system("../amp/ampoff")`, `libssh`. Windows native build is not feasible — use **WSL2 Ubuntu**.
2. **Working directory matters**. The executable must be launched from `bin/` because main.cpp references `../bin/font/Inconsolata.ttf`, `../data/BEE_TC_Log/`, `../amp/ampoff` as relative paths.
3. **Hardware-coupled**. Without `/dev/GS102` (UHF transceiver), `/dev/GS232B` (rotator), `/dev/KTA223` (switch) the GUI still appears but device init logs `[ERROR]`. Do not "fix" those errors — they are expected on a dev machine.
4. **The header `src/miman_config.h` is the choke point**. 5,477 lines, 70+ includes, defines all packet/command types. Editing it triggers a full rebuild. See [docs/MIMAN_CONFIG_ANALYSIS.md](docs/MIMAN_CONFIG_ANALYSIS.md).
5. **Global state hub**: `StateCheckUnit State` (defined in miman_config.h, instantiated in main.cpp) carries ~35 boolean flags that drive every module's control flow. Threads communicate via these flags, not direct calls.

## Detailed docs

| Topic | File |
|---|---|
| Folder layout, module dependencies, data flow, 5-layer architecture | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |
| `miman_config.h` deep dive (Setup / SatelliteObject / StateCheckUnit / Command) | [docs/MIMAN_CONFIG_ANALYSIS.md](docs/MIMAN_CONFIG_ANALYSIS.md) |
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

- **Do not amend `miman_config.h` lightly** — full rebuild. Add new files under `src/` and include them only where needed.
- **Don't add new pthread slots without updating the comment block at [src/main.cpp:43-59](src/main.cpp#L43)**. The 16-slot array uses fixed indices.
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

### Per-window MVVM migration order (agreed)

Work **one window at a time**, and within each window do steps 1→2 together
before moving on (so the window is fully working before the next):
1. Fix/restyle the window's **View** (ImGui calls only).
2. Wire its **ViewModel/Model** logic so it works.
(Only after ALL windows are done) 3. Split into MVVM files (view/ viewmodel/ model/).

Window sequence: Frequency (done as sample) → Rotator → Track → Beacon →
Control → Command (largest, last). Then `StateCheckUnit` decomposition (riskiest, last).

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

- Global style is applied by `mim::ApplyTheme()` (in miman_theme.cpp), called
  from main.cpp. The old `ImGuiCustomStyle()` is dead code, kept for rollback;
  delete only after the navy theme is fully settled.
- Color tokens live in `miman_theme.h` as `mim::` constants (BG_MAIN, ACCENT,
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
  (the latter defined inside miman_imgui.cpp). It draws: satellite tabs host →
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
