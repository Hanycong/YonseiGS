// ViewModel - FDS Manager 팝업 (FDS 궤도 파일을 기본 위성(State.Fatellites)으로 읽기)
#pragma once

#include <stddef.h>

namespace mim {
namespace vm {

class FdsVM
{
public:
    char * Path();
    size_t PathSize() const;
    void Load();                        // "Load FDS"

private:
    char path_[256] = "../bin/fds/fds.dat";
};

FdsVM & FdsViewModel();

} // namespace vm
} // namespace mim
