/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk) 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "csp_conn.h"
#include "csp_promisc.h"

/* Custom only includes */
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_config.h"
#include "miman_radial.h"

#ifdef CSP_USE_PROMISC
extern csp_queue_handle_t csp_promisc_queue;
#endif

extern Console console;
extern StateCheckUnit State;
extern pthread_mutex_t conn_lock;
extern int PingCounter;

// csp_iface_t csp_if_kiss;
// csp_kiss_handle_t csp_kiss_driver;
int fd_transciever;
int fd_transciever_serial;

usart_callback_t mimsart_callback = NULL;

//Temp
//// GS100
csp_iface_t csp_if_kiss;
csp_kiss_handle_t csp_kiss_driver;
char WatchdogReadBuf[32];
extern Setup * setup;
struct usart_conf conf;

static const unsigned int k_rdp_default_window_size = 6;
static const unsigned int k_rdp_default_conn_timeout_ms = 30000;
static const unsigned int k_rdp_default_packet_timeout_ms = 16000;
static const unsigned int k_rdp_default_delayed_acks = 1;
static const unsigned int k_rdp_default_ack_timeout_ms = 8000;
static const unsigned int k_rdp_default_ack_delay_count = 3;

static const unsigned int k_rdp_ftp_conn_timeout_ms = 120000;
static const unsigned int k_rdp_ftp_packet_timeout_ms = 30000;

static pthread_mutex_t g_rdp_profile_lock = PTHREAD_MUTEX_INITIALIZER;
static bool g_rdp_ftp_profile_active = false;
static unsigned int g_saved_rdp_window_size = 0;
static unsigned int g_saved_rdp_conn_timeout_ms = 0;
static unsigned int g_saved_rdp_packet_timeout_ms = 0;
static unsigned int g_saved_rdp_delayed_acks = 0;
static unsigned int g_saved_rdp_ack_timeout_ms = 0;
static unsigned int g_saved_rdp_ack_delay_count = 0;

static void miman_apply_rdp_profile(unsigned int window_size,
                                    unsigned int conn_timeout_ms,
                                    unsigned int packet_timeout_ms,
                                    unsigned int delayed_acks,
                                    unsigned int ack_timeout_ms,
                                    unsigned int ack_delay_count)
{
    csp_rdp_set_opt(window_size, conn_timeout_ms, packet_timeout_ms,
                    delayed_acks, ack_timeout_ms, ack_delay_count);
}

void miman_restore_default_rdp_profile(void)
{
    miman_apply_rdp_profile(k_rdp_default_window_size,
                            k_rdp_default_conn_timeout_ms,
                            k_rdp_default_packet_timeout_ms,
                            k_rdp_default_delayed_acks,
                            k_rdp_default_ack_timeout_ms,
                            k_rdp_default_ack_delay_count);
}

void miman_begin_ftp_rdp_profile(uint32_t ftp_timeout_ms)
{
    pthread_mutex_lock(&g_rdp_profile_lock);
    if (g_rdp_ftp_profile_active) {
        pthread_mutex_unlock(&g_rdp_profile_lock);
        return;
    }

    csp_rdp_get_opt(&g_saved_rdp_window_size,
                    &g_saved_rdp_conn_timeout_ms,
                    &g_saved_rdp_packet_timeout_ms,
                    &g_saved_rdp_delayed_acks,
                    &g_saved_rdp_ack_timeout_ms,
                    &g_saved_rdp_ack_delay_count);

    unsigned int ftp_conn_timeout_ms = ftp_timeout_ms;
    if (ftp_conn_timeout_ms < k_rdp_ftp_conn_timeout_ms) {
        ftp_conn_timeout_ms = k_rdp_ftp_conn_timeout_ms;
    }
    if (ftp_conn_timeout_ms < g_saved_rdp_conn_timeout_ms) {
        ftp_conn_timeout_ms = g_saved_rdp_conn_timeout_ms;
    }

    unsigned int ftp_packet_timeout_ms = g_saved_rdp_packet_timeout_ms;
    if (ftp_packet_timeout_ms < k_rdp_ftp_packet_timeout_ms) {
        ftp_packet_timeout_ms = k_rdp_ftp_packet_timeout_ms;
    }

    csp_log_info("FTP RDP profile enable: conn_timeout=%u ms packet_timeout=%u ms window=%u ack_timeout=%u ms",
                 ftp_conn_timeout_ms,
                 ftp_packet_timeout_ms,
                 g_saved_rdp_window_size,
                 g_saved_rdp_ack_timeout_ms);

    miman_apply_rdp_profile(g_saved_rdp_window_size,
                            ftp_conn_timeout_ms,
                            ftp_packet_timeout_ms,
                            g_saved_rdp_delayed_acks,
                            g_saved_rdp_ack_timeout_ms,
                            g_saved_rdp_ack_delay_count);
    g_rdp_ftp_profile_active = true;
    pthread_mutex_unlock(&g_rdp_profile_lock);
}

void miman_end_ftp_rdp_profile(void)
{
    pthread_mutex_lock(&g_rdp_profile_lock);
    if (!g_rdp_ftp_profile_active) {
        pthread_mutex_unlock(&g_rdp_profile_lock);
        return;
    }

    miman_apply_rdp_profile(g_saved_rdp_window_size,
                            g_saved_rdp_conn_timeout_ms,
                            g_saved_rdp_packet_timeout_ms,
                            g_saved_rdp_delayed_acks,
                            g_saved_rdp_ack_timeout_ms,
                            g_saved_rdp_ack_delay_count);
    g_rdp_ftp_profile_active = false;

    csp_log_info("FTP RDP profile restore: conn_timeout=%u ms packet_timeout=%u ms window=%u ack_timeout=%u ms",
                 g_saved_rdp_conn_timeout_ms,
                 g_saved_rdp_packet_timeout_ms,
                 g_saved_rdp_window_size,
                 g_saved_rdp_ack_timeout_ms);
    pthread_mutex_unlock(&g_rdp_profile_lock);
}

void miman_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
	csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
}


int init_transceiver()
{
	// csp_debug_toggle_level(CSP_PACKET);
	// csp_debug_toggle_level(CSP_INFO);
	// csp_debug_toggle_level(CSP_PROTOCOL);
	// csp_debug_toggle_level(CSP_LOCK);
	State.GS100_Connection = true;
	csp_buffer_init(400, 2048);
	csp_init((uint8_t)setup->kiss_node);
	csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_kiss, CSP_NODE_MAC);
	// csp_route_set(setup->obc_node, &csp_if_kiss, setup->gs100_node);
	// csp_route_set(setup->eps_node, &csp_if_kiss, setup->gs100_node);
	for (int i = 1; i < 32; ++i) {
		int ret;
		if (i == setup->gs100_node || i == setup->kiss_node)
			continue;
		if ((ret = csp_route_set(i, &csp_if_kiss, setup->gs100_node)) != CSP_ERR_NONE) {
			printf("csp_rtable_set err: returned %d\n", ret);
		}
	}
	csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, usart_putc, usart_insert, "KISS");
	
	struct usart_conf usart;
	usart.device = setup->Transciever_devname;
	usart.baudrate = setup->Transceiver_baud;
	usart_init(&usart);

	usart_set_callback(miman_usart_rx);

	miman_restore_default_rdp_profile();
	csp_route_start_task(0, 0);
	
	return 0;
}



int ResetWatchdog()
{
	struct termios options;
	fd_transciever_serial = open("/dev/GS101", O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (fd_transciever_serial < 0) {
		return -1;
	}


	tcgetattr(fd_transciever_serial, &options);
	options.c_cflag = B500000;
    options.c_cflag |= (CLOCAL | CREAD); 	// Receiver and local mode
    options.c_cflag &= ~HUPCL;				// Do not change modem signals
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;					// 8 bit // @@ S대역 포지셔너(로테이터) 컨트롤러이랑 같은 옵션이듯 // @@ 시리얼 통신 설정방법: https://softtone-someday.tistory.com/15, https://blog.naver.com/choi125496/130034222760
    options.c_cflag &= ~CSTOPB;				// 1 stop bit
    options.c_cflag &= ~PARENB;				// No parity check
    options.c_cflag &= ~CRTSCTS;			// Enable hardware / software flow control

    options.c_iflag &= ~IXON;				// No handshaking

    // Raw output
    options.c_oflag &= ~OPOST; 				// Prevent special interpretation of output bytes (e.g. newline chars)

    // Canonical input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_cc[VTIME] = 5;
	options.c_cc[VMIN] = 1;

	tcsetattr(fd_transciever_serial, TCSANOW, &options);
	if (tcgetattr(fd_transciever_serial, &options) == -1)
		perror("error setting options");
	fcntl(fd_transciever_serial, F_SETFL, 0);

	/* Flush old transmissions */
	if (tcflush(fd_transciever_serial, TCIOFLUSH) == -1)
		printf("Error flushing serial port - %s(%d).\n", strerror(errno), errno);

	write(fd_transciever_serial, "\n\r", 2);
	read(fd_transciever_serial, WatchdogReadBuf, sizeof(WatchdogReadBuf));
	printf(WatchdogReadBuf);
	printf("---Done\n");
	write(fd_transciever_serial, "\n\r", 2);
	read(fd_transciever_serial, WatchdogReadBuf, sizeof(WatchdogReadBuf));
	printf(WatchdogReadBuf);
	printf("---Done\n");
	write(fd_transciever_serial, "\n\r", 2);
	read(fd_transciever_serial, WatchdogReadBuf, sizeof(WatchdogReadBuf));
	printf(WatchdogReadBuf);
	printf("---Done\n");
	sleep(0.5);
	write(fd_transciever_serial, "config gnd_wdt 172800\n\r", 23);
	read(fd_transciever_serial, WatchdogReadBuf, sizeof(WatchdogReadBuf));
	printf(WatchdogReadBuf);
	printf("---Done\n");
	sleep(0.5);
	write(fd_transciever_serial, "reset\n\r", 7);
	read(fd_transciever_serial, WatchdogReadBuf, sizeof(WatchdogReadBuf));
	printf(WatchdogReadBuf);
	printf("---Done\n");

	close(fd_transciever_serial);

	return 0;
}

void fin_transceiver()
{
	close(fd_transciever);
}


// csp_ping_scan -> model/device/csp_diagnostics.cpp 로 이동

// csp_baud_calibration -> model/device/csp_diagnostics.cpp 로 이동

// csp_freq_calibration -> model/device/csp_diagnostics.cpp 로 이동

// csp_ping_console -> model/device/csp_diagnostics.cpp 로 이동

// csp_ping_rdp_crc32 -> model/device/csp_diagnostics.cpp 로 이동


// For ping every node
// csp_ping_everynode_console -> model/device/csp_diagnostics.cpp 로 이동








//Cause Endian Issue, this function is modified.
void csp_debug_callback(char * filename)
{
	char cwd[4096];
	char today[64];
	getcwd(cwd, 4096);
	todaystr(today, 64);
	strcat(cwd, "/log/csp_debug_");
	strcat(cwd, today);
	strcat(cwd, ".log");
	memcpy(filename, cwd, 4096);
}

