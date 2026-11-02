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


// 섹션 머리글: 흐린 글씨 + 구분선
static void SectionHeader(const char * title)
{
    ImGui::Spacing();
    ImGui::TextDisabled("%s", title);
    ImGui::Separator();
}

// 왼쪽 라벨 열 너비를 맞춘 행 라벨
static void RowLabel(const char * label)
{
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    ImGui::SameLine(ImGui::GetFontSize() * 6.0f);
}

static float ButtonHeight() { return ImGui::GetFrameHeight() * 1.3f; }

// 파일 선택 콤보 + 실행 버튼 한 줄
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
    return ImGui::Button(button, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight()));
}

// 라벨 + 전체 폭 입력칸
static void PathInput(const char * label, const char * id, char * buf, size_t size)
{
    RowLabel(label);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText(id, buf, size);
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
    ImGui::Spacing();

    switch(*vm.Task())
    {
    case FTP_UPLOAD_REQUEST :{
        if (FileComboWithButton(vm, "Upload"))
        {
            vm.Upload();
        }
        PathInput("Local", "##FTP_local_filepath : ", vm.SelectedFile()->local_path, sizeof(vm.SelectedFile()->local_path));
        PathInput("Remote", "##FTP_remote_filepath : ", vm.SelectedFile()->remote_path, sizeof(vm.SelectedFile()->remote_path));
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
    if (amp.on)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
    if (ImGui::Button(amp.label, ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight())))
    {
        if (amp.on || amp.timer)
            amp_vm.TurnAmpOff();
        else
            amp_vm.TurnAmpOn();
    }
    if (amp.on)
        ImGui::PopStyleColor();
}

void ImGui_CommandWindow(float fontscale)
{  
    ImGui::Begin("Command GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_CommandWindow_Body(fontscale);
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}
