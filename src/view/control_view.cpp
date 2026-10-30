// View - Control 창 (Pass Operation 탭 왼쪽 열: 위성 전환, 핑, RParam, 명령 생성기 팝업, 캘리브레이션, AMP)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것 (MVVM 1단계: View 추출).
// 다음 단계에서 State. / console / svc 호출 / 명령 큐(SatCMD)를 ControlVM 으로 옮긴다.
#include <inttypes.h>
#include <time.h>
#include <gs/param/rparam.h>
#include <csp/csp.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_csp.h"
#include "svc_tx.h"
#include "svc_radio.h"
#include "control_view.h"

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern char AmpTimeBuf[32];
extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

// 명령 생성기 본체 (아직 miman_imgui.cpp 에 있다. Command 단계에서 옮긴다)
void DrawCmdGeneratorBody(bool initial_mode);

static bool show_rparamWindow         = false;      // rparam 가져오기 custom
static bool show_pingWindow           = false;      // ping 난사
static bool show_tmtcWindow           = false;      // 초기관제
static bool show_initialControlWindow = false;      // 초기관제
static int current_sat_mode = 0; // 0: BEE1000, 1: BEE1012, 2: Uelysys
static bool show_satellite_select = false;
static void PingWindow() {
    static uint8_t node = 0;

    ImGui::InputScalar("node", ImGuiDataType_U8, &node);

    if (ImGui::Button("Send Ping")) {

        mim::svc::WaitTx();

        uint8_t *arg = (uint8_t*)malloc(sizeof(uint8_t));
        *arg = node; 

        mim::svc::StartPingEveryNode((void*)arg);
    }
}

static void HMACWindow()
{
    static int mode_arg;

    ImGui::Text("Select Satellite");

    for (int i = 0; i < 3; i++)
    {
        if (ImGui::RadioButton(mim::svc::SwitchSatelliteName(i), &current_sat_mode, i))
        {
            mode_arg = current_sat_mode;
            mim::svc::SwitchSatellite(&mode_arg);
        }
    }
}

static void RparamWindow()
{
    static uint8_t node = 0;
    static uint8_t table_id = 0;
    static uint16_t addr = 0;
    static int type_index = 0;

    static uint8_t  value_u8 = 0;
    static uint16_t value_u16 = 0;
    static uint32_t value_u32 = 0;
    static int8_t   value_s8 = 0;
    static int16_t  value_s16 = 0;
    static int32_t  value_s32 = 0;
    static float    value_float = 0;
    // static char     value_char = 0;

    const char* type_names[] = { "uint8", "uint16", "uint32", "int8", "int16", "int32", "float"};

    ImGui::Text("Node");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_node", ImGuiDataType_U8, &node);

    ImGui::Text("Table ID");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_table", ImGuiDataType_U8, &table_id);


    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));
    ImGui::Text("!!!WARNING!!! Your Addr must be decimal, not hex");
    ImGui::PopStyleColor();

    ImGui::Text("Address");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_addr", ImGuiDataType_U16, &addr);



    ImGui::Text("Type");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::BeginCombo("##rparam_type", type_names[type_index]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(type_names); ++i)
        {
            bool is_selected = (type_index == i);
            if (ImGui::Selectable(type_names[i], is_selected))
                type_index = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (type_index == 0)
    {
        ImGui::Text("Value (uint8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u8", ImGuiDataType_U8, &value_u8, NULL, NULL, "%u");
    }
    else if (type_index == 1)
    {
        ImGui::Text("Value (uint16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u16", ImGuiDataType_U16, &value_u16, NULL, NULL, "%u");
    }
    else if (type_index == 2)
    {
        ImGui::Text("Value (uint32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u32", ImGuiDataType_U32, &value_u32, NULL, NULL, "%u");
    }
        else if (type_index == 3)
    {
        ImGui::Text("Value (int8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s8", ImGuiDataType_S8, &value_s8, NULL, NULL, "%u");
    }
        else if (type_index == 4)
    {
        ImGui::Text("Value (int16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s16", ImGuiDataType_S16, &value_s16, NULL, NULL, "%u");
    }
        else if (type_index == 5)
    {
        ImGui::Text("Value (int32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s32", ImGuiDataType_S32, &value_s32, NULL, NULL, "%u");
    }
        else if (type_index == 6)
    {
        ImGui::Text("Value (float)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_float", ImGuiDataType_Float, &value_float, NULL, NULL, "%u");
    }


    float w = ImGui::GetContentRegionAvail().x;
    float h = ImGui::GetFontSize() * 1.5f;
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 215, 0, 255));
    ImGui::Text("Please Try: (get AX100 active_conf)");

    ImGui::Text("node = 5, table_id = 4, address = 24");
    ImGui::Text("Type: uint8");

    ImGui::PopStyleColor();
    if (ImGui::Button("Load##rparam_load", ImVec2(w * 0.5f, h)))
    {
        if (type_index == 0)
        {
            // eps 안불러와지면 0x0BB0 시도!
            if (gs_rparam_get_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u8) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 1)
        {
            if (gs_rparam_get_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u16) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 2)
        {
            if (gs_rparam_get_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u32) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 3)
        {
            if (gs_rparam_get_int8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s8) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi8, node, table_id, addr, value_s8);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 4)
        {
            if (gs_rparam_get_int16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s16) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi16, node, table_id, addr, value_s16);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 5)
        {
            if (gs_rparam_get_int32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s32) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi32, node, table_id, addr, value_s32);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 6)
        {
            if (gs_rparam_get_float(node, table_id, addr, 0xB00B, setup->default_timeout, &value_float) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%f", node, table_id, addr, value_float);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }




    }

    ImGui::SameLine();

    if (ImGui::Button("Save##rparam_save", ImVec2(w * 0.5f, h)))
    {
        if (type_index == 0)
        {
            if (gs_rparam_set_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, value_u8) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
        else if (type_index == 1)
        {
            if (gs_rparam_set_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, value_u16) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
        else if (type_index == 2)
        {
            if (gs_rparam_set_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, value_u32) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
    }
}

void ImGui_ControlWindow(float fontscale)
{
    ImGui::Begin("Control GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_ControlWindow_Body(fontscale);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void ImGui_ControlWindow_Body(float fontscale)
{

    ImGuiStyle& style = ImGui::GetStyle();
    float buttonHeight = ImGui::GetFontSize() * 1.5f;

    float rows = 3.0f;
    float pingRegionHeight =
        buttonHeight * rows
        + style.FramePadding.y * 2.5f
        + style.ItemSpacing.y * (rows + 2);

    ImGui::BeginChild("PingRegion",
                      ImVec2(ImGui::GetContentRegionAvail().x, pingRegionHeight),
                      true,
                      mim_winflags | ImGuiWindowFlags_NoScrollbar);

    float regionW = ImGui::GetContentRegionAvail().x;
    float eachW   = (regionW - style.ItemSpacing.x) * 0.5f; 

    if (ImGui::Button("Switch Satellite", ImVec2(eachW, buttonHeight)))
    {
        show_satellite_select = true;
    }

    if (ImGui::Button("Ping(AX100)", ImVec2(eachW, buttonHeight)))
    {
        mim::svc::WaitTx();
        mim::svc::StartPingConsole();
    }

    ImGui::SameLine();
    if (ImGui::Button("Ping(OBC)", ImVec2(eachW, buttonHeight)))
    {
        mim::svc::WaitTx();
        FSWTle* MIM_TLEinfo = (FSWTle*)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
        int NowPingInfo = GetNowTracking();

        if (NowPingInfo < -1)
        {
            console.AddLog("[ERROR]##You Tried Ping without Tracking. Please Check Again.");
        }
        else if (NowPingInfo == -1)
        {
            FSWTleConverter(State.Fatellites->tle, MIM_TLEinfo);
            packetsign* TestPacket = PingInit(MIM_TLEinfo);
            mim::svc::SendUplink((void*)TestPacket);
        }
        else
        {
            FSWTleConverter(State.Satellites[NowPingInfo]->tle, MIM_TLEinfo);
            packetsign* TestPacket = PingInit(MIM_TLEinfo);
            mim::svc::SendUplink((void*)TestPacket);
        }
    }

    if (ImGui::Button("Ping each node", ImVec2(eachW, buttonHeight)))
    {
        show_pingWindow = true;
    }


    ImGui::SameLine();
    if (ImGui::Button("Custom RParam Get", ImVec2(eachW, buttonHeight)))
    {
        show_rparamWindow = true;
    }

    if (ImGui::Button("Send Telecommand", ImVec2(eachW, buttonHeight)))
    {
        show_tmtcWindow = true;
        csp_rtable_print();
    }

    ImGui::SameLine();
    if (ImGui::Button("Initial Ops", ImVec2(eachW, buttonHeight)))
    {
        show_initialControlWindow = true;
    }

    ImGui::EndChild();

    ImGui::SetWindowFontScale(1.0f);



    if (ImGui::Button("Baud Rate Calibration",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        mim::svc::StartBaudCalibration();
    }

    if (ImGui::Button("Frequency Calibration",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        mim::svc::StartFreqCalibration();
    }

    ImGui::SameLine();
    if (!State.AMPON)
    {
        if (State.AmpTime > 0)
        {
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "Disabled  %M:%S", State.AmpTM);
            if (ImGui::Button(AmpTimeBuf,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                mim::svc::AmpOff();
            }
        }
        else
        {
            sprintf(AmpTimeBuf, "AMP ON");
            if (ImGui::Button(AmpTimeBuf,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                mim::svc::AmpOn();
            }
        }
    }
    else
    {
        if (State.AmpTime > 0)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "AMP OFF %M:%S", State.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP OFF");

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button(AmpTimeBuf,
                          ImVec2(ImGui::GetContentRegionAvail().x,
                                 ImGui::GetFontSize() * 1.5f)))
        {
            mim::svc::AmpOff();
        }
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("CMD Manager",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        State.Display_CMD = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear",
                      ImVec2(ImGui::GetContentRegionAvail().x,
                             ImGui::GetFontSize() * 1.5f)))
    {
        NowCMD = 0;
        for (int i = 0; i < 256; i++)
        {
            if (SatCMD[i] != NULL)
            {
                delete SatCMD[i];
                SatCMD[i] = NULL;
            }
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 popup_size(600.0f, 380.0f);
    ImVec2 popup_pos((io.DisplaySize.x - popup_size.x) * 0.5f,
                     (io.DisplaySize.y - popup_size.y) * 0.5f);

    if (show_tmtcWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("TMTC Command Test##msgid_fnccode_TypeWindow", &show_tmtcWindow);
        DrawCmdGeneratorBody(false);
        ImGui::End();
    }

    if (show_initialControlWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Initial Operations##INITIAL_CMD_WINDOW", &show_initialControlWindow);
        DrawCmdGeneratorBody(true);
        ImGui::End();
    }

    if (show_pingWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Ping each node##PING_WINDOW", &show_pingWindow);
        PingWindow();
        ImGui::End();
    }

    if (show_rparamWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Get Custom Rparam##RPARAM_WINDOW", &show_rparamWindow);
        RparamWindow();
        ImGui::End();
    }

    if (show_satellite_select)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Switch HMAC Key##SATELLITE_SELECT", &show_satellite_select);
        HMACWindow();
        ImGui::End();
    }
}

