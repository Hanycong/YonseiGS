//PROJECT       MIMAN_GS
//VERSION       0.0.0
//AUTHORS       MIMAN COMS/GS Team
//REFERENCE     CANYVAL_GS

//Define Envs
#define STB_IMAGE_IMPLEMENTATION
#define READ_BUF_SIZE   1024




//Custom Headers
#include "YonseiGS_config.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_satcontext.h"
#include "YonseiGS_theme.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_model.h"
#include "YonseiGS_s_control.h"
#include "YonseiGS_autopilot.h"
#include "YonseiGS_ftp.h"
#include "svc_station.h"
#include "svc_tracking.h"
#include "tc_log.h"
#include "sat_mode.h"

#undef CSP_USE_RDP

FILE *log_ptr;

////Temp
StateCheckUnit State;
Command * command;
Setup * setup = new Setup;
dlreqdata * request = (dlreqdata *)malloc(sizeof(dlreqdata));
bool Showstate = true;

////GUI
extern Console console;
extern Observer Yonsei;
extern int Requested;
extern Sband * sgs;
volatile sig_atomic_t g_shutdown_requested = 0;
////Thread
/* pthread index */
// 0 : Radio Control
// 1 : Rotator Control
// 2 : Rotator Read
// 3 : TLE Trackings
// 4 : TX scheduler
// 5 : Calibration
// 6 : AMP Timer
// 7 : Doppler Shift Correction
// 8 : FTP downlink/uplink
// 9 : 
//10 : Signaltest
//11 : TRX Controller
//12 : Radio Controller
//13 : AutoPilot
//14 : 
//15 : TLE load
pthread_t p_thread[16];
pthread_mutex_t conn_lock = PTHREAD_MUTEX_INITIALIZER;

csp_debug_level_t debug_level;

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void shutdown_handler(int sig) {
    g_shutdown_requested = 1;
}


void Initialize_ConfigSettings()
{
    memset(setup->Transciever_devname, 0, sizeof(setup->Transciever_devname));
    memset(setup->Switch_devname, 0, sizeof(setup->Switch_devname));
    memset(setup->Rotator_devname, 0, sizeof(setup->Rotator_devname));
    memset(setup->S_Username, 0, sizeof(setup->S_Username));
    memset(setup->S_Address, 0, sizeof(setup->S_Address));
    memset(setup->S_passwd, 0, sizeof(setup->S_passwd));
    sprintf(setup->Transciever_devname, "/dev/GS102");
    sprintf(setup->Switch_devname, "/dev/KTA223");
    sprintf(setup->Rotator_devname, "/dev/GS232B");
    sprintf(setup->S_Username, "s-band");
    sprintf(setup->S_Address, "165.132.28.235");
    sprintf(setup->S_passwd, "qwe123");
    setup->Transceiver_baud = 500000; // These three baud rate is between computer & GS machine - USART
    setup->Rotator_baud = 9600; 
    setup->Switch_baud = 115200; 
    setup->gs100_node = 20;
    setup->kiss_node = 8;
    setup->obc_node = 10;
    setup->ax100_node = 5;
    setup->default_freq = 436500000; // UHF freq?
    setup->default_timeout = 1000;
    setup->pingsize = 4;
    setup->guard_delay = 150;
    setup->queue_delay = 150;     //255
}

char * fontfile = "../bin/font/Inconsolata.ttf";

//Main
int main(int, char**)
{


    Initialize_ConfigSettings();
    Initialize_TLESettings();
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //// Create Main window with graphics context
    GLFWwindow * window = glfwCreateWindow(1920, 1080, "BEE-1000 Ground Station", NULL, NULL);
    if (window == NULL)
        return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    
    if(glewInit() != GLEW_OK)
        return -1;


    //// ImGUI Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGuiStyle * style = &ImGui::GetStyle();
    // 2026.06.10 테마 색 변경
    mim::ApplyTheme(style);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Load Fonts
    // 본문은 DroidSans(가독성 좋은 산세리프) 하나로 통일, 고정폭이 필요한 콘솔만 Inconsolata
    // 한글: 나눔고딕을 본문/콘솔 글꼴에 합친다 (DroidSans / Inconsolata 에 없는 글자만 채움, OFL)
    auto MergeKorean = [&io](const char * file, float size) {
        ImFontConfig cfg;
        cfg.MergeMode = true;
        io.Fonts->AddFontFromFileTTF(file, size, &cfg, io.Fonts->GetGlyphRangesKorean());
    };
    io.Fonts->AddFontFromFileTTF("../bin/font/DroidSans.ttf", 20.0f);                          // [0] 기본 본문
    MergeKorean("../bin/font/NanumGothic.ttf", 20.0f);
    mim::font_title    = io.Fonts->AddFontFromFileTTF("../bin/font/DroidSans-Bold.ttf", 36.0f);   // 타이틀
    mim::font_subtitle = io.Fonts->AddFontFromFileTTF("../bin/font/DroidSans.ttf", 18.0f);        // 부제
    mim::font_section  = io.Fonts->AddFontFromFileTTF("../bin/font/DroidSans-Bold.ttf", 15.0f);   // 섹션 머리글
    mim::font_mono     = io.Fonts->AddFontFromFileTTF(fontfile, 18.0f);                           // 콘솔 (Inconsolata)  
    MergeKorean("../bin/font/NanumGothicCoding.ttf", 18.0f);
    
    ImVec4 clear_color = ImVec4(0.925f, 0.870f, 0.945f, 1.00f);  
    int WinWidth;
    int WinHeight;
    float ReactiveWidth;
    float ReactiveHeight;

    //MIMAN GS
    OrbitData.Fatellites = new SatelliteObject(Yonsei);
    for(int index = 0 ; index < sizeof(OrbitData.Satellites) / sizeof(SatelliteObject *); index++)
        OrbitData.Satellites[index] = new SatelliteObject(Yonsei);
    mim::Initialize_SatContext();

    for(int index = 0; index < 2; index++)
        FtpShared.ChunkState = new chunkstate_t;
    
    // Shader * ShaderPolarmap2D_bg = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
    // Shader * ShaderPolarmap2D_el0 = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
    // Shader * ShaderPolarmap2D_el30 = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
    // Shader * ShaderPolarmap2D_el60 = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
    // Shader * ShaderPolarmap2D_azNS = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
    // Shader * ShaderPolarmap2D_azEW = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);

    // Rectangle * Worldmap2D = new Rectangle();
    // Rectangle * Polarmap2D_bg = new Rectangle();
    // Circle * Polarmap2D_el0 = new Circle();
    // Circle * Polarmap2D_el30 = new Circle();
    // Circle * Polarmap2D_el60 = new Circle();
    // StraightLine * Polarmap2D_azNS = new StraightLine();
    // StraightLine * Polarmap2D_azEW = new StraightLine();

    // PathInitializer();
    

    

    // GLint TextureWorldmap2D = TextureFromFile("Earth3686.jpg", "./bin/image");
    // GLint TextureSatellites2D = TextureFromFile("Satellite.png", "./bin/image");

    // Polarmap2D_el0->Setcolor(0.00f, 0.65f, 0.00f);
    // Polarmap2D_el0->scale(0.9f);
    // Polarmap2D_el30->Setcolor(0.00f, 0.65f, 0.00f);
    // Polarmap2D_el30->scale(0.6f);
    // Polarmap2D_el60->Setcolor(0.00f, 0.65f, 0.00f);
    // Polarmap2D_el60->scale(0.3f);

    // Polarmap2D_azNS->Setpoint(0.0f, 0.95f, 0.0f, 0.0f, -0.95f, 0.0f);
    // Polarmap2D_azNS->Setcolor(0.00f, 0.65f, 0.00f);
    // Polarmap2D_azEW->Setpoint(0.95f, 0.0f, 0.0f, -0.95f, 0.0f, 0.0f);
    // Polarmap2D_azEW->Setcolor(0.00f, 0.65f, 0.00f);

    State.GUI = true;
    bool GUIneedshutdown = true;

    FtpShared.gslistup = new gsftp_listup_t;
    FtpShared.gsmove = new gsftp_move_t;
    FtpShared.gscopy = new gsftp_copy_t;
    FtpShared.gsremove = new gsftp_remove_t;
    FtpShared.gsmkdir = new gsftp_mkdir_t;
    FtpShared.gsrmdir = new gsftp_rmdir_t;
    memset(FtpShared.gslistup, 0, sizeof(gsftp_listup_t));
    memset(FtpShared.gsmove, 0, sizeof(gsftp_move_t));
    memset(FtpShared.gscopy, 0, sizeof(gsftp_copy_t));
    memset(FtpShared.gsremove, 0, sizeof(gsftp_remove_t));
    memset(FtpShared.gsmkdir, 0, sizeof(gsftp_mkdir_t));
    memset(FtpShared.gsrmdir, 0, sizeof(gsftp_rmdir_t));

    command = new Command;
    memset(command, 0, sizeof(Command));

    csp_debug_hook_set((csp_debug_hook_func_t)csp_debug_callback);

    // TC 로그: 시작 위성(BEE1000) 폴더 ../log/TC_Log. 위성을 바꾸면 그 위성 폴더로 바뀐다
    if (!mim::svc::SwitchTcLogFile(mim::sat::SAT_BEE1000)) {
        printf("Invalid log file pointer.\n");
    }
    while(State.AllThread)
    {

        if (g_shutdown_requested) {
        // 시그널로 인한 종료 요청 들어옴
        printf("[MAIN-DIAG] signal shutdown detected (g_shutdown_requested=1)\n");

        State.GUI = false;       // GUI 루프 정지 -> 창 닫기와 같은 정리 경로 (StopAllThreads 후 while 탈출)
    }
        if(State.GUI)
        {   
            GUIneedshutdown = true;

            bool should_close = glfwWindowShouldClose(window);

if (should_close) {
    printf("[MAIN-DIAG] glfwWindowShouldClose(window) became TRUE\n");
}

            State.GUI = !glfwWindowShouldClose(window);
            glfwPollEvents();
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGuiIO& io = ImGui::GetIO();
            ImDrawList* bg = ImGui::GetBackgroundDrawList();




            bg->AddRectFilled(
                ImVec2(0, 0),
                io.DisplaySize,
                mim::U32(mim::BG_DEEP)                  // 테마의 최심 배경 (#0a0e1a)
            );


            glfwGetWindowSize(window, &WinWidth, &WinHeight);
            ReactiveWidth = (float)WinWidth / DISPLAY_WINDWIDTH;
            ReactiveHeight = (float)WinHeight / DISPLAY_WINDHEIGHT;
            now_rx_bytes_update();
            
            //Get Current Window Size

            
            ImGui_MainMenu();
            ImGui_MainWorkspace(ReactiveWidth, ReactiveHeight);;

            // // Show Console Window
            // console.ChangeWindowSize(DISPLAY_COL1 * ReactiveWidth, MENUPADDING, DISPLAY_CONSOLE_W * ReactiveWidth, ReactiveHeight * DISPLAY_CONSOLE_H);
            // console.Draw("General Console", &Showstate, ReactiveHeight * 0.6);

            // //Show Beacon State Window
            // ImGui::SetNextWindowPos(ImVec2(DISPLAY_COL3 * ReactiveWidth, MENUPADDING), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_STATE_W * ReactiveWidth, ReactiveHeight * DISPLAY_STATE_H), ImGuiCond_Always);
            // ImGui_BeaconWindow(ReactiveHeight * 0.65);

            // //Show Control Window
            // ImGui::SetNextWindowPos(ImVec2(DISPLAY_COL1 * ReactiveWidth, DISPLAY_ROW1 * ReactiveHeight), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_CONTROL_W * ReactiveWidth, DISPLAY_CONTROL_H * ReactiveHeight), ImGuiCond_Always);
            // ImGui_ControlWindow(ReactiveHeight * 0.7);

            // //Show Rotator Window
            // ImGui::SetNextWindowPos(ImVec2(0, ReactiveHeight * DISPLAY_ROW1), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_ROTAT_W * ReactiveWidth, ReactiveHeight * DISPLAY_ROTAT_H ), ImGuiCond_Always);
            // ImGui_RotatorWindow(ReactiveHeight * 0.5);

            // // Show Tracking Window
            // ImGui::SetNextWindowPos(ImVec2(0 * ReactiveWidth, DISPLAY_ROW3 * ReactiveHeight), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_TRACK_W * ReactiveWidth, DISPLAY_TRACK_H * ReactiveHeight), ImGuiCond_Always);
            // ImGui_TrackWindow(ReactiveHeight * 0.5);


            // if (State.Display_Autopilot)
            // {
            //     float x = DISPLAY_COL1 * ReactiveWidth *1.924;
            //     float y = DISPLAY_ROW2 * ReactiveHeight;
            //     float w = DISPLAY_MODEL_W * ReactiveWidth/2.38;
            //     float h = DISPLAY_MODEL_H * ReactiveHeight;

            //     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
            //     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
            //     ImGui_AutoPilotWindow(ReactiveHeight * 0.5f);
            // }

            // //Show Frequency Window
            // ImGui::SetNextWindowPos(ImVec2(0, MENUPADDING), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_FREQ_W * ReactiveWidth, ReactiveHeight * DISPLAY_FREQ_H), ImGuiCond_Always);
            // ImGui_FrequencyWindow(ReactiveHeight * 0.5);
            
            // //Show Command Window
            // ImGui::SetNextWindowPos(ImVec2(0 * ReactiveWidth, DISPLAY_ROW2 * ReactiveHeight), ImGuiCond_Always);
            // ImGui::SetNextWindowSize(ImVec2(DISPLAY_COMMAND_W * ReactiveWidth, DISPLAY_COMMAND_H * ReactiveHeight), ImGuiCond_Always);
            // ImGui_CommandWindow(ReactiveHeight * 0.5);


            //Display Popup Windows
            if (AppUi.Display_paramt0)
                AppUi.Display_paramt0 = popup_param_table0();
            
            if (AppUi.Display_paramt1)
                AppUi.Display_paramt1 = popup_param_table1();

            if (AppUi.Display_paramt5)
                AppUi.Display_paramt5 = popup_param_table5();

            // if (State.Display_CMD)
            //     State.Display_CMD = popup_cmd();

            if (AppUi.Display_Setup)
                AppUi.Display_Setup = popup_setup(setup);

            if(AppUi.Display_load)
                AppUi.Display_load = popup_load();
            else
            {
                if (AppUi.Display_TLE)
                    AppUi.Display_TLE = popup_tle();
            }

            if (AppUi.Display_FDS)
                AppUi.Display_FDS = popup_fds();

            if (AppUi.Display_Satinfo)
                AppUi.Display_Satinfo = popup_satinfo(Requested);

            if (AppUi.Display_Sband)
                AppUi.Display_Sband = popup_s_band();



            if(State.InitializeRequired)
            {
                mim::svc::InitializeHardware(shutdown_handler);
            }

            if(OrbitData.ModelRefreshRequired)
            {
                SatelliteModelInitialize(GetNowTracking());
                OrbitData.ModelRefreshRequired = false;
            }

            // Rendering
            ImGui_ClearColorBuf(window, clear_color);
            
            
            // Exclude about 35 pixels in map's height not to invade tab bar
            // Note that unlike ImGui, origin of openGL screen coordinate locates at bottom left corner
            // (top left for ImGui)
            ImGui::Render();
            glViewport(DISPLAY_COL1 * ReactiveWidth, DISPLAY_WINDHEIGHT * ReactiveHeight - (DISPLAY_ROW2 * ReactiveHeight), DISPLAY_MODEL_W * ReactiveWidth, ReactiveHeight * DISPLAY_MODEL_H );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffers
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);  // Enable transparency

            // ShaderWorldmap2D->use();
            // glActiveTexture(GL_TEXTURE0 + 1);
            // glBindTexture(GL_TEXTURE_2D, TextureWorldmap2D);
            // ShaderWorldmap2D->setInt("texture1", 1);
            // Worldmap2D->Draw(ShaderWorldmap2D);
            // // Sattext2D->RenderText("Test", 100.0, 100.0, 1.0, glm::vec3(1.0f, 1.0f, 1.0f));

            // SatDisplayer(&TextureSatellites2D);

            // glViewport(0, DISPLAY_WINDHEIGHT * ReactiveHeight - (DISPLAY_ROW3 * ReactiveHeight), DISPLAY_COL1 * ReactiveWidth, ReactiveHeight * (DISPLAY_ROW3 - DISPLAY_ROW2) );
            // // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffers
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // glEnable(GL_BLEND);  // Enable transparency

            // ShaderPolarmap2D_el0->use();
            // Polarmap2D_el0->Update();
            // ShaderPolarmap2D_el60->setFloat("alpha", 1.0f);
            // Polarmap2D_el0->drawLoop(ShaderPolarmap2D_el0);

            // ShaderPolarmap2D_el30->use();
            // Polarmap2D_el30->Update();
            // ShaderPolarmap2D_el30->setFloat("alpha", 1.0f);
            // Polarmap2D_el30->drawLoop(ShaderPolarmap2D_el30);

            // ShaderPolarmap2D_el60->use();
            // Polarmap2D_el60->Update();
            // ShaderPolarmap2D_el0->setFloat("alpha", 1.0f);
            // Polarmap2D_el60->drawLoop(ShaderPolarmap2D_el60);

            // ShaderPolarmap2D_azNS->use();
            // Polarmap2D_azNS->Update();
            // ShaderPolarmap2D_azNS->setFloat("alpha", 1.0f);
            // Polarmap2D_azNS->draw(ShaderPolarmap2D_azNS);

            // ShaderPolarmap2D_azEW->use();
            // Polarmap2D_azEW->Update();
            // ShaderPolarmap2D_azEW->setFloat("alpha", 1.0f);
            // Polarmap2D_azEW->draw(ShaderPolarmap2D_azEW);

            //PathDisplayer(GetNowTracking());
            //SatpointDisplayer();


            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            // sleep(1/MIMAN_FRAMERATE);
            continue;
        }
        printf("[MAIN-DIAG] leaving GUI loop: State.GUI=%d State.AllThread=%d\n",
        State.GUI, State.AllThread);
        // Cleanup
        if(GUIneedshutdown)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            GUIneedshutdown = false;
        }
        

        // Stop 직후 종료하면 AutoPilot 정리(RotatorSetTo 등)가 닫힌 장치를 쓰지 않도록 먼저 기다린다
        mim::svc::WaitAutoPilotStopped();
        mim::svc::FinishHardware();

        if (State.AllThread) // "Start" at popup
        {
            mim::svc::StopAllThreads();
        }

        if(AutoPilotCfg.Autopilot)
        {
            sleep(10);
        }
        else
        {
            State.AllThread = false;
        }
        
    }

    

    delete setup; 
    setup = NULL;
    // delete ShaderWorldmap2D;
    // ShaderWorldmap2D = NULL;
    // delete Worldmap2D;
    // Worldmap2D = NULL;
    // delete ShaderPolarmap2D_el0;
    // ShaderPolarmap2D_el0 = NULL;
    // delete ShaderPolarmap2D_el30;
    // ShaderPolarmap2D_el30 = NULL;
    // delete ShaderPolarmap2D_el60;
    // ShaderPolarmap2D_el60 = NULL;
    // delete ShaderPolarmap2D_azNS;
    // ShaderPolarmap2D_azNS = NULL;
    // delete ShaderPolarmap2D_azEW;
    // ShaderPolarmap2D_azEW = NULL;
    // delete Polarmap2D_el0;
    // Polarmap2D_el0 = NULL;
    // delete Polarmap2D_el30;
    // Polarmap2D_el30 = NULL;
    // delete Polarmap2D_el60;
    // Polarmap2D_el60 = NULL;
    // delete Polarmap2D_azNS;
    // Polarmap2D_azNS = NULL;
    // delete Polarmap2D_azEW;
    // Polarmap2D_azEW = NULL;
    free(command);
    for(int i = 0; i < sizeof(OrbitData.Satellites) / sizeof(SatelliteObject *); i++)
    {
        delete OrbitData.Satellites[i];
        OrbitData.Satellites[i] = NULL;
    }
    PathDelete();
    if(sgs != NULL)
        sgs->finSband();
    sleep(0.5);
mim::svc::CloseTcLogFile();
system("../amp/ampoff");
printf("[MAIN-DIAG] program exiting normally\n");

printf("Finish BEE-1000 GS.\n");
return 0;

    
}
