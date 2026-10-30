// Model - 다운링크 파일 요청 (MIM_PT_DLREQ 패킷)
//
// 위성에 저장된 텔레메트리 파일(HK/AOD/LOG/SNSR/GPS/CTRLO)을 번호/스텝/오프셋으로 요청한다.
// 요청 내용은 miman_imgui.cpp 의 전역 버퍼 request(dlreqdata)에 저장해 두고 Send 때 패킷으로 만든다.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {

class DownlinkRequest
{
public:
    void Store(uint16_t target, uint16_t filestatus, uint32_t filenum, uint32_t offset, uint32_t step);
    void Send();                        // 저장된 요청으로 packetsign 을 만들어 업링크 (svc_tx)
};

DownlinkRequest & Downlink();

} // namespace model
} // namespace mim
