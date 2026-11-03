// View - 명령 생성기 입력 폼: BEE1012 pay_slt (명령 번호 2300, 2301, 2302, 2303, 2304, 2305, 2306, 2307, 2308, 2309, 2310, 2311)
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
#include "sat_mode.h"
#include "cmdgen5_common.h"
#include "cmdgen_forms.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1012PaySltForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 2300: {
            static BEE1012_PAY_SLT_NoArgsCmd_t cmd = {};
            static uint16_t msgid = (uint16_t)(BEE1012_PAY_SLT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_PAY_SLT_NOOP_CC);
            ImGui::InputScalar("msgid##direct_2300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2300")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(BEE1012_PAY_SLT_NoArgsCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(BEE1012_PAY_SLT_NoArgsCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(BEE1012_PAY_SLT_NoArgsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(BEE1012_PAY_SLT_NoArgsCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(BEE1012_PAY_SLT_NoArgsCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2301: { // PAY_SLT RESET_COUNTERS (CC 1)
            static BEE1012_PAY_SLT_NoArgsCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##pay_slt_610", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_610", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##pay_slt_610")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2302: { // PAY_SLT REPORT_BCN (CC 2)
            static BEE1012_PAY_SLT_NoArgsCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_REPORT_BCN_CC;
            ImGui::InputScalar("msgid##pay_slt_611", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_611", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##pay_slt_611")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2303: { // PAY_SLT OUTPUT_ENABLED (CC 3)
            static BEE1012_PAY_SLT_OutputEnabledCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_OUTPUT_ENABLED_CC;
            static int hk_enabled = 0;
            static int bcn_enabled = 0;
            static int ifb_enabled = 0;
            static int exp_enabled = 0;
            const char *enabled_items[] = {"0 - DISABLED", "1 - ENABLED"};
            ImGui::InputScalar("msgid##pay_slt_612", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_612", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("HK Output##pay_slt_612", &hk_enabled, enabled_items,
                         IM_ARRAYSIZE(enabled_items));
            ImGui::Combo("Beacon Output##pay_slt_612", &bcn_enabled, enabled_items,
                         IM_ARRAYSIZE(enabled_items));
            ImGui::Combo("PAY-IFB Communication##pay_slt_612", &ifb_enabled, enabled_items,
                         IM_ARRAYSIZE(enabled_items));
            ImGui::Combo("PAY-EXP Communication##pay_slt_612", &exp_enabled, enabled_items,
                         IM_ARRAYSIZE(enabled_items));
            if (ImGui::Button("Generate CMD##pay_slt_612")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                const BEE1012_PAY_SLT_OutputEnabled_Payload_t payload = {
                    (uint8_t)hk_enabled, (uint8_t)bcn_enabled,
                    (uint8_t)ifb_enabled, (uint8_t)exp_enabled};
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2304: { // PAY_SLT RS422_PING (CC 4)
            static BEE1012_PAY_SLT_NoArgsCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_RS422_PING_CC;
            ImGui::InputScalar("msgid##pay_slt_613", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_613", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##pay_slt_613")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2305: { // PAY_SLT PAR_GET (CC 5)
            static BEE1012_PAY_SLT_ParGetCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_PAR_GET_CC;
            static int node_index = 0;
            static uint8_t table = 0;
            static uint16_t addr = 0;
            static int type_index = 0;
            static uint8_t len_value = 1;
            const char *node_items[] = {
                "11 - PAY-EXP-A7", "12 - PAY-EXP-M7", "13 - PAY-IFB"};
            const char *type_items[] = {
                "UINT8 (0)", "UINT16 (1)", "UINT32 (2)", "INT8 (4)",
                "INT16 (5)", "FLOAT (13)", "STRING (14)"};
            const uint8_t type_values[] = {0, 1, 2, 4, 5, 13, 14};
            ImGui::InputScalar("msgid##pay_slt_614", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_614", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("Node##pay_slt_614", &node_index, node_items,
                         IM_ARRAYSIZE(node_items));
            ImGui::InputScalar("UINT8 Table (0..7)##pay_slt_614", ImGuiDataType_U8, &table);
            ImGui::InputScalar("UINT16 Address##pay_slt_614", ImGuiDataType_U16, &addr);
            ImGui::Combo("Parameter Type##pay_slt_614", &type_index, type_items,
                         IM_ARRAYSIZE(type_items));
            const uint8_t selected_type = type_values[type_index];
            const uint8_t max_len = selected_type == 14 ? 178
                                    : (selected_type == 0 || selected_type == 4) ? 60
                                    : (selected_type == 1 || selected_type == 5) ? 45
                                                                                : 30;
            char length_label[64];
            snprintf(length_label, sizeof(length_label),
                     selected_type == 14
                         ? "UINT8 String Length (1..%u)##pay_slt_614"
                         : "UINT8 Element Count (1..%u)##pay_slt_614",
                     max_len);
            if (len_value < 1) len_value = 1;
            if (len_value > max_len) len_value = max_len;
            if (ImGui::InputScalar(length_label, ImGuiDataType_U8, &len_value)) {
                if (len_value < 1) len_value = 1;
                if (len_value > max_len) len_value = max_len;
            }
            if (ImGui::Button("Generate CMD##pay_slt_614")) {
                if (table > 7) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_GET table=%u. Valid range: 0-7", table);
                    break;
                }
                if (len_value < 1 || len_value > max_len) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_GET length=%u. Valid range: 1-%u",
                                   len_value, max_len);
                    break;
                }
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                const BEE1012_PAY_SLT_ParGet_Payload_t payload = {
                    (uint8_t)(11 + node_index), table, addr, selected_type, len_value
                };
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2306: { // PAY_SLT PAR_SET (CC 6)
            static BEE1012_PAY_SLT_ParSetCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_PAR_SET_CC;
            static int node_index = 0;
            static uint8_t table = 0;
            static uint16_t addr = 0;
            static int type_index = 0;
            static uint8_t value_u8 = 0;
            static uint16_t value_u16 = 0;
            static uint32_t value_u32 = 0;
            static char string_value[96] = "";
            const char *node_items[] = {
                "11 - PAY-EXP-A7", "12 - PAY-EXP-M7", "13 - PAY-IFB"};
            const char *type_items[] = {
                "UINT8 (0)", "UINT16 (1)", "UINT32 (2)", "STRING (14)"};
            const uint8_t type_values[] = {0, 1, 2, 14};
            ImGui::InputScalar("msgid##pay_slt_615", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_615", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("Node##pay_slt_615", &node_index, node_items,
                         IM_ARRAYSIZE(node_items));
            ImGui::InputScalar("UINT8 Table (0..7)##pay_slt_615", ImGuiDataType_U8, &table);
            ImGui::InputScalar("UINT16 Address##pay_slt_615", ImGuiDataType_U16, &addr);
            ImGui::Combo("Parameter Type##pay_slt_615", &type_index, type_items,
                         IM_ARRAYSIZE(type_items));
            const uint8_t selected_type = type_values[type_index];
            if (selected_type == 0) {
                ImGui::InputScalar("UINT8 Value##pay_slt_615", ImGuiDataType_U8, &value_u8);
            }
            else if (selected_type == 1) {
                ImGui::InputScalar("UINT16 Value##pay_slt_615", ImGuiDataType_U16, &value_u16);
            }
            else if (selected_type == 2) {
                ImGui::InputScalar("UINT32 Value##pay_slt_615", ImGuiDataType_U32, &value_u32);
            }
            else {
                ImGui::InputText("STRING Value (max 95 bytes)##pay_slt_615",
                                 string_value, sizeof(string_value));
            }
            if (ImGui::Button("Generate CMD##pay_slt_615")) {
                if (table > 7) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_SET table=%u. Valid range: 0-7", table);
                    break;
                }
                const size_t string_length = strnlen(string_value, sizeof(string_value));
                if (selected_type == 14 && string_length >= sizeof(string_value)) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_SET string exceeds 95 bytes.");
                    break;
                }
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                BEE1012_PAY_SLT_ParSet_Payload_t payload = {};
                payload.node = (uint8_t)(11 + node_index);
                payload.table = table;
                payload.addr = addr;
                payload.type = selected_type;
                if (selected_type == 14) {
                    memcpy(payload.data.str, string_value, string_length + 1);
                }
                else if (selected_type == 0) {
                    payload.data.value = value_u8;
                }
                else if (selected_type == 1) {
                    payload.data.value = value_u16;
                }
                else {
                    payload.data.value = value_u32;
                }
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2307: { // PAY_SLT SCAN_FILES (CC 7)
            static BEE1012_PAY_SLT_ScanFilesCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_SCAN_FILES_CC;
            static int node_index = 0;
            const char *node_items[] = {"11 - PAY-EXP-A7", "12 - PAY-EXP-M7"};
            ImGui::InputScalar("msgid##pay_slt_616", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_616", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("Node##pay_slt_616", &node_index, node_items,
                         IM_ARRAYSIZE(node_items));
            if (ImGui::Button("Generate CMD##pay_slt_616")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                cmd.Payload.node = (uint8_t)(11 + node_index);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2308: { // PAY_SLT DOWNLOAD_FILE_I2C (CC 8)
            static BEE1012_PAY_SLT_DownloadFileCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC;
            static uint32_t file_index = 0;
            static uint32_t start_chunk = 0;
            ImGui::InputScalar("msgid##pay_slt_617", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_617", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("UINT32 File Index##pay_slt_617", ImGuiDataType_U32, &file_index);
            ImGui::InputScalar("UINT32 Start Chunk##pay_slt_617", ImGuiDataType_U32, &start_chunk);
            if (ImGui::Button("Generate CMD##pay_slt_617")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                const BEE1012_PAY_SLT_DownloadFile_Payload_t payload = {file_index, start_chunk};
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2309: { // PAY_SLT DOWNLOAD_FILE_RS422 (CC 9)
            static BEE1012_PAY_SLT_DownloadFileCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC;
            static uint32_t file_index = 0;
            static uint32_t start_chunk = 0;
            ImGui::InputScalar("msgid##pay_slt_618", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_618", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("UINT32 File Index##pay_slt_618", ImGuiDataType_U32, &file_index);
            ImGui::InputScalar("UINT32 Start Chunk##pay_slt_618", ImGuiDataType_U32, &start_chunk);
            if (ImGui::Button("Generate CMD##pay_slt_618")) {
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                const BEE1012_PAY_SLT_DownloadFile_Payload_t payload = {file_index, start_chunk};
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2310: { // PAY_SLT GET_FULL_TABLE (CC 10)
            static BEE1012_PAY_SLT_GetFullTableCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_GET_FULL_TABLE_CC;
            static int node_index = 0;
            static uint8_t table = 0;
            const char *node_items[] = {
                "11 - PAY-EXP-A7", "12 - PAY-EXP-M7", "13 - PAY-IFB"};
            ImGui::InputScalar("msgid##pay_slt_619", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_619", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("Node##pay_slt_619", &node_index, node_items,
                         IM_ARRAYSIZE(node_items));
            ImGui::InputScalar("UINT8 Table (0..7)##pay_slt_619", ImGuiDataType_U8, &table);
            if (ImGui::Button("Generate CMD##pay_slt_619")) {
                if (table > 7) {
                    console.AddLog("[ERROR]##PAY_SLT GET_FULL_TABLE table=%u. Valid range: 0-7", table);
                    break;
                }
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                const BEE1012_PAY_SLT_GetFullTable_Payload_t payload = {
                    (uint8_t)(11 + node_index), table};
                memcpy(&cmd.Payload, &payload, sizeof(payload));
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2311: { // PAY_SLT PAR_SET_ARRAY (CC 11)
            static BEE1012_PAY_SLT_ParSetArrayCmd_t cmd = {};
            static uint16_t msgid = BEE1012_PAY_SLT_CMD_ID;
            static uint8_t fnccode = BEE1012_PAY_SLT_PAR_SET_ARRAY_CC;
            static int node_index = 0;
            static uint8_t table = 0;
            static uint16_t addr = 0;
            static int type_index = 0;
            static uint8_t len_value = 1;
            static uint8_t values_u8[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN] = {};
            static uint16_t values_u16[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN] = {};
            static uint32_t values_u32[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN] = {};
            const char *node_items[] = {
                "11 - PAY-EXP-A7", "12 - PAY-EXP-M7", "13 - PAY-IFB"};
            const char *type_items[] = {"UINT8 (0)", "UINT16 (1)", "UINT32 (2)"};
            const uint8_t type_values[] = {0, 1, 2};

            ImGui::InputScalar("msgid##pay_slt_620", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##pay_slt_620", ImGuiDataType_U8, &fnccode);
            ImGui::Combo("Node##pay_slt_620", &node_index, node_items,
                         IM_ARRAYSIZE(node_items));
            ImGui::InputScalar("UINT8 Table (0..7)##pay_slt_620", ImGuiDataType_U8, &table);
            ImGui::InputScalar("UINT16 Start Address##pay_slt_620", ImGuiDataType_U16, &addr);
            ImGui::Combo("Parameter Type##pay_slt_620", &type_index, type_items,
                         IM_ARRAYSIZE(type_items));
            if (ImGui::InputScalar("Number of Values (1..8)##pay_slt_620",
                                   ImGuiDataType_U8, &len_value)) {
                if (len_value < 1) len_value = 1;
                if (len_value > BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN)
                    len_value = BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN;
            }
            const uint8_t selected_type = type_values[type_index];
            const size_t visible_count =
                len_value <= BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN
                    ? len_value
                    : BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN;
            for (size_t i = 0; i < visible_count; ++i) {
                char label[48];
                if (selected_type == 0) {
                    snprintf(label, sizeof(label), "UINT8 Value %zu##pay_slt_620", i);
                    ImGui::InputScalar(label, ImGuiDataType_U8, &values_u8[i]);
                }
                else if (selected_type == 1) {
                    snprintf(label, sizeof(label), "UINT16 Value %zu##pay_slt_620", i);
                    ImGui::InputScalar(label, ImGuiDataType_U16, &values_u16[i]);
                }
                else {
                    snprintf(label, sizeof(label), "UINT32 Value %zu##pay_slt_620", i);
                    ImGui::InputScalar(label, ImGuiDataType_U32, &values_u32[i]);
                }
            }

            if (ImGui::Button("Generate CMD##pay_slt_620")) {
                if (table > 7) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_SET_ARRAY table=%u. Valid range: 0-7", table);
                    break;
                }
                if (len_value < 1 || len_value > BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN) {
                    console.AddLog("[ERROR]##PAY_SLT PAR_SET_ARRAY count=%u. Valid range: 1-%u",
                                   len_value, BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN);
                    break;
                }
                WriteSystemName(msgid);
                memset(&cmd, 0, sizeof(cmd));
                cmd.Payload.node = (uint8_t)(11 + node_index);
                cmd.Payload.table = table;
                cmd.Payload.addr = addr;
                cmd.Payload.type = selected_type;
                cmd.Payload.len = len_value;
                if (selected_type == 0) {
                    memcpy(cmd.Payload.data.u8, values_u8,
                           (size_t)len_value * sizeof(values_u8[0]));
                }
                else if (selected_type == 1) {
                    memcpy(cmd.Payload.data.u16, values_u16,
                           (size_t)len_value * sizeof(values_u16[0]));
                }
                else {
                    memcpy(cmd.Payload.data.u32, values_u32,
                           (size_t)len_value * sizeof(values_u32[0]));
                }

                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                uint8_t len[2] = {
                    (uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(cmd);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        // ==================== paybee_kisscam ====================
    default:
        return false;
    }
    return true;
}
