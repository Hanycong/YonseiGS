#include "ccsds_cmd.h"

#include <string.h>
#include <arpa/inet.h>

namespace mim {
namespace tc {

void EncodeCfeCommand(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode)
{
    EncodeCfeCommand(cmd, size, msgid, fnccode, (uint8_t)(size - 7));
}

void EncodeCfeCommand(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode, uint8_t length)
{
    uint8_t * header = (uint8_t *)cmd;
    uint16_t msgid_be = htons(msgid);
    memcpy(header + 0, &msgid_be, 2);
    header[2] = 0xC0;
    header[3] = 0x00;
    header[4] = 0x00;
    header[5] = length;
    header[6] = fnccode;
    header[7] = 0x00;

    uint8_t checksum = 0xFF;
    for (uint16_t i = 0; i < size; i++) checksum ^= header[i];
    header[7] = checksum;
}

void EncodeCfeCommandFcAt7(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode)
{
    uint8_t * header = (uint8_t *)cmd;
    uint16_t msgid_be = htons(msgid);
    memcpy(header + 0, &msgid_be, 2);
    header[2] = 0xC0;
    header[3] = 0x00;
    header[4] = 0x00;
    header[5] = (uint8_t)(size - 7);
    header[6] = 0x00;
    header[7] = fnccode;

    uint8_t checksum = 0xFF;
    for (uint16_t i = 0; i < size; i++) checksum ^= header[i];
    header[6] = checksum;
}

} // namespace tc
} // namespace mim
