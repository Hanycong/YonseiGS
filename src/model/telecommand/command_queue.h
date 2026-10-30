// Model - 업링크 대기 명령 큐 (CMD Manager 가 보여 주는 SatCMD[256], 다음 칸 번호 NowCMD)
//
// 현재는 miman_coms.cpp 의 SatCMD 와 miman_imgui.cpp 의 NowCMD 전역을 감싼다.
#pragma once

namespace mim {
namespace model {

class CommandQueue
{
public:
    void Clear();                       // 쌓인 명령을 모두 delete 하고 NowCMD = 0
};

CommandQueue & Commands();

} // namespace model
} // namespace mim
