// View - 명령 생성기 입력 폼: BEE1012 aiobc (명령 번호 1000, 1001, 1002, 1007, 1003, 1004, 1005, 1006)
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

bool DrawBee1012AiobcForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 1000: { // AIOBC HEALTH CHECK (CC 1)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 1;
            ImGui::InputScalar("msgid##aiobc_health", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_health", ImGuiDataType_U8, &fnccode);

            if (ImGui::Button("Generate CMD##aiobc_health")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[5] = 0x01;
                cmd[6] = fnccode;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC health-check packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1001: { // AIOBC MISSION CLEAR (CC 3)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 3;
            ImGui::InputScalar("msgid##aiobc_mission_clear", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_mission_clear", ImGuiDataType_U8, &fnccode);

            if (ImGui::Button("Generate CMD##aiobc_mission_clear")) {
                WriteSystemName(msgid);
                AIOBC_HealthChckCmd_t &cmd = command5->aiobchealthchckcmd;
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                memcpy(cmd.CmdHeader, &mid, sizeof(mid));
                cmd.CmdHeader[2] = 0xC0;
                cmd.CmdHeader[4] = (uint8_t)(wire_length >> 8);
                cmd.CmdHeader[5] = (uint8_t)wire_length;
                cmd.CmdHeader[6] = fnccode;
                uint8_t checksum = 0xFF;
                const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&cmd);
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= bytes[i];
                cmd.CmdHeader[7] = checksum;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC mission-clear packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1002: { // AIOBC DATA CHUNK (CC 5)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 5;
            static uint16_t envelope_magic = 0x465A;
            static uint16_t flags = 0;
            static uint16_t raw_data_length = sizeof(float) * 34;
            static uint16_t chunk_seq = 0;
            static uint16_t total_chunks = 0;
            static uint32_t chunk_crc32 = 0;
            static char path[100] = {0};
            static uint8_t intercmd_delay = 1;

            ImGui::InputScalar("msgid##aiobc_datachunk", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_datachunk", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EnvelopeMagic##aiobc_datachunk", ImGuiDataType_U16, &envelope_magic);
            ImGui::InputScalar("flags##aiobc_datachunk", ImGuiDataType_U16, &flags);
            ImGui::InputScalar("RawDataLength##aiobc_datachunk", ImGuiDataType_U16, &raw_data_length);
            ImGui::InputScalar("Chunk_seq##aiobc_datachunk", ImGuiDataType_U16, &chunk_seq);
            ImGui::InputScalar("Tot_chunks##aiobc_datachunk", ImGuiDataType_U16, &total_chunks);
            ImGui::Text("ChunkCRC32 = 0x%08X", chunk_crc32);
            ImGui::InputText("data file path##aiobc_datachunk", path, sizeof(path));
            ImGui::InputScalar("InterCommand Delay##aiobc_datachunk", ImGuiDataType_U8, &intercmd_delay);

            if (ImGui::Button("Generate CMD##aiobc_datachunk")) {
                vector<vector<float>> data = AIOBC_datachunk_parser(path);
                if (data.empty()) {
                    console.AddLog("[ERROR]##AIOBC data is empty.");
                    break;
                }
                if (raw_data_length > sizeof(command5->aiobcdatachunkcmd.Data)) {
                    console.AddLog("[ERROR]##AIOBC DataChunk RawDataLength too large: input=%u max=%zu",
                                   raw_data_length, sizeof(command5->aiobcdatachunkcmd.Data));
                    break;
                }

                WriteSystemName(msgid);
                const uint16_t actual_total_chunks = total_chunks ? total_chunks : (uint16_t)data.size();
                for (size_t row = 0; row < data.size(); ++row) {
                    if (data[row].size() < 34) {
                        console.AddLog("[ERROR]##AIOBC row %zu data size insufficient (%zu/34)",
                                       row, data[row].size());
                        break;
                    }

                    AIOBC_DataChunkCmd_t &cmd = command5->aiobcdatachunkcmd;
                    if (!BuildAIOBCDataChunkCommand(
                            cmd, msgid, fnccode, envelope_magic, flags,
                            raw_data_length, (uint16_t)(chunk_seq + row),
                            actual_total_chunks, data[row], chunk_crc32)) {
                        console.AddLog("[ERROR]##Failed to build AIOBC data-chunk row %zu.", row);
                        break;
                    }

                    packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                    if (!pkt) {
                        console.AddLog("[ERROR]##Failed to allocate AIOBC data-chunk packet.");
                        break;
                    }
                    pkt->Identifier = HVD_TEST;
                    pkt->PacketType = B12_UL_AIOBC;
                    pkt->Length = (uint16_t)sizeof(cmd);
                    memcpy(pkt->Data, &cmd, sizeof(cmd));
                    mim::svc::WaitTx();
                    mim::svc::SendUplink((void *)pkt);
                    console.AddLog("[AIOBC]##DataChunk generated [%zu/%u]. Seq=%u CRC32=0x%08X",
                                   row + 1, actual_total_chunks, (uint16_t)(chunk_seq + row), chunk_crc32);
                    sleep(intercmd_delay);
                }
            }
            break;
        }

        case 1007: { // AIOBC SINGLE DATA CHUNK (CC 5)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 5;
            static uint16_t envelope_magic = 0x465A;
            static uint16_t flags = 0;
            static uint16_t raw_data_length = sizeof(float) * 34;
            static uint16_t chunk_seq = 0;
            static uint16_t total_chunks = 0;
            static uint32_t chunk_crc32 = 0;
            static char path[100] = {0};

            ImGui::InputScalar("msgid##aiobc_single_datachunk", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_single_datachunk", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EnvelopeMagic##aiobc_single_datachunk", ImGuiDataType_U16,
                               &envelope_magic);
            ImGui::InputScalar("flags##aiobc_single_datachunk", ImGuiDataType_U16, &flags);
            ImGui::InputScalar("RawDataLength##aiobc_single_datachunk", ImGuiDataType_U16,
                               &raw_data_length);
            ImGui::InputScalar("Chunk_seq##aiobc_single_datachunk", ImGuiDataType_U16,
                               &chunk_seq);
            ImGui::InputScalar("Tot_chunks##aiobc_single_datachunk", ImGuiDataType_U16,
                               &total_chunks);
            ImGui::Text("ChunkCRC32 = 0x%08X", chunk_crc32);
            ImGui::InputText("data file path##aiobc_single_datachunk", path, sizeof(path));

            if (ImGui::Button("Generate CMD##aiobc_single_datachunk")) {
                vector<vector<float>> data = AIOBC_datachunk_parser(path);
                if (data.empty()) {
                    console.AddLog("[ERROR]##AIOBC data is empty.");
                    break;
                }
                if (raw_data_length > sizeof(command5->aiobcdatachunkcmd.Data)) {
                    console.AddLog(
                        "[ERROR]##AIOBC Single DataChunk RawDataLength too large: input=%u max=%zu",
                        raw_data_length, sizeof(command5->aiobcdatachunkcmd.Data));
                    break;
                }
                if ((size_t)chunk_seq >= data.size()) {
                    console.AddLog("[ERROR]##Invalid AIOBC chunk_seq=%u. Valid range: 0-%zu",
                                   chunk_seq, data.size() - 1);
                    break;
                }
                if (data[chunk_seq].size() < 34) {
                    console.AddLog("[ERROR]##AIOBC row %u data size insufficient (%zu/34)",
                                   chunk_seq, data[chunk_seq].size());
                    break;
                }

                WriteSystemName(msgid);
                AIOBC_DataChunkCmd_t &cmd = command5->aiobcdatachunkcmd;
                if (!BuildAIOBCDataChunkCommand(
                        cmd, msgid, fnccode, envelope_magic, flags,
                        raw_data_length, chunk_seq, total_chunks,
                        data[chunk_seq], chunk_crc32)) {
                    console.AddLog("[ERROR]##Failed to build AIOBC single data-chunk %u.",
                                   chunk_seq);
                    break;
                }

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC single data-chunk packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
                console.AddLog("[AIOBC]##Single DataChunk generated. Seq=%u Total=%u CRC32=0x%08X",
                               chunk_seq, total_chunks, chunk_crc32);
            }
            break;
        }

        case 1003: // AIOBC DATA END (CC 7)
        case 1004: { // AIOBC REPORT CHECK (CC 9)
            static uint16_t msgid = 0x18F0;
            const bool data_end = gen_fnccode == 1003;
            static uint8_t data_end_fnccode = 7;
            static uint8_t report_fnccode = 9;
            uint8_t &fnccode = data_end ? data_end_fnccode : report_fnccode;
            ImGui::InputScalar(data_end ? "msgid##aiobc_data_end" : "msgid##aiobc_report_check",
                               ImGuiDataType_U16, &msgid);
            ImGui::InputScalar(data_end ? "fnccode##aiobc_data_end" : "fnccode##aiobc_report_check",
                               ImGuiDataType_U8, &fnccode);

            if (ImGui::Button(data_end ? "Generate CMD##aiobc_data_end" : "Generate CMD##aiobc_report_check")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[5] = 0x01;
                cmd[6] = fnccode;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC no-argument packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1005: { // AIOBC FDIR REQUEST (CC 14)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 14;
            static uint32_t scenarioid = 0;
            static uint8_t commandid = 14;
            static uint32_t flags = 0;
            ImGui::InputScalar("msgid##aiobc_fdir", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_fdir", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("scenarioid##aiobc_fdir", ImGuiDataType_U32, &scenarioid);
            ImGui::InputScalar("commandid##aiobc_fdir", ImGuiDataType_U8, &commandid);
            ImGui::InputScalar("flags##aiobc_fdir", ImGuiDataType_U32, &flags);

            if (ImGui::Button("Generate CMD##aiobc_fdir")) {
                WriteSystemName(msgid);
                AIOBC_FDIRCmd_t &cmd = command5->aiobcfdircmd;
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                memcpy(cmd.CmdHeader, &mid, sizeof(mid));
                cmd.CmdHeader[2] = 0xC0;
                cmd.CmdHeader[4] = (uint8_t)(wire_length >> 8);
                cmd.CmdHeader[5] = (uint8_t)wire_length;
                cmd.CmdHeader[6] = fnccode;
                cmd.scenarioid = csp_hton32(scenarioid);
                cmd.commandid = commandid;
                cmd.flags = csp_hton32(flags);
                uint8_t checksum = 0xFF;
                const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&cmd);
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= bytes[i];
                cmd.CmdHeader[7] = checksum;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC FDIR packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1006: { // AIOBC AI MODE (CC 5)
            static uint16_t msgid = 0x18F0;
            static uint8_t fnccode = 5;
            static uint16_t envelope_magic = 0x465A;
            static uint16_t flags = 1;
            static uint16_t chunk_seq = 0;
            static uint16_t total_chunks = 1;
            static uint16_t raw_data_length = sizeof(uint32_t);
            static uint32_t scenario_id = 1;
            static uint32_t chunk_crc32 = 0;
            ImGui::InputScalar("msgid##aiobc_aimode", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##aiobc_aimode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EnvelopeMagic##aiobc_aimode", ImGuiDataType_U16, &envelope_magic);
            ImGui::InputScalar("flags##aiobc_aimode", ImGuiDataType_U16, &flags);
            ImGui::InputScalar("Chunk_seq##aiobc_aimode", ImGuiDataType_U16, &chunk_seq);
            ImGui::InputScalar("Tot_chunks##aiobc_aimode", ImGuiDataType_U16, &total_chunks);
            ImGui::InputScalar("RawDataLength##aiobc_aimode", ImGuiDataType_U16, &raw_data_length);
            ImGui::InputScalar("Scenario ID##aiobc_aimode", ImGuiDataType_U32, &scenario_id);
            ImGui::Text("ChunkCRC32 = 0x%08X", chunk_crc32);

            if (ImGui::Button("Generate CMD##aiobc_aimode")) {
                if (raw_data_length > sizeof(scenario_id)) {
                    console.AddLog("[ERROR]##AIOBC AI Mode RawDataLength too large: input=%u max=%zu",
                                   raw_data_length, sizeof(scenario_id));
                    break;
                }
                WriteSystemName(msgid);
                AIOBC_AIModeCmd_t &cmd = command5->aiobcaimodecmd;
                memset(&cmd, 0, sizeof(cmd));
                uint16_t mid = htons(msgid);
                uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                memcpy(cmd.CmdHeader, &mid, sizeof(mid));
                cmd.CmdHeader[2] = 0xC0;
                cmd.CmdHeader[4] = (uint8_t)(wire_length >> 8);
                cmd.CmdHeader[5] = (uint8_t)wire_length;
                cmd.CmdHeader[6] = fnccode;
                cmd.EnvelopeMagic = csp_hton16(envelope_magic);
                cmd.flags = csp_hton16(flags);
                cmd.chunk_seq = csp_hton16(chunk_seq);
                cmd.total_chunks = csp_hton16(total_chunks);
                cmd.RawDataLength = csp_hton16(raw_data_length);
                cmd.scenario_id = csp_hton32(scenario_id);
                chunk_crc32 = crc32_compute(reinterpret_cast<const uint8_t *>(&cmd.scenario_id), raw_data_length);
                cmd.ChunkCRC32 = csp_hton32(chunk_crc32);
                uint8_t checksum = 0xFF;
                const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&cmd);
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= bytes[i];
                cmd.CmdHeader[7] = checksum;

                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                if (!pkt) {
                    console.AddLog("[ERROR]##Failed to allocate AIOBC AI Mode packet.");
                    break;
                }
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = B12_UL_AIOBC;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, &cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // ==================== GENERIC / MANUAL COMMAND BUILDER ====================
    default:
        return false;
    }
    return true;
}
