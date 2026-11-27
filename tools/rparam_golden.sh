#!/bin/bash
# 원격 파라미터(GomSpace rparam) 호출 기록
#
# 사용법: tools/rparam_golden.sh <OUT_DIR>
#   결과: OUT_DIR/rparam.txt  (GS100 테이블 0/1/5 읽기·쓰기, AX100 원격 파라미터, Custom RParam 이
#                              라이브러리에 한 호출과 그 뒤의 값 칸, 콘솔 로그)
# 두 커밋의 rparam.txt 를 diff 해서 장비로 가는 읽기/쓰기 요청이 바뀌었는지 확인한다.
#
# 본 프로그램과 같은 Release 플래그로 빌드한 오브젝트를 재사용하고, main.cpp 만 하네스로 대체한다.
# gs_rparam_get / gs_rparam_set / Console::AddLog 는 하네스의 가짜가 먼저 링크된다.
set -eu
REPO="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:?usage: $0 <OUT_DIR>}"
mkdir -p "$OUT/build"
OUT="$(cd "$OUT" && pwd)"

( cd "$OUT/build" && cmake "$REPO" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 \
  && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 ) || { echo "[rparam] BUILD FAILED (see $OUT/build.log)"; exit 1; }

DIR="$OUT/build/CMakeFiles/BEE-1000.dir"
FLAGS="$(sed -n 's/^CXX_FLAGS = //p' "$DIR/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$DIR/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$DIR/flags.make")"
OBJS=$(find "$DIR/src" -name '*.o' ! -name 'main.cpp.o' | sort)
LIBS="$(sed -e 's/.* -o BEE-1000 //' "$DIR/link.txt")"

g++ $FLAGS -w -c "$REPO/tools/rparam_golden/main.cpp" -o "$OUT/rparam_main.o"
( cd "$OUT/build" && g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/rparam_main.o" $OBJS $LIBS -o "$OUT/rparam_golden" )
( cd "$REPO/bin" && "$OUT/rparam_golden" ) > "$OUT/rparam.txt" 2> "$OUT/run.log"
echo "[rparam] $(grep -c ' GET ' "$OUT/rparam.txt") get, $(grep -c ' SET ' "$OUT/rparam.txt") set, $(grep -c ' LOG ' "$OUT/rparam.txt") log lines -> $OUT/rparam.txt"
