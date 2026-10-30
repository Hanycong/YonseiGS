#!/bin/bash
# 명령 생성기 골든 패킷 덤프
#
# 사용법: tools/cmdgen_golden.sh <OUT_DIR>
#   결과: OUT_DIR/golden.txt  (명령별 3회 "Generate CMD" 결과 packetsign 바이트)
#         OUT_DIR/forms.txt   (명령 선택 콤보 항목 목록 + 명령별 입력 폼 위젯 목록)
#
# 두 커밋의 golden.txt 를 diff 해서 위성으로 나가는 패킷이 바뀌었는지 확인한다.
#   diff A/golden.txt B/golden.txt
#   diff A/forms.txt  B/forms.txt
#
# 본 프로그램과 같은 Release 플래그로 빌드한 오브젝트를 재사용하고,
# main.cpp / miman_imgui.cpp / service/svc_tx.cpp 만 하네스로 대체한다.

set -eu
REPO="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:?usage: $0 <OUT_DIR>}"
mkdir -p "$OUT/build"
OUT="$(cd "$OUT" && pwd)"

( cd "$OUT/build" && cmake "$REPO" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 \
  && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 ) || { echo "[golden] BUILD FAILED (see $OUT/build.log)"; exit 1; }

DIR="$OUT/build/CMakeFiles/BEE-1000.dir"
FLAGS="$(sed -n 's/^CXX_FLAGS = //p' "$DIR/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$DIR/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$DIR/flags.make")"
OBJS=$(find "$DIR/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'miman_imgui.cpp.o' ! -name 'svc_tx.cpp.o' | sort)
# 링크 라이브러리: 본 프로그램 link.txt 에서 오브젝트 목록 뒤의 -o BEE-1000 이후 부분
LIBS="$(sed -e 's/.* -o BEE-1000 //' "$DIR/link.txt")"

echo "[golden] compile harness"
g++ $FLAGS -w -c "$REPO/tools/cmdgen_golden/golden_main.cpp" -o "$OUT/golden_main.o"
g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/golden_main.o" $OBJS $LIBS -o "$OUT/cmdgen_golden"

echo "[golden] run"
MARKER="$OUT/.start_marker"
touch "$MARKER"
( cd "$REPO/bin" && "$OUT/cmdgen_golden" "$OUT/forms.txt" ) 2> "$OUT/golden.txt" > "$OUT/run.log"
# Console 객체가 data/BEE_Console_Log 에 남기는 로그는 저장소 밖으로 옮긴다
mkdir -p "$OUT/applogs"
find "$REPO/data" -type f -newer "$MARKER" -exec mv {} "$OUT/applogs/" \;
echo "[golden] $(grep -c ' id=' "$OUT/golden.txt") packets -> $OUT/golden.txt"
echo "[golden] $(grep -c '^form ' "$OUT/forms.txt") forms, $(grep -c '^picker ' "$OUT/forms.txt") picker lists -> $OUT/forms.txt"
