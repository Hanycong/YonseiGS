// View - Beacon 창 "Report" 탭: 5차 위성 앱 리포트 공통 머리/꼬리, GPIO 리포트
//
// GS_nuri5 miman_imgui.cpp Report 탭의 UELYSYS/BEE1012/SD1SAT 앱 리포트 case 의 머리(앱 이름, 크기)와 꼬리([Raw ReturnValue]), GPIO case 를 옮긴 것 (행 이름/형식 그대로).
// 앱별 ReturnValue 해석은 report_uelysys.cpp / report_bee1012.cpp / report_sd1sat.cpp.
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawAppReport(ReportCtx & c)
{
    ReportView_t & view = c.view;

    switch (view.kind)
    {
    case REPORT_KIND_UELYSYS_TTC:
    case REPORT_KIND_UELYSYS_MEOW:
    case REPORT_KIND_UELYSYS_STX:
    case REPORT_KIND_UELYSYS_UTRX:
    case REPORT_KIND_UELYSYS_UANT:
    case REPORT_KIND_UELYSYS_GPIO:
    case REPORT_KIND_UELYSYS_PAYUEL_OBC:
    case REPORT_KIND_UELYSYS_PAYUEL_CAM:
    case REPORT_KIND_UELYSYS_LGBAT:
    case REPORT_KIND_UELYSYS_AOS:
    case REPORT_KIND_UELYSYS_PAYUEL_YONSEI:
    case REPORT_KIND_UELYSYS_CI_LAB:
    case REPORT_KIND_UELYSYS_EO:
    case REPORT_KIND_UELYSYS_GPS:
    case REPORT_KIND_UELYSYS_SCH_LAB:
    case REPORT_KIND_UELYSYS_SP:
    case REPORT_KIND_UELYSYS_TO_LAB:
    case REPORT_KIND_BEE1012_LTRX:
    case REPORT_KIND_BEE1012_KISSCAM:
    case REPORT_KIND_BEE1012_PAY_SLT:
    case REPORT_KIND_BEE1012_THRUST:
    case REPORT_KIND_PAYYONSEI_TEMP:
    case REPORT_KIND_PAYYONSEI_IMU:
    case REPORT_KIND_PAYYONSEI_IWSB:
    case REPORT_KIND_PAYYONSEI_I2C:
    case REPORT_KIND_SDR_CONTROLLER:
    case REPORT_KIND_SD1SAT_CR:
    case REPORT_KIND_SD1SAT_EPS:
    case REPORT_KIND_SD1SAT_GPS:
    case REPORT_KIND_SD1SAT_MEOW:
    case REPORT_KIND_SD1SAT_OPS:
    case REPORT_KIND_SD1SAT_PAYSD1_LOTHAN:
    case REPORT_KIND_SD1SAT_PAYSD1_ROMA:
    case REPORT_KIND_SD1SAT_SNBP:
    case REPORT_KIND_SD1SAT_TTC:
    case REPORT_KIND_SD1SAT_UTRX:
    {
        const uint8_t *p = view.raw_bytes;
        const uint16_t n = view.raw_size;
        const char *app = "UELYSYS";
        if (view.kind == REPORT_KIND_UELYSYS_TTC) app = "TTC";
        else if (view.kind == REPORT_KIND_UELYSYS_MEOW)
            app = (view.reflected_msg_id == BEE1012_MEOW_CMD_ID || view.reflected_msg_id == 0x8218) ? "BEE1012 MEOW" : "UELYSYS MEOW";
        else if (view.kind == REPORT_KIND_UELYSYS_STX) app = "STX";
        else if (view.kind == REPORT_KIND_UELYSYS_UTRX) app = "UTRX";
        else if (view.kind == REPORT_KIND_UELYSYS_UANT) app = "UANT";
        else if (view.kind == REPORT_KIND_UELYSYS_GPIO) app = "GPIO";
        else if (view.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC) app = "PAYUEL OBC";
        else if (view.kind == REPORT_KIND_UELYSYS_PAYUEL_CAM) app = "PAYUEL CAM";
        else if (view.kind == REPORT_KIND_UELYSYS_LGBAT) app = "PAYUEL LGBAT";
        else if (view.kind == REPORT_KIND_UELYSYS_AOS) app = "PAYUEL AOS";
        else if (view.kind == REPORT_KIND_UELYSYS_PAYUEL_YONSEI) app = "PAYUEL YONSEI";
        else if (view.kind == REPORT_KIND_UELYSYS_CI_LAB) app = "CI_LAB";
        else if (view.kind == REPORT_KIND_UELYSYS_EO) app = "EO";
        else if (view.kind == REPORT_KIND_UELYSYS_GPS) app = "GPS";
        else if (view.kind == REPORT_KIND_UELYSYS_SCH_LAB) app = "SCH_LAB";
        else if (view.kind == REPORT_KIND_UELYSYS_SP) app = "SP";
        else if (view.kind == REPORT_KIND_UELYSYS_TO_LAB) app = "TO_LAB";
        else if (view.kind == REPORT_KIND_BEE1012_LTRX) app = "LTRX";
        else if (view.kind == REPORT_KIND_BEE1012_KISSCAM) app = "KISSCAM";
        else if (view.kind == REPORT_KIND_BEE1012_PAY_SLT) app = "PAY_SLT";
        else if (view.kind == REPORT_KIND_BEE1012_THRUST) app = "THRUST";
        else if (view.kind == REPORT_KIND_PAYYONSEI_TEMP) app = "PAYYONSEI TEMP";
        else if (view.kind == REPORT_KIND_PAYYONSEI_IMU) app = "PAYYONSEI IMU";
        else if (view.kind == REPORT_KIND_PAYYONSEI_IWSB) app = "PAYYONSEI IWSB";
        else if (view.kind == REPORT_KIND_PAYYONSEI_I2C) app = "PAYYONSEI I2C";
        else if (view.kind == REPORT_KIND_SDR_CONTROLLER) app = "SDR_CONTROLLER";
        else if (view.kind == REPORT_KIND_SD1SAT_CR) app = "SD1SAT CR";
        else if (view.kind == REPORT_KIND_SD1SAT_EPS) app = "SD1SAT EPS";
        else if (view.kind == REPORT_KIND_SD1SAT_GPS) app = "SD1SAT GPS";
        else if (view.kind == REPORT_KIND_SD1SAT_MEOW) app = "SD1SAT MEOW";
        else if (view.kind == REPORT_KIND_SD1SAT_OPS) app = "SD1SAT OPS";
        else if (view.kind == REPORT_KIND_SD1SAT_PAYSD1_LOTHAN) app = "SD1SAT PAYSD1_LOTHAN";
        else if (view.kind == REPORT_KIND_SD1SAT_PAYSD1_ROMA) app = "SD1SAT PAYSD1_ROMA";
        else if (view.kind == REPORT_KIND_SD1SAT_SNBP) app = "SD1SAT SNBP";
        else if (view.kind == REPORT_KIND_SD1SAT_TTC) app = "SD1SAT TTC";
        else if (view.kind == REPORT_KIND_SD1SAT_UTRX) app = "SD1SAT UTRX";
        RowText("Type", app);
        RowFmt("Parsed payload size", "%" PRIu16, n);
        HeaderRow("[Parsed ReturnValue]");

        // 앱별 해석 (해당 앱 파일이 처리하지 않으면 바이트만)
        if (!DrawUelysysAppBody(c) && !DrawBee1012AppBody(c) && !DrawSd1satAppBody(c))
        {
            RowText("Payload", n ? "FSW-defined binary response; all bytes shown below" : "No return payload");
        }

        HeaderRow("[Raw ReturnValue]");
        if (n) RowArr_Hex("bytes", p, n, 16);
        else RowText("bytes", "empty");
        break;
    }

    case REPORT_KIND_GPIO_DEP1_EN_ON:
    case REPORT_KIND_GPIO_DEP1_EN_OFF:
    case REPORT_KIND_GPIO_DEP2_EN_ON:
    case REPORT_KIND_GPIO_DEP2_EN_OFF:
    case REPORT_KIND_GPIO_SP_IN_READ_5S:
    {
        const char *type = "GPIO Unknown";
        if (view.kind == REPORT_KIND_GPIO_DEP1_EN_ON) type = "GPIO DEP1 EN ON";
        else if (view.kind == REPORT_KIND_GPIO_DEP1_EN_OFF) type = "GPIO DEP1 EN OFF";
        else if (view.kind == REPORT_KIND_GPIO_DEP2_EN_ON) type = "GPIO DEP2 EN ON";
        else if (view.kind == REPORT_KIND_GPIO_DEP2_EN_OFF) type = "GPIO DEP2 EN OFF";
        else if (view.kind == REPORT_KIND_GPIO_SP_IN_READ_5S) type = "GPIO SP IN READ 5S";

        RowText("Type", type);
        HeaderRow("[Report Data]");
        RowText("Payload", view.payload_valid ? "No return payload" : "Unexpected payload (see hex dump below)");
        break;
    }
    default:
        return false;
    }
    return true;
}
