// View - Beacon 창 "Report" 탭: 누리호 5차 위성 리포트 행 도우미와 리포트 종류별 함수
//
// GS_nuri5 miman_imgui.cpp 의 Report 탭 람다(HeaderRow, RowFmt, RowArr_*, ...)를 같은 이름의
// 함수로 옮긴 것. 5차 리포트 파일(report_<위성/앱>.cpp)은 nuri5 의 case 본문을 그대로 쓰므로
// 이름과 출력 형식이 nuri5 와 같아야 한다. 배열 행은 std::string 으로 만들어 잘리지 않는다.
//
// 각 Draw*Report 함수는 c.view.kind 가 자기 종류면 행을 그리고 true, 아니면 아무것도 하지 않고 false.
// 행은 report_tab.cpp 의 표(##ReportTable) 안에 그려진다.
#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "YonseiGS_coms.h"     // ReportView_t, EPS_Report_State_t, ...

// Report 탭이 잠금 아래에서 복사해 온 값들 (프레임마다 새로)
struct ReportCtx
{
    ReportView_t &                        view;
    const EPS_Report_State_t &            eps_state;     // EPS HK 마지막 값 (HK ALL 요약)
    const EPS_RParam_Reassembly_State_t & eps_rparam;    // EPS CC42 RPARAM 조각 재조립 진행
    bool &                                show_hexdump;  // 표 아래 원시 바이트 덤프를 보일지
};

namespace rpt5 {

// 회색 글자의 소제목 줄
inline void HeaderRow(const char* title)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();
    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted("");
}

// 이름 | printf 형식 값
template <typename... Args>
inline void RowFmt(const char* label, const char* fmt, Args... args)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text(fmt, args...);
}

// 이름 | 문자열 그대로
inline void RowText(const char* label, const char* text)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);
    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted(text);
}

// 배열 한 줄: 값을 공백으로 잇고 break_after 개마다 줄바꿈 (0 이면 줄바꿈 없음)
template <typename T>
inline void RowArrFmt(const char* label, const T* aa, int n, int break_after, const char* fmt)
{
    std::string s;
    char tmp[32];
    for (int i = 0; i < n; i++)
    {
        snprintf(tmp, sizeof(tmp), fmt, aa[i]);
        if (i != 0)
        {
            if (break_after > 0 && (i % break_after) == 0) s.push_back('\n');
            else s.push_back(' ');
        }
        s += tmp;
    }
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);
    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted(s.c_str());
}

inline void RowArr_Hex(const char* label, const uint8_t* aa, int n, int break_after)  { RowArrFmt(label, aa, n, break_after, "%02X"); }
inline void RowArr_u8 (const char* label, const uint8_t* aa, int n, int break_after)  { RowArrFmt(label, aa, n, break_after, "%" PRIu8); }
inline void RowArr_i16(const char* label, const int16_t* aa, int n, int break_after)  { RowArrFmt(label, aa, n, break_after, "%" PRIi16); }
inline void RowArr_u16(const char* label, const uint16_t* aa, int n, int break_after) { RowArrFmt(label, aa, n, break_after, "%" PRIu16); }
inline void RowArr_u32(const char* label, const uint32_t* aa, int n, int break_after) { RowArrFmt(label, aa, n, break_after, "%" PRIu32); }

// STX 장치 반환값(esup_ret_t)을 바이트별로 ("FRMAE" 철자는 GS_nuri5 그대로)
inline void RowSTXRet(const esup_ret_t ret)
{
    RowFmt("ENGINE" , "0x%02hhx", (uint8_t)(ESUP_RET_ENGINE(ret) >> 24));
    RowFmt("FRMAE", "0x%02hhx", (uint8_t)(ESUP_RET_FRAME(ret) >> 20));
    RowFmt("WIRE", "0x%02hhx", (uint8_t)(ESUP_RET_WIRE(ret) >> 16));
    RowFmt("LOCAL", "0x%02hhx", (uint8_t)(ESUP_RET_DEVICE_LOCAL(ret) >> 8));
    RowFmt("EXEC", "0x%02hhx", (uint8_t)(ESUP_RET_EXEC_STATUS(ret)));
}

// EPS 질의 리포트의 머리 (source / data_id / 조각 정보)
inline void RowEPSMeta(const EPS_Report_Metadata_t &meta)
{
    HeaderRow("[EPS Query]");
    RowFmt("source", "%" PRIu8, meta.source);
    RowFmt("data_id", "%" PRIu8, meta.data_id);
    RowFmt("total_size", "%" PRIu16, meta.total_size);
    RowFmt("chunk_size", "%" PRIu16, meta.chunk_size);
    RowFmt("sequence", "%" PRIu16, meta.sequence);
    RowFmt("offset", "%" PRIu16, meta.offset);
}

// EPS HK ALL: 장치별 마지막으로 받은 HK 의 주요 값
void RowEPSHKAllSummary(const EPS_Report_State_t & eps_state);

// 8바이트 이름(널 종료 없음)을 문자열로
inline void CopyName8(char (&dst)[9], const char src[8])
{
    memset(dst, 0, 9);
    memcpy(dst, src, 8);
}

} // namespace rpt5

// 리포트 종류별 (report_<파일>.cpp)
bool DrawAppReport(ReportCtx & c);        // report_app.cpp: 5차 위성 앱 리포트 (UELYSYS / BEE1012 / SD1SAT 앱) + GPIO
bool DrawUelysysAppBody(ReportCtx & c);   // report_uelysys.cpp: UELYSYS 앱 ReturnValue 해석
bool DrawBee1012AppBody(ReportCtx & c);   // report_bee1012.cpp: BEE1012 LTRX/KISSCAM/PAY_SLT/THRUST, PAYYONSEI, SDR
bool DrawSd1satAppBody(ReportCtx & c);    // report_sd1sat.cpp: SD1SAT 앱
bool DrawEpsP80Report(ReportCtx & c);     // report_eps_p80.cpp: EPS 질의/HK/HK ALL/BCN/POWER_IF/BP8, EPS COMMAND STATUS
bool DrawAdcsGetReport(ReportCtx & c);    // report_adcs_get.cpp: ADCS GET 계열 (5차 형식)
bool DrawRomaLgpmReport(ReportCtx & c);   // report_roma_lgpm.cpp: PAYUEL ROMA / LGPM
