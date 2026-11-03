#!/bin/bash
# Beacon / Report 탭 화면 지문: 이 저장소와 GS_nuri5 를 같은 사례로 그려 글자 출력을 비교
#
# 사용법: tools/beacon_report_golden.sh <OUT_DIR> [<GS_nuri5 소스> <GS_nuri5 빌드 폴더>] [list 인자...]
#   GS_nuri5 인자를 주면 그 빌드 폴더(CMake, Release)의 오브젝트로 기준 하네스를 만든다.
#   결과: OUT_DIR/ours.txt (이 저장소), OUT_DIR/n5.txt (GS_nuri5) — 사례마다 지문 한 줄
#         OUT_DIR/ours_beacon.txt, n5_beacon.txt — Beacon 탭 사례 전체 기록
#   list 인자 기본값: "123 255 128" (위성 BEE1012/UELYSYS/SD1SAT, cc 0~255, raw_size 0~128)
#   다른 사례 하나 보기: OUT_DIR/ours_golden dump <sat> <kind> <cc> <n>  (n5_golden 도 같음)
#   BEE-1000 모드끼리(예전 커밋 대비) 비교하려면 두 커밋에서 list 0 ... 결과를 diff 한다.
set -eu
REPO="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:?usage: $0 <OUT_DIR> [<n5 src> <n5 build>] [list args]}"; shift
N5SRC=""; N5BUILD=""
if [ $# -ge 2 ] && [ -d "$1" ] && [ -d "$2" ]; then N5SRC="$(cd "$1" && pwd)"; N5BUILD="$(cd "$2" && pwd)"; shift 2; fi
ARGS=("${@:-}"); [ -z "${ARGS[0]}" ] && ARGS=(123 255 128)
mkdir -p "$OUT/build"
OUT="$(cd "$OUT" && pwd)"
H="$REPO/tools/beacon_report_golden"

( cd "$OUT/build" && cmake "$REPO" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 \
  && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 ) || { echo "[brg] BUILD FAILED (see $OUT/build.log)"; exit 1; }
DIR="$OUT/build/CMakeFiles/BEE-1000.dir"
FLAGS="$(sed -n 's/^CXX_FLAGS = //p' "$DIR/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$DIR/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$DIR/flags.make")"
OBJS=$(find "$DIR/src" -name '*.o' ! -name 'main.cpp.o' | sort)
LIBS="$(sed -e 's/.* -o BEE-1000 //' "$DIR/link.txt")"
g++ $FLAGS -I"$REPO/lib/imgui" -w -c "$H/ours_main.cpp" -o "$OUT/ours_main.o"
( cd "$OUT/build" && g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/ours_main.o" $OBJS $LIBS -o "$OUT/ours_golden" )
( cd "$REPO/bin" && "$OUT/ours_golden" beacon > "$OUT/ours_beacon.txt" && "$OUT/ours_golden" list "${ARGS[@]}" > "$OUT/ours.txt" )
echo "[brg] ours: $(wc -l < "$OUT/ours.txt") lines -> $OUT/ours.txt"

if [ -n "$N5SRC" ]; then
    D5="$N5BUILD/CMakeFiles/BEE-1000.dir"
    F5="$(sed -n 's/^CXX_FLAGS = //p' "$D5/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$D5/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$D5/flags.make")"
    O5=$(find "$D5/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'miman_imgui.cpp.o' | sort)
    L5="$(sed -e 's/.* -o BEE-1000 //' "$D5/link.txt")"
    g++ $F5 -I"$N5SRC/src" -I"$H" -w -c "$H/n5_main.cpp" -o "$OUT/n5_main.o"
    ( cd "$N5BUILD" && g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/n5_main.o" $O5 $L5 -o "$OUT/n5_golden" )
    ( cd "$N5SRC/bin" 2>/dev/null || cd "$REPO/bin"; "$OUT/n5_golden" beacon > "$OUT/n5_beacon.txt" && "$OUT/n5_golden" list "${ARGS[@]}" > "$OUT/n5.txt" )
    echo "[brg] n5: $(wc -l < "$OUT/n5.txt") lines -> $OUT/n5.txt"
    echo "[brg] differing report cases: $(diff "$OUT/ours.txt" "$OUT/n5.txt" | grep -c '^<' || true)"
fi
