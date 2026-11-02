// App - 배치 코드(메인 메뉴, 작업 공간)가 아직 직접 쓰는 전역 선언
#pragma once

#include <GL/glew.h>
#include "YonseiGS_config.h"

extern StateCheckUnit State;
extern Console console;
extern ImGuiWindowFlags mim_winflags;
extern bool Showstate;

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int width, int height);
