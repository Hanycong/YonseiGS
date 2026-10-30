// View - 팝업 공통: 팝업들이 아직 직접 쓰는 전역 선언 (팝업별 ViewModel 로 옮기면 줄어든다)
#pragma once

#include "miman_config.h"
#include "miman_radial.h"
#include "miman_s_control.h"

extern StateCheckUnit State;
extern Console console;
extern ImGuiWindowFlags mim_winflags;
extern Ptable_0 param0;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Sband * sgs;
extern int NowTLE;
extern int tlepopupindex;
extern bool sgp4check;
extern char FDSFilePath[256];
extern char SatWindowLabelBuf[64];
extern char SearchBuf[256];
extern char SelectButtonTextBuf[32];
extern char DeleteButtonTextBuf[32];
extern char inputbuf[1024];

// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수, 바뀌지 않음)
static int freeze_cols = 1;
static int freeze_rows = 1;
