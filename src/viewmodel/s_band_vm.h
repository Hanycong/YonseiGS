// ViewModel - S-Band Manager 팝업 (S-Band 지상국 PC 원격 명령)
#pragma once

#include <stddef.h>

namespace mim {
namespace vm {

class SbandVM
{
public:
    char * Command();
    size_t CommandSize() const;
    void Send();                        // "Send"
    const char * Output() const;        // S-Band PC 응답 버퍼

private:
    char command_[1024] = {0};
};

SbandVM & SbandViewModel();

} // namespace vm
} // namespace mim
