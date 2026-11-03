#include "cmdgen5_common.h"

#include <string.h>
#include <arpa/inet.h>
#include <csp/csp_endian.h>
#include <fstream>
#include <sstream>
#include <string>

extern Console console;

static Command5 command5_storage;
Command5 * command5 = &command5_storage;

// ── CRC-32/ISO-HDLC ──────────────────────────────────────────────────────────
// init = 0xFFFFFFFF, poly = 0xEDB88320(reflected), xorout = 0xFFFFFFFF
uint32_t crc32_compute(const uint8_t* data, std::size_t len)
{
    uint32_t crc = 0xFFFFFFFFu;

    for (std::size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint32_t>(data[i]);

        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^
                  (0xEDB88320u &
                   static_cast<uint32_t>(
                       -(static_cast<int32_t>(crc & 1u))
                   ));
        }
    }

    return ~crc;
}

bool BuildAIOBCDataChunkCommand(AIOBC_DataChunkCmd_t &cmd,
                                       uint16_t msgid,
                                       uint8_t fnccode,
                                       uint16_t envelope_magic,
                                       uint16_t flags,
                                       uint16_t raw_data_length,
                                       uint16_t chunk_seq,
                                       uint16_t total_chunks,
                                       const std::vector<float> &row,
                                       uint32_t &chunk_crc32)
{
    if (row.size() < 34 || raw_data_length > sizeof(cmd.Data))
        return false;

    memset(&cmd, 0, sizeof(cmd));
    const uint16_t mid = htons(msgid);
    const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
    memcpy(cmd.CmdHeader, &mid, sizeof(mid));
    cmd.CmdHeader[2] = 0xC0;
    cmd.CmdHeader[4] = (uint8_t)(wire_length >> 8);
    cmd.CmdHeader[5] = (uint8_t)wire_length;
    cmd.CmdHeader[6] = fnccode;

    cmd.EnvelopeMagic = csp_hton16(envelope_magic);
    cmd.flags = csp_hton16(flags);
    cmd.chunk_seq = csp_hton16(chunk_seq);
    cmd.total_chunks = csp_hton16(total_chunks);
    cmd.RawDataLength = csp_hton16(raw_data_length);

    for (std::size_t i = 0; i < 34; ++i) {
        uint32_t bits;
        memcpy(&bits, &row[i], sizeof(bits));
        bits = csp_hton32(bits);
        memcpy(&cmd.Data[i], &bits, sizeof(bits));
    }

    chunk_crc32 = crc32_compute(reinterpret_cast<const uint8_t *>(cmd.Data),
                                raw_data_length);
    cmd.ChunkCRC32 = csp_hton32(chunk_crc32);

    uint8_t checksum = 0xFF;
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&cmd);
    for (std::size_t i = 0; i < sizeof(cmd); ++i)
        checksum ^= bytes[i];
    cmd.CmdHeader[7] = checksum;
    return true;
}

size_t UelysysCopyHexBytes(uint8_t *dst, size_t dst_size, const char *hex_text)
{
    auto HexValue = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };

    memset(dst, 0, dst_size);
    size_t count = 0;
    int high = -1;
    for (const char *p = hex_text; *p != '\0' && count < dst_size; ++p)
    {
        int value = HexValue(*p);
        if (value < 0)
            continue;
        if (high < 0)
        {
            high = value;
        }
        else
        {
            dst[count++] = (uint8_t)((high << 4) | value);
            high = -1;
        }
    }
    return count;
}

// AIOBC DATA CHUNK 의 CSV 파일: 한 줄에 float 34개. 숫자가 아닌 칸이 있으면 그 줄을 버리고 알린다
// (5차 GS 는 stof 예외로 프로그램이 죽었다)
std::vector<std::vector<float>> AIOBC_datachunk_parser(std::string path)
{
    std::vector<std::vector<float>> data;
    std::ifstream file(path);

    if (!file.is_open()){
        console.AddLog("Fail to open TC File");
        return data;
    }

    std::string line;
    int line_no = 0;
    while (std::getline(file, line)) {
        line_no++;
        std::string cell;
        std::stringstream ss(line);
        const int num_field = 34;
        std::vector<float> row;
        bool ok = true;

        for (int i = 0 ; i < num_field ; i++)
        {
            if (!std::getline(ss, cell, ','))
                break;
            try {
                row.push_back(std::stof(cell));
            } catch (const std::exception &) {
                console.AddLog("[ERROR]##AIOBC CSV line %d: not a number '%s'. Line skipped.", line_no, cell.c_str());
                ok = false;
                break;
            }
        }

        if (ok)
            data.push_back(row);
    }

    file.close();
    return data;
}
