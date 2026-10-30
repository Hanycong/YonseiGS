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
