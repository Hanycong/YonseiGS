#!/bin/bash
# 누리호 5차 수신(비콘/리포트) 비교 테스트: 이 저장소 vs GS_nuri5
#
# 사용법: tools/n5_downlink_test.sh <OUR_BUILD_DIR> <N5_SRC_DIR> <N5_BUILD_DIR> <OUT_DIR>
#   OUR_BUILD_DIR : 이 저장소를 Release 로 빌드한 폴더 (CMakeFiles/BEE-1000.dir 가 있는 곳)
#   N5_SRC_DIR    : GS_nuri5 저장소 (src/miman_coms.cpp)
#   N5_BUILD_DIR  : GS_nuri5 를 Release 로 빌드한 폴더
# 실행 시간: 해석 전수 비교(MID x CC 0..63 x 길이 0..512) 때문에 양쪽 합쳐 약 13 분.
# 결과: OUT_DIR/ours.txt, OUT_DIR/n5.txt. "#### END COMMON" 앞부분이 같아야 하고 (diff 0),
#       ours.txt 끝의 ours-only 검사(GS_nuri5 와 일부러 다르게 고친 부분)가 모두 OK 여야 한다.
set -eu
HERE="$(cd "$(dirname "$0")" && pwd)"
OB="$(cd "${1:?usage}" && pwd)"; N5="$(cd "${2:?usage}" && pwd)"; NB="$(cd "${3:?usage}" && pwd)"
OUT="${4:?usage}"; mkdir -p "$OUT"; OUT="$(cd "$OUT" && pwd)"

flags() { echo "$(sed -n 's/^CXX_FLAGS = //p' "$1/flags.make") $(sed -n 's/^CXX_INCLUDES = //p' "$1/flags.make") $(sed -n 's/^CXX_DEFINES = //p' "$1/flags.make")"; }

# GS_nuri5 소스는 CRLF 라 LF 로 바꾼 사본을 쓴다
mkdir -p "$OUT/n5src"
for f in "$N5"/src/*.h "$N5"/src/*.cpp; do tr -d '\r' < "$f" > "$OUT/n5src/$(basename "$f")"; done

D="$OB/CMakeFiles/BEE-1000.dir"
OBJS=$(find "$D/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'n5_report_logger.cpp.o' ! -name 'n5_beacon.cpp.o' ! -name 'n5_report_assembler.cpp.o' | sort)
g++ $(flags "$D") -w -c "$HERE/n5_downlink_test/ours_main.cpp" -o "$OUT/ours_main.o"
( cd "$OB" && g++ -O2 -Wl,--allow-multiple-definition "$OUT/ours_main.o" $OBJS $(sed -e 's/.* -o BEE-1000 //' "$D/link.txt") -o "$OUT/ours_test" )

D5="$NB/CMakeFiles/BEE-1000.dir"
OBJS5=$(find "$D5/src" -name '*.o' ! -name 'main.cpp.o' ! -name 'miman_coms.cpp.o' | sort)
g++ $(flags "$D5") -I"$OUT/n5src" -w -c "$HERE/n5_downlink_test/n5_main.cpp" -o "$OUT/n5_main.o"
( cd "$NB" && g++ -O2 -Wl,--allow-multiple-definition "$OUT/n5_main.o" $OBJS5 $(sed -e 's/.* -o BEE-1000 //' "$D5/link.txt") -o "$OUT/n5_test" )

# 각자 빈 폴더의 bin/ 에서 실행 (저장 경로가 ../data/...)
rm -rf "$OUT/run_ours" "$OUT/run_n5"; mkdir -p "$OUT/run_ours/bin" "$OUT/run_n5/bin"
set +e
( cd "$OUT/run_ours/bin" && "$OUT/ours_test" ) > "$OUT/ours.txt" 2>/dev/null; RC=$?
( cd "$OUT/run_n5/bin" && "$OUT/n5_test" ) > "$OUT/n5.txt" 2>/dev/null
set -e
sed '/^#### END COMMON/q' "$OUT/ours.txt" > "$OUT/ours_common.txt"
sed '/^#### END COMMON/q' "$OUT/n5.txt" > "$OUT/n5_common.txt"
if diff "$OUT/n5_common.txt" "$OUT/ours_common.txt" > "$OUT/common.diff"; then
    echo "[n5dl] common: same ($(wc -l < "$OUT/ours_common.txt") lines)"
else
    echo "[n5dl] common: DIFFERENT ($(grep -c '^[<>]' "$OUT/common.diff") lines, see $OUT/common.diff)"; RC=1
fi
grep '^check\|^ours-only' "$OUT/ours.txt" || true
exit $RC
