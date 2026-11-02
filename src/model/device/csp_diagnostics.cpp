// Model - CSP 진단 스레드 (노드 핑 스캔·콘솔 핑, 보드레이트/주파수 캘리브레이션)
//
// miman_csp.cpp 에서 내용 그대로 옮긴 것. 선언은 miman_csp.h 그대로.
#include "csp_conn.h"
#include "csp_promisc.h"
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
extern int fd_transciever;
extern int fd_transciever_serial;
extern csp_iface_t csp_if_kiss;
extern csp_kiss_handle_t csp_kiss_driver;
extern char WatchdogReadBuf[32];
extern Setup * setup;
extern struct usart_conf conf;

void * csp_ping_scan(void *) {
	pthread_mutex_lock(&conn_lock);
	uint32_t timeout = 1000;
	uint16_t size = setup->pingsize;
	uint8_t conn_options = 0;
	uint8_t nownode;
	bool scanmode = false;
	uint32_t start, time = 0;
	// uint8_t node = *((uint8_t *) nodedata);
	bool dlstate = State.downlink_mode;
	if((dlstate))
        State.downlink_mode = false;
	while(!State.uplink_mode)
        continue;
	State.uplink_mode = true;
	for(uint8_t i = 0; i < 32; i++)
	{
		int pingResult = csp_ping(i, 2000, size, CSP_O_NONE);
		if(pingResult >= 0)
			console.AddLog("Scanned AX100. Node : %"PRIu8", %"PRIu32"ms.\n", i, pingResult);
	}
	State.uplink_mode = false;
	if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
	pthread_mutex_unlock(&conn_lock);
    return NULL;
}

void * csp_baud_calibration(void *) {
	console.AddLog("[OK]##Baud rate Calibration Start");
	uint32_t baud_arr[]={1200,2400,4800,9600,19200}; // baud rate candidates
	uint32_t baud_cur_tx = 0;
	uint32_t baud_cur_rx = 0;
	bool flag = false;
	if(gs_rparam_get_uint32(setup->gs100_node, 1, 0x0004,0xB00B,100, &baud_cur_tx) ==GS_OK && gs_rparam_get_uint32(setup->gs100_node,5,0x0004,0xB00B,100,&baud_cur_rx)) {
		console.AddLog("[OK]##Current Baud rate TX: %u | RX: %u",baud_cur_tx,baud_cur_rx);
	}
	else {
		console.AddLog("[ERROR]##Can not Get baud rate. Try this later.");
	}
	// Change both (Rx & Tx) baud for Calibration
	for (int i=0; i<sizeof(baud_arr)/sizeof(uint32_t); i++) {
		baud_cur_tx = baud_arr[i];
		if(gs_rparam_set_uint32(setup->gs100_node, 5, 0x0004,0xB00B,100,baud_cur_tx)==GS_OK) { // Set Tx baud
			usleep(10000); // Sleep 0.01 Second per case
			for (int j= 0; j<sizeof(baud_arr)/sizeof(uint32_t); j++) {
				baud_cur_rx = baud_arr[j];
				if(gs_rparam_set_uint32(setup->gs100_node,1,0x0004,0xB00B,100,baud_cur_rx)==GS_OK) { //Set Rx baud
					usleep(10000); // Sleep 0.01 Second per case
					console.AddLog("[OK]##Current Baud rate TX: %u | RX: %u",baud_cur_tx,baud_cur_rx);
					int pingResult = csp_ping(setup->ax100_node,1000,setup->pingsize,CSP_O_NONE);
					usleep(10000); // Sleep 0.01 Second per case
					if (pingResult>=0) { // if ping success,
						console.AddLog("[OK]##Baud rate Calibration Success. Baud TX: %u | RX: %u",baud_cur_tx,baud_cur_rx);
						flag = true;
						break; //stop process
					}
					else {
						console.AddLog("[ERROR]##Ping Fail. Trying next candidate.");
						continue; //if ping fail, try next candidate
					}
				}
				else { // if Rxbaud rate change fail, try next candidate
					console.AddLog("[ERROR]##Can not Set Rx baud rate. Try this later.");
					continue;
				}
			}
			if (flag) break; // if ping success, stop process
		}
		else { // if Tx baud rate change fail, try next candidate
			console.AddLog("[ERROR]##Can not Set baud rate. Try this later.");
			continue;
		}
	}
	console.AddLog("[OK]##Frequency Calibration Finished. Freq Rx : %u | Tx : %u", baud_cur_rx, baud_cur_tx);
    return NULL;
}

void * csp_freq_calibration(void *) {
	console.AddLog("[OK]##Frequency Calibration Start");
	// similar to baud rate calibration
	// table #4 rssi -> get receive power
	// choose best rssi (received signal strength indicator)
	// Add check box in autopliot
	double center_freq = setup->default_freq; // 436500000 = 436.5MHz
	double offset = 100000; // step size: 0.1 MHz
	double freq_cur_rx = center_freq;
	double freq_cur_tx = center_freq;
	int16_t rssi; // dBm scale. often -100 is minimum.
	int16_t max_rssi = -100; // store current max rssi value
	double freq[2] = {0,}; // store current max Rx, Tx freq
	for (int i=-65; i<15; i++) { // Range: 430 ~ 438 MHz
		freq_cur_rx = center_freq + i*offset;
		if(gs_rparam_set_uint32(setup->gs100_node, 1, 0x0000,0xB00B,100,freq_cur_rx)==GS_OK) { //Rx
			usleep(10000); // Sleep 0.01 Second per case
			for (int j=-65; j<15; j++) { // Range: 430 ~ 438 MHz
				freq_cur_tx = center_freq + j*offset;
				if(gs_rparam_set_uint32(setup->gs100_node,5,0x0000,0xB00B,100,freq_cur_tx)==GS_OK) { //Tx
					usleep(10000); // Sleep 0.01 Second per case
					console.AddLog("[OK]##Current Frequency Tx: %u | Rx: %u",freq_cur_tx,freq_cur_rx);
					int pingResult = csp_ping(setup->ax100_node,1000,setup->pingsize,CSP_O_NONE);
					usleep(10000); // Sleep 0.01 Second per case
					if (pingResult>=0) { // if ping success,
						if(gs_rparam_get_int16(setup->gs100_node,4,0x0004,0xB00B,100,&rssi) ==GS_OK) {
							if (rssi > max_rssi) {
								freq[0] = freq_cur_rx;
								freq[1] = freq_cur_tx;
								max_rssi = rssi;
								console.AddLog("[OK]##Max RSSI Updated. Freq Tx: %u Rx: %u",freq_cur_tx,freq_cur_rx);
								continue;
							}
						}
					}
					else  {
						console.AddLog("[ERROR]##Ping Fail. Trying next candidate.");
						continue;
					}
				}
				else { // Tx setting fail
					console.AddLog("[ERROR]##Can not Set Tx Frequency. Try this later.");
					continue;
				}
				usleep(100000); // Sleep 0.1 Second per case
			}
		}
		else { // Rx setting fail
			console.AddLog("[ERROR]##Can not Set Rx Frequency. Try this later.");
			continue;
		}
	}
	// If whole loof finished, Setting frequency to Max RSSI.
	gs_rparam_set_uint32(setup->gs100_node,1,0x0000,0xB00B,100,freq[0]); // Set Rx
	gs_rparam_set_uint32(setup->gs100_node,5,0x0000,0xB00B,100,freq[1]); // Set Tx
	console.AddLog("[OK]##Frequency Calibration Finished. Freq Rx : %u | Tx : %u | RSSI : %d",freq[0],freq[1],max_rssi);
    return NULL;
}

void * csp_ping_console(void *)
{
	int pingResult = csp_ping(setup->ax100_node, 1000, setup->pingsize, CSP_O_NONE);
	if(pingResult >= 0)
	{
		console.AddLog("[OK]##Ping Success at node %u, %dms", setup->ax100_node, pingResult);
		PingCounter++;
	}
		
	else
		console.AddLog("[ERROR]##Ping Failed.");
    return NULL;
}

void * csp_ping_rdp_crc32(void *)
{
	int pingResult = csp_ping(setup->ax100_node, 1000, setup->pingsize, CSP_O_RDP | CSP_O_CRC32);
	if(pingResult >= 0)
		console.AddLog("[OK]##Ping Success at node %u, %dms", setup->ax100_node, pingResult);
	else
		console.AddLog("[ERROR]##Ping Failed.");
    return NULL;
}

void* csp_ping_everynode_console(void *arg)
{
    uint8_t node = *(uint8_t*)arg;  
    free(arg);                     

    int pingResult = csp_ping(node, 1500, setup->pingsize, CSP_O_NONE);

    if (pingResult >= 0) {
        console.AddLog("[OK]##Ping Success at node %u, %dms", node, pingResult);
        PingCounter++;
    } else {
        console.AddLog("[ERROR]##Ping Failed at node %u.", node);
    }

    return NULL;
}
