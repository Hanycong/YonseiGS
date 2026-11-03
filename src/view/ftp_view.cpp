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
#include "ui_kit.h"
#include "YonseiGS_theme.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;


using namespace mim::ui;

// 업/다운로드 진행 중 실행 버튼 자리에 그리는 Stop 버튼 (누리호 5차 GS draw_ftp_stop_button).
// Stop 을 누른 뒤 워커가 정리될 때까지는 회색 "Stopping..." (누를 수 없음)
static void FtpStopButton(mim::vm::FtpVM & vm)
{
    ImVec2 size(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight());
    if (vm.TransferStopping())
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        mim::BG_PANEL2);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mim::BG_PANEL2);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  mim::BG_PANEL2);
        ImGui::PushStyleColor(ImGuiCol_Text,          mim::TEXT_DIM);
        ImGui::Button("Stopping...##ftpstop", size);
        ImGui::PopStyleColor(4);
        return;
    }
    ImGui::PushStyleColor(ImGuiCol_Button,        mim::DANGER);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mim::DANGER_HOV);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  mim::DANGER_HOV);
    if (ImGui::Button("Stop##ftpstop", size))
        vm.Stop();
    ImGui::PopStyleColor(3);
}

// 파일 선택 콤보 + 실행 버튼 한 줄 (업/다운로드 중이면 실행 버튼 대신 Stop)
static bool FileComboWithButton(mim::vm::FtpVM & vm, const char * button)
{
    RowLabel("File");
    float bw = ImGui::GetFontSize() * 7.0f;
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - bw - ImGui::GetStyle().ItemSpacing.x);
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
    ImGui::SameLine();
    if (vm.TransferBusy())
    {
        FtpStopButton(vm);
        return false;
    }
    return ImGui::Button(button, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight()));
}

// 라벨 + 전체 폭 입력칸
static void PathInput(const char * label, const char * id, char * buf, size_t size)
{
    RowLabel(label);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText(id, buf, size);
}

// FTP 목적지 노드 (누리호 5차 GS "Dest Node"). 작업별 입력칸 아래에 둔다 (위 버튼 위치가 바뀌지 않게)
static void DestNodeInput(mim::vm::FtpVM & vm)
{
    RowLabel("Dest Node");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##ftp_dest_node", ImGuiDataType_U8, vm.DestNode());
}

void ImGui_CommandWindow_Body(float fontscale)
{
    mim::vm::FtpVM & vm = mim::vm::FtpViewModel();

    // ───── 다운링크 요청 (위성 파일 목록에서 번호로 요청)
    SectionHeader("DOWNLINK REQUEST");
    RowLabel("Telemetry");
    ImGui::RadioButton("HK##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_HK);
    ImGui::SameLine();
    ImGui::RadioButton("AOD##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_AOD);
    ImGui::SameLine();
    ImGui::RadioButton("LOG##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_LOG);
    RowLabel("");
    ImGui::RadioButton("SNSR##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_SNSR);
    ImGui::SameLine();
    ImGui::RadioButton("GPS##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_GPS);
    ImGui::SameLine();
    ImGui::RadioButton("CTRLO##ftpwindow", vm.DownlinkTarget(), MIM_DLTYPE_CTRLO);

    RowLabel("Status");
    ImGui::RadioButton("NOW##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_NEW);
    ImGui::SameLine();
    ImGui::RadioButton("OLD##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_OLD);
    ImGui::SameLine();
    ImGui::RadioButton("CUR##ftpwindow", vm.DownlinkStatus(), MIM_DLSTAT_ING);

    vm.ClampDownlinkRequest();
    if (ImGui::BeginTable("##filelisttable", 3, ImGuiTableFlags_SizingStretchSame))
    {
        ImGui::TableSetupColumn("Number");
        ImGui::TableSetupColumn("Step");
        ImGui::TableSetupColumn("Offset");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##ftpwindow_filenum", vm.FileNumber());
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##ftpwindow_step", vm.Step());
        ImGui::TableSetColumnIndex(2);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##ftpwindow_offset", vm.Offset());
        ImGui::EndTable();
    }

    float half = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
    if (ImGui::Button("Save##ftpwindow", ImVec2(half, ButtonHeight())))
    {
        vm.SaveDownlinkRequest();
    }
    ImGui::SameLine();
    if (ImGui::Button("Request##ftpwindow", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
    {
        vm.SendDownlinkRequest();
    }

    // ───── FTP 전송
    SectionHeader("FTP TRANSFER");
    RowLabel("Version");
    ImGui::RadioButton("V1##FTPVersionSelect1", vm.Version(), 1);
    ImGui::SameLine();
    ImGui::RadioButton("V2##FTPVersionSelect2", vm.Version(), 2);
    ImGui::SameLine();
    ImGui::RadioButton("Ryu##FTPVersionSelect3", vm.Version(), 3);
    ImGui::SameLine();
    ImGui::TextUnformatted("  Chunk");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##ftpchunksize", ImGuiDataType_U32, vm.ChunkSize(), NULL, NULL, "%u", NULL);
    vm.ClampChunkSize();

    RowLabel("Task");
    ImGui::RadioButton("Upload##FTPTaskSelect00", vm.Task(), FTP_UPLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Download##FTPTaskSelect02", vm.Task(), FTP_DOWNLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Listup##FTPTaskSelect07", vm.Task(), FTP_LIST_REQUEST);
    if (*vm.Version() == 1)
    {
        // V1 force 반복 횟수 (V1 일 때만)
        RowLabel("Iteration");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputInt("##ftpforceiteration", vm.ForceIteration());
        vm.ClampForceIteration();
    }
    ImGui::Spacing();

    vm.DefaultEmptyPaths();
    switch(*vm.Task())
    {
    case FTP_UPLOAD_REQUEST :{
        if (FileComboWithButton(vm, "Upload"))
        {
            vm.Upload();
        }
        PathInput("Local", "##FTP_local_filepath : ", vm.SelectedFile()->local_path, sizeof(vm.SelectedFile()->local_path));
        PathInput("Remote", "##FTP_remote_filepath : ", vm.SelectedFile()->remote_path, sizeof(vm.SelectedFile()->remote_path));
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_DOWNLOAD_REQUEST :{
        if (FileComboWithButton(vm, "Download"))
        {
            vm.Download();
        }
        PathInput("Local", "##FTP_local_filepath : ", vm.SelectedFile()->local_path, sizeof(vm.SelectedFile()->local_path));
        PathInput("Remote", "##FTP_remote_filepath : ", vm.SelectedFile()->remote_path, sizeof(vm.SelectedFile()->remote_path));
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_LIST_REQUEST :{
        vm.RequireV2();
        PathInput("Path", "##FTP_listup_path : ", vm.ListRequest()->fpathbuf, sizeof(vm.ListRequest()->fpathbuf));
        if (ImGui::Button("Listup", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.List();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        ImGui::Text(vm.ListRequest()->fdispbuf);
        break;
    }
    case FTP_MOVE_REQUEST :{
        vm.RequireV2();
        PathInput("From", "##FTP_movefrom_path : ", vm.MoveRequest()->from, sizeof(vm.MoveRequest()->from));
        PathInput("To", "##FTP_moveto_path : ", vm.MoveRequest()->to, sizeof(vm.MoveRequest()->to));
        if (ImGui::Button("Move", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.Move();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_REMOVE_REQUEST :{
        vm.RequireV2();
        PathInput("Path", "##FTP_REMOVE_path : ", vm.RemoveRequest()->path, sizeof(vm.RemoveRequest()->path));
        if (ImGui::Button("Remove", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.Remove();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_COPY_REQUEST :{
        vm.RequireV2();
        PathInput("From", "##FTP_copyfrom_path : ", vm.CopyRequest()->from, sizeof(vm.CopyRequest()->from));
        PathInput("To", "##FTP_copyto_path : ", vm.CopyRequest()->to, sizeof(vm.CopyRequest()->to));
        if (ImGui::Button("Copy", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.Copy();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_MKDIR_REQUEST :{
        vm.RequireV2();
        PathInput("Path", "##FTP_mkdir_path : ", vm.MkdirRequest()->path, sizeof(vm.MkdirRequest()->path));
        if (ImGui::Button("Make Directory", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.Mkdir();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    case FTP_RMDIR_REQUEST :{
        vm.RequireV2();
        PathInput("Path", "##FTP_rmdir_path : ", vm.RmdirRequest()->path, sizeof(vm.RmdirRequest()->path));
        if (ImGui::Button("Remove Directory", ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
        {
            vm.Rmdir();
        }
        DestNodeInput(vm);
        ImGui::TextWrapped("%s", vm.StatusText());
        break;
    }
    default : {
        vm.ResetTask();
        break;
    }
    }

    // ───── AMP (Control 창과 같은 버튼: 전송 중 바로 끄고 켤 수 있게)
    ImGui::Spacing();
    ImGui::Separator();
    mim::vm::ControlVM & amp_vm = mim::vm::ControlViewModel();
    mim::vm::AmpButton amp = amp_vm.Amp();
    if (ToggleWideButton(amp.label, amp.on))
        amp_vm.PressAmp(amp);
}

void ImGui_CommandWindow(float fontscale)
{  
    ImGui::Begin("Command GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_CommandWindow_Body(fontscale);
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}
