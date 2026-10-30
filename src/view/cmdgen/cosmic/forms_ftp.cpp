// View - 명령 생성기 입력 폼: COSMIC FTP 앱 (명령 번호 203)
//
// cmdgen_view.cpp 의 명령 번호 switch 에서 이 앱의 case 를 내용 그대로 옮긴 것.
// 처리한 명령이면 true.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawCosmicFtpForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 203: { 
        static uint16_t msgid = 0;
        static uint8_t  fnccode = 0;
        static char     name_buf[64] = "";


        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputText("name", name_buf, sizeof(name_buf));
        ImGui::InputScalar("start_byte u32", ImGuiDataType_U32, &command->ftpsendfilecmd.start_byte);
        ImGui::InputScalar("end_byte u32",   ImGuiDataType_U32, &command->ftpsendfilecmd.end_byte);
        ImGui::InputScalar("interval u8",    ImGuiDataType_U8,  &command->ftpsendfilecmd.interval);

        ImGui::Separator();
        ImGui::Text("Padding (3 bytes)");
        ImGui::Separator();

        ImGui::InputScalar("padding[0] u8", ImGuiDataType_U8, &command->ftpsendfilecmd.padding[0]);
        ImGui::InputScalar("padding[1] u8", ImGuiDataType_U8, &command->ftpsendfilecmd.padding[1]);
        ImGui::InputScalar("padding[2] u8", ImGuiDataType_U8, &command->ftpsendfilecmd.padding[2]);

        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);

            memset(command->ftpsendfilecmd.name, 0, sizeof(command->ftpsendfilecmd.name));
            strncpy(reinterpret_cast<char*>(command->ftpsendfilecmd.name), name_buf, sizeof(command->ftpsendfilecmd.name)-1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t length[2]   = {0x00, 0x4D};

            memcpy(command->ftpsendfilecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->ftpsendfilecmd.CmdHeader + 2, sequence,  sizeof(uint16_t));
            memcpy(command->ftpsendfilecmd.CmdHeader + 4, length,    sizeof(uint16_t));
            memcpy(command->ftpsendfilecmd.CmdHeader + 6, &fnccode,  sizeof(uint8_t));

            command->ftpsendfilecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(FTP_sendfileCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length     = sizeof(FTP_sendfileCmd_t);



            
            uint16_t len = sizeof(FTP_sendfileCmd_t);
            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->ftpsendfilecmd);
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);

            memcpy(command->ftpsendfilecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));

            memcpy(TestPacket->Data, &command->ftpsendfilecmd, sizeof(FTP_sendfileCmd_t));

            mim::svc::SendUplink((void*)TestPacket);


        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
            command->ftpsendfilecmd.CmdHeader[0],
            command->ftpsendfilecmd.CmdHeader[1],
            command->ftpsendfilecmd.CmdHeader[2],
            command->ftpsendfilecmd.CmdHeader[3],
            command->ftpsendfilecmd.CmdHeader[4],
            command->ftpsendfilecmd.CmdHeader[5],
            command->ftpsendfilecmd.CmdHeader[6],
            command->ftpsendfilecmd.CmdHeader[7]);

        ImGui::Text("Params:");
        ImGui::Text("  name        = %s", name_buf);
        ImGui::Text("  start_byte  = %u", command->ftpsendfilecmd.start_byte);
        ImGui::Text("  end_byte    = %u", command->ftpsendfilecmd.end_byte);
        ImGui::Text("  interval    = %u", command->ftpsendfilecmd.interval);
        ImGui::Text("  padding[0..2] = {%u, %u, %u}",
            command->ftpsendfilecmd.padding[0],
            command->ftpsendfilecmd.padding[1],
            command->ftpsendfilecmd.padding[2]);

        break;
    }            





    default:
        return false;
    }
    return true;
}
