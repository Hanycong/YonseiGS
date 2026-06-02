/* ============================================================
 *  miman_satcontext.h
 *
 *  활성 위성 컨텍스트 관리 — Phase 1
 *  ------------------------------------------------------------
 *  • StateCheckUnit에 추가된 ActiveSatIndex / VisibleSats 를
 *    안전하게 다루는 헬퍼.
 *  • 기존 GetNowTracking() / SetNowTracking()은 그대로 유지.
 *    (cFS-orbit code가 광범위 사용 중)
 *  • 위성 탭 UI는 VisibleSats[]만 참조.
 *  • 활성 위성 전환 시 NowTracking과 자동 동기화.
 * ============================================================ */
#pragma once
#ifndef __MIMAN_SATCONTEXT_H_
#define __MIMAN_SATCONTEXT_H_

#include "miman_config.h"

namespace mim {

constexpr int VISIBLE_SAT_MAX = 8;   /* 위성 탭 최대 개수 */

/* =========================================================
 *  Active Satellite
 * ========================================================= */

/* 현재 활성화된 위성 인덱스(State.Satellites[N]의 N).
 * - 위성 탭에서 선택된 위성.
 * - 콘솔/Command/Beacon 윈도우가 참조.                          */
int  GetActiveSat(void);

/* 활성 위성 변경. NowTracking과 함께 갱신.
 * sat_idx가 유효하지 않으면 무시(현재 값 유지).                   */
void SetActiveSat(int sat_idx);

/* =========================================================
 *  Visible Satellites (탭으로 표시되는 위성들)
 * ========================================================= */

/* 가시 위성 슬롯 개수 (탭으로 표시되는 위성 수). */
int  GetVisibleSatCount(void);

/* slot(0..N-1) 위치의 위성 인덱스.
 * 반환 -1: 빈 슬롯.                                              */
int  GetVisibleSatAt(int slot);

/* slot의 컬러 (mim::SAT_COLORS의 슬롯 인덱스로 결정).
 * 위성 위치(슬롯)가 바뀌어도 같은 색으로 보이려면 SatColorByName 사용 권장.
 * 단순 구현은 슬롯 인덱스 기반.                                   */
ImVec4 GetVisibleSatColor(int slot);

/* sat_idx가 가시 슬롯에 있는가? */
bool IsSatVisible(int sat_idx);

/* sat_idx의 슬롯 인덱스. 없으면 -1. */
int  FindVisibleSlot(int sat_idx);

/* sat_idx를 가시 슬롯에 추가. 이미 있으면 그 슬롯 반환.
 * 슬롯이 다 찬 경우 -1 반환.                                       */
int  AddVisibleSat(int sat_idx);

/* 슬롯에서 위성 제거 (마지막 위성은 제거 불가, -1 반환).
 * 활성 위성을 제거한 경우 인접 슬롯으로 자동 이동.                 */
int  RemoveVisibleSat(int slot);

/* 슬롯 순서 바꾸기 (드래그앤드롭 reorder용) */
void MoveVisibleSat(int from_slot, int to_slot);

/* =========================================================
 *  Initialization
 *  main()의 setup phase에서 호출. State.Satellites[]를 훑어서
 *  use=true 인 위성 중 처음 1개를 활성 + 슬롯 0에 배치.
 * ========================================================= */
void Initialize_SatContext(void);

} // namespace mim

#endif /* __MIMAN_SATCONTEXT_H_ */
