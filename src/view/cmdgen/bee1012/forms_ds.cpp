// View - 명령 생성기 입력 폼: BEE1012 ds (명령 번호 1700, 1701, 1702, 1703, 1704, 1705, 1706, 1707, 1708, 1709, 1710, 1711, 1712, 1713, 1714, 1715, 1716, 1717, 1718)
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
    {  1700, BEE1012_DS_CMD_ID, BEE1012_DS_NOOP_CC,           "##direct_1700" },
    {  1701, BEE1012_DS_CMD_ID, BEE1012_DS_RESET_COUNTERS_CC, "##direct_1701" },   // DS RESET_COUNTERS
    {  1715, BEE1012_DS_CMD_ID, BEE1012_DS_GET_FILE_INFO_CC,  "##direct_1715" },   // DS GET_FILE_INFO
    {  1718, BEE1012_DS_CMD_ID, 18,                           "##direct_1718" },   // ds CLOSD_ALL
};

bool DrawBee1012DsForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 1702: { // ds SET_APP_STATE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 2;
        struct Payload {
            uint16_t EnableState;
            uint16_t Padding;
        };
        static_assert(sizeof(Payload) == 4, "ds SET_APP_STATE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1702", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1702", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EnableState##direct_1702", ImGuiDataType_U16, &payload.EnableState);
            if (ImGui::Button("Generate CMD##direct_1702")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1703: { // ds SET_FILTER_FILE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 3;
        struct Payload {
            uint32_t MessageID;
            uint16_t FilterParmsIndex;
            uint16_t FileTableIndex;
        };
        static_assert(sizeof(Payload) == 8, "ds SET_FILTER_FILE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1703", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1703", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MessageID##direct_1703", ImGuiDataType_U32, &payload.MessageID);
            ImGui::InputScalar("FilterParmsIndex##direct_1703", ImGuiDataType_U16, &payload.FilterParmsIndex);
            ImGui::InputScalar("FileTableIndex##direct_1703", ImGuiDataType_U16, &payload.FileTableIndex);
            if (ImGui::Button("Generate CMD##direct_1703")) {
                WriteSystemName(msgid);
                uint8_t cmd[16] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1704: { // ds SET_FILTER_TYPE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 4;
        struct Payload {
            uint32_t MessageID;
            uint16_t FilterParmsIndex;
            uint16_t FilterType;
        };
        static_assert(sizeof(Payload) == 8, "ds SET_FILTER_TYPE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1704", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1704", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MessageID##direct_1704", ImGuiDataType_U32, &payload.MessageID);
            ImGui::InputScalar("FilterParmsIndex##direct_1704", ImGuiDataType_U16, &payload.FilterParmsIndex);
            ImGui::InputScalar("FilterType##direct_1704", ImGuiDataType_U16, &payload.FilterType);
            if (ImGui::Button("Generate CMD##direct_1704")) {
                WriteSystemName(msgid);
                uint8_t cmd[16] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1705: { // ds SET_FILTER_PARMS
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 5;
        struct Payload {
            uint32_t MessageID;
            uint16_t FilterParmsIndex;
            uint16_t Algorithm_N;
            uint16_t Algorithm_X;
            uint16_t Algorithm_O;
        };
        static_assert(sizeof(Payload) == 12, "ds SET_FILTER_PARMS payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1705", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1705", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MessageID##direct_1705", ImGuiDataType_U32, &payload.MessageID);
            ImGui::InputScalar("FilterParmsIndex##direct_1705", ImGuiDataType_U16, &payload.FilterParmsIndex);
            ImGui::InputScalar("Algorithm_N##direct_1705", ImGuiDataType_U16, &payload.Algorithm_N);
            ImGui::InputScalar("Algorithm_X##direct_1705", ImGuiDataType_U16, &payload.Algorithm_X);
            ImGui::InputScalar("Algorithm_O##direct_1705", ImGuiDataType_U16, &payload.Algorithm_O);
            if (ImGui::Button("Generate CMD##direct_1705")) {
                WriteSystemName(msgid);
                uint8_t cmd[20] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1706: { // ds SET_DEST_TYPE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 6;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t FileNameType;
        };
        static_assert(sizeof(Payload) == 4, "ds SET_DEST_TYPE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1706", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1706", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1706", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputScalar("FileNameType##direct_1706", ImGuiDataType_U16, &payload.FileNameType);
            if (ImGui::Button("Generate CMD##direct_1706")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1707: { // ds SET_DEST_STATE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 7;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t EnableState;
        };
        static_assert(sizeof(Payload) == 4, "ds SET_DEST_STATE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1707", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1707", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1707", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputScalar("EnableState##direct_1707", ImGuiDataType_U16, &payload.EnableState);
            if (ImGui::Button("Generate CMD##direct_1707")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1708: { // ds SET_DEST_PATH
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 8;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            char Pathname[64];
        };
        static_assert(sizeof(Payload) == 68, "ds SET_DEST_PATH payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1708", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1708", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1708", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputText("Pathname##direct_1708", payload.Pathname, sizeof(payload.Pathname));
            if (ImGui::Button("Generate CMD##direct_1708")) {
                WriteSystemName(msgid);
                uint8_t cmd[76] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1709: { // ds SET_DEST_BASE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 9;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            char Basename[64];
        };
        static_assert(sizeof(Payload) == 68, "ds SET_DEST_BASE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1709", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1709", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1709", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputText("Basename##direct_1709", payload.Basename, sizeof(payload.Basename));
            if (ImGui::Button("Generate CMD##direct_1709")) {
                WriteSystemName(msgid);
                uint8_t cmd[76] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1710: { // ds SET_DEST_EXT
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 10;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            char Extension[8];
        };
        static_assert(sizeof(Payload) == 12, "ds SET_DEST_EXT payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1710", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1710", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1710", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputText("Extension##direct_1710", payload.Extension, sizeof(payload.Extension));
            if (ImGui::Button("Generate CMD##direct_1710")) {
                WriteSystemName(msgid);
                uint8_t cmd[20] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1711: { // ds SET_DEST_SIZE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 11;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            uint32_t MaxFileSize;
        };
        static_assert(sizeof(Payload) == 8, "ds SET_DEST_SIZE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1711", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1711", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1711", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputScalar("MaxFileSize##direct_1711", ImGuiDataType_U32, &payload.MaxFileSize);
            if (ImGui::Button("Generate CMD##direct_1711")) {
                WriteSystemName(msgid);
                uint8_t cmd[16] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1712: { // ds SET_DEST_AGE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 12;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            uint32_t MaxFileAge;
        };
        static_assert(sizeof(Payload) == 8, "ds SET_DEST_AGE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1712", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1712", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1712", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputScalar("MaxFileAge##direct_1712", ImGuiDataType_U32, &payload.MaxFileAge);
            if (ImGui::Button("Generate CMD##direct_1712")) {
                WriteSystemName(msgid);
                uint8_t cmd[16] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1713: { // ds SET_DEST_COUNT
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 13;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
            uint32_t SequenceCount;
        };
        static_assert(sizeof(Payload) == 8, "ds SET_DEST_COUNT payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1713", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1713", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1713", ImGuiDataType_U16, &payload.FileTableIndex);
            ImGui::InputScalar("SequenceCount##direct_1713", ImGuiDataType_U32, &payload.SequenceCount);
            if (ImGui::Button("Generate CMD##direct_1713")) {
                WriteSystemName(msgid);
                uint8_t cmd[16] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1714: { // ds CLOSE_FILE
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 14;
        struct Payload {
            uint16_t FileTableIndex;
            uint16_t Padding;
        };
        static_assert(sizeof(Payload) == 4, "ds CLOSE_FILE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1714", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1714", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FileTableIndex##direct_1714", ImGuiDataType_U16, &payload.FileTableIndex);
            if (ImGui::Button("Generate CMD##direct_1714")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1716: { // ds ADD_MID
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 16;
        struct Payload {
            uint32_t MessageID;
        };
        static_assert(sizeof(Payload) == 4, "ds ADD_MID payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_1716", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1716", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MessageID##direct_1716", ImGuiDataType_U32, &payload.MessageID);
            if (ImGui::Button("Generate CMD##direct_1716")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1717: { // DS REMOVE_MID
            static uint16_t msgid = 0x1813;
            static uint8_t fnccode = 17;
            struct Payload {
                uint32_t MessageID;
            };
            static_assert(sizeof(Payload) == 4, "ds REMOVE_MID payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1718", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1718", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MessageID##direct_1718", ImGuiDataType_U32, &payload.MessageID);
            if (ImGui::Button("Generate CMD##direct_1718")) {
                WriteSystemName(msgid);
                uint8_t cmd[12] = {0};
                memcpy(cmd+8,&payload,sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }


        // ==================== fm ====================
    default:
        return false;
    }
    return true;
}
