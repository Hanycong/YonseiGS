#!/bin/bash
# 운용 기능 테스트 (AMP 타이머, FTP Stop, Control WDT 버튼)
#
# 사용법: tools/ops_test.sh <OUT_DIR>
#   결과: OUT_DIR/result.txt (항목별 ok / FAIL), 실패가 있으면 종료 코드 1
#
# 본 프로그램과 같은 Release 플래그로 빌드한 오브젝트를 재사용하고,
# main.cpp / model/service/svc_tx.cpp 만 하네스로 대체한다 (cmdgen_golden.sh 와 같은 방식).

set -eu
REPO="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:?usage: $0 <OUT_DIR>}"
mkdir -p "$OUT/build"
OUT="$(cd "$OUT" && pwd)"

( cd "$OUT/build" && cmake "$REPO" -DCMAKE_BUILD_TYPE=Release > "$OUT/build.log" 2>&1 \
  && make -j"$(nproc)" >> "$OUT/build.log" 2>&1 ) || { echo "[ops_test] BUILD FAILED (see $OUT/build.log)"; exit 1; }

DIR="$OUT/build/CMakeFiles/BEE-1000.dir"
FLAGS="$(sed -n 's/^CXX_FLAGS = //p' "$DIR/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$DIR/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$DIR/flags.make")"
OBJS=$(find "$DIR/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'svc_tx.cpp.o' | sort)
# 링크 라이브러리: 본 프로그램 link.txt 에서 -o BEE-1000 이후 부분 (빌드 폴더 기준 상대 경로가 있어 빌드 폴더에서 링크)
LIBS="$(sed -e 's/.* -o BEE-1000 //' "$DIR/link.txt")"

echo "[ops_test] compile test"
g++ $FLAGS -w -c "$REPO/tools/ops_test/test_main.cpp" -o "$OUT/test_main.o"
( cd "$OUT/build" && g++ -O3 -rdynamic -Wl,--allow-multiple-definition "$OUT/test_main.o" $OBJS $LIBS -o "$OUT/ops_test" )

echo "[ops_test] run"
mkdir -p "$OUT/tmp"
set +e
( cd "$REPO/bin" && "$OUT/ops_test" "$OUT/tmp" ) > "$OUT/result.txt" 2>&1
RC=$?
set -e
tail -n 1 "$OUT/result.txt"
grep '^FAIL' "$OUT/result.txt" || true
exit $RC
