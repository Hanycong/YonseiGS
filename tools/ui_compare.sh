#!/bin/bash
# 두 ui_snapshot 결과(A, B)를 비교한다. 리팩토링 전후 A/B 검증용.
#
# 사용법: tools/ui_compare.sh <A_DIR> <B_DIR>
#   - 콘솔 로그: 시각을 지우고 정렬 + 중복 제거 후 비교
#     (스레드끼리 출력 순서, "Cannot Read N parameters", "Rotator Read Failed" 반복 횟수는
#      원본 코드에서도 실행마다 달라서 횟수가 아니라 나온 메시지 종류로 비교한다)
#   - 표준출력: 정렬 + 스레드 주소 제거 후 비교
#   - 스크린샷: 하단 콘솔(시간 표시)을 뺀 위쪽 840px 의 다른 픽셀 수 (0 이어야 함)
A=$1; B=$2
norm(){ sed -E 's/[0-9]{2}:[0-9]{2}:[0-9]{2}/T/g; s/[0-9]{4}-[0-9]{2}-[0-9]{2}/D/g' "$@"; }
norm2(){ norm "$@" | sed -E "s/[0-9]{2}:[0-9]{2}/T/g; s/Cannot Read [0-9]+ parameters/Cannot Read N parameters/" | sort -u; }
echo "== console log diff (정렬+중복 제거: 스레드 출력 순서·반복 횟수는 원래 비결정적)"; diff <(norm2 $A/applogs/console--*.log) <(norm2 $B/applogs/console--*.log) && echo same
echo "== stdout diff (sorted, thread ids removed)"; diff <(norm $A/run.log | sed -E 's/0x[0-9a-f]+/X/g' | sort) <(norm $B/run.log | sed -E 's/0x[0-9a-f]+/X/g' | sort) | head -20 && echo same
for f in 0_setup 1_pre_pass 1a_frequency 1b_pre_pass_started 1c_pre_pass_stopped 2_pass_operation 3_ftp_upload; do
  # 하단 콘솔(시간 표시) 영역 제외 비교: 상단 840px 만
  convert $A/$f.png -crop 1920x840+0+0 +repage $A/.c_$f.png
  convert $B/$f.png -crop 1920x840+0+0 +repage $B/.c_$f.png
  n=$(compare -metric AE $A/.c_$f.png $B/.c_$f.png $B/diff_$f.png 2>&1)
  echo "== $f: differing pixels (top 840px) = $n"
done
