#pragma once
#ifndef _MIMAN_FTP_H_
#define _MIMAN_FTP_H_

int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t * listent, void * data);
// void ftp_callback(const gs_ftp_info_t * info);
void * ftp_uplink_onorbit(void * param);
void * ftp_uplink_force(void * param);      // V1 (ftp_transfer.h)
void * ftp_downlink_onorbit(void * param);
void * ftp_downlink_force(void * param);
void * ftp_list_onorbit(void * );
void * ftp_move_onorbit(void * );
void * ftp_remove_onorbit(void * );
void * ftp_copy_onorbit(void * );
void * ftp_mkdir_onorbit(void * );
void * ftp_rmdir_onorbit(void * );
// 다운링크 파일 목록의 index 번째 번호 -> 원격(/mnt/ds/...)/로컬(data/...) 경로를 out 에 채운다.
// 모르는 종류(MIM_DLTYPE_*)/상태(MIM_DLSTAT_*)면 false. (누리호 5차 GS 판은 ftpinfo* 를 malloc 해 돌려주고 새었다)
bool filelisthandler(const filelist * list, uint16_t target, uint16_t status, int index, ftpinfo * out);


#endif