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
#   결과: OUT_DIR/0_setup.png 1_pre_pass.png 1b_pre_pass_started.png 1c_pre_pass_stopped.png
#         2_pass_operation.png run.log build.log applogs/
#
# 화면 좌표는 1920x1080 고정 창 기준이다 (main.cpp glfwCreateWindow).
#
# Release(-O3)로 빌드한다: 빌드 타입 없이(-O0) 빌드하면 return 문이 빠진
# 스레드 함수(RadInitialize -> Load_Paramtable0 등)가 GCC 13에서 SIGILL로 죽는다.
# (CLAUDE.md "Known defects" 참고)

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

# Pre-Pass: Rotator On -> 캡처 -> Rotator OFF -> 캡처
# (Start Doppler Correction / Start Rotator Engaging 은 누르지 않는다:
#  TLE 없이 누르면 원본 코드도 세그폴트로 죽는다. CLAUDE.md "Known defects" 참고)
xdotool mousemove 1437 626 click 1     # Rotator On
sleep 3
import -window root "$OUT/1b_pre_pass_started.png"
xdotool mousemove 1741 626 click 1     # Rotator OFF
sleep 3
import -window root "$OUT/1c_pre_pass_stopped.png"

# 2. Pass Operation 탭
xdotool mousemove 342 216 click 1
sleep 4
import -window root "$OUT/2_pass_operation.png"

if kill -0 "$APP_PID" 2>/dev/null; then
    echo "[snapshot] app alive after scenario"
else
    echo "[snapshot] APP EXITED during scenario"
fi
kill "$APP_PID" 2>/dev/null
sleep 1
kill -9 "$APP_PID" 2>/dev/null
kill "$XVFB_PID" 2>/dev/null
wait 2>/dev/null
# 실행이 만든 로그(data/BEE_*_Log)는 저장소를 더럽히지 않도록 OUT_DIR/applogs 로 이동
mkdir -p "$OUT/applogs"
find "$REPO/data" -type f -newer "$MARKER" -exec mv {} "$OUT/applogs/" \;
echo "[snapshot] done -> $OUT"
