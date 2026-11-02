//Custom Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <alloca.h>
#include <malloc.h>

#include <csp/csp.h>
#include <csp/csp_error.h>
#include <csp/csp_endian.h>
#include <csp/csp_rtable.h>
#include <csp/csp_endian.h>
#include <csp/arch/csp_time.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_thread.h> 
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/drivers/usart.h>

//PARAM
#include <gs/param/rparam.h>
#include <gs/param/serialize.h>
#include <gs/param/table.h>
#include <gs/param/types.h>
#include <gs/util/log.h>
#include <gs/util/vmem.h>

//Custom CSP
#include <csp/switch.h>
#include <csp/delay.h>
// include <csp/csp_custom.h>
// #include "MIMAN_CSP/csp_io_custom.h"
// #include "MIMAN_CSP/csp_conn.h"

#include "miman_config.h"
#include "miman_radial.h"
#include "miman_csp.h"
#include "miman_imgui.h"
#include "miman_orbital.h"

extern Console console;

////rparam
Ptable_0 param0;
Ptable_1 param1;
Ptable_5 param5;
extern CoordTopocentric TrackingTopo;
extern StateCheckUnit State;


// GetResult / SetResult -> model/device/gs100_param_tables.cpp (파라미터 테이블 읽기만 쓴다)

//Actual Result Value is here.
uint8_t val_buf_u8;
uint16_t val_buf_u16;
uint32_t val_buf_u32;
int8_t val_buf_i8;
int16_t val_buf_i16;
int32_t val_buf_i32;
float val_buf_f;
extern Setup * setup;
int16_t lastRSSI = 0;
int def_timeout = 100;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;

void RadBufFree()
{
    val_buf_u8 = 0;
    val_buf_u16 = 0;
    val_buf_u32 = 0;
    val_buf_i8 = 0;
    val_buf_i16 = 0;
    val_buf_i32 = 0;
    val_buf_f = 0;
}

void * RadInitialize(void * args)
{
    Setup * setups = (Setup *) args;
    // rparam_set_quiet(RPARAM_NOT_QUIET);
    // rparam_query_reset();
    RadBufFree();

    Load_Paramtable0(NULL);
    Load_Paramtable1(NULL);
    Load_Paramtable5(NULL);
    set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
    return NULL;
}

void RSSI_Monitoring()
{
    gs_rparam_get_int16(setup->gs100_node, 4, 0x0004, 0xB00B, 100, &lastRSSI);
}

int16_t Return_RSSI()
{
    return lastRSSI;
}

// Load_Paramtable0 -> model/device/gs100_param_tables.cpp 로 이동

// Load_Paramtable1 -> model/device/gs100_param_tables.cpp 로 이동

// Load_Paramtable5 -> model/device/gs100_param_tables.cpp 로 이동



// Save_Paramtable0 -> model/device/gs100_param_tables.cpp 로 이동

// Save_Paramtable1 -> model/device/gs100_param_tables.cpp 로 이동

// Save_Paramtable5 -> model/device/gs100_param_tables.cpp 로 이동

void * Save2FRAM(void* param_id)
{
    int8_t table_id = *(int8_t*) param_id;
    uint8_t node = (uint8_t)setup->gs100_node;
    uint8_t port = AX100_PORT_RPARAM;
    gs_rparam_save(node, def_timeout, table_id, (uint8_t)table_id);
    set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
    usleep(500);
    // csp_reboot(setup->gs100_node);
    return NULL;
}

void SetRxFreq(uint32_t freq)
{
    //Set freq
    gs_rparam_set_uint32(setup->gs100_node, 1, param1.freq_addr, 0xB00B, 100, freq);
}

void SetTxFreq(uint32_t freq)
{
    //Set freq
    gs_rparam_set_uint32(setup->gs100_node, 5, param5.freq_addr, 0xB00B, 100, freq);
}

void SetRxBaud(uint32_t baud)
{
    //Set Baud
    gs_rparam_set_uint32(setup->gs100_node, 1, param1.baud_addr, 0xB00B, 100, baud);
}

void SetTxBaud(uint32_t baud)
{
    //Set Baud
    gs_rparam_set_uint32(setup->gs100_node, 5, param5.baud_addr, 0xB00B, 100, baud);
}

void UpdateFreq()
{
    gs_rparam_get_uint32(setup->gs100_node, 1, param1.freq_addr, 0xB00B, 100, &param1.freq);
    gs_rparam_get_uint32(setup->gs100_node, 5, param5.freq_addr, 0xB00B, 100, &param5.freq);
}

void UpdateBaud()
{
    gs_rparam_get_uint32(setup->gs100_node, 1, param1.baud_addr, 0xB00B, 100, &param1.baud);
    gs_rparam_get_uint32(setup->gs100_node, 5, param5.baud_addr, 0xB00B, 100, &param5.baud);
}

uint32_t GetRXbaud()
{
    return param1.baud;
}

uint32_t GetTXbaud()
{
    return param5.baud;
}