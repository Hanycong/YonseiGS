# 누리호 5차 GS(GS_nuri5) 기능 비교

- **BASE**: YonseiGS가 갈라져 나온 코드. YonseiGS `be6d8c5`(2026-06-02)
- **NURI5**: `DSS/GS_nuri5` main `5ad9f91`(2026-11-03, 56커밋). 실제 5차 발사 운용 버전
- **CURRENT**: 리팩토링 브랜치 `claude/magical-clarke-1nv81b`. 구조는 이쪽을 유지

| 파일 | BASE | NURI5 |
|---|---|---|
| `miman_imgui.cpp` | 15,746줄 | 40,611줄 (TC 생성기 28.8k, 비콘·리포트 창 6.5k) |
| `miman_coms.cpp` / `.h` | 2,645 / 875줄 | 8,119 / 2,650줄 |
| `miman_config.h` | 5,488줄 | 8,220줄 |

- **이력**: NURI5 첫 커밋 `8b17432`(06-04)가 트리 전체를 넣은 import라서 BASE와 공통 이력이 없다. 그래서 06-04 이전부터 달랐던 부분은 "포크 시점부터 다름"으로 적었다.
- **비교 방법**: CRLF 변경이 섞인 커밋이 많아 `diff -w --strip-trailing-cr`로 비교했다.
- **결론**: CURRENT는 사실상 BASE를 MVVM으로 나눈 것이다. 아래 NURI5 신규 기능은 표에 따로 적지 않았으면 모두 **CURRENT에 없음**이다.


> **이식 상태 (2026-11-04)**: 아래 표의 기능은 모두 이 저장소로 옮겼다 (브랜치 `claude/magical-clarke-1nv81b`).
> 예외와 결정: BEE-1000 은 예전 동작 유지(EPS P60, 포트 25 리포트 540B 파싱, 포트 23/13, 명령 목록, 예약 송신),
> Nuri5 에서 지운 기능은 지우지 않음, 7절의 Nuri5 버그는 옮기지 않고 고침(TC 메뉴 결함은 CLAUDE.md Known defects).

---

## 1. 한눈에 보기

| # | 영역 | NURI5에서 추가/변경된 것 | 규모 | CURRENT |
|---|---|---|---|---|
| 1 | 위성 모드 | BEE1000/BEE1012/UELYSYS/SD1SAT 4종. 위성별 HMAC 키 전환, TC 로그, 명령 목록 | 핵심 기반 | 3종, HMAC 전환은 버그 상태 |
| 2 | TC 생성기 | 명령 약 1,000개(BEE1012 cFE, UELYSYS, SD1SAT, ROMA, LGPM, Lothan, AIOBC…) | 28.8k줄 | BEE1000/COSMIC만 |
| 3 | 비콘 수신·표시 | BEE1012 / UELYSYS / SD1SAT 비콘 포트·구조체·화면. FTP 중 비콘 버퍼링 | 중 | 없음 |
| 4 | 리포트 | CCSDS 길이 기반 조립기, 다중/Critical/OPS 리포트, SD1SAT 조각 재조립, 리포트 종류 137개 추가 | 대 | 없음(고정 540B) |
| 5 | 업링크 | 패킷 타입 `BEE1000`(노드 지정), AIOBC, PAYYONSEI/SDR. `Record_TC_Packet` | 중 | 없음 |
| 6 | EPS | P60 → **P80**(같은 MID 0x1875, CC 다름) | 중 | P60 |
| 7 | FTP | Ryu 엔진(V3), Stop/이어받기, Dest Node(SLT), V1 force 부활, gscsp libftp_client 수정 | 대 | 없음 |
| 8 | Control | UTRX WDT CLEAR, EPS WDT CLEAR ALL 버튼. Ping(OBC) 삭제 | 소 | 없음 |
| 9 | AMP | `AmpOn()` 단일 진입점과 타이머 중단 플래그. Disabled 클릭 시 재가동 | 소 | 부분 |
| 10 | 빌드/CSP | `libmimancsp.a`(XTEA 끔, QoS 켬), `csp_buffer_init` 2048, `FTP_Ryu::client` | 중 | ilovemozart, 400 |
| 11 | 기타 | `obc_node` 3→10, 로그 경로 `../log/*`, `../data/<SAT>/...` | 소 | 없음 |

NURI5에서 **빠진 것**도 있다. CURRENT에는 남아 있으므로 지우면 안 된다.
- BEE1000/COSMIC TC 약 140개
- 초기 운용 명령 세트
- Scheduler(STCMD), NCCMD 송신
- SC/PAYUZUC/UANT Burn TC, EPS P60, 포트 13 TM
- Ping(OBC) 버튼, FTP V1/Listup 라디오, AutoPilot 간이 CMD 생성기
- 위성 탭(`ActiveSatIndex`), ConsoleLine

NURI5에서 왜 지웠는지는 커밋 메시지로 알 수 없다. 운용에서 쓰는지 확인이 필요하다.

---

## 2. 위성 모드 / 공통 기반

| 기능 | NURI5 위치 | 도입 | CURRENT | 비고 |
|---|---|---|---|---|
| `SatelliteGuiProfile{name,hmac_key}` 4종, `GetCurrentSatelliteMode/Key()` | imgui 276–325 | `2e3ccb4` 06-05, SD1SAT은 `40b28ac` 08-25 | 없음(`svc_radio.cpp:22` 3종) | 849곳에서 사용 |
| 위성을 바꾸면 GS100 HMAC 자동 설정. BEE1000이면 TX HMAC off(table5 0x0010=0), 나머지는 16B 0-패딩 키를 table5 0x0020에 GS_PARAM_DATA로 기록 | imgui 327–371 | `2e3ccb4` | **버그**: BASE 그대로라 빈 키를 table1·5에 `set_string` | 반환값을 확인하지 않음 |
| 위성별 TC 로그 `SwitchTcLogFile`, `../data/<SAT>/tc_log`(BEE1000은 `../log/TC_Log`), `tc_log_lock` | main.cpp 29–80 | `6847e6a` 08-06 | 없음(`../data/BEE_TC_Log`) | |
| TC마다 파일로 기록하는 `Record_TC_Packet(pkt, satKey)` | coms 8066–8119 | `66b2fd1` 10-15 | 없음 | **버그**: fopen 실패 시 NULL에 fprintf, `data_dir[50]` strcat |
| 로그 경로 `../log/{Console,Debug,TC}_Log` | config.h 186–217 | `5ded872` | 없음 | |
| `obc_node` 기본값 3 → 10 | main 147 | `8830df1` 06-08 | 없음(3) | 송신 노드도 하드코딩 3에서 `setup->obc_node`로 바뀜 |
| State: `AmpTimerStop`, `dest_node` 추가. `Scheduled` 등 삭제 | config.h 422–505 | | 없음 | |

## 3. TC 명령 생성기

- **공통 형식**: cFE 8B 헤더 `[MID BE][C0 00][len=size-7][CC][XOR 체크섬(시작 0xFF)]` + LE payload를 `packetsign{HVD_TEST, MIM_PT_TMTC_TEST}`로 감싸 `p_thread[4]`로 송신한다. 패킷 안에 HMAC은 없고 GS100에서 처리한다.
- **번호 체계**: 범용 0–9, 위성/앱별 1000~12xxx. `CMD_LABEL_MAX` 13000. 위성 모드에 따라 서브시스템 콤보가 바뀐다(bee1012 29, uelysys 35, sd1sat 19).
- **표 기반 명령**: `Bee1012CfeCommands[]` 표(필드 offset/kind/검증)와 범용 편집기 `DrawBee1012CfeCommandEditor`로 180개를 처리한다. CURRENT의 `CommandDef` + `EncodeCfeCommand`를 확장할 모델로 쓰기 좋다.

| 위성 | 앱/그룹 (MID) | 명령 수 | NURI5 위치 | CURRENT |
|---|---|---|---|---|
| 공통 | 범용/수동(u8~float). NURI5에서 CSP node 2B 접두 + `PacketType=BEE1000`(`5ad9f91` 11-03), Schedule 삭제, FC/체크섬 뒤바뀜(D13) 수정 | 10 | 9611–10105 | BASE판(D13 보존, node 없음) |
| BEE1012 | AIOBC (0x18F0, node 14 / port 10, BE payload, DataChunk CRC-32, CSV 업로드) | 8 | 9236–9610 | 없음 |
| BEE1012 (UELYSYS 재사용) | cFE 코어 표: ES 25 / EVS 22 / SB 12 / TBL 11 / TIME 21 / SRL 8 | 99 | 표 8325–8866 | ES 4개만 |
| BEE1012 | THRUST (0x1870: ARM / MAIN_FIRE / CG_PULSE …) | 16 | 표 | 없음 |
| BEE1012/UELYSYS | ci_lab 3, ds 19, fm 18, hk 2, to_lab 9, rpt 5, gpio 18, mission 3, ltrx 9, sch_lab 5 | 91 | 10106–25230 | fm 4, to_lab 7(BASE판) |
| BEE1012 | pay_slt (0x1830) 12, paybee_kisscam (0x1833, PAYUZUC 개명) 12 | 24 | 12590–13995 | PAYUZUC만 |
| BEE1012/UELYSYS | utrx AX100 29, stx 42, adcs 99(SET 이전 + GET 37 / SEQ / COMM 신규), eps **P80** 24, ttc 타임라인 21, MEOW 36, gps OEM 35 | 286 | 13996–24864 | adcs SET, utrx GNDWDT만 |
| UELYSYS | PAYYONSEI_OBC 원격(TEMP / AOS / LGBAT / IMU / IWSB) 43, SDR_CONTROLLER 21, LGPM 1 (표 기반, 전용 PacketType) | 65 | 표 9600–9920 | 없음 |
| UELYSYS | eo 6, payuel_aos 6*, payuel_lgbat 5*, payuel_yonsei 3, payuel_roma 34, ROMA ISL 7(가변 길이 SHOUT), payuel_lgpm 24(전원/RWA), payuel_obc 13, payuel_cam 11, uant 14, gpio 10, sch_lab 1, sp(AR6 전개) 10, stx QUICKFORMAT 2, ttc BEACON_BURST 1 | 약 147 | 25233–28910, 37752–37910 | uant 4(BEE1000 같은 MID), ROMA/LGPM/LGBAT는 카테고리 이름만 |
| SD1SAT | ci 7, cr 8, ops 6, ttc 21, meow 24, uant 9, utrx AX100 21, to 24, eps P60(0x1980) 24, paysd1_roma 41, **paysd1_lothan** 28, SNBP 13, gps OEM 38, fm 14 + 레거시 4 | 약 300 | 28911–37751 | 없음 |

\* aos/lgbat는 필터가 주석 처리되어 콤보에서 비어 있다(죽은 코드).

## 4. 비콘 / 리포트 / 다운링크

| 기능 | NURI5 위치 | 도입 | CURRENT |
|---|---|---|---|
| RX 포트 bind 확장(15개). BEE1000 31/25/27/23, UELYSYS 12/10/11/14, BEE1012 17/15/16/18/29, SD1SAT 20/21/22. **포트 13 TM 삭제**, 23은 FM GETFILEINFO로 변경 | coms 3435–3495 | `5ded872` 외 | 13/31/25/27/23 |
| BEE1012 비콘(포트 17, 210B) 수신·저장 + 화면 약 53행 | coms 4180–, imgui 1020–1103 | `85d27fc` 06-06 | 없음 |
| UELYSYS 비콘(포트 12, 211B) 수신·저장 + 화면 약 57행 | coms 4241–, imgui 1105–1192 | `85d27fc` | 없음 |
| SD1SAT 비콘(포트 22, 93B) 수신·저장 + 화면 약 31행 | coms 4302–, imgui 1194–1247 | `8b86729` 08-26 | 없음 |
| FTP 중 비콘 버퍼링(17/12/22를 메모리에 모았다가 FTP 후 저장, CSP_CONN_MAX 고갈 방지) | coms 3112–3256 | `5ad9f91` 11-03 | 없음 |
| BEE1000 비콘 저장 경로 `../data/BEE1000/beacon/...`, 구조체 이름 `BEE1000_Beacon_t`(레이아웃 같음) | coms 7101– | | BASE 경로 |
| BEE1000 비콘 화면 "RPT Command Counter"가 ErrCounter를 보여 주던 버그 수정 | imgui 1322 | `85d27fc` | **버그 남아 있음**(`beacon_tab.cpp:71`) |
| 리포트 조립기 `AssembleReportPortChunk`: 고정 540B 대신 CCSDS 길이로 자르고 버퍼 21KB, raw는 `.bin` | coms 2996–3110 | `934e60a` 08-28 | 없음 |
| 다중 엔트리 리포트(0x0825), Critical(0x0826), RPT OPS(0x0827) | coms 2582–2690 | `934e60a` | 없음 |
| SD1SAT 포트 20 조각 재조립(4B 헤더, 5s timeout) + CR envelope 파싱 | coms 2703–2995 | `76b9687` | 없음 |
| 리포트 디코더: EPS P80(PowerIf / HK / HK_ALL / BCN / RPARAM 재조립), UELYSYS 앱 17종, BEE1012 LTRX / KISSCAM / PAY_SLT / THRUST, PAYYONSEI / SDR, SD1SAT 10종, ADCS GET 약 30종, ROMA / LGPM | coms 98–2542 | 여러 커밋 | ADCS 일부만 |
| 리포트 화면: 위 종류별 표, `payload_valid` 검사, hex dump 개선, RPT Ops 헤더 | imgui 1766–7222 | | 없음 |
| 위성별 Report Saver `../data/<SAT>/report/...` | coms 6464–6676 | | BASE 경로 |
| FM GETFILEINFO(포트 23/18/14), AIOBC TM(29), SD1SAT TO(21) 저장 | coms 6762–7280 | | 없음 |

## 5. 업링크 / Control / AMP

| 기능 | NURI5 위치 | 도입 | CURRENT |
|---|---|---|---|
| PacketType `BEE1000`(=14): Data 앞 2B를 목적 노드로 떼어 포트 13으로 송신 | coms 7736–7782 | `5ad9f91` | 없음 |
| PacketType `B12_UL_AIOBC`(node 14, port 10), `UEL_UL_PAYYONSEI_OBC` / `SDR_CONTROLLER`(node 13/14, port 13) | coms 7784–7880 | | 없음 |
| `MIM_PT_STCMD` / `NCCMD`, 명령 응답 수신, Scheduling 삭제. 업링크 종료 시 `downlink_mode=true` 강제 삭제, `free(sign)` 추가 | | `5ded872` | BASE 동작 |
| Control: UTRX WDT CLEAR(0x1850 CC6), EPS WDT CLEAR ALL(P80) 버튼 추가, Ping(OBC) 삭제 | imgui 37946–38030 | `66b2fd1` 10-15 | 없음 |
| `AmpOn()` 단일화(mutex, `AmpTimerStop`으로 이전 타이머 중단, 타이머 100ms sleep). AutoPilot도 이것을 사용 | config.cpp 17–89 | `5ad9f91` | 부분: `svc::AmpOn`은 있지만 중단 플래그가 없어 busy-loop. JoinThread가 최대 14분 블록될 수 있음 |
| AMP 버튼 "Disabled" 클릭 시 동작: BASE는 OFF, NURI5는 **재가동** | imgui 228 | `5ad9f91` | BASE 동작 |
| `log_ptr` 접근을 `LockTcLog`로 보호 | coms 7685–, imgui 38679 | | 없음 |

## 6. FTP / CSP / 빌드

| 기능 | NURI5 위치 | 도입 | CURRENT |
|---|---|---|---|
| Ryu FTP 엔진(`lib/libftp_client_ryu`, CSP 포트 19). `ftp_version==3`이면 사용, OLD는 포트 9 | ftprdp 66–98, 331–389 | `c195b0a` 06-16 | 없음 |
| FTP Stop + 이어받기: OLD/Ryu 모두 200ms 단위 중단, map 보존, `gs_ftp_process` / `busy`, GUI "Stopping..." | ftprdp 117–464, imgui 144–166 | `66b2fd1`, `5ad9f91` | 없음 |
| Ryu 다운로드 이어받기 타임아웃 무한 반복 수정(FTP-MISSING-COUNT) | ftpnew_client.c | `5ad9f91` | 없음 (OLD client.c에도 같은 버그가 있는데 고치지 않음) |
| "FTP with SLT": 모든 FTP host를 GUI Dest Node(`State.dest_node`)로 지정 | ftprdp 전체, imgui 38417– | `49beec2` 07-22 | 없음 |
| V1 force 경로(포트 15, force.c) 부활 | ftpfcd 219–305 | | 없음 (`/mnt/ds/*` 하드코딩) |
| gscsp libftp_client 대폭 수정(UL/DL info enum, `ftp_abort` / `ftp_avail`, chunk 185) | lib/gscsp | 여러 커밋 | 없음 (API 비호환) |
| 링크 라이브러리 `libmimancsp.a`(XTEA **off**, QoS **on**) | CMakeLists 90, wscript | 포크 시점부터 다름 | `libilovemozart.a`(XTEA on, QoS off) |
| `csp_buffer_init(2048, 2048)` | csp.cpp 65 | | 400 |
| csp_debug.c 강화(포맷 스트링, fopen NULL 검사) | libcsp | `106d8e5` | 없음 |
| 로테이터 읽기 실패 시 1s 대기(BASE는 30s) | orbital 907 | | 30s |
| "fixed TLE error" = `GeneratePassList` 등의 return 누락 UB 수정. **Alpha-5와 다른 문제** | orbital | `92b4991` 08-13 | 이미 수정됨. Alpha-5는 CURRENT에만 있음 |

`docs/ftp_ryu_migration`의 최종 판정은 **NO_GO**(06-16)였다. 사유는 OBC wire 호환 미입증, 실기 전송 없음이다. 이 문서는 이후 삭제됐다. 10월 운용 로그에는 Ryu를 쓴 기록이 있지만, 다시 GO로 판정한 문서는 찾지 못했다.

CURRENT(BASE)에만 있는 FTP 안전장치가 있다. 30s 쿨다운, 중복 실행 차단, chunk 200 상한, FTP RDP 프로파일, `conn_lock`이다. NURI5 기능을 옮길 때 이것들과 합쳐야 한다.

---

## 7. 이식 전에 결정·확인할 것 (위험도 순)

1. **MID 충돌로 리포트 분류가 틀어진다.** `DetermineReportKind`가 UELYSYS MID를 먼저 검사한다. 그래서 BEE-1000 UANT GET_STATUS(0x1840 = UELYSYS_UANT)와 TO_LAB(0x1823)이 UELYSYS로 분류된다. SDR(0x1840), PAYYONSEI IMU(0x1894 = GPS), I2C(0x1870 = THRUST)는 도달할 수 없다. SD1SAT 안에서도 MEOW = HK(0x1950), TTC = DS(0x1930)로 겹친다. → 이식할 때 **수신 포트(위성)를 1차 키**로 판별해야 한다.
2. **EPS P60 → P80**은 같은 MID에 CC 의미가 다르다. 위성별로 EPS 정의를 분리하지 않고 덮어쓰면 P60 위성에 잘못된 TC가 나간다. 어느 위성이 P60/P80인지 확인이 필요하다.
3. **BEE-1000 리포트 프레이밍이 바뀐다**(540B 고정 → CCSDS 길이). FSW의 길이 필드가 정확해야 한다. raw 저장도 hex txt에서 `.bin`으로 바뀌어 후처리 도구 호환을 확인해야 한다.
4. **gscsp 두 갈래**: XTEA/QoS 설정과 libftp_client API가 다르다. 운용 기준(NURI5)으로 통일할지 먼저 결정해야 한다.
5. **Ryu FTP**: NO_GO 문서가 남아 있으므로 OLD/RYU를 함께 두고 선택하는 구조를 유지하는 것이 안전하다.
6. **NURI5 버그(그대로 옮기지 말 것)**
   - uplink와 autopilot이 둘 다 `free(sign)`/`free(pkt)` → double free
   - NCCMD 패킷이 조용히 송신되지 않음
   - 포트 29가 길이 확인 없이 `event`에 memcpy
   - SD1SAT 리포트 12B over-read
   - `Record_TC_Packet` NULL fp
   - `#define a sizeof(...)`가 헤더로 이동함
   - HMAC 전환 결과를 확인하지 않음 + GUI 스레드에서 `pthread_join`
   - Beacon 테이블 `BeginTable` 중괄호 누락(세 버전 공통)
   - gps 12010 fnccode 기본값 3(10이어야 함), 숨은 case 8209/10523
   - DS REMOVE_MID/CLOSE_ALL CC가 뒤바뀐 것으로 의심(FSW 확인 필요)
   - 디렉터리를 만들지 않아 새 경로 저장이 조용히 실패
   - parsed 파일명이 초 단위라 덮어써짐
7. **위험 명령에 확인 절차가 없다.** THRUST FIRE/ARM, DEP_BURN, gpio, LGPM 전원, MEOW SHELL/SHUTDOWN, APPS_PERM_OFF, 펌웨어 갱신, ci GATE, utrx BAUD/REBOOT, Lothan OTA. CURRENT의 `Criticality::Hazardous`에 매핑해야 한다.
8. **NURI5 저장소에는 바이너리와 로그가 커밋되어 있다.** `build/BEE-1000`, `.a`, `.so`, `build/log` 216MB, `lib/gscsp` 루트의 실수로 생긴 빈 파일들. 이식할 때 가져오지 않는다.

## 8. 제안하는 이식 순서 (CURRENT 구조 기준)

1. 기반: 위성 모드 4종, HMAC 전환 수정, 위성별 TC 로그. 위치는 `svc_radio`, `control_vm`, `app/`.
2. 공통 정의: `YonseiGS_config.h`에 넣지 말고 위성/앱별 헤더로 분리한다(full rebuild를 피하기 위해).
3. 다운링크: 포트 bind와 dispatch는 `YonseiGS_coms`, 조립기와 디코더는 `telemetry/report_parser`, Saver는 `telemetry_logger`. 판별은 포트 우선으로 한다.
4. 비콘·리포트 화면: `view/beacon/<sat>/`, `view/beacon/report/<sat>_*.cpp`.
5. TC: `command_catalog`에 미션 BEE1012 / UELYSYS / SD1SAT를 추가하고 표 기반 `CommandDef`로 데이터화한다. 범용 명령은 NURI5판(node, D13 수정)을 따른다.
6. 업링크 타입과 Control 버튼, `AmpOn` 개선.
7. FTP: Ryu, Stop, Dest Node. gscsp 결정 후 진행하고 CURRENT 안전장치와 합친다.
8. CSP 설정(버퍼, XTEA/QoS), `obc_node`, 로그 경로.
