// View - FTP 창 (Pass Operation 탭 가운데 열: 다운링크 요청, FTP 업로드/다운로드/목록, AMP)
//
// 함수 이름은 원래 이름(ImGui_CommandWindow*) 그대로다. ImGui 호출과 FtpVM / ControlVM(AMP) 호출만 있다.
#include <stdio.h>
#include <unistd.h>

#include "imgui.h"
#include "YonseiGS_config.h"     // MIM_DLTYPE_*, MIM_DLSTAT_*, FTP_*_REQUEST
#include "ftp_vm.h"
#include "control_vm.h"       // AMP 버튼 (Control 창과 같은 버튼)
#include "ftp_view.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

// YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void ImGui_CommandWindow_Body(float fontscale)
{
    mim::vm::FtpVM & vm = mim::vm::FtpViewModel();
    ImGui::BeginChild("##FTPManager", ImVec2(ImGui::GetContentRegionAvail().x * 0.4, ImGui::GetContentRegionAvail().y), true, mim_winflags);
    ImGui::Text("File List");
    ImGui::Text("Telemetry : ");
    ImGui::SameLine();
    ImGui::RadioButton("HK ##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_HK);
    ImGui::SameLine();
    ImGui::RadioButton("AOD##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_AOD);
    ImGui::SameLine();
    ImGui::RadioButton("LOG##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_LOG);
    ImGui::SameLine();
    ImGui::RadioButton("SNSR##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_SNSR);
    ImGui::Text("            ");
    ImGui::SameLine();
    ImGui::RadioButton("GPS##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_GPS);
    ImGui::SameLine();
    ImGui::RadioButton("CTRLO##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_CTRLO);

    ImGui::Text("Status    : ");
    ImGui::SameLine();
    ImGui::RadioButton("NOW##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_NEW);
    ImGui::SameLine();
    ImGui::RadioButton("OLD##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_OLD);
    ImGui::SameLine();
    ImGui::RadioButton("CUR##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_ING);
    ImGui::BeginTable("##filelisttable", 3, false, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.0f));
    
    ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
    ImGui::TableSetupColumn("Number##tle",                ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Step##tle",                 ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Offset##tle",            ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableHeadersRow();
    vm.ClampDownlinkRequest();
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::InputInt("##ftpwindow_filenum", vm.FileNumber());
    ImGui::TableSetColumnIndex(1);
    ImGui::InputInt("##ftpwindow_step", vm.Step());
    ImGui::TableSetColumnIndex(2);
    ImGui::InputInt("##ftpwindow_offset", vm.Offset());
    ImGui::EndTable();
    

    if (ImGui::Button("Save##ftpwindow", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        vm.SaveDownlinkRequest();
    }
    ImGui::SameLine();
    if (ImGui::Button("Request##ftpwindow", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        vm.SendDownlinkRequest();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("##FTPSending", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, mim_winflags);
    ImGui::Text("FTP  ");
    ImGui::SameLine();
    ImGui::RadioButton("V1      ##FTPVersionSelect1", vm.Version(), 1);
    ImGui::SameLine();
    ImGui::RadioButton("V2      ##FTPVersionSelect2", vm.Version(), 2);
    ImGui::SameLine();
    ImGui::Text("           ");
    ImGui::SameLine();
    ImGui::Text("Chunk Size ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##ftpchunksize", ImGuiDataType_U32, vm.ChunkSize(), NULL, NULL, "%u", NULL);
    vm.ClampChunkSize();
    ImGui::Text("Task ");
    ImGui::SameLine();
    ImGui::RadioButton("Upload  ##FTPTaskSelect00", vm.Task(), FTP_UPLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Download##FTPTaskSelect02", vm.Task(), FTP_DOWNLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Listup  ##FTPTaskSelect07", vm.Task(), FTP_LIST_REQUEST);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    
    mim::vm::ControlVM & amp_vm = mim::vm::ControlViewModel();
    mim::vm::AmpButton amp = amp_vm.Amp();
    if(!amp.on)
    {
        if(amp.timer)
        {
            if (ImGui::Button(amp.label, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
            {
                amp_vm.TurnAmpOff();
            }
        }
        else
        {
            if (ImGui::Button(amp.label, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
            {
                amp_vm.TurnAmpOn();
            }
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button(amp.label, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
        {
            amp_vm.TurnAmpOff();
        }
        ImGui::PopStyleColor();
    }
    // ImGui::Text("     ");
    // ImGui::SameLine();
    // ImGui::RadioButton("Move    ##FTPTaskSelect10", vm.Task(), FTP_MOVE_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("Remove  ##FTPTaskSelect12", vm.Task(), FTP_REMOVE_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("Copy    ##FTPTaskSelect23", vm.Task(), FTP_COPY_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("MKDIR   ##FTPTaskSelect25", vm.Task(), FTP_MKDIR_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("RMDIR   ##FTPTaskSelect27", vm.Task(), FTP_RMDIR_REQUEST);
    switch(*vm.Task())
    {
    case FTP_UPLOAD_REQUEST :{
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.8);
        if (ImGui::BeginCombo("##FTPCombobox", vm.SelectedFile()->name))
        {
            for (int FTPindex = 0; FTPindex < vm.FileSlotCount(); FTPindex++)
            {
                if (vm.FileSlot(FTPindex)->name[0] == 0)
                    continue;
                bool SelectedFTP = (*vm.SelectedFileIndex() == FTPindex);
                char Selectedlabel[30];
                sprintf(Selectedlabel, "%s##%d", vm.FileSlot(FTPindex)->name, FTPindex);
                if (ImGui::Selectable(Selectedlabel, SelectedFTP))
                    *vm.SelectedFileIndex() = FTPindex;
                        
                if (SelectedFTP)
                    ImGui::SetItemDefaultFocus();
                usleep(10);
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Upload", ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
        {
            vm.Upload();
        }
        ImGui::Text("Local  ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_local_filepath : ", vm.SelectedFile()->local_path, sizeof(vm.SelectedFile()->local_path));
        ImGui::PopItemWidth();
        ImGui::Text("Remote ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_remote_filepath : ", vm.SelectedFile()->remote_path, sizeof(vm.SelectedFile()->remote_path));
        ImGui::PopItemWidth();
        ImGui::Text(vm.StatusText());
        break;
    }
    case FTP_DOWNLOAD_REQUEST :{
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.8);
        if (ImGui::BeginCombo("##FTPCombobox", vm.SelectedFile()->name))
        {
            for (int FTPindex = 0; FTPindex < vm.FileSlotCount(); FTPindex++)
            {
                if (vm.FileSlot(FTPindex)->name[0] == 0)
                    continue;
                bool SelectedFTP = (*vm.SelectedFileIndex() == FTPindex);
                char Selectedlabel[30];
                sprintf(Selectedlabel, "%s##%d", vm.FileSlot(FTPindex)->name, FTPindex);
                if (ImGui::Selectable(Selectedlabel, SelectedFTP))
                    *vm.SelectedFileIndex() = FTPindex;
                        
                if (SelectedFTP)
                    ImGui::SetItemDefaultFocus();
                usleep(10);
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Download", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Download();
        }
        ImGui::Text("Local  ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_local_filepath : ", vm.SelectedFile()->local_path, sizeof(vm.SelectedFile()->local_path));
        ImGui::PopItemWidth();
        ImGui::Text("Remote ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_remote_filepath : ", vm.SelectedFile()->remote_path, sizeof(vm.SelectedFile()->remote_path));
        ImGui::PopItemWidth();
        ImGui::Text(vm.StatusText());

        break;
    }
    case FTP_LIST_REQUEST :{
        vm.RequireV2();
        ImGui::Text("Path   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_listup_path : ", vm.ListRequest()->fpathbuf, sizeof(vm.ListRequest()->fpathbuf));
        ImGui::PopItemWidth();
        if (ImGui::Button("Listup", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.List();
        }
        ImGui::Text(vm.StatusText());
        ImGui::Text(vm.ListRequest()->fdispbuf);
        break;
    }
    case FTP_MOVE_REQUEST :{
        vm.RequireV2();
        ImGui::Text("From   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_movefrom_path : ", vm.MoveRequest()->from, sizeof(vm.MoveRequest()->from));
        ImGui::PopItemWidth();
        ImGui::Text("To     ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_moveto_path : ", vm.MoveRequest()->to, sizeof(vm.MoveRequest()->to));
        ImGui::PopItemWidth();
        if (ImGui::Button("Move", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Move();
        }
        ImGui::Text(vm.StatusText());
        break;
    }
    case FTP_REMOVE_REQUEST :{
        vm.RequireV2();
        ImGui::Text("Path   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_REMOVE_path : ", vm.RemoveRequest()->path, sizeof(vm.RemoveRequest()->path));
        ImGui::PopItemWidth();
        if (ImGui::Button("Remove", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Remove();
        }
        ImGui::Text(vm.StatusText());
        break;
    }
    case FTP_COPY_REQUEST :{
        vm.RequireV2();
        ImGui::Text("From   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_copyfrom_path : ", vm.CopyRequest()->from, sizeof(vm.CopyRequest()->from));
        ImGui::PopItemWidth();
        ImGui::Text("To     ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_copyto_path : ", vm.CopyRequest()->to, sizeof(vm.CopyRequest()->to));
        ImGui::PopItemWidth();
        if (ImGui::Button("Copy", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Copy();
        }
        ImGui::Text(vm.StatusText());
        break;
    }
    case FTP_MKDIR_REQUEST :{
        vm.RequireV2();
        ImGui::Text("Path   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_mkdir_path : ", vm.MkdirRequest()->path, sizeof(vm.MkdirRequest()->path));
        ImGui::PopItemWidth();
        if (ImGui::Button("Make Directory", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Mkdir();
        }
        ImGui::Text(vm.StatusText());
        break;
    }
    case FTP_RMDIR_REQUEST :{
        vm.RequireV2();
        ImGui::Text("Path   ");
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##FTP_rmdir_path : ", vm.RmdirRequest()->path, sizeof(vm.RmdirRequest()->path));
        ImGui::PopItemWidth();
        if (ImGui::Button("Remove Directory", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.Rmdir();
        }
        ImGui::Text(vm.StatusText());
        break;
    }
    default : {
        vm.ResetTask();
        break;
    }
    }

    ImGui::EndChild();
}

void ImGui_CommandWindow(float fontscale)
{  
    ImGui::Begin("Command GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_CommandWindow_Body(fontscale);
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}
