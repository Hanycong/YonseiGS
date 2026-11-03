#!/bin/bash
# 누리호 5차 GS(GS_nuri5) 명령 생성기 기준값 덤프
#
# 사용법: tools/nuri5_reference.sh <GS_nuri5 저장소> <OUT_DIR>
#   결과: OUT_DIR/picker.txt  (위성별 서브시스템 콤보 항목과 명령 번호, 명령 라벨)
#         OUT_DIR/packets.txt (위성별 모든 명령의 "Generate CMD" 패킷)
# 이 저장소의 tools/cmdgen_golden.sh 결과(picker5.txt, golden5.txt)와 비교한다:
#   diff <(grep '^picker' OUT_DIR/picker.txt) GOLDEN/picker5.txt
#   패킷: sat/cmd/press 가 같은 줄끼리 비교 (GS_nuri5 메뉴에 숨겨진 6400~6405, 6800~6804 는 golden5 에 없음)
set -eu
N5="$(cd "${1:?usage: $0 <GS_nuri5> <OUT_DIR>}" && pwd)"
OUT="${2:?usage: $0 <GS_nuri5> <OUT_DIR>}"
HERE="$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$OUT/src" "$OUT/build"
OUT="$(cd "$OUT" && pwd)"
( cd "$N5" && git archive HEAD ) | tar -x -C "$OUT/src"
( cd "$OUT/build" && cmake "$OUT/src" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 ) \
    || { echo "[n5] BUILD FAILED (see $OUT/build.log)"; exit 1; }
DIR="$OUT/build/CMakeFiles/BEE-1000.dir"
FLAGS="$(sed -n 's/^CXX_FLAGS = //p' "$DIR/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$DIR/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$DIR/flags.make")"
OBJS=$(find "$DIR/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'miman_imgui.cpp.o' | sort)
LIBS="$(sed -e 's/.* -o BEE-1000 //' "$DIR/link.txt")"
g++ $FLAGS -I"$OUT/src/src" -w -c "$HERE/nuri5_reference/n5_main.cpp" -o "$OUT/n5_main.o"
( cd "$OUT/build" && g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/n5_main.o" $OBJS $LIBS -o "$OUT/n5_golden" )
( cd "$OUT/src/bin" && "$OUT/n5_golden" "$OUT/picker.txt" ) 2> "$OUT/packets.txt" > "$OUT/run.log"
echo "[n5] $(grep -c ' id=' "$OUT/packets.txt") packets -> $OUT/packets.txt"
