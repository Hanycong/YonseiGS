// Model - 수신 데이터 저장 파일용 글자 출력 도우미
//
// 리포트 저장 파일(telemetry_logger.cpp: BEE-1000, n5_report_logger.cpp: 누리호 5차 위성)이 같이 쓴다.
// 출력 형식은 저장 파일 형식 그대로다 (바꾸면 지상 분석 도구가 읽는 파일이 달라진다).
#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// 16진수 덤프: "XX " 를 16개마다 줄바꿈, 마지막 줄이 16개 미만이면 줄바꿈으로 끝낸다
inline void DumpHex(FILE *fp, const uint8_t *p, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        fprintf(fp, "%02X ", p[i]);
        if ((i + 1) % 16 == 0) fprintf(fp, "\n");
    }
    if (n % 16 != 0) fprintf(fp, "\n");
}

// "label: v0 v1 ... vn" 한 줄 (값 사이 공백 하나, 끝에 공백 없음)
template <typename T>
inline void DumpArrFmt(FILE *fp, const char *label, const T *aa, int n, const char *fmt)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) {
        fprintf(fp, fmt, aa[i]);
        if (i != n - 1) fputc(' ', fp);
    }
    fprintf(fp, "\n");
}

inline void DumpArr_i16(FILE *fp, const char *label, const int16_t *aa, int n)  { DumpArrFmt(fp, label, aa, n, "%" PRIi16); }
inline void DumpArr_u16(FILE *fp, const char *label, const uint16_t *aa, int n) { DumpArrFmt(fp, label, aa, n, "%" PRIu16); }
inline void DumpArr_u8 (FILE *fp, const char *label, const uint8_t *aa, int n)  { DumpArrFmt(fp, label, aa, n, "%" PRIu8); }
inline void DumpArr_u32(FILE *fp, const char *label, const uint32_t *aa, int n) { DumpArrFmt(fp, label, aa, n, "%" PRIu32); }
