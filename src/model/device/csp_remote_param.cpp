#include "csp_remote_param.h"

#include <gs/param/rparam.h>

#include "YonseiGS_config.h"

extern Setup * setup;

namespace mim {
namespace model {

bool CspRemoteParam::GetU8(uint8_t node, uint8_t table, uint16_t addr, uint8_t * value)
{
    return gs_rparam_get_uint8(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetU16(uint8_t node, uint8_t table, uint16_t addr, uint16_t * value)
{
    return gs_rparam_get_uint16(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetU32(uint8_t node, uint8_t table, uint16_t addr, uint32_t * value)
{
    return gs_rparam_get_uint32(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetS8(uint8_t node, uint8_t table, uint16_t addr, int8_t * value)
{
    return gs_rparam_get_int8(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetS16(uint8_t node, uint8_t table, uint16_t addr, int16_t * value)
{
    return gs_rparam_get_int16(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetS32(uint8_t node, uint8_t table, uint16_t addr, int32_t * value)
{
    return gs_rparam_get_int32(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::GetFloat(uint8_t node, uint8_t table, uint16_t addr, float * value)
{
    return gs_rparam_get_float(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::SetU8(uint8_t node, uint8_t table, uint16_t addr, uint8_t value)
{
    return gs_rparam_set_uint8(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::SetU16(uint8_t node, uint8_t table, uint16_t addr, uint16_t value)
{
    return gs_rparam_set_uint16(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

bool CspRemoteParam::SetU32(uint8_t node, uint8_t table, uint16_t addr, uint32_t value)
{
    return gs_rparam_set_uint32(node, table, addr, 0xB00B, setup->default_timeout, value) == GS_OK;
}

CspRemoteParam & RemoteParam()
{
    static CspRemoteParam rparam;
    return rparam;
}

} // namespace model
} // namespace mim
