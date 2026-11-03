// View - 명령 생성기 입력 폼 (FSW 앱별 파일)
//
// 명령 번호(gen_fnccode)마다 입력 위젯과 "Generate CMD" 처리를 그린다. 처리한 번호면 true.
// 파일 위치: view/cmdgen/raw/ (플랫폼 공통 원시 명령), view/cmdgen/<위성>/forms_<앱>.cpp
// 명령 번호 -> (위성, 앱) 은 명령 카탈로그(model/missions/*)와 같다.
#pragma once

#include <stdint.h>

bool DrawRawForms(uint16_t fnccode);                            // RAW RAW
bool DrawBee1000EpsForms(uint16_t fnccode);                     // BEE1000 EPS
bool DrawBee1000AdcsForms(uint16_t fnccode);                    // BEE1000 ADCS
bool DrawBee1000ScForms(uint16_t fnccode);                      // BEE1000 SC
bool DrawBee1000ToLabForms(uint16_t fnccode);                   // BEE1000 TO_LAB
bool DrawBee1000UantForms(uint16_t fnccode);                    // BEE1000 UANT
bool DrawBee1000SpForms(uint16_t fnccode);                      // BEE1000 SP
bool DrawBee1000PayuzucForms(uint16_t fnccode);                 // BEE1000 PAYUZUC
bool DrawBee1000FmForms(uint16_t fnccode);                      // BEE1000 FM
bool DrawCosmicFtpForms(uint16_t fnccode);                      // COSMIC FTP
bool DrawBee1000UnknownForms(uint16_t fnccode);                 // BEE1000 UNKNOWN
bool DrawBee1000UtrxForms(uint16_t fnccode);                    // BEE1000 UTRX
bool DrawCosmicToLabForms(uint16_t fnccode);                    // COSMIC TO_LAB
bool DrawCosmicEpsForms(uint16_t fnccode);                      // COSMIC EPS
bool DrawCosmicSpForms(uint16_t fnccode);                       // COSMIC SP
bool DrawCosmicUelPiForms(uint16_t fnccode);                    // COSMIC UEL_PI
bool DrawBee1000EsForms(uint16_t fnccode);                      // BEE1000 ES

// ---- 누리호 5차 위성 (BEE1012 / UELYSYS / SD1-SAT). 위성 모드가 BEE1000 이 아닐 때만 부른다 ----
// 명령 번호는 GS_nuri5 와 같다 (0~9 공용 원시 명령은 5차 판: 목적 노드 지정, FC/체크섬 위치 수정)
bool DrawBee1012AiobcForms(uint16_t fnccode);                   // BEE1012 aiobc
bool DrawGeneric5Forms(uint16_t fnccode);                       // GENERIC generic
bool DrawBee1012CiLabForms(uint16_t fnccode);                   // BEE1012 ci_lab
bool DrawBee1012DsForms(uint16_t fnccode);                      // BEE1012 ds
bool DrawBee1012FmForms(uint16_t fnccode);                      // BEE1012 fm
bool DrawBee1012HkForms(uint16_t fnccode);                      // BEE1012 hk
bool DrawBee1012ToLabForms(uint16_t fnccode);                   // BEE1012 to_lab
bool DrawBee1012RptForms(uint16_t fnccode);                     // BEE1012 rpt
bool DrawBee1012GpioForms(uint16_t fnccode);                    // BEE1012 gpio
bool DrawBee1012PaySltForms(uint16_t fnccode);                  // BEE1012 pay_slt
bool DrawBee1012PaybeeKisscamForms(uint16_t fnccode);           // BEE1012 paybee_kisscam
bool DrawBee1012UtrxForms(uint16_t fnccode);                    // BEE1012 utrx
bool DrawBee1012StxForms(uint16_t fnccode);                     // BEE1012 stx
bool DrawBee1012AdcsForms(uint16_t fnccode);                    // BEE1012 adcs
bool DrawBee1012EpsForms(uint16_t fnccode);                     // BEE1012 eps
bool DrawBee1012TtcForms(uint16_t fnccode);                     // BEE1012 ttc
bool DrawBee1012MeowForms(uint16_t fnccode);                    // BEE1012 meow
bool DrawBee1012MissionForms(uint16_t fnccode);                 // BEE1012 mission
bool DrawBee1012GpsForms(uint16_t fnccode);                     // BEE1012 gps
bool DrawBee1012LtrxForms(uint16_t fnccode);                    // BEE1012 ltrx
bool DrawBee1012SchLabForms(uint16_t fnccode);                  // BEE1012 sch_lab
bool DrawUelysysEoForms(uint16_t fnccode);                      // UELYSYS eo
bool DrawUelysysPayuelAosForms(uint16_t fnccode);               // UELYSYS payuel_aos
bool DrawUelysysPayuelLgbatForms(uint16_t fnccode);             // UELYSYS payuel_lgbat
bool DrawUelysysPayuelYonseiForms(uint16_t fnccode);            // UELYSYS payuel_yonsei
bool DrawUelysysPayuelRomaForms(uint16_t fnccode);              // UELYSYS payuel_roma
bool DrawUelysysUantForms(uint16_t fnccode);                    // UELYSYS uant
bool DrawUelysysGpioForms(uint16_t fnccode);                    // UELYSYS gpio
bool DrawUelysysSchLabForms(uint16_t fnccode);                  // UELYSYS sch_lab
bool DrawUelysysSpForms(uint16_t fnccode);                      // UELYSYS sp
bool DrawUelysysStxForms(uint16_t fnccode);                     // UELYSYS stx
bool DrawUelysysTtcForms(uint16_t fnccode);                     // UELYSYS ttc
bool DrawUelysysPayuelObcForms(uint16_t fnccode);               // UELYSYS payuel_obc
bool DrawUelysysPayuelCamForms(uint16_t fnccode);               // UELYSYS payuel_cam
bool DrawUelysysPayuelLgpmForms(uint16_t fnccode);              // UELYSYS payuel_lgpm
bool DrawSd1SatCiForms(uint16_t fnccode);                       // SD1SAT ci
bool DrawSd1SatCrForms(uint16_t fnccode);                       // SD1SAT cr
bool DrawSd1SatOpsForms(uint16_t fnccode);                      // SD1SAT ops
bool DrawUelysysMeowForms(uint16_t fnccode);                    // UELYSYS MEOW
bool DrawSd1SatUantForms(uint16_t fnccode);                     // SD1SAT uant
bool DrawSd1SatUtrxForms(uint16_t fnccode);                     // SD1SAT utrx
bool DrawSd1SatToForms(uint16_t fnccode);                       // SD1SAT to
bool DrawSd1SatEpsForms(uint16_t fnccode);                      // SD1SAT eps
bool DrawSd1SatPaysd1RomaForms(uint16_t fnccode);               // SD1SAT paysd1_roma
bool DrawSd1SatPaysd1LothanForms(uint16_t fnccode);             // SD1SAT paysd1_lothan
bool DrawSd1SatSnbpForms(uint16_t fnccode);                     // SD1SAT SNBP
bool DrawSd1SatGpsForms(uint16_t fnccode);                      // SD1SAT GPS
bool DrawSd1SatFmForms(uint16_t fnccode);                       // SD1SAT FM
