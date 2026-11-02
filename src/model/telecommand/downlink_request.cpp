#include "downlink_request.h"

#include <stdlib.h>
#include <string.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"

extern dlreqdata * request;

namespace mim {
namespace model {

void DownlinkRequest::Store(uint16_t target, uint16_t filestatus, uint32_t filenum, uint32_t offset, uint32_t step)
{
    request->target = target;
    request->filestatus = filestatus;
    request->filenum = filenum;
    request->offset = offset;
    request->step = step;
}

void DownlinkRequest::Send()
{
    packetsign * request_ = (packetsign *)malloc(sizeof(dlreqdata) + 8);
    request_->Identifier = MIM_ID;
    request_->PacketType = MIM_PT_DLREQ;
    request_->Length = sizeof(dlreqdata);
    memcpy(request_->Data, request, sizeof(dlreqdata));
    mim::svc::SendUplink((void *)request_); 
}

DownlinkRequest & Downlink()
{
    static DownlinkRequest dl;
    return dl;
}

} // namespace model
} // namespace mim
