// View - Beacon 창 "AX100 RPARAM" 탭
//
// beacon_view.cpp 의 ImGui_BeaconWindow_Body 에서 탭 블록을 내용 그대로 옮긴 것.
#include <inttypes.h>
#include <mutex>
#include <gs/param/rparam.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_coms.h"
#include "miman_radial.h"
#include "miman_csp.h"
#include "miman_theme.h"
#include "svc_tx.h"
#include "beacon_tabs.h"

using namespace mim;

extern Setup * setup;
extern StateCheckUnit State;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern Beacon * beacon;
extern MissionBeacon * missionbeacon;
extern GETFILEINFO * getfileinfo;
extern Event * event;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;
extern uint8_t remote_activeconf;
extern int16_t remote_lastrssi;
extern int16_t remote_rssibusy;
extern cmd_packet_t * shellcmd;
// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawAx100RparamTab(float fontscale)
{
    if(ImGui::BeginTabItem("AX100 RPARAM"))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        ImGui::Text("Total RX Bytes(Beacon) : %"PRIu32, get_rx_bytes());
        ImGui::SetWindowFontScale(0.6 * fontscale);
        ImGui::PopStyleColor();
        
        ImGui::BeginChild("##AX100_shell_typeselect", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.8), true, NULL);
        
        


        if(get_boot_count() == 0)
            ImGui::Text("AX100 Boot Count   ");
        else
            ImGui::Text("AX100 Boot Count : %"PRIu16"", get_boot_count());
        if(ImGui::Button("Load##ax100_direct4_boot_count", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint16(setup->ax100_node, 4, 0x0020, 0xB00B, setup->default_timeout, get_boot_count_address()) == GS_OK)
                console.AddLog("[OK]##AX100 Boot Count : %u", get_boot_count());
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();



        




        ImGui::Text("RSSI busy        ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct4_rssi_busy", ImGuiDataType_S16, &remote_rssibusy, NULL, NULL, "%d");
        if(ImGui::Button("Load##ax100_direct4_rssi_busy", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, &remote_rssibusy) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. rssi_busy : %"PRIu8, rparam0.csp_node);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct4_rssi_busy", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, remote_rssibusy) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();


        if(get_rx_bytes() == 0)
            ImGui::Text("Total RX Bytes   ");
        else
            ImGui::Text("Total RX bytes : %"PRIu8"", get_rx_bytes());
        if(ImGui::Button("Load##ax100_direct4_total_rx_bytes", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK)
                console.AddLog("[OK]##Total RX Bytes : %u", get_rx_bytes());
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();




        
        ImGui::Text("CAN enable       ");
        ImGui::SameLine();
        ImGui::Checkbox("##ax100_direct0_can_en", &rparam0.can_en);
        if(ImGui::Button("Load##ax100_direct0_can_en", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (int8_t *)&rparam0.can_en) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. can_en : %s", rparam0.can_en ? "true" : "false");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_can_en", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (bool)rparam0.can_en) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();




        ImGui::Text("TX inhibit       ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct0_tx_inhibit", ImGuiDataType_U32, &rparam0.tx_inhibit, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct0_tx_inhibit", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, &rparam0.tx_inhibit) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. tx_inhibit : %"PRIu32, rparam0.tx_inhibit);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_tx_inhibit", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, rparam0.tx_inhibit) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();
        




        ImGui::Text("TX power         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct0_tx_pwr", ImGuiDataType_U8, &rparam0.tx_pwr, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct0_tx_pwr", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, &rparam0.tx_pwr) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. tx_pwr : %"PRIu8, rparam0.tx_pwr);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_tx_pwr", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, rparam0.tx_pwr) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }






        ImGui::Spacing();
        
        ImGui::Text("CSP rtable       ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##ax100_direct0_csp_rtable", rparam0.csp_rtable, sizeof(rparam0.csp_rtable));
        if(ImGui::Button("Load##ax100_direct0_csp_rtable", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. csp_rtable : %s", rparam0.csp_rtable);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_csp_rtable", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        if(remote_activeconf > 3)
            ImGui::Text("Active Conf      ");
        else
            ImGui::Text("Active Conf : %"PRIu8"", remote_activeconf);
        if(ImGui::Button("Load##ax100_direct0_active_conf", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint8(setup->ax100_node, 4, 0x0018, 0xB00B, setup->default_timeout, &remote_activeconf) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. Active Conf : %"PRId8, remote_activeconf);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();

        if(remote_lastrssi > 3)
            ImGui::Text("Last RSSI        ");
        else
            ImGui::Text("Last RSSI : %"PRId16"", remote_lastrssi);
        if(ImGui::Button("Load##ax100_direct0_last_rssi", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_int16(setup->ax100_node, 4, 0x0004, 0xB00B, setup->default_timeout, &remote_lastrssi) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. Last RSSI : %"PRId16, remote_lastrssi);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();

        ImGui::Text("RX guard         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct1_rx_guard", ImGuiDataType_U16, &rparam1.guard, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct1_rx_guard", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, &rparam1.guard) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. rx_guard : %"PRIu16, rparam1.guard);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct1_rx_guard", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, rparam1.guard) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        ImGui::Text("TX guard         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct5_tx_guard", ImGuiDataType_U16, &rparam5.guard, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct5_tx_guard", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, &rparam5.guard) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. tx_guard : %"PRIu16, rparam5.guard);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct5_tx_guard", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, rparam5.guard) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        ImGui::Text("RX Baudrate         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct1_rxbaud", ImGuiDataType_U32, &rparam1.baud, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct1_rx_baud", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, &rparam1.baud) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. rx_baud : %"PRIu16, rparam1.baud);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct1_rxbaud", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, rparam1.baud) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        ImGui::Text("TX Baudrate         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct5_txbaud", ImGuiDataType_U32, &rparam5.baud, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct5_txbaud", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, &rparam5.baud) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK. tx_baud : %"PRIu16, rparam5.baud);
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct5_txbaud", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, rparam5.baud) == GS_OK)
                console.AddLog("[OK]##Remote Param Control OK.");
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        if(ImGui::Button("Save to FRAM##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y)))
        {
            if(gs_rparam_save(setup->ax100_node, setup->default_timeout, 0, 0) == GS_OK)
            {
                gs_rparam_save(setup->ax100_node, setup->default_timeout, 1, 1);
                gs_rparam_save(setup->ax100_node, setup->default_timeout, 5, 5);
                set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
                console.AddLog("[OK]##Remote Param Control OK.");
            }
                
            else
                console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset Watchdog##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
        {
            if(csp_transaction(CSP_PRIO_HIGH, setup->ax100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0)
                console.AddLog("Remote Param Control OK.(No Reply)");
            else
                console.AddLog("[ERROR]##Remote Param Control Failed.(No Reply)");
        }
        ImGui::SameLine();
        if(ImGui::Button("Reboot AX100##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            csp_reboot(setup->ax100_node);
            console.AddLog("Remote Param Control OK.(No Reply)");
        }
        ImGui::EndChild();

        ImGui::BeginChild("##MEOW_shell_typeselect", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        ImGui::RadioButton("System CMD   ##shelltype_syscmd", &State.Shelltype, TYPE_SYSCMD);
        ImGui::SameLine();
        ImGui::RadioButton("Redirect CMD ##shelltype_rmdir ", &State.Shelltype, TYPE_SYSCMD_REDIR);
        ImGui::SameLine();
        ImGui::RadioButton("Remove Dir   ##shelltype_rmdirp", &State.Shelltype, TYPE_SET_REDIR_PATH);
        ImGui::SameLine();
        ImGui::RadioButton("DS Cleanup   ##shelltype_cleand", &State.Shelltype, TYPE_DS_CLEANUP);

        ImGui::RadioButton("HK Request   ##shelltype_requhk", &State.Shelltype, TYPE_HK_REQUEST);
        ImGui::SameLine();
        ImGui::RadioButton("Reserved     ##shelltype_reserv", &State.Shelltype, TYPE_RESERVED);
        ImGui::SameLine();
        ImGui::RadioButton("KILL         ##shelltype_killin", &State.Shelltype, TYPE_KILL);

        if(State.Shelltype != (int)shellcmd->type)
        {
            shellcmd->type = (uint8_t)State.Shelltype;
            memset(&shellcmd->required, 0, sizeof(shellcmd->required));
        }

        switch (State.Shelltype)
        {
        case TYPE_SYSCMD:
        {   
            ImGui::Text("Out U8   ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputScalar("##shell_syscmd_redirout", ImGuiDataType_U8, &(shellcmd->required.syscmd.redir_out), NULL, NULL, "%u");
            ImGui::Text("CMD      ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd", shellcmd->required.syscmd.cmd, sizeof(shellcmd->required.syscmd.cmd));
            break;
        }
        case TYPE_SYSCMD_REDIR:
        {   
            ImGui::Text("CMD      ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd1", shellcmd->required.syscmd_redir.cmd, sizeof(shellcmd->required.syscmd_redir.cmd));
            ImGui::Text("Redirect ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd2", shellcmd->required.syscmd_redir.redir_path, sizeof(shellcmd->required.syscmd_redir.redir_path));
            break;
        }
        case TYPE_SET_REDIR_PATH:
        {   
            ImGui::Text("Path     ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd4", shellcmd->required.set_redir.redir_path, sizeof(shellcmd->required.set_redir.redir_path));
            ImGui::Text("");
            break;
        }
        case TYPE_DS_CLEANUP:
        {   
            ImGui::Text("Path     ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputScalar("##shell_syscmd_redirout5", ImGuiDataType_U8, &(shellcmd->required.ds_cleanup.opt), NULL, NULL, "%u");
            ImGui::Text("");
            break;
        }
        
        default:
        {
            ImGui::Text("ERROR!");
            break;
        }
        }

        if(ImGui::Button("Send##SendShellcmd", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3)))
        {
            if(State.Debugmode)
            {
                printf("Input Shell Data : ");
                for(int i = 0; i < sizeof(cmd_packet_t); i++)
                    printf("%u\t", ((uint8_t *)shellcmd)[i]);
                printf("\n");
            }
            mim::svc::StartDirectShell((void *)shellcmd); 
        }
        ImGui::EndChild();
        ImGui::SetWindowFontScale(1.0 * fontscale);
        ImGui::EndTabItem();
    }
}
