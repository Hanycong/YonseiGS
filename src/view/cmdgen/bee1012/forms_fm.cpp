// View - 명령 생성기 입력 폼: BEE1012 fm (명령 번호 1800, 1801, 1802, 1803, 1804, 1805, 1807, 1808, 1809, 1810, 1811, 1812, 1813, 1814, 1815, 1816, 1817, 1819)
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

bool DrawBee1012FmForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 1800: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_FM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_FM_NOOP_CC);
            ImGui::InputScalar("msgid##direct_1800", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1800", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1800")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_NoArgCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_NoArgCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1801: { // FM RESET_COUNTERS
            static uint16_t msgid = BEE1012_FM_CMD_ID;
            static uint8_t fnccode = BEE1012_FM_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_1801", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1801", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1801")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1802: { // fm COPY_FILE
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=2;
            struct Payload {
                uint16_t Overwrite;
                char Source[64];
                char Target[64];
            };
            static_assert(sizeof(Payload)==130, "fm COPY_FILE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1802",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1802",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Overwrite##direct_1802",ImGuiDataType_U16,&payload.Overwrite);
            ImGui::InputText("Source##direct_1802",payload.Source,sizeof(payload.Source));
            ImGui::InputText("Target##direct_1802",payload.Target,sizeof(payload.Target));
            if(ImGui::Button("Generate CMD##direct_1802")){
                WriteSystemName(msgid);
                uint8_t cmd[138]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1803: { // fm MOVE_FILE
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=3;
            struct Payload {
                uint16_t Overwrite;
                char Source[64];
                char Target[64];
            };
            static_assert(sizeof(Payload)==130, "fm MOVE_FILE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1803",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1803",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Overwrite##direct_1803",ImGuiDataType_U16,&payload.Overwrite);
            ImGui::InputText("Source##direct_1803",payload.Source,sizeof(payload.Source));
            ImGui::InputText("Target##direct_1803",payload.Target,sizeof(payload.Target));
            if(ImGui::Button("Generate CMD##direct_1803")){
                WriteSystemName(msgid);
                uint8_t cmd[138]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

    case 1804: { // ADDED FOR FTP TEST 1210, rename To_lab.so
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 4;
        static char source_buf[64] = "/cf/to_lab23.so";
        static char target_buf[64] = "/cf/to_lab.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Source (char 64)", source_buf, sizeof(source_buf));
        ImGui::InputText("Target (char 64)", target_buf, sizeof(target_buf));


        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t msgid_be = htons(msgid);
            memset(command5->torenamecmd.source, 0, sizeof(command5->torenamecmd.source));
            strncpy(command5->torenamecmd.source, source_buf, sizeof(command5->torenamecmd.source) - 1);

            memset(command5->torenamecmd.target, 0, sizeof(command5->torenamecmd.target));
            strncpy(command5->torenamecmd.target, target_buf, sizeof(command5->torenamecmd.target) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(TO_RenameCmd_t) - 7)};
            memcpy(command5->torenamecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command5->torenamecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command5->torenamecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command5->torenamecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command5->torenamecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(TO_RenameCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(TO_RenameCmd_t);
            uint16_t len = sizeof(TO_RenameCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command5->torenamecmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command5->torenamecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command5->torenamecmd, sizeof(TO_RenameCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }








    case 1805: { // ADDED FOR FTP TEST 1210, delete To_lab.so
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 5;
        static char filename_buf[64] = "/cf/to_lab.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Filename (char 64)", filename_buf, sizeof(filename_buf));


        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t msgid_be = htons(msgid);

            memset(command5->todeletecmd.filename, 0, sizeof(command5->todeletecmd.filename));
            strncpy(command5->todeletecmd.filename, filename_buf, sizeof(command5->todeletecmd.filename) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(TO_DeleteCmd_t) - 7)};
            memcpy(command5->todeletecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command5->todeletecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command5->todeletecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command5->todeletecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command5->todeletecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(TO_DeleteCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(TO_DeleteCmd_t);
            uint16_t len = sizeof(TO_DeleteCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command5->todeletecmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command5->todeletecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command5->todeletecmd, sizeof(TO_DeleteCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }










        case 1807: { // fm DELETE_ALL_FILES
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=7;
            struct Payload {
                char Directory[64];
            };
            static_assert(sizeof(Payload)==64, "fm DELETE_ALL_FILES payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1807",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1807",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Directory##direct_1807",payload.Directory,sizeof(payload.Directory));
            if(ImGui::Button("Generate CMD##direct_1807")){
                WriteSystemName(msgid);
                uint8_t cmd[72]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1808: { // fm DECOMPRESS_FILE
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=8;
            struct Payload {
                char Source[64];
                char Target[64];
            };
            static_assert(sizeof(Payload)==128, "fm DECOMPRESS_FILE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1808",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1808",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Source##direct_1808",payload.Source,sizeof(payload.Source));
            ImGui::InputText("Target##direct_1808",payload.Target,sizeof(payload.Target));
            if(ImGui::Button("Generate CMD##direct_1808")){
                WriteSystemName(msgid);
                uint8_t cmd[136]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1809: { // fm CONCAT_FILES
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=9;
            struct Payload {
                char Source1[64];
                char Source2[64];
                char Target[64];
            };
            static_assert(sizeof(Payload)==192, "fm CONCAT_FILES payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1809",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1809",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Source1##direct_1809",payload.Source1,sizeof(payload.Source1));
            ImGui::InputText("Source2##direct_1809",payload.Source2,sizeof(payload.Source2));
            ImGui::InputText("Target##direct_1809",payload.Target,sizeof(payload.Target));
            if(ImGui::Button("Generate CMD##direct_1809")){
                WriteSystemName(msgid);
                uint8_t cmd[200]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

    case 1810: { // FTP_filenameCmd
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 10;
        static char path_buf[64] = "/cf/adcs.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("path", path_buf, sizeof(path_buf));
        ImGui::InputScalar("FileInfoCRC", ImGuiDataType_U32,
                           &command5->ftpfilenamecmd.FileInfoCRC);


        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t msgid_be = htons(msgid);
            memset(command5->ftpfilenamecmd.path, 0, sizeof(command5->ftpfilenamecmd.path));
            strncpy(command5->ftpfilenamecmd.path, path_buf, sizeof(command5->ftpfilenamecmd.path) - 1);
            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t length[2] = {0x00, 0x45};
            memcpy(command5->ftpfilenamecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command5->ftpfilenamecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command5->ftpfilenamecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command5->ftpfilenamecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));
            command5->ftpfilenamecmd.CmdHeader[7] = 0x00;
            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(FTP_filenameCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = sizeof(FTP_filenameCmd_t);
            uint16_t len = sizeof(FTP_filenameCmd_t);
            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command5->ftpfilenamecmd);
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command5->ftpfilenamecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command5->ftpfilenamecmd, sizeof(FTP_filenameCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
            command5->ftpfilenamecmd.CmdHeader[0],
            command5->ftpfilenamecmd.CmdHeader[1],
            command5->ftpfilenamecmd.CmdHeader[2],
            command5->ftpfilenamecmd.CmdHeader[3],
            command5->ftpfilenamecmd.CmdHeader[4],
            command5->ftpfilenamecmd.CmdHeader[5],
            command5->ftpfilenamecmd.CmdHeader[6],
            command5->ftpfilenamecmd.CmdHeader[7]);

        ImGui::Text("Params: path = %s", path_buf);
        ImGui::Text("  FileInfoCRC = %u", command5->ftpfilenamecmd.FileInfoCRC);

        break;
    }


        case 1811: { // FM GET_OPEN_FILES
            static uint16_t msgid = BEE1012_FM_CMD_ID;
            static uint8_t fnccode = BEE1012_FM_GET_OPEN_FILES_CC;
            ImGui::InputScalar("msgid##direct_1811", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1811", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1811")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 1812: { // fm CREATE_DIRECTORY
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=12;
            struct Payload {
                char Directory[64];
            };
            static_assert(sizeof(Payload)==64, "fm CREATE_DIRECTORY payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1812",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1812",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Directory##direct_1812",payload.Directory,sizeof(payload.Directory));
            if(ImGui::Button("Generate CMD##direct_1812")){
                WriteSystemName(msgid);
                uint8_t cmd[72]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1813: { // fm DELETE_DIRECTORY
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=13;
            struct Payload {
                char Directory[64];
            };
            static_assert(sizeof(Payload)==64, "fm DELETE_DIRECTORY payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1813",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1813",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Directory##direct_1813",payload.Directory,sizeof(payload.Directory));
            if(ImGui::Button("Generate CMD##direct_1813")){
                WriteSystemName(msgid);
                uint8_t cmd[72]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1814: { // fm GET_DIR_LIST_FILE
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=14;
            struct Payload {
                char Directory[64];
                char Filename[64];
                uint8_t GetSizeTimeMode;
                uint8_t Spare01[3];
            };
            static_assert(sizeof(Payload)==132, "fm GET_DIR_LIST_FILE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1814",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1814",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("Directory##direct_1814",payload.Directory,sizeof(payload.Directory));
            ImGui::InputText("Filename##direct_1814",payload.Filename,sizeof(payload.Filename));
            ImGui::InputScalar("GetSizeTimeMode##direct_1814",ImGuiDataType_U8,&payload.GetSizeTimeMode);
            if(ImGui::Button("Generate CMD##direct_1814")){
                WriteSystemName(msgid);
                uint8_t cmd[140]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

case 1815: { // FTP_filenameCmd
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 15;
        static char path_buf[64] = "/cf/sdcard/";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("path", path_buf, sizeof(path_buf));
        ImGui::InputScalar("DirListOffset u32",   ImGuiDataType_U32, &command5->ftpdirlistpktcmd.DirListOffset);
        ImGui::InputScalar("GetSizeTimeMode u8",   ImGuiDataType_U8, &command5->ftpdirlistpktcmd.GetSizeTimeMode);
        ImGui::InputScalar("Spare01[0] u8",   ImGuiDataType_U8, &command5->ftpdirlistpktcmd.Spare01_0);
        ImGui::InputScalar("Spare01[1] u8",   ImGuiDataType_U8, &command5->ftpdirlistpktcmd.Spare01_1);
        ImGui::InputScalar("Spare01[2] u8",   ImGuiDataType_U8, &command5->ftpdirlistpktcmd.Spare01_2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t msgid_be = htons(msgid);
            memset(command5->ftpdirlistpktcmd.path, 0, sizeof(command5->ftpdirlistpktcmd.path));
            strncpy(command5->ftpdirlistpktcmd.path, path_buf, sizeof(command5->ftpdirlistpktcmd.path) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t length[2] = {0x00, (uint8_t)(sizeof(FTP_dirlistpktCmd_t) - 7)};
            memcpy(command5->ftpdirlistpktcmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command5->ftpdirlistpktcmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command5->ftpdirlistpktcmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command5->ftpdirlistpktcmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));
            command5->ftpdirlistpktcmd.CmdHeader[7] = 0x00;
            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(FTP_dirlistpktCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = sizeof(FTP_dirlistpktCmd_t);
            uint16_t len = sizeof(FTP_dirlistpktCmd_t);
            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command5->ftpdirlistpktcmd);
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command5->ftpdirlistpktcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command5->ftpdirlistpktcmd, sizeof(FTP_dirlistpktCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
            command5->ftpdirlistpktcmd.CmdHeader[0],
            command5->ftpdirlistpktcmd.CmdHeader[1],
            command5->ftpdirlistpktcmd.CmdHeader[2],
            command5->ftpdirlistpktcmd.CmdHeader[3],
            command5->ftpdirlistpktcmd.CmdHeader[4],
            command5->ftpdirlistpktcmd.CmdHeader[5],
            command5->ftpdirlistpktcmd.CmdHeader[6],
            command5->ftpdirlistpktcmd.CmdHeader[7]);

        // ImGui::Text("Params: path = %s", path_buf);
        ImGui::Text("Params:  Path            : %s", command5->ftpdirlistpktcmd.path);

        // 2. 주요 설정값 확인
        ImGui::Text("  DirListOffset   : %u", command5->ftpdirlistpktcmd.DirListOffset);
        ImGui::Text("  GetSizeTimeMode : %u", command5->ftpdirlistpktcmd.GetSizeTimeMode);

        // 3. Spare(Padding) 값 확인 - 16진수로 보는 것이 편함
        ImGui::Text("  Spare Bytes     : [%02X] [%02X] [%02X]",
            command5->ftpdirlistpktcmd.Spare01_0,
            command5->ftpdirlistpktcmd.Spare01_1,
            command5->ftpdirlistpktcmd.Spare01_2);

        break;
    }
        case 1816: { // FM MONITOR_FILESYSTEM_SPACE
            static uint16_t msgid = BEE1012_FM_CMD_ID;
            static uint8_t fnccode = BEE1012_FM_MONITOR_FILESYSTEM_SPACE_CC;
            ImGui::InputScalar("msgid##direct_1816", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1816", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1816")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        case 1817: { // fm SET_TABLE_STATE
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=17;
            struct Payload {
                uint32_t TableEntryIndex;
                uint32_t TableEntryState;
            };
            static_assert(sizeof(Payload)==8, "fm SET_TABLE_STATE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1817",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1817",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("TableEntryIndex##direct_1817",ImGuiDataType_U32,&payload.TableEntryIndex);
            ImGui::InputScalar("TableEntryState##direct_1817",ImGuiDataType_U32,&payload.TableEntryState);
            if(ImGui::Button("Generate CMD##direct_1817")){
                WriteSystemName(msgid);
                uint8_t cmd[16]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 1819: { // fm SET_PERMISSIONS
            static uint16_t msgid=0x1815;
            static uint8_t fnccode=19;
            struct Payload {
                char FileName[64];
                uint32_t Mode;
            };
            static_assert(sizeof(Payload)==68, "fm SET_PERMISSIONS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_1819",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_1819",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("FileName##direct_1819",payload.FileName,sizeof(payload.FileName));
            ImGui::InputScalar("Mode##direct_1819",ImGuiDataType_U32,&payload.Mode);
            if(ImGui::Button("Generate CMD##direct_1819")){
                WriteSystemName(msgid);
                uint8_t cmd[76]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7);
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0;
                uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST;
                pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        // ==================== hk ====================
    default:
        return false;
    }
    return true;
}
