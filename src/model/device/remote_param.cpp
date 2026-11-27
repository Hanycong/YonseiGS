#include "remote_param.h"

#include <gs/param/rparam.h>

namespace mim {
namespace model {

static const uint16_t kMagicChecksum = 0xB00B;

RemoteParam RemoteParam::Int8  (uint16_t addr, int8_t * value)   { return RemoteParam(kInt8,   addr, value, sizeof(int8_t)); }
RemoteParam RemoteParam::Int8  (uint16_t addr, uint8_t * value)  { return RemoteParam(kInt8,   addr, value, sizeof(int8_t)); }
RemoteParam RemoteParam::Uint8 (uint16_t addr, uint8_t * value)  { return RemoteParam(kUint8,  addr, value, sizeof(uint8_t)); }
RemoteParam RemoteParam::Int16 (uint16_t addr, int16_t * value)  { return RemoteParam(kInt16,  addr, value, sizeof(int16_t)); }
RemoteParam RemoteParam::Uint16(uint16_t addr, uint16_t * value) { return RemoteParam(kUint16, addr, value, sizeof(uint16_t)); }
RemoteParam RemoteParam::Int32 (uint16_t addr, int32_t * value)  { return RemoteParam(kInt32,  addr, value, sizeof(int32_t)); }
RemoteParam RemoteParam::Uint32(uint16_t addr, uint32_t * value) { return RemoteParam(kUint32, addr, value, sizeof(uint32_t)); }
RemoteParam RemoteParam::Float (uint16_t addr, float * value)    { return RemoteParam(kFloat,  addr, value, sizeof(float)); }
RemoteParam RemoteParam::String(uint16_t addr, char * text, size_t size) { return RemoteParam(kString, addr, text, size); }
RemoteParam RemoteParam::Flag  (uint16_t addr, bool * value)     { return RemoteParam(kFlag,   addr, value, sizeof(int8_t)); }

RemoteParam RemoteParam::Int8AtOwnValue(int8_t * value)
{
    RemoteParam p(kInt8, 0, value, sizeof(int8_t));
    p.addr_from_value_ = true;
    return p;
}

uint16_t RemoteParam::Address() const
{
    if (addr_from_value_)
        return (uint16_t)(*(const int8_t *)value_);
    return addr_;
}

int RemoteParam::WireType() const
{
    switch (type_)
    {
        case kInt8:   return GS_PARAM_INT8;
        case kUint8:  return GS_PARAM_UINT8;
        case kInt16:  return GS_PARAM_INT16;
        case kUint16: return GS_PARAM_UINT16;
        case kInt32:  return GS_PARAM_INT32;
        case kUint32: return GS_PARAM_UINT32;
        case kFloat:  return GS_PARAM_FLOAT;
        case kString: return GS_PARAM_STRING;
        case kFlag:   return GS_PARAM_INT8;
    }
    return GS_PARAM_INT8;
}

bool RemoteParam::Load(const RemoteTable & where) const
{
    const uint16_t addr = Address();
    if (type_ == kFlag)
    {
        int8_t raw = 0;
        if (gs_rparam_get(where.node, where.table, addr, GS_PARAM_INT8, kMagicChecksum, where.timeout_ms, &raw, sizeof(raw)) != GS_OK)
            return false;
        *(bool *)value_ = (bool)raw;
        return true;
    }
    // 라이브러리는 성공했을 때만 값을 쓴다 (실패하면 값 칸은 그대로)
    return gs_rparam_get(where.node, where.table, addr, (gs_param_type_t)WireType(), kMagicChecksum, where.timeout_ms, value_, size_) == GS_OK;
}

bool RemoteParam::Save(const RemoteTable & where) const
{
    const uint16_t addr = Address();
    if (type_ == kFlag)
    {
        const int8_t raw = *(const bool *)value_;
        return gs_rparam_set(where.node, where.table, addr, GS_PARAM_INT8, kMagicChecksum, where.timeout_ms, &raw, sizeof(raw)) == GS_OK;
    }
    return gs_rparam_set(where.node, where.table, addr, (gs_param_type_t)WireType(), kMagicChecksum, where.timeout_ms, value_, size_) == GS_OK;
}

int RemoteParamTable::LoadAll(const RemoteTable & where, std::vector<bool> * ok) const
{
    int failed = 0;
    if (ok) ok->clear();
    for (const RemoteParam & p : params_)
    {
        const bool loaded = p.Load(where);
        if (!loaded) failed++;
        if (ok) ok->push_back(loaded);
    }
    return failed;
}

int RemoteParamTable::SaveAll(const RemoteTable & where) const
{
    int failed = 0;
    for (const RemoteParam & p : params_)
        if (!p.Save(where)) failed++;
    return failed;
}

} // namespace model
} // namespace mim
