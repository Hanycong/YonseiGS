// View - 팝업 공통: 창 플래그와 표 머리글 고정 행/열 수 (상태·동작은 viewmodel/*_vm 이 가진다)
#pragma once

#include "YonseiGS_config.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_s_control.h"

extern ImGuiWindowFlags mim_winflags;

// YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수, 바뀌지 않음)
static int freeze_cols = 1;
static int freeze_rows = 1;
