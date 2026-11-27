// View - 명령 생성기 입력 폼: BEE1012 stx (명령 번호 2600, 2601, 2602, 2603, 2604, 2605, 2610, 2611, 2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2620, 2630, 2631, 2632, 2633, 2634, 2635, 2636, 2637, 2638, 2639, 2640, 2650, 2651, 2652, 2653, 2654, 2655, 2656, 2657, 2658, 2659, 2662, 2663, 2664, 2665)
//
// 누리호 5차 GS(GS_nuri5) DrawCmdGeneratorBody 의 case 를 내용 그대로 옮긴 것.
// 송신은 svc_tx(WaitTx/SendUplink), TC 패킷 기록은 업링크 스레드가 한다. 처리한 명령이면 true.
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <string.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "YonseiGS_config.h"
#include "mission_defs.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"
#include "telecommand_sender.h"
#include "ccsds_cmd.h"
#include "sat_mode.h"
#include "cmdgen5_common.h"
#include "cmdgen_forms.h"
#include "noarg_form.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

// 인자 없는 명령 (머리 8바이트뿐): { 명령 번호, 기본 MID, 기본 CC, 위젯 ID 꼬리표 }. 폼은 noarg_form 이 그린다
static mim::ui::NoArgCommandForm kNoArgForms[] = {
    {  2600, UELYSYS_STX_CMD_ID, UELYSYS_STX_NOOP_CC,                 "##direct_2600" },
    {  2601, 0x1854,             1,                                   "##direct_2601" },   // stx RESET_COUNTERS
    {  2602, UELYSYS_STX_CMD_ID, UELYSYS_STX_INITIALIZE_CC,           "##direct_2602" },   // stx INITIALIZE
    {  2604, UELYSYS_STX_CMD_ID, UELYSYS_STX_PARAM_INIT_CC,           "##direct_2604" },   // stx PARAM_INIT
    {  2605, UELYSYS_STX_CMD_ID, UELYSYS_STX_GET_FILEHANDLE_CC,       "##direct_2605" },   // stx GET_FILEHANDLE
    {  2630, 0x1854,             30,                                  "##direct_2630" },   // stx GET_SYMBOL_RATE
    {  2631, 0x1854,             31,                                  "##direct_2631" },   // stx GET_TX_POWER
    {  2632, 0x1854,             32,                                  "##direct_2632" },   // stx GET_CENTER_FREQ
    {  2633, 0x1854,             33,                                  "##direct_2633" },   // stx GET_MODCOD
    {  2634, 0x1854,             34,                                  "##direct_2634" },   // stx GET_ROLL_OFF
    {  2635, 0x1854,             35,                                  "##direct_2635" },   // stx GET_PILOT_SIGNAL
    {  2636, 0x1854,             36,                                  "##direct_2636" },   // stx GET_FEC_FRAME_SIZE
    {  2637, 0x1854,             37,                                  "##direct_2637" },   // stx GET_PRETX_DELAY
    {  2638, UELYSYS_STX_CMD_ID, UELYSYS_STX_GET_ALL_PRAMETERS_CC,    "##direct_2638" },
    {  2639, 0x1854,             39,                                  "##direct_2639" },   // stx GET_REPORT
    {  2640, 0x1854,             40,                                  "##direct_2640" },   // stx GET_MODULATOR_DATA_INTERFACE
    {  2650, UELYSYS_STX_CMD_ID, UELYSYS_STX_FILESYS_CC_DIR,          "##direct_2650" },
    {  2651, 0x1854,             51,                                  "##direct_2651" },   // stx FILESYS_CC_DIRNEXT
    {  2653, 0x1854,             53,                                  "##direct_2653" },   // stx FILESYS_CC_DELALLFILE
    {  2657, 0x1854,             57,                                  "##direct_2657" },   // stx FILESYS_CC_READFILE
    {  2662, UELYSYS_STX_CMD_ID, UELYSYS_STX_SYSCONF_CC_TRANSMITMODE, "##direct_2662" },
    {  2663, UELYSYS_STX_CMD_ID, UELYSYS_STX_SYSCONF_CC_IDLEMODE,     "##direct_2663" },
    {  2665, 0x1854,             65,                                  "##direct_2665" },   // stx SYSCONF_CC_SAFESHUTDOWN
};

bool DrawBee1012StxForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 2603: {
            UELYSYS_STX_SetModuleIdCmd_t &cmd = command5->uelysys_stx_setmoduleidcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_STX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_STX_SET_MODULE_ID_CC);
            ImGui::InputScalar("msgid##direct_2603", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2603", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ModuleId", ImGuiDataType_U16, &cmd.ModuleId);
            if (ImGui::Button("Generate CMD##direct_2603")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2610: { // stx SET_SYMBOLRATE
            static uint16_t msgid=0x1854; static uint8_t fnccode=10;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_SYMBOLRATE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2610",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2610",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2610",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2610")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2611: { // stx SET_TRANSMITPW
            static uint16_t msgid=0x1854; static uint8_t fnccode=11;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_TRANSMITPW payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2611",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2611",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2611",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2611")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2612: { // stx SET_CENTERFREQ
            static uint16_t msgid=0x1854; static uint8_t fnccode=12;
            struct Payload { float data; };
            static_assert(sizeof(Payload)==4,"stx SET_CENTERFREQ payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2612",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2612",ImGuiDataType_U8,&fnccode);
            ImGui::InputFloat("data##direct_2612",&payload.data);
            if(ImGui::Button("Generate CMD##direct_2612")){ WriteSystemName(msgid); uint8_t cmd[12]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2613: { // stx SET_MODCOD
            static uint16_t msgid=0x1854; static uint8_t fnccode=13;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_MODCOD payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2613",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2613",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2613",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2613")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2614: { // stx SET_ROLLOFF
            static uint16_t msgid=0x1854; static uint8_t fnccode=14;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_ROLLOFF payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2614",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2614",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2614",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2614")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2615: { // stx SET_PILOTSIG
            static uint16_t msgid=0x1854; static uint8_t fnccode=15;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_PILOTSIG payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2615",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2615",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2615",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2615")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2616: { // stx SET_FECFRAMESZ
            static uint16_t msgid=0x1854; static uint8_t fnccode=16;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_FECFRAMESZ payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2616",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2616",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2616",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2616")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2617: { // stx SET_PRETX_DELAY
            static uint16_t msgid=0x1854; static uint8_t fnccode=17;
            struct Payload { uint16_t data; };
            static_assert(sizeof(Payload)==2,"stx SET_PRETX_DELAY payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2617",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2617",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2617",ImGuiDataType_U16,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2617")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2618: { // stx SET_ALL_PRAMETERS
            static uint16_t msgid=0x1854; static uint8_t fnccode=18;
            struct __attribute__((packed)) Payload { uint8_t symbol_rate; uint8_t transmit_power; uint8_t modcod; uint8_t roll_off; uint8_t pilot_signal; uint8_t fec_frame_size; uint16_t pretransmission_delay; float center_frequency; };
            static_assert(sizeof(Payload)==12,"stx SET_ALL_PRAMETERS payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2618",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2618",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("symbol_rate##direct_2618",ImGuiDataType_U8,&payload.symbol_rate);
            ImGui::InputScalar("transmit_power##direct_2618",ImGuiDataType_U8,&payload.transmit_power);
            ImGui::InputScalar("modcod##direct_2618",ImGuiDataType_U8,&payload.modcod);
            ImGui::InputScalar("roll_off##direct_2618",ImGuiDataType_U8,&payload.roll_off);
            ImGui::InputScalar("pilot_signal##direct_2618",ImGuiDataType_U8,&payload.pilot_signal);
            ImGui::InputScalar("fec_frame_size##direct_2618",ImGuiDataType_U8,&payload.fec_frame_size);
            ImGui::InputScalar("pretransmission_delay##direct_2618",ImGuiDataType_U16,&payload.pretransmission_delay);
            ImGui::InputFloat("center_frequency##direct_2618",&payload.center_frequency);
            if(ImGui::Button("Generate CMD##direct_2618")){ WriteSystemName(msgid); uint8_t cmd[20]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2619: { // stx SET_RS485BAUD
            static uint16_t msgid=0x1854; static uint8_t fnccode=19;
            struct Payload { uint8_t data; };
            static_assert(sizeof(Payload)==1,"stx SET_RS485BAUD payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2619",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2619",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("data##direct_2619",ImGuiDataType_U8,&payload.data);
            if(ImGui::Button("Generate CMD##direct_2619")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2620: { // stx SET_MODULATOR_DATA_INTERFACE
            static uint16_t msgid=0x1854; static uint8_t fnccode=20;
            struct Payload { uint8_t modulator_interface_type; uint8_t lvds_io_type; };
            static_assert(sizeof(Payload)==2,"stx SET_MODULATOR_DATA_INTERFACE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2620",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2620",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("modulator_interface_type##direct_2620",ImGuiDataType_U8,&payload.modulator_interface_type);
            ImGui::InputScalar("lvds_io_type##direct_2620",ImGuiDataType_U8,&payload.lvds_io_type);
            if(ImGui::Button("Generate CMD##direct_2620")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }


        case 2652: { // stx FILESYS_CC_DELFILE
            static uint16_t msgid=0x1854; static uint8_t fnccode=52;
            struct Payload { char filename_max[30]; int8_t filename_len; };
            static_assert(sizeof(Payload)==31,"stx FILESYS_CC_DELFILE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2652",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2652",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("filename_max##direct_2652",payload.filename_max,sizeof(payload.filename_max));
            ImGui::InputScalar("filename_len##direct_2652",ImGuiDataType_S8,&payload.filename_len);
            if(ImGui::Button("Generate CMD##direct_2652")){ WriteSystemName(msgid); uint8_t cmd[39]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2654: {
            UELYSYS_STX_CREATEFILE_t &cmd = command5->uelysys_stx_createfilecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_STX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_STX_FILESYS_CC_CREATEFILE);
            ImGui::InputScalar("msgid##direct_2654", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2654", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("filename_max", cmd.filename_max, sizeof(cmd.filename_max));
            ImGui::InputScalar("filename_len", ImGuiDataType_S8, &cmd.filename_len);
            ImGui::InputScalar("file_size", ImGuiDataType_U32, &cmd.file_size);
            if (ImGui::Button("Generate CMD##direct_2654")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2655: {
            UELYSYS_STX_WRITEFILE_t &cmd = command5->uelysys_stx_writefilecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_STX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_STX_FILESYS_CC_WRITEFILE);
            ImGui::InputScalar("msgid##direct_2655", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2655", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("filename", cmd.filename, sizeof(cmd.filename));
            ImGui::InputScalar("size", ImGuiDataType_U32, &cmd.size);
            ImGui::InputScalar("offset", ImGuiDataType_U32, &cmd.offset);
            ImGui::InputScalar("interpacket_delay", ImGuiDataType_U8, &cmd.interpacket_delay);
            if (ImGui::Button("Generate CMD##direct_2655")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2656: { // stx FILESYS_CC_OPENFILE
            static uint16_t msgid=0x1854; static uint8_t fnccode=56;
            struct __attribute__((packed)) Payload { char filename_max[30]; int8_t filename_len; };
            static_assert(sizeof(Payload)==31,"stx FILESYS_CC_OPENFILE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2656",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2656",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("filename_max##direct_2656",payload.filename_max,sizeof(payload.filename_max));
            ImGui::InputScalar("filename_len##direct_2656",ImGuiDataType_S8,&payload.filename_len);
            if(ImGui::Button("Generate CMD##direct_2656")){ WriteSystemName(msgid); uint8_t cmd[39]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2658: {
            UELYSYS_STX_SENDFILE_t &cmd = command5->uelysys_stx_sendfilecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_STX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_STX_FILESYS_CC_SENDFILE);
            ImGui::InputScalar("msgid##direct_2658", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2658", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("filename_max", cmd.filename_max, sizeof(cmd.filename_max));
            ImGui::InputScalar("filename_len", ImGuiDataType_S8, &cmd.filename_len);
            if (ImGui::Button("Generate CMD##direct_2658")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2659: { // stx FILESYS_CC_SENDFILERTI
            static uint16_t msgid=0x1854; static uint8_t fnccode=59;
            struct Payload { char filename_max[30]; int8_t filename_len; };
            static_assert(sizeof(Payload)==31,"stx FILESYS_CC_SENDFILERTI payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2659",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2659",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("filename_max##direct_2659",payload.filename_max,sizeof(payload.filename_max));
            ImGui::InputScalar("filename_len##direct_2659",ImGuiDataType_S8,&payload.filename_len);
            if(ImGui::Button("Generate CMD##direct_2659")){ WriteSystemName(msgid); uint8_t cmd[39]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }


        case 2664: { // stx SYSCONF_CC_UPDATEFW
            static uint16_t msgid=0x1854; static uint8_t fnccode=(uint8_t)(UELYSYS_STX_SYSCONF_CC_UPDATEFW);
            struct __attribute__((packed)) Payload { char filename[30]; int8_t filename_len; };
            static_assert(sizeof(Payload)==31,"stx SYSCONF_CC_UPDATEFW payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2664",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2664",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("filename##direct_2664",payload.filename,sizeof(payload.filename));
            ImGui::InputScalar("filename_len##direct_2664",ImGuiDataType_S8,&payload.filename_len);
            if(ImGui::Button("Generate CMD##direct_2664")){ WriteSystemName(msgid); uint8_t cmd[39]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        // ==================== adcs ====================
    default:
        return false;
    }
    return true;
}
