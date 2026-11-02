/* Customs */
#include "miman_config.h"
#include "miman_csp.h"
#include "miman_coms.h"
#include "miman_imgui.h"
#include "miman_config.h"
#include "miman_orbital.h"
#include "miman_radial.h"
#include "miman_ftp.h"
#include <netinet/in.h>

#include <mutex>

#include "report_parser.h"
#include "telemetry_logger.h"


#define a sizeof(Beacon);
#define a sizeof(MissionBeacon);
#define a sizeof(GETFILEINFO);
#define a sizeof(Report)
#define a sizeof(Event)

#define _CRT_SECURE_NO_WARNINGS

extern FILE *log_ptr;
extern bool raw_data;
std::mutex g_report_view_mtx;
ReportView_t g_report_view;
//int signallen = 8;
Beacon* beacon = (Beacon *)malloc(MIM_LEN_BEACON);
MissionBeacon* missionbeacon = (MissionBeacon *)malloc(BEE_LEN_MISSIONBEACON);
Report* report = (Report *)malloc(BEE_LEN_REPORT);
Event* event = (Event *)malloc(BEE_LEN_EVENT);
GETFILEINFO* getfileinfo = (GETFILEINFO *)malloc(BEE_LEN_GETFILEINFO);
int NowFTP = 0;

char HKbuf[202];
int HKbufCursor = 0;
char AODbuf[131];
int AODbufCufsor = 0;
CmdGenerator_GS * SatCMD[256];
extern StateCheckUnit State;
extern pthread_mutex_t conn_lock;

// HK *NowHK;
// AOD *NowAOD;

extern Console console;
extern Setup * setup;

pthread_t LinkTrhead;

int BeaconCounter;
int MissionBeaconCounter;
int ReportCounter;
int PingCounter;
uint32_t remote_total_rx_bytes = 0;
uint16_t remote_boot_count = 0;


// REPORT_WIRE_SIZE -> model/telemetry/report_parser.h
static const unsigned int REPORT_CONN_DRAIN_TIMEOUT_MS = 100;

static uint8_t g_report_wire[REPORT_WIRE_SIZE];
static size_t  g_report_off = 0;
static bool    g_report_collecting = false;

static time_t  g_report_last_chunk_time = 0;
static bool    g_report_have_last_time = false;


// be16 -> model/telemetry/report_parser.cpp 로 이동
// be32u -> model/telemetry/report_parser.cpp 로 이동
// be32s -> model/telemetry/report_parser.cpp 로 이동


// DecodePayloadToView -> model/telemetry/report_parser.cpp 로 이동


// ParseReportWire540 -> model/telemetry/report_parser.cpp 로 이동


void * TRxController(void *)
{
    csp_socket_t * DLsocket = DL_sock_initialize();
    while(State.TRx_mode)
    {
        if(State.downlink_mode)
        {
            printf("Create downlink onorbit...\n");
            pthread_create(&LinkTrhead, NULL, task_downlink_onorbit, DLsocket);
            pthread_join(LinkTrhead, NULL);
            State.uplink_mode = true;
        }
        else
            continue;
    }
    return NULL;
}

void buf_allclear()
{
    memset(HKbuf, 0, sizeof(HKbuf));
    memset(AODbuf, 0, sizeof(HKbuf));
    HKbufCursor = 0;
    //AODBufCursor = 0;
}

void * SignalTest(void*)
{
    State.Signaltest = true;
    while(State.Signaltest)
    {
        if(csp_ping(19, 100, 1, 0))
            continue;
    }
    return NULL;
}

void now_rx_bytes_update()
{
    // if(beacon->UXTotRXByte != remote_total_rx_bytes)
    //     remote_total_rx_bytes = beacon->UXTotRXByte;
}

void set_rx_bytes(uint32_t nowbytes)
{
    remote_total_rx_bytes = nowbytes;
}

uint32_t get_rx_bytes()
{
    return remote_total_rx_bytes;
}

uint16_t get_boot_count()
{
    return remote_boot_count;
}

uint32_t * get_rx_bytes_address()
{
    return &remote_total_rx_bytes;
}

uint16_t * get_boot_count_address()
{
    return &remote_boot_count;
}

CmdGenerator_GS::CmdGenerator_GS(void) {
    uint8_t* pool = new uint8_t[MIM_DEFAULT_DATALEN];
    if (!pool) {
        console.AddLog("Generator init error: alloc failed");
        return;
    }
    memset(pool, 0, MIM_DEFAULT_DATALEN);
    this->CmdHeader = (CFE_MSG_CommandHeader*) pool;
}

CmdGenerator_GS::~CmdGenerator_GS(void) {
    uint8_t* pool = (uint8_t*) this->CmdHeader;
    delete[] CmdHeader;
}

int CmdGenerator_GS::GenerateCmdHeader(uint32_t MsgId, uint16_t FncCode, uint32_t Size, void* Data) {
    memset(this->CmdHeader->Msg.Byte, 0, Size);

    if (this->SetHasSecondaryHeader(true) != 0) {
        return -1;
    }
    if (this->SetSegmentationFlag(CFE_MSG_SEGFLG_UNSEG) != 0) {
        return -1;
    }
    if (this->SetMsgId(MsgId) != 0) {
        return -1;
    }
    if (this->SetSize(Size) != 0) {
        return -1;
    }
    if (this->SetFncCode(FncCode) != 0) {
        return -1;
    }
    if (this->GenerateChecksum() != 0) {
        return -1;
    }
    if (Data && (Size - sizeof(CFE_MSG_CommandHeader)) > 0)
        memcpy(this->CmdHeader->Msg.Byte + sizeof(CFE_MSG_CommandHeader), Data, Size - sizeof(CFE_MSG_CommandHeader));
    return 0;
}

void CmdGenerator_GS::CopyCmdHeaderToBuffer(uint8_t* Buffer) {
    memcpy(Buffer, this->CmdHeader->Msg.Byte, this->GetSize());
}

int CmdGenerator_GS::SetFncCode(uint16_t FncCode) {
    if (!this->HasSecondaryHeader()) {
        return -1;
    }
    this->CmdHeader->Sec.FunctionCode = FncCode;
    return 0;
}


void CmdGenerator_GS::SetHeaderWord(uint8_t* Word16, uint16_t Value, uint16_t Mask) {
    Word16[0] = (Word16[0] & ~(Mask >> 8)) | ((Value & Mask) >> 8);
    Word16[1] = ((Word16[1] & ~Mask) | (Value & Mask)) & 0xFF;
}


void CmdGenerator_GS::GetHeaderWord(const uint8_t* ByteField, uint16_t& Value, uint16_t Mask) {
    Value = (ByteField[0] << 8 | ByteField[1]) & Mask;
}

void CmdGenerator_GS::SetHeaderByte(uint8_t* Byte, uint8_t Value, uint8_t Mask) {
    Byte[0] = (Byte[0] & ~Mask) | (Value & Mask);
}

int CmdGenerator_GS::SetHasSecondaryHeader(bool HasSec) {
    SetHeaderByte(&this->CmdHeader->Msg.CCSDS.Pri.StreamId[0], HasSec, 0x08);
    return 0;
}

bool CmdGenerator_GS::HasSecondaryHeader(void) const {
    return (this->CmdHeader->Msg.CCSDS.Pri.StreamId[0] & (CFE_MSG_SHDR_MASK >> 8)) != 0;
}


int CmdGenerator_GS::SetSize(uint16_t Size) {
    if (Size < CFE_MSG_SIZE_OFFSET || Size > (0xFFFF + CFE_MSG_SIZE_OFFSET)) {
        return -1;
    }
    Size -= CFE_MSG_SIZE_OFFSET;
    SetHeaderWord(this->CmdHeader->Msg.CCSDS.Pri.Length, Size, 0xFFFF);
    return 0;
}

uint16_t CmdGenerator_GS::GetSize(void) {
    uint16_t RetVal;
    GetHeaderWord(this->CmdHeader->Msg.CCSDS.Pri.Length, RetVal, 0xFFFF);
    return RetVal + CFE_MSG_SIZE_OFFSET;
}

uint16_t CmdGenerator_GS::GetFncCode(void) const
{
    return this->CmdHeader->Sec.FunctionCode;
}

int CmdGenerator_GS::SetMsgId(uint16_t MsgId) {
    SetHeaderWord(this->CmdHeader->Msg.CCSDS.Pri.StreamId, MsgId, 0xFFFF);
    return 0;
}

int CmdGenerator_GS::SetSegmentationFlag(uint16_t SegFlag) {
    SetHeaderWord(this->CmdHeader->Msg.CCSDS.Pri.Sequence, SegFlag, CFE_MSG_SEGFLG_MASK);
    return 0;
}


int CmdGenerator_GS::GenerateChecksum(void) {
    CFE_MSG_CommandHeader* Cmd = this->CmdHeader;
    Cmd->Sec.Checksum = 0;
    Cmd->Sec.Checksum = this->ComputeCheckSum();
    return 0;
}

uint32_t CmdGenerator_GS::ComputeCheckSum(void) {
    uint16_t Len = this->GetSize();
    const uint8_t* BytePtr = this->CmdHeader->Msg.Byte;
    uint32_t chksum  = 0xFF;

    while (Len--) {
        chksum ^= *(BytePtr++);
    }
    return chksum;
}


csp_socket_t * DL_sock_initialize()
{
	csp_socket_t * sock = csp_socket(0);
	if(!csp_bind(sock, 13)) {
        console.AddLog("[OK]##TM Port 13 bind success.");
    }
    if(!csp_bind(sock, 31)) {
        console.AddLog("[OK]##BCN Port 31 bind success.");
    }
    if(!csp_bind(sock, 25)) {
        console.AddLog("[OK]##RPT Port 25 bind success.");
    }
    if(!csp_bind(sock, 27)) {
        console.AddLog("[OK]##Event Port 27 bind success.");
    }
        if(!csp_bind(sock, 23)) {
        console.AddLog("[OK]##Response Port 23 bind success.");
    }
        
        
    // while(true) {
    //     if (csp_bind(sock, 23) == 0) { // Add for HVD_TMTC_TEST
    //     console.AddLog("[OK]Bind Success.");
    //     break;
    //     };
    // }
	csp_listen(sock, 10);
    //Fail preventation would be needed!
    return sock;
}

// void * PacketDebugMsg(csp_packet_t * packet)
// {
//     char DebugMsg[1024];
//     (char *)packet->data
//     for(int i = 0; i < packet->length; i++)
//     {
//         sprintf(DebugMsg + i, "%")
//     }
// }
// UpdateReportViewFromReport -> model/telemetry/report_parser.cpp 로 이동


// BeaconSaver -> model/telemetry/telemetry_logger.cpp 로 이동



// MissionBeaconSaver -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpHex -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpArr_i16 -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpArr_u16 -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpArr_u8 -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpArr_u32 -> model/telemetry/telemetry_logger.cpp 로 이동

// DumpReportPayloadParsed_ByMidCc -> model/telemetry/telemetry_logger.cpp 로 이동




// ReportSaver -> model/telemetry/telemetry_logger.cpp 로 이동

// EventSaver -> model/telemetry/telemetry_logger.cpp 로 이동

void * task_downlink_onorbit(void * socketinfo) 
{
    csp_socket_t * sock = (csp_socket_t *)socketinfo;

    csp_packet_t * packet = NULL;
    packetsign * confirm = (packetsign *)malloc(MIM_LEN_PACKET);
    csp_conn_t * conn = NULL;

    //////////////////////////////////////////////////////////////////////////////
    bool image_packet_received = false;

    std::string filename = "/home/miman/Downloads/FTP_TEST.jpg";
    std::vector<uint8_t> image_packet_data(14956);
    std::vector<int> received_index;
    int index;
    int count;
    std::ofstream fout;

    std::map<uint16_t, std::map<uint16_t, std::map<uint8_t, std::vector<uint8_t>>>> rpt_map;
    /////////////////////////////////////////////////////////////////////////

    float seconds = 0.0f;

    while (State.downlink_mode) {

        conn = csp_accept(sock, setup->default_timeout);
        if (conn == NULL) {
            seconds += 0.5f;
            continue;
        }

        const int dport = csp_conn_dport(conn);
        unsigned int read_timeout = setup->default_timeout;

        if (dport == 25) {
            read_timeout = REPORT_CONN_DRAIN_TIMEOUT_MS;
        }

        while ((packet = csp_read(conn, read_timeout)) != NULL) {

            switch (dport) {

                case 23: {
                    if (packet->length == BEE_LEN_GETFILEINFO) {
                        char getfileinfofilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(getfileinfofilename,
                                "../data/response/GETFILEINFO--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("Received GETFILEINFO Response from port : %d.\n", dport);

                        FILE *GETFILEINFO_fp = fopen(getfileinfofilename, "wb");
                        printf("Received GETFILEINFO response Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (GETFILEINFO_fp) fprintf(GETFILEINFO_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (GETFILEINFO_fp) fprintf(GETFILEINFO_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(getfileinfo, 0, sizeof(*getfileinfo));
                        memcpy(getfileinfo, packet->data, BEE_LEN_GETFILEINFO);

                        if (GETFILEINFO_fp) fclose(GETFILEINFO_fp);
                    }

                    else {

                    char cosbcnfilename[128];
                    time_t tmtime = time(0);
                    struct tm *local = localtime(&tmtime);

                    sprintf(cosbcnfilename,
                            "../data/response/unknown--%04d-%02d-%02d-%02d-%02d-%02d--",
                            local->tm_year + 1900,
                            local->tm_mon + 1,
                            local->tm_mday,
                            local->tm_hour,
                            local->tm_min,
                            local->tm_sec);

                    console.AddLog("Received Response from port : %d.!!!!!!!!!\n", dport);

                    FILE *cosbcn_fp = fopen(cosbcnfilename, "wb");
                    printf("\nDownlink Length: %u", packet->length);

                    for (int i = 0; i < packet->length; i++) {
                        if (!(i % 10) && i != 0) {
                            printf("\n");
                            if (cosbcn_fp) fprintf(cosbcn_fp, "\n");
                        }
                        printf("0x%x ", packet->data[i]);
                        if (cosbcn_fp) fprintf(cosbcn_fp, "%02hhx\t", packet->data[i]);
                    }

                    if (cosbcn_fp) fclose(cosbcn_fp);

                    printf("Downlink Packet Length: %u\n", packet->length);
                    printf("===== Downlink PACKET DUMP =====\n");
                    for (int i = 0; i < packet->length; i++) {
                        if (!(i % 10) && i != 0) printf("\n");
                        printf("0x%02X ", packet->data[i]);
                    }
                    printf("\n===============================\n");
                }
                    break;
                }


                case 27: {
                    if (packet->length == BEE_LEN_EVENT) {
                        char eventfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(eventfilename,
                                "../data/event/event--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("Received Event from port : %d.\n", dport);

                        FILE *evnt_fp = fopen(eventfilename, "wb");
                        printf("\nEvent Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (evnt_fp) fprintf(evnt_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (evnt_fp) fprintf(evnt_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(event, 0, sizeof(*event));
                        memcpy(event, packet->data, BEE_LEN_EVENT);
                        EventSaver(event);

                        if (evnt_fp) fclose(evnt_fp);
                    }

                    printf("Event Packet Length: %u\n", packet->length);
                    printf("===== Event PACKET DUMP =====\n");
                    for (int i = 0; i < packet->length; i++) {
                        if (!(i % 10) && i != 0) printf("\n");
                        printf("0x%02X ", packet->data[i]);
                    }
                    printf("\n===============================\n");
                    break;
                }

                case 25: {
                    FILE *rpt_fp = NULL;

                    char rptpktfilename[128];
                    time_t tmtime = time(0);
                    struct tm *local = localtime(&tmtime);

                    struct timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);

                    sprintf(rptpktfilename,
                        "../data/report/rpt_raw--%04d-%02d-%02d-%02d-%02d-%02d-%03ld--",
                        local->tm_year + 1900,
                        local->tm_mon + 1,
                        local->tm_mday,
                        local->tm_hour,
                        local->tm_min,
                        local->tm_sec,
                        ts.tv_nsec / 1000000);

                    console.AddLog("Received Report from port : %d.", dport);
                    rpt_fp = fopen(rptpktfilename, "wb");

                    const time_t now = time(NULL);
                    const size_t chunk_len = packet->length;

                    printf("case25: chunk_len=%zu\n", chunk_len);

                    // ===== RAW DUMP (terminal + file) =====
                    printf("===== REPORT RAW CHUNK DUMP (%zu bytes) =====\n", chunk_len);
                    if (rpt_fp) fprintf(rpt_fp, "===== REPORT RAW CHUNK DUMP (%zu bytes) =====\n", chunk_len);

                    for (size_t i = 0; i < chunk_len; i++) {
                        printf("%02X ", packet->data[i]);
                        if (rpt_fp) fprintf(rpt_fp, "%02hhx ", packet->data[i]);

                        if ((i + 1) % 16 == 0) {
                            printf("\n");
                            if (rpt_fp) fprintf(rpt_fp, "\n");
                        }
                    }

                    if (chunk_len % 16 != 0) {
                        printf("\n");
                        if (rpt_fp) fprintf(rpt_fp, "\n");
                    }

                    printf("===========================================\n");
                    if (rpt_fp) fprintf(rpt_fp, "===========================================\n");
                    // ===== END RAW DUMP =====

                    if (g_report_collecting && g_report_have_last_time) {
                        double dt = difftime(now, g_report_last_chunk_time);
                        if (dt > 5.0) {
                            printf("case25: timeout dt=%.1f sec (>5). drop current report assembler.\n", dt);

                            g_report_collecting = false;
                            g_report_off = 0;
                            memset(g_report_wire, 0, sizeof(g_report_wire));

                            g_report_have_last_time = false;
                            g_report_last_chunk_time = 0;
                        }
                    }

                    if (!g_report_collecting) {
                        g_report_collecting = true;
                        g_report_off = 0;
                        memset(g_report_wire, 0, sizeof(g_report_wire));
                    }

                    g_report_last_chunk_time = now;
                    g_report_have_last_time = true;

                    if (g_report_off > REPORT_WIRE_SIZE) {
                        g_report_collecting = false;
                        g_report_off = 0;
                        memset(g_report_wire, 0, sizeof(g_report_wire));
                        g_report_have_last_time = false;
                        g_report_last_chunk_time = 0;
                    }

                    size_t remain = REPORT_WIRE_SIZE - g_report_off;

                    if (chunk_len > remain) {
                        printf("case25: overflow (off=%zu, chunk=%zu). drop current and restart with this chunk.\n",
                            g_report_off, chunk_len);

                        g_report_collecting = true;
                        g_report_off = 0;
                        memset(g_report_wire, 0, sizeof(g_report_wire));
                        remain = REPORT_WIRE_SIZE;
                    }

                    if (chunk_len <= remain) {
                        memcpy(g_report_wire + g_report_off, packet->data, chunk_len);
                        g_report_off += chunk_len;

                        printf("case25: assembled %zu/%zu\n", g_report_off, (size_t)REPORT_WIRE_SIZE);

                        if (g_report_off == REPORT_WIRE_SIZE) {
                            Report rpt;
                            bool ok = ParseReportWire540(g_report_wire, REPORT_WIRE_SIZE, rpt);

                            if (ok) {
                                printf("\ncase25: REPORT COMPLETE \nMsgId=0x%04x \nRefMID=0x%04x \nCC=0x%02x \nRetSize=%u\n",
                                    rpt.CCSDS_MsgId, rpt.ReflectedMID, rpt.ReflectedCC, (unsigned)rpt.RetValSize);
                                UpdateReportViewFromReport(rpt);
                                ReportSaver(&rpt);
                            } else {
                                printf("case25: ParseReportWire540 failed\n");
                            }

                            g_report_collecting = false;
                            g_report_off = 0;
                            memset(g_report_wire, 0, sizeof(g_report_wire));
                            g_report_have_last_time = false;
                            g_report_last_chunk_time = 0;
                        }
                    } else {
                        printf("case25: chunk too big to fit even after restart (%zu)\n", chunk_len);
                    }

                    if (rpt_fp) fclose(rpt_fp);
                    break;
                }


                case 13: {
                    if (packet->length == MIM_LEN_BEACON) {
                        char bcnpktfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(bcnpktfilename,
                                "../data/bcnpkt/bcnpktp13--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("Received Beacon from port : %d.", dport);

                        FILE *bcn_fp = fopen(bcnpktfilename, "wb");
                        printf("\nBeacon Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (bcn_fp) fprintf(bcn_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (bcn_fp) fprintf(bcn_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(beacon, 0, sizeof(*beacon));
                        memcpy(beacon, packet->data, MIM_LEN_BEACON);
                        BeaconSaver(beacon);

                        if (bcn_fp) fclose(bcn_fp);
                    }
                    else if (packet->length == BEE_LEN_MISSIONBEACON) {
                        char misnbcnpktfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(misnbcnpktfilename,
                                "../data/bcnpkt/misnbcnpkt--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("!!!!!!!!!Received Mission Beacon from port : %d.!!!!!!!!!", dport);

                        FILE *misnbcn_fp = fopen(misnbcnpktfilename, "wb");
                        printf("\nMission Beacon Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (misnbcn_fp) fprintf(misnbcn_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (misnbcn_fp) fprintf(misnbcn_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(missionbeacon, 0, sizeof(*missionbeacon));
                        memcpy(missionbeacon, packet->data, BEE_LEN_MISSIONBEACON);
                        MissionBeaconSaver(missionbeacon);

                        if (misnbcn_fp) fclose(misnbcn_fp);
                    }
                    else if (packet->length == BEE_LEN_GETFILEINFO) {
                        char getfileinfofilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(getfileinfofilename,
                                "../data/response/GETFILEINFO--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("Received GETFILEINFO Response from port : %d.", dport);

                        FILE *GETFILEINFO_fp = fopen(getfileinfofilename, "wb");
                        printf("Received GETFILEINFO response Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (GETFILEINFO_fp) fprintf(GETFILEINFO_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (GETFILEINFO_fp) fprintf(GETFILEINFO_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(getfileinfo, 0, sizeof(*getfileinfo));
                        memcpy(getfileinfo, packet->data, BEE_LEN_GETFILEINFO);

                        if (GETFILEINFO_fp) fclose(GETFILEINFO_fp);
                    }
                    else {
                        console.AddLog("Received Something but brocken.");

                        char unknownfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(unknownfilename,
                                "../data/unknown/port13/unknown_--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        FILE *unk_fp = fopen(unknownfilename, "wb");

                        printf("Unknown Packet Length: %u\n", packet->length);
                        printf("===== UNKNOWN PACKET DUMP =====\n");
                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) printf("\n");
                            printf("0x%02X ", packet->data[i]);

                            if (unk_fp) {
                                if (!(i % 10) && i != 0) fprintf(unk_fp, "\n");
                                fprintf(unk_fp, "%02hhx\t", packet->data[i]);
                            }
                        }
                        printf("\n===============================\n");

                        if (unk_fp) fclose(unk_fp);
                    }

                    break;
                }

                case 31: {
                    if (packet->length == MIM_LEN_BEACON) {
                        char bcnpktfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(bcnpktfilename,
                                "../data/bcnpkt/bcnpktp13--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("Received Beacon from port : %d.\n", dport);

                        FILE *bcn_fp = fopen(bcnpktfilename, "wb");
                        printf("\nBeacon Length: %u", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (bcn_fp) fprintf(bcn_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (bcn_fp) fprintf(bcn_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(beacon, 0, sizeof(*beacon));
                        memcpy(beacon, packet->data, MIM_LEN_BEACON);
                        BeaconSaver(beacon);

                        if (bcn_fp) fclose(bcn_fp);
                    }
                    else if (packet->length == BEE_LEN_MISSIONBEACON) {
                        char misnbcnpktfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(misnbcnpktfilename,
                                "../data/bcnpkt/misnbcnpkt--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        console.AddLog("!!!!!!!!!Received Mission Beacon from port : %d.!!!!!!!!!", dport);

                        FILE *misnbcn_fp = fopen(misnbcnpktfilename, "wb");
                        printf("\nMission Beacon Length: %u\n", packet->length);

                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) {
                                printf("\n");
                                if (misnbcn_fp) fprintf(misnbcn_fp, "\n");
                            }
                            printf("0x%x ", packet->data[i]);
                            if (misnbcn_fp) fprintf(misnbcn_fp, "%02hhx\t", packet->data[i]);
                        }

                        memset(missionbeacon, 0, sizeof(*missionbeacon));
                        memcpy(missionbeacon, packet->data, BEE_LEN_MISSIONBEACON);
                        MissionBeaconSaver(missionbeacon);

                        if (misnbcn_fp) fclose(misnbcn_fp);
                    }
                    else {
                        console.AddLog("Received Beacon but brocken.");

                        char unknownfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(unknownfilename,
                                "../data/unknown/port31/unknown--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        FILE *unk_fp = fopen(unknownfilename, "wb");

                        printf("Unknown Packet Length: %u\n", packet->length);
                        printf("===== UNKNOWN PACKET DUMP =====\n");
                        for (int i = 0; i < packet->length; i++) {
                            if (!(i % 10) && i != 0) printf("\n");
                            printf("0x%02X ", packet->data[i]);

                            if (unk_fp) {
                                if (!(i % 10) && i != 0) fprintf(unk_fp, "\n");
                                fprintf(unk_fp, "%02hhx\t", packet->data[i]);
                            }
                        }
                        printf("\n===============================\n");

                        if (unk_fp) fclose(unk_fp);
                    }
                    break;
                }

                case 12: {
                    char bcnpktfilename[128];
                    time_t tmtime = time(0);
                    struct tm *local = localtime(&tmtime);

                    sprintf(bcnpktfilename,
                            "../data/bcnpkt/bcnpktp13--%04d-%02d-%02d-%02d-%02d-%02d--",
                            local->tm_year + 1900,
                            local->tm_mon + 1,
                            local->tm_mday,
                            local->tm_hour,
                            local->tm_min,
                            local->tm_sec);

                    console.AddLog("Received Beacon from port : %d.", dport);

                    FILE *bcn_fp = fopen(bcnpktfilename, "wb");
                    printf("Beacon Length: %u", packet->length);

                    for (int i = 0; i < packet->length; i++) {
                        if (!(i % 10) && i != 0) {
                            printf("\n");
                            if (bcn_fp) fprintf(bcn_fp, "\n");
                        }
                        printf("0x%x ", packet->data[i]);
                        if (bcn_fp) fprintf(bcn_fp, "%02hhx\t", packet->data[i]);
                    }

                    if (packet->length == MIM_LEN_BEACON) {
                        memcpy(beacon, packet->data, MIM_LEN_BEACON);
                    } else {
                        console.AddLog("Received Beacon but brocken.");
                    }

                    if (bcn_fp) fclose(bcn_fp);
                    break;
                }

                default: {
                    if (dport == 1) {
                        csp_service_handler(conn, packet);
                    } else {
                        console.AddLog("Packet Received on unknown port %d", dport);
                        console.AddLog("Packet length is %d", packet->length);

                        char unknownfilename[128];
                        time_t tmtime = time(0);
                        struct tm *local = localtime(&tmtime);

                        sprintf(unknownfilename,
                                "../data/unknown/unknown--%04d-%02d-%02d-%02d-%02d-%02d--",
                                local->tm_year + 1900,
                                local->tm_mon + 1,
                                local->tm_mday,
                                local->tm_hour,
                                local->tm_min,
                                local->tm_sec);

                        uint16_t PacketLength = packet->length;
                        memcpy(confirm, packet->data, PacketLength);

                        FILE *TMTC_fp = fopen(unknownfilename, "wb");
                        if (TMTC_fp) {
                            for (int i = 0; i < PacketLength; i++) {
                                fprintf(TMTC_fp, "Data %d: %u\n", i, packet->data[i]);
                            }
                            fclose(TMTC_fp);
                        }
                    }
                    break;
                }
            }

            csp_buffer_free(packet);
            packet = NULL;
        }

        csp_close(conn);
        conn = NULL;

        RSSI_Monitoring();
    }

    printf("Downlink thread dead.\n");

    if (confirm) {
        free(confirm);
        confirm = NULL;
    }

    return NULL;
}





void * task_uplink_onorbit(void * sign_)
{

    State.uplink_mode = true;
    //State.downlink_mode = false;
    pthread_mutex_lock(&conn_lock);
    //while(!State.RotatorReadReady)
    //    continue;
    bool dlstate = true;


    //This funcion must be on p_thread[4]
    if((dlstate))
    {
        State.downlink_mode = false;
    }
        
    while(!State.uplink_mode)
        continue;
    uint32_t start, time = 0;
    start = csp_get_ms();
    packetsign * sign;
    csp_packet_t * packet;
    csp_conn_t* txconn;
    console.AddLog("[DEBUG]## Start Uplink Task.");

    sign = (packetsign *) sign_;
    // for(int i =0; i < sign->Length; i++)
    //     printf("PacketByte : %d", sign->Data[i]);
    //packet = PacketEncoder(sign);

    

    uint16_t Ptype = sign->PacketType;
    uint32_t Plen = sign->Length;
    uint16_t filetype;
    uint16_t filestatus;

    if(Ptype == MIM_PT_DLREQ)
    {
        filetype = ((dlreqdata *)(sign->Data))->target;
        filestatus = ((dlreqdata *)(sign->Data))->filestatus;
    }
    
    
    if(State.Debugmode)
    {
        for(int i = 0 ; i < sign->Length; i++)
            printf("%x\t", sign->Data[i]);
        printf("\n");
    }
    uint16_t tx_length = (uint16_t)(Plen + MIM_HAND_DATAFDSTART + 4);
    packet = PacketEncoder(sign);
    console.AddLog("[DEBUG]##Send Packet of Ptype : %u. Check Terminal Log.", Ptype);
    

    switch(Ptype){
        //Ptype 1 : Ping
        case MIM_PT_PING : {
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            if((confirm_ = csp_read(txconn, rx_delay_ms(Plen, setup->ax100_node))) != NULL && State.uplink_mode)
            {
                packetsign * confirm = PacketDecoder(confirm_);
                if(State.Debugmode)
                {
                    for(int i = 0; i < confirm->Length; i++)
                        printf("%d\t", confirm->Data[i]);
                    printf("\n");
                }
                

                uint16_t * retcode = (uint16_t *)&confirm->Data[0];
                uint16_t * pingcount = (uint16_t *)&confirm->Data[2];

                if(*retcode == 0)
                {
                    console.AddLog("[OK]##[OBC]Ping Success. retcode : %u, pingcount : %u", *retcode, *pingcount);
                    PingCounter ++;
                }
                else
                    console.AddLog("[ERROR]##[OBC]Ping Received but brocken. retcode : %u, pingcount : %u", *retcode, *pingcount);
                if(confirm != NULL)
                {
                    free(confirm);
                    confirm = NULL;
                }
            }
            else
                console.AddLog("[ERROR]##[OBC]Ping Failed.");
            
            if(confirm_!=NULL)
            {
                csp_buffer_free(confirm_);
                confirm_ = NULL;
            }
            break;
        }
        //Ptype 2 : Uplink Signal
        case MIM_PT_SIGNAL : {
            if(State.Debugmode)
            {
                for(int i = 0; i < packet->length; i++)
                {
                    printf("%x", packet[i]);
                }
                printf("\n");
            }
            
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                break;
            }
            console.AddLog("Uplink Signal TX Done.");
            //Ping Timeout : 10 sec
            break;
        }
        case MIM_PT_DLREQ : {
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            console.AddLog("Request FTP filelist.");
            if((confirm_ = csp_read(txconn, rx_delay_ms(Plen, setup->ax100_node))) != NULL && State.uplink_mode)
            {
                filelist * confirm = (filelist *)malloc(confirm_->length);
                packetsign* confirmlist =  PacketDecoder(confirm_);
                memcpy(confirm, confirmlist->Data, confirmlist->Length);
                if(confirm->filenum > 0)
                {
                    for(int i = 0; i < 64; i++)
                    {
                        memset(&State.ftplistup[i], 0, sizeof(ftpinfo));
                    }
                    console.AddLog("[OK]##Received File list.");
                    for(int i = 0; i < 64; i++)
                    {
                        
                        if(i < confirm->filenum)
                        {
                            console.AddLog("[DEBUG]##Make Lists : %d\tFileName: %u", i, confirm->file[i]);
                            // memcpy(&State.ftplistup[i], filelisthandler(confirm, filetype, filestatus, i), sizeof(ftpinfo));
                        }
                        else
                            continue;
                    }
                }
                else if(confirm->filenum == 0)
                    console.AddLog("[ERROR]##No more files.");
                else   
                    console.AddLog("[ERROR]##IO Error.");
                if(confirm!=NULL)
                {
                    free(confirm);
                    confirm = NULL;
                    sign = NULL;
                }
                    
            }
            else
            {
                console.AddLog("[ERROR]##Cannot Receive File List.");
            }
            //Ping Timeout : 10 sec
            if(confirm_!=NULL)
            {
                csp_buffer_free(confirm_);
                confirm_ = NULL;
            }
            break;
        }
        case MIM_PT_CMD : { //according to Ptype
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            if((confirm_ = csp_read(txconn, rx_delay_ms(Plen, setup->ax100_node))) != NULL)
            {
                packetsign* confirmlist =  PacketDecoder(confirm_);
                uint8_t * retcode = (uint8_t *)&confirmlist->Data[0];
                console.AddLog("[OK]Received Command Reply. Retcode %d CMD Count %d", *retcode, State.CMDCount);
                State.CMDCount ++;
                if(confirmlist!=NULL)
                {
                    free(confirmlist);
                    confirmlist = NULL;
                    sign = NULL;
                } 
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
            }
            else
                console.AddLog("[ERROR]##No Command Reply.");
            break;
        }
        case MIM_PT_TMTC_TEST : { // == 10. Add for TMTC Test
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            while(State.uplink_mode)
            {
                if(State.Scheduled)
                {
                    if ((txconn = csp_connect(CSP_PRIO_HIGH, 3, 13, MIM_DEFAULT_TIMEOUT, 0)) == NULL) {
                    /*!!!!!!!!!!!Revise setup->obc_node!!!!!!!!!!*/ //-> Change to 28.
                    /*!!!!!!!!!!!!Need to revise Port!!!!!!!!!!!*/
                        continue;
                    }
                    else
                        break;
                }
                else
                {
                    if ((txconn = csp_connect(CSP_PRIO_HIGH, 3, 13, MIM_DEFAULT_TIMEOUT, 0)) == NULL) {
                    /*!!!!!!!!!!!Revise setup->obc_node!!!!!!!!!!*/ //-> Change to 28.
                    /*!!!!!!!!!!!!Need to revise Port!!!!!!!!!!!*/
                        continue;
                    }
                    else
                        break;
                }

            }
            while (State.uplink_mode && txconn != NULL)
            {
                console.AddLog("[OK]CMD Packet Header: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
                    packet->data[0],packet->data[1],packet->data[2],packet->data[3],packet->data[4],packet->data[5],packet->data[6],packet->data[7]);
                fprintf(log_ptr, "|| Uplink || Length: %d\n",packet->length);
                    for(int i=0; i<packet->length; i++) {
                        if(!(i%10) && i !=0) {
                            fprintf(log_ptr, "\n");
                        }
                        fprintf(log_ptr, "%02hhx\t",packet->data[i]);
                    } fprintf(log_ptr,"\n\n");
                if(csp_send(txconn, packet, setup->default_timeout)) // Success. then,
                {   
                    packet = NULL; // discard packet and,
                    break; // End process.
                }
                else // Fail. then,
                    continue; //Go to loof, and try again.
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            // if((confirm_ = csp_read(txconn, rx_delay_ms(Plen, setup->ax100_node))) != NULL)
            // {
            //     packetsign* confirmlist =  PacketDecoder(confirm_);
            //     uint8_t * retcode = (uint8_t *)&confirmlist->Data[0];
            //     console.AddLog("[OK]Received Command Reply. Retcode %d CMD Count %d", *retcode, State.CMDCount);
            //     State.CMDCount ++;
            //     if(confirmlist!=NULL)
            //     {
            //         free(confirmlist);
            //         confirmlist = NULL;
            //         sign = NULL;
            //     } 
            //     if(confirm_!=NULL)
            //     {
            //         csp_buffer_free(confirm_);
            //         confirm_ = NULL;
            //     }
            // }
            // else
            //     console.AddLog("[ERROR]##No Command Reply.");
            break;
        }
        case MIM_PT_STCMD : {
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            if((confirm_ = csp_read(txconn, rx_delay_ms(Plen, setup->ax100_node))) != NULL && State.uplink_mode)
            {
                if(State.uplink_mode)
                {
                    packetsign* confirmlist =  PacketDecoder(confirm_);
                    uint8_t * retcode = (uint8_t *)&confirmlist->Data[0];
                    console.AddLog("[OK]##Get Command Reply. Retcode : %d", *retcode);
                    if(confirmlist!=NULL)
                    {
                        free(confirmlist);
                        confirmlist = NULL;
                        sign = NULL;
                    } 
                    if(confirm_!=NULL)
                    {
                        csp_buffer_free(confirm_);
                        confirm_ = NULL;
                    }
                }
            }
            else
                console.AddLog("[ERROR]##Cannot Receive Command Reply.");
            break;
        }
        case MIM_PT_NCCMD : {
            csp_packet_t * confirm_ = (csp_packet_t *)csp_buffer_get(MIM_LEN_PACKET);
            packetsign* confirmlist =  PacketDecoder(confirm_);
            while(State.uplink_mode)
            {
                if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, TX_PORT, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
                    continue;
                else
                    break;
            }
            while (State.uplink_mode && txconn != NULL)
            {
                if(csp_send(txconn, packet, setup->default_timeout))
                {
                    packet = NULL;
                    break;
                }
                else
                    continue;
            }
            if(txconn == NULL)
            {
                console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
                if(confirm_!=NULL)
                {
                    csp_buffer_free(confirm_);
                    confirm_ = NULL;
                }
                break;
            }
            if((confirm_ = csp_read(txconn,rx_delay_ms(Plen, setup->ax100_node))) != NULL)
            {
                if(State.uplink_mode)
                {
                    console.AddLog("[OK]##Get Command Reply. No Checksum.");
                    if(confirmlist!=NULL)
                    {
                        free(confirmlist);
                        confirmlist = NULL;
                        sign = NULL;
                    } 
                    if(confirm_!=NULL)
                    {
                        csp_buffer_free(confirm_);
                        confirm_ = NULL;
                    }
                }
            }
            else
                console.AddLog("[ERROR]##Cannot Receive Command Reply.");
            break;
        }
        default : {
            console.AddLog("[ERROR]##Undefied type of uplink packetsign. Type : %"PRIu16, Ptype);
            break;
        }
    }
    
    if(txconn != NULL)
    {
        csp_close(txconn);
        txconn = NULL;
    }
    
    if(packet!=NULL)
    {
        csp_buffer_free(packet);
        packet = NULL;
    }
    time = csp_get_ms() - start;
    console.AddLog("[DEBUG]##Communication Time : %u", time);

    State.uplink_mode = false;
    if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
    State.downlink_mode = true;
    pthread_mutex_unlock(&conn_lock);
    return NULL;
}

int PacketHandler(csp_packet_t *packet, int type, int NowCursor)
{
    while(1) {
        switch(type) {
            case 0: {
                type = packet->data[NowCursor];
                NowCursor ++;
                return PacketHandler(packet, type, NowCursor);
            }
            case 1: {
                // Declare NowHK as a pointer to HK struct
                // HK *NowHK; 
                for (HKbufCursor; HKbufCursor < sizeof(HKbuf); HKbufCursor ++)
                {
                    HKbuf[HKbufCursor] = packet->data[NowCursor];
                    NowCursor ++;
                    if (NowCursor > MIM_LEN_PACKET)
                    {
                        //Read All Packet
                        //Need to go next packet
                        NowCursor = 0;
                        return 0;
                    }
                }
                // NowHK = (HK *)HKbuf;
                buf_allclear();
                return NowCursor;
            }
            case 3: {
                // for (AODbufCursor; AODbufCursor < sizeof(AODbuf); AODbufCursor ++)
                // {
                //     AODbuf[AODbufCursor] = packet->data[NowCursor];
                //     NowCursor ++;
                //     if (NowCursor > MIM_LEN_PACKET)
                //     {
                //         //Read All Packet
                //         //Need to go next packet
                //         NowCursor = 0;
                //         return 0;
                //     }
                // }
                // NowAOD = (AOD *)HKbuf;
                // buf_allclear();
                // return NowCursor;
            }
            default :{
                
            }
        }
    }
}

packetsign * PingInit(FSWTle * FSWTleinfo)
{
    packetsign * sign;
    sign = (packetsign *)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
    sign->Identifier = MIM_ID;
    sign->PacketType = MIM_PT_PING;
    sign->Length = sizeof(FSWTle);
    // printf("Start. Length : %u, Second : %u\n", sign->Length, FSWTleinfo->sec);
    memcpy(sign->Data, (void *)FSWTleinfo, sizeof(FSWTle));

    if(FSWTleinfo != NULL)
    {
        // free(FSWTleinfo);
        FSWTleinfo = NULL;
    }
    

    return sign;
}

csp_packet_t * PacketEncoder(packetsign * sign,bool freeer)
{
    if(sign == NULL)
    {
        console.AddLog("[DEBUG]##NULL POINTER GIVEN TO PACKETENCODER");
        return NULL;
    }
    uint32_t len = sign->Length;
    csp_packet_t *packet = (csp_packet_t *)csp_buffer_get(len);    
    packet->length = len;
    printf("packet len: %u\n", packet->length);
    memcpy(packet->data, sign->Data, len);
    // uint32_t packetsignlen = sign->Length +4; // Data size + Identifier(uint16) + PacketType(uint16)
    // csp_packet_t * packet = (csp_packet_t *)csp_buffer_get(packetsignlen);
    // packet->length = packetsignlen;
    // console.AddLog("[DEBUG]##Encoding Pakcets...Lnegth : %u", packetsignlen);
    // //Copy Header
    // memcpy((packet->data), sign, 4);
    // memcpy((packet->data) + 4, sign->Data, packetsignlen-4 ); // copy data
    for (int i =0; i < packet->length; i++) {
        printf("0x%x ", packet->data[i]);
    }printf("\n");
    if(freeer)
        sign = NULL;
    return packet;
}

packetsign * PacketDecoder(csp_packet_t * packet)
{
    uint32_t packetsignlen = packet->length +4;
    packetsign * sign = (packetsign *)malloc(packetsignlen);
    memcpy(sign, (packet->data), 4);
    sign->Length = packet->length-4;
    memcpy(sign->Data, packet->data + 4, sign->Length);
    return sign;
}

int CmdGenerator_GS::Scheduling(uint32_t ExecutionTime, uint32_t ExecutionWindow, uint32_t EntryID, uint16_t GroupID)
{
    uint16_t CMDlen = this->GetSize() + 12;
    Book * container = (Book *)malloc(CMDlen);
    memset(container, 0, CMDlen);
    container->ExTime = ExecutionTime;
    container->ExWindow = ExecutionWindow;
    container->EntryID = EntryID;
    container->GroupID = GroupID;
    this->CopyCmdHeaderToBuffer(container->cmd);
    this->GenerateCmdHeader(TS_CMD_MID, TS_INSERT_SCHEDULE_ENTRY_CC, sizeof(CFE_MSG_CommandHeader) + CMDlen, (void *)container);
    
    return 0;
}

packetsign * CmdGenerator_GS::GenerateCMDPacket(void)
{
    packetsign * ResultPacket = (packetsign * )malloc(this->GetSize() + MIM_HAND_DATAFDSTART);
    ResultPacket->Identifier = MIM_ID;
    if(this->Scheduled)
        ResultPacket->PacketType = MIM_PT_STCMD;
    else if(!this->Checksum)
        ResultPacket->PacketType = MIM_PT_NCCMD;
    else
        ResultPacket->PacketType = MIM_PT_CMD;
    ResultPacket->Length = this->GetSize();
    console.AddLog("[DEBUG]##Packetsign Length : %u", ResultPacket->Length);
    memcpy(ResultPacket->Data, this->CmdHeader, ResultPacket->Length);
    return ResultPacket;
}

void * Direct_Shell(void * data)
{
    pthread_mutex_lock(&conn_lock);
    while(!State.RotatorReadReady)
        continue;
    bool dlstate = true;


    //This funcion must be on p_thread[4]
    if((dlstate))
    {
        State.downlink_mode = false;
    }
        
    while(!State.uplink_mode)
        continue;

    cmd_packet_t * ResultCMD = (cmd_packet_t *)data;
    reply_packet_t * ResultReply = new reply_packet_t;
    csp_conn_t * txconn;
    csp_packet_t * packet = (csp_packet_t *)csp_buffer_get(sizeof(cmd_packet_t));
    csp_packet_t * confirm = (csp_packet_t *)csp_buffer_get(sizeof(reply_packet_t));

    uint32_t start, time = 0;
    start = csp_get_ms();

    memcpy(packet->data, ResultCMD, sizeof(cmd_packet_t));
    packet->length = sizeof(cmd_packet_t);

    if(State.Debugmode)
    {
        printf("Direct Shell Data : ");
        for(int i = 0; i < sizeof(cmd_packet_t); i++)
            printf("%u\t", ((uint8_t *)packet->data)[i]);
        printf("\n");
    }


    while(State.uplink_mode)
    {
        if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, 27, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
            continue;
        else
            break;
    }
    while (State.uplink_mode && txconn != NULL)
    {
        if(csp_send(txconn, packet, setup->default_timeout *2))
        {
            packet = NULL;
            break;
        }
        else
            continue;
    }
    if(txconn == NULL)
    {
        console.AddLog("[ERROR]##Connection Buffer Busy. Skip this command.");
        if(confirm!=NULL)
        {
            csp_buffer_free(confirm);
            confirm = NULL;
        }
    }
    if((confirm = csp_read(txconn, 2 *rx_delay_ms(sizeof(MIM_LEN_PACKET), setup->ax100_node))) != NULL && State.uplink_mode)
    {
        if(State.uplink_mode)
        {
            memcpy(ResultReply, confirm->data, confirm->length);
            console.AddLog("[OK]## Received Shell Reply. Type : %"PRIu8", Result : %"PRIu8", Retcode : %d", ResultReply->type, ResultReply->result, ResultReply->retcode);
            if(confirm!=NULL)
            {
                csp_buffer_free(confirm);
                confirm = NULL;
            }
        }
    }
    else
        console.AddLog("[ERROR]##Cannot Receive Shell Reply.");
    
    if(txconn != NULL)
    {
        csp_close(txconn);
        txconn = NULL;
    }
    
    if(packet!=NULL)
    {
        csp_buffer_free(packet);
        packet = NULL;
    }
    time = csp_get_ms() - start;
    console.AddLog("[DEBUG]##Communication Time : %u", time);

    State.uplink_mode = false;
    if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
    pthread_mutex_unlock(&conn_lock);
    return NULL;
}