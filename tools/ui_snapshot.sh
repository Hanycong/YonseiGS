#!/bin/bash
# UI 회귀 확인용 스냅샷 스크립트 (리팩토링 A/B 비교용)
#
# 하는 일:
#   1. 저장소를 OUT_DIR/build 에 빌드 (저장소의 build/ 폴더는 건드리지 않음)
#   2. 가상 디스플레이(Xvfb)에서 bin/ 기준으로 실행
#   3. Setup 팝업 -> Start 클릭 -> Pre-Pass / Pass Operation 탭 캡처
#   4. 실행 로그(stdout/stderr) 저장
#
# 필요 패키지 (Ubuntu): xvfb xdotool imagemagick + 빌드 의존성
#   libglfw3-dev libglew-dev freeglut3-dev libglbinding-dev libssh-dev
#   libcurl4-openssl-dev libassimp-dev libglm-dev libstb-dev
#
# 사용법: tools/ui_snapshot.sh <OUT_DIR>
#   결과: OUT_DIR/0_setup.png 1_pre_pass.png 1a_frequency.png 1b_pre_pass_started.png 1c_pre_pass_stopped.png
#         1d_rotator.png 1e_track_select.png 1f_track_on.png 1g_track_search.png 1h_track_cleared.png
#         2_pass_operation.png 3_ftp_upload.png run.log build.log applogs/
#
# 화면 좌표는 1920x1080 고정 창 기준이다 (main.cpp glfwCreateWindow).
#
# Release(-O3)로 빌드한다: 기준 스냅샷들이 Release 로 만들어졌으므로 비교 조건을 유지한다.
# (return 누락 수정 전에는 -O0 빌드가 GCC 13에서 SIGILL로 죽었다. 지금은 -O0 도 동작한다)

set -u
REPO="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:?usage: $0 <OUT_DIR>}"
mkdir -p "$OUT/build"
OUT="$(cd "$OUT" && pwd)"
DISP=":$(( 90 + RANDOM % 9 ))"

echo "[snapshot] build -> $OUT/build"
( cd "$OUT/build" && cmake "$REPO" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 )
if [ $? -ne 0 ]; then
    echo "[snapshot] BUILD FAILED (see $OUT/build.log)"
    grep -E "error" "$OUT/build.log" | head -20
    exit 1
fi
echo "[snapshot] build OK"

# 실행 중 data/ 아래 생기는 로그 파일을 끝나고 OUT_DIR로 옮기기 위한 기준 시각
MARKER="$OUT/.start_marker"
touch "$MARKER"

Xvfb "$DISP" -screen 0 1920x1080x24 > /dev/null 2>&1 &
XVFB_PID=$!
sleep 2

( cd "$REPO/bin" && DISPLAY="$DISP" LIBGL_ALWAYS_SOFTWARE=1 exec "$OUT/build/BEE-1000" ) > "$OUT/run.log" 2>&1 &
APP_PID=$!
sleep 10

export DISPLAY="$DISP"
# Setup 팝업을 끝까지 스크롤한 뒤 캡처
xdotool mousemove 950 600
for _ in $(seq 30); do xdotool click 5; sleep 0.05; done
sleep 1
import -window root "$OUT/0_setup.png"

# Start 버튼
xdotool mousemove 719 1042 click 1
sleep 6
import -window root "$OUT/1_pre_pass.png"

# Pre-Pass / Frequency 창: Set(중심 주파수) -> Set to RX -> Set to TX -> Apply(지연 설정) -> 캡처
xdotool mousemove 1161 287 click 1     # Set (Center Frequency)
sleep 3
xdotool mousemove 805 402 click 1      # Set to RX
sleep 2
xdotool mousemove 1109 402 click 1     # Set to TX
sleep 2
xdotool mousemove 955 594 click 1      # Apply (Default/Guard/Queue Delay)
sleep 2
import -window root "$OUT/1a_frequency.png"

# Pre-Pass: Rotator On -> 캡처 -> Rotator OFF -> 캡처
# (Start Doppler Correction / Start Rotator Engaging 은 누르지 않는다:
#  TLE 없이 누르면 원본 코드도 세그폴트로 죽는다. CLAUDE.md "Known defects" 참고)
xdotool mousemove 1437 626 click 1     # Rotator On
sleep 3
import -window root "$OUT/1b_pre_pass_started.png"
xdotool mousemove 1741 626 click 1     # Rotator OFF
sleep 3
import -window root "$OUT/1c_pre_pass_stopped.png"

# Pre-Pass / Rotator 창: Set(목표 AZ/EL 로 이동) -> Init(0,0) -> 캡처
# (장비가 없으므로 쓰기 실패 로그가 남는다. Set/Init 은 원래 UI 스레드에서 1초 sleep)
# (Rotator IO Buffer Free 는 시나리오에 넣지 않았다: return 누락 수정 전 기준과 비교할 수 없어서.
#  수정 후에는 멈추지 않음을 별도로 확인했다)
xdotool mousemove 1437 455 click 1     # Set
sleep 3
xdotool mousemove 1741 455 click 1     # Init
sleep 3
import -window root "$OUT/1d_rotator.png"

# Pre-Pass / Track 창 (왼쪽 열, 현재는 예시 위성 20개 표):
# 행 선택(ISS) -> Track 버튼(NOAA-19) -> 검색어 입력 -> 검색어 지우기
xdotool mousemove 100 328 click 1      # ISS 행 선택
sleep 2
import -window root "$OUT/1e_track_select.png"
xdotool mousemove 497 346 click 1      # NOAA-19 Track
sleep 2
import -window root "$OUT/1f_track_on.png"
xdotool mousemove 290 263 click 1      # 검색창
sleep 1
for k in S E N T; do xdotool keydown $k; sleep 0.15; xdotool keyup $k; sleep 0.3; done
sleep 2
import -window root "$OUT/1g_track_search.png"
# 키를 누르고 바로 떼면 느린(소프트웨어 렌더링) 프레임 사이에서 입력이 유실되므로
# 한 프레임 이상, ImGui 키 반복 지연(0.275s)보다 짧게 누른다
for _ in $(seq 8); do xdotool keydown BackSpace; sleep 0.15; xdotool keyup BackSpace; sleep 0.3; done
sleep 1
xdotool mousemove 540 760 click 1      # 빈 곳 클릭: 검색창 비활성화 (안 하면 다음 탭 클릭이 무시됨)
sleep 2
import -window root "$OUT/1h_track_cleared.png"

# 2. Pass Operation 탭
xdotool mousemove 342 216 click 1
sleep 4
import -window root "$OUT/2_pass_operation.png"

# Pass Operation: FTP Upload 버튼 (선택된 파일 없음 -> 워커 시작/에러 경로)
xdotool mousemove 1215 383 click 1
sleep 5
import -window root "$OUT/3_ftp_upload.png"

if kill -0 "$APP_PID" 2>/dev/null; then
    echo "[snapshot] app alive after scenario"
else
    echo "[snapshot] APP EXITED during scenario"
fi

# 종료 시퀀스 확인: Ctrl+C 와 같은 SIGINT (GS100 연결 성공 시 shutdown_handler 등록됨)
APP_REAL_PID=$(pgrep -f "^$OUT/build/BEE-1000" | head -1)
if [ -n "$APP_REAL_PID" ]; then
    kill -INT "$APP_REAL_PID"
    for _ in $(seq 60); do kill -0 "$APP_REAL_PID" 2>/dev/null || break; sleep 1; done
    if kill -0 "$APP_REAL_PID" 2>/dev/null; then
        echo "[snapshot] shutdown: still running 60s after SIGINT" | tee -a "$OUT/run.log"
    else
        echo "[snapshot] shutdown: exited after SIGINT" | tee -a "$OUT/run.log"
    fi
fi
# 종료 코드 (0 = 정상, 128+N = 시그널 N 으로 죽음. 예: 139 = SIGSEGV)
wait "$APP_PID" 2>/dev/null
echo "[snapshot] exit status: $?" | tee -a "$OUT/run.log"
kill -9 "$APP_PID" $APP_REAL_PID 2>/dev/null
kill "$XVFB_PID" 2>/dev/null
wait 2>/dev/null
# 실행이 만든 로그(data/BEE_*_Log)는 저장소를 더럽히지 않도록 OUT_DIR/applogs 로 이동
mkdir -p "$OUT/applogs"
find "$REPO/data" -type f -newer "$MARKER" -exec mv {} "$OUT/applogs/" \;
# 정상 종료 시 ImGui 가 bin/imgui.ini 를 새로 만든다 (추적 대상 아님)
[ "$REPO/bin/imgui.ini" -nt "$MARKER" ] && mv "$REPO/bin/imgui.ini" "$OUT/applogs/"
echo "[snapshot] done -> $OUT"
