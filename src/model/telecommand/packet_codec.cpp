// Model - 지상국 내부 패킷(packetsign) <-> CSP 패킷 변환, 핑 패킷 생성, 수신 조각 모으기
//
// miman_coms.cpp 에서 PacketHandler / PingInit / PacketEncoder / PacketDecoder 를 내용 그대로 옮긴 것.
// 선언은 miman_coms.h 그대로.
#include "miman_config.h"
#include "miman_csp.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include <netinet/in.h>

extern Console console;
extern StateCheckUnit State;
extern Setup * setup;
extern char HKbuf[202];
extern int HKbufCursor;
extern char AODbuf[131];
extern int AODbufCufsor;

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
