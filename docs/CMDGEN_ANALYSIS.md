# DrawCmdGeneratorBody 분석과 분할 설계

> 대상: `src/miman_imgui.cpp` `DrawCmdGeneratorBody(bool initial_mode)` (L4768–13618, 8,851줄)
> 분석 기준 커밋: `5a9137d`. 이 문서는 코드를 바꾸지 않는다. 설계안은 사용자 승인 후 단계별로 진행한다.

## 1. 이 함수가 실제로 하는 일

한 줄 요약: **운용자가 고른 위성 명령(telecommand)의 파라미터를 입력받아, cFS 비행 소프트웨어가 이해하는
CCSDS 명령 패킷으로 조립하고, 지상국 송신 큐(TX 슬롯)에 넣는다.**

호출하는 곳은 두 군데이고, 같은 함수와 같은 상태(static 변수, 전역 `gen_fnccode`)를 공유한다.

| 창 | 호출 | 의미 |
|---|---|---|
| `TMTC Command Test` | `DrawCmdGeneratorBody(false)` | 전체 명령 (서브시스템 필터 가능) |
| `Initial Operations` | `DrawCmdGeneratorBody(true)` | 초기 운용(LEOP)용 23개 명령만 (`INITIAL_CMD_INDICES`) |

### 1-1. 명령 한 개가 위성까지 가는 데이터 흐름

```
[View]     서브시스템 콤보(FilterRules) → 명령 콤보(Templabels) → 파라미터 입력 위젯
              │  (입력 위젯이 전역 Command 구조체 필드나 static 변수에 직접 바인딩됨)
[Encode]   CCSDS Primary Header  : StreamId(MID, BE) | Seq 0xC000 | Length = 전체-7
           cFE Secondary Header  : FunctionCode(CC) | Checksum(0xFF XOR 전체)
           Payload               : 명령별 packed struct (리틀엔디안, 타깃 CPU 그대로)
[Envelope] packetsign { Identifier = HVD_TEST(23), PacketType = MIM_PT_TMTC_TEST(10), Length, Data }
[Audit]    WriteSystemName(msgid) → TC 로그 파일에 서브시스템 이름 기록, printf 헥사 덤프
[Transport] mim::svc::WaitTx() → mim::svc::SendUplink(pkt)
           → task_uplink_onorbit: MIM_PT_TMTC_TEST 분기 → csp_connect(node 3 = OBC, port 13) → csp_send
```

### 1-2. 이미 있는 것과의 관계

- `miman_coms`의 **`CmdGenerator_GS` 클래스**가 같은 일(CCSDS 헤더, 체크섬, packetsign 생성)을 이미 한다.
  다만 CMD Manager(`SatCMD[]`, `popup_cmd`) 경로에서만 쓰이고, 엔벨로프가 다르다
  (`MIM_ID` + `MIM_PT_CMD/STCMD/NCCMD` → 응답을 기다리는 다른 송신 분기).
  DrawCmdGeneratorBody는 이 클래스를 쓰지 않고 헤더 조립을 152번 손으로 반복한다.
- 명령의 MID, CC, payload 구조체(사실상 ICD)는 `miman_config.h` L700–5450에 있다.
  전역 `Command * command`는 131개 명령 구조체를 모두 담은 거대한 구조체이며 **입력 폼의 저장소로 쓰인다.**

## 2. 기능 분류 (cFS 비행 소프트웨어 앱 기준)

| 분류 | case | 명령 수 | 줄 | 대상 (MID) | 성격 |
|---|---|---|---|---|---|
| Generic (raw typed) | 0–9 | 10 | 555 | 사용자 입력 MID/CC | 디버그: 임의 MID+CC+인자 1개(u8~float) |
| EPS (GomSpace P60) | 10–46 | 37 | 1,656 | 0x1875 | 채널 on/off, 테이블 조회, GND WDT, 리셋 |
| ADCS | 47–89 | 39 | 2,706 | 0x1865 | 모드/구성/기준벡터. 72–86은 고정 프리셋 |
| SC (Stored Command) | 136–152 | 17 | 797 | 0x1820 | ATS/RTS 시작·정지·그룹 (시간 지정 명령) |
| TO_LAB (Telemetry Output) | 153–159 | 7 | 418 | 0x1823 | 비콘/텔레메트리 스트림 추가·제거 |
| UANT (UHF 안테나) | 160–163 | 4 | 252 | 0x1840 | 상태 조회, 버닝, 자동 전개 |
| SP (태양전지판 전개) | 164–167 | 4 | 203 | 0x1871 | 패널 1~4 전개 |
| PAYUZUC (카메라 탑재체) | 168–180 | 13 | 790 | 0x1830 | 촬영, 노출, 다운로드, 레지스터 |
| FM / TO 파일 | 201, 202, 204, 213 | 4 | 235 | 0x1815 | 파일 삭제·이름변경·정보·디렉터리 |
| UTRX | 206 | 1 | 53 | 0x1850 | GND WDT 클리어 |
| ES (cFE Executive) | 216–221 | 6 | 329 | 0x1800 | 앱 시작·정지·재로드·조회 |
| COSMIC (다른 위성) | 203, 207–215 | 9 | 694 | 십진 MID (6257 등) | 타 미션 명령, 다중 패킷 시퀀스 |
| 실험 | 205 | 1 | 78 | 0x1819 | "Beacon Mukbang" |
| **합계** | | **152** | **8,766** | | |

운용 관점의 두 번째 축도 있다 (지금 코드에는 표현되어 있지 않음):

- **조회/무해**: NoOp, Get*, Query*, Report* — 몇 번 보내도 상태가 바뀌지 않음
- **설정**: Set*Config, Set Mode, Add/Remove Packet — 상태를 바꾸지만 되돌릴 수 있음
- **위험(되돌릴 수 없음)**: SP Deploy, UANT Burn/Auto Deploy, ADCS Reset, EPS Reset, ES Stop App — 확인 절차가 필요한 명령

## 3. 패킷 조립 방식의 변형 (같아 보이지만 다른 코드)

| 변형 | 대상 case | 특징 |
|---|---|---|
| A. 전역 구조체 + 체크섬 바이트 초기화 | 69개 (EPS 10–21·25·34–36, ADCS 72–86, TO_LAB, UANT 161–163, PAYUZUC, FM, ES 217–221, COSMIC 209–213) | `CmdHeader[7]=0` 후 XOR |
| B. 전역 구조체 + **초기화 없음** | 62개 (EPS 22–24·26–33·37–46, ADCS 47–71·88·89, SC 136–140·144–151, 216) | 이전 체크섬이 남은 채 XOR (결함 D1) |
| C. 지역 배열 + 손으로 쓴 길이 | 0–9, 160, 164–167, 214, 215 | `cmd[5]=0x01` 등 길이를 상수로 기입 |
| D. 지역 배열 + `htons(sizeof-7)` | 205, 206 | 길이를 계산 |
| E. 바이트 하드코딩 + 고정 CRC | 207, 208 | 여러 패킷 연속 전송, `sleep()`으로 간격 |

공통점: 152개 모두 엔벨로프는 `HVD_TEST / MIM_PT_TMTC_TEST`로 같다. Payload 바이트 순서 변환은 없다
(리틀엔디안 그대로). 길이 필드는 대부분 하위 바이트만 쓰지만(`{0x00, sizeof-7}`), 현재 가장 큰
명령이 136바이트라 넘치지 않는다 (255+7을 넘는 명령이 생기면 깨진다).

## 4. 분석 중 발견한 결함 (리팩토링과 분리해서 처리)

| ID | 내용 | 영향 | 근거 |
|---|---|---|---|
| D1 (수정됨) | 변형 B 62개 명령은 체크섬 바이트를 비우지 않고 XOR한다. 전역 구조체에 이전 체크섬이 남아 있어서 **같은 명령을 다시 보내면 체크섬이 틀린다** (1·3·5번째만 유효) | 위성이 명령을 거부할 수 있음 (cFE 표준 검증 기준) | ADCS NoOp 시뮬레이션: 0x43 → 0x00 → 0x43 … |
| D2 | Schedule 체크 시(case 0, 4) `sch_cmd_t*`를 `packetsign*`로 넘긴다. 송신 태스크가 entnum/exenum을 Identifier/PacketType/Length로 읽음 | 예약 명령 경로가 동작하지 않거나 잘못된 길이로 버퍼 요청 | case 0 L4912 |
| D3 | case 203(COSMIC FTP Send File) 길이 필드가 0x4D(77) 고정, 실제 `sizeof-7`=80 | 타깃이 길이 검증하면 거부 | sizeof 측정 |
| D4 | case 216 라벨은 "ES NoOp"인데 구현은 EPS Get Dock Info(case 45 복사, 같은 구조체 공유) | 운용자가 다른 명령을 보냄 | 구조체 `epsp60getdockinfocmd` 공유 |
| D5 | case 89(ADCS Open-Loop RW)가 ADCS 필터 범위(47–88) 밖 | "ADCS" 필터에서 안 보임 | FilterRules |
| D6 | 라벨만 있고 구현 없는 명령 14개 (78, 83, 84, 87, 126–135) | 골라도 폼이 비어 있음 | Templabels |
| D7 | 필터 항목 5개(LGPM, LTRX, STX, PAYUEL_ROMA, PAYUEL_LGBAT)에 명령이 하나도 없음 | 빈 목록 | FilterRules 230–342 |
| D8 | `WriteSystemName`의 MID→이름 표가 BEE-1000 MID와 맞지 않음 (EPS 0x1875=6261이 "FM"으로, ADCS 0x1865는 기록 안 됨) | TC 로그 오기 | miman_config.cpp L140 |
| D9 | "Write Raw Data" 체크박스(`raw_data`)를 읽는 코드가 없음 | 효과 없는 UI | 전역 검색 |
| D10 | 207/208이 UI 스레드에서 `sleep(1)`·`WaitTx()`로 대기, `sleep(0.1)`은 정수 0초 | 화면 멈춤, 의도한 간격 아님 | case 207/208 |
| D11 | `malloc`한 packetsign을 아무도 `free`하지 않음 | 명령마다 메모리 누수 | task_uplink_onorbit |
| D12 | case 2("u16")가 9바이트 버퍼(`HDR+sizeof(uint8_t)`)에 2바이트 인자를 복사하고 10바이트로 체크섬 계산, 전송 길이는 9 | 스택 1바이트 넘침. Release(_FORTIFY_SOURCE) 빌드는 버튼 누르면 abort, 아니면 인자 상위 바이트가 잘림 | 골든 하네스 실행 중 fortify abort |

## 5. 분할 설계안

### 5-1. 목표 구조 (텔레커맨드 체인을 계층별로 분리)

```
View        view/cmdgen/cmd_generator_view.cpp     명령 선택 콤보, 공통 버튼
            view/cmdgen/forms_<subsystem>.cpp      서브시스템별 입력 폼 (ImGui만)
              │
ViewModel   viewmodel/cmd_generator_vm.cpp         선택 상태, 필터, 초기운용 목록, Generate 동작
              │
Model       model/cmd/cmd_dictionary.cpp           명령 사전: id, 라벨, 앱, MID, CC, 분류, 초기운용 여부
            model/cmd/ccsds_cmd.cpp                CCSDS/cFE 인코더 (헤더, 길이, 체크섬) — 순수 함수
            model/cmd/cmds_<app>.cpp               앱별 payload 기본값과 조립 (EPS, ADCS, SC, ...)
            model/cmd/adcs_presets.cpp             ADCS 고정 구성값 (휠, 위성, 제어기 …)
              │
Service     svc_tx: SendTelecommand(bytes)         엔벨로프 + TC 로그 + WaitTx + SendUplink
            svc_procedure (후순위)                  다중 패킷 시퀀스(207/208)를 TX 스레드에서 실행
```

- `cmds_<app>` 의 `<app>` 은 **cFS 앱 단위**로 나눈다: generic, eps, adcs, sc, to_lab, uant, sp, payuzuc,
  fm, utrx, es, cosmic. 위성 FSW의 앱 경계와 같아서, 위성 쪽 ICD가 바뀌면 고칠 파일이 하나로 정해진다.
- 명령의 "정체"(MID, CC, 라벨, 분류)는 **데이터(사전)** 로, "조립"은 **인코더 하나**로, "입력"은 **폼**으로 분리한다.
  지금은 이 셋이 case 안에 섞여 있고, 라벨·필터·초기운용 목록은 서로 다른 세 곳의 숫자 범위로 따로 관리된다.

### 5-2. 단계 (각 단계 = 커밋, 동작 동일성 검증)

| # | 단계 | 바뀌는 것 | 바뀌지 않는 것 | 검증 |
|---|---|---|---|---|
| 0 | **골든 패킷 하네스** | 모든 명령의 조립 결과 바이트를 덤프하는 테스트 도구 | 소스 | 기준 바이트 파일 생성 |
| 1 | 명령 사전 추출 | 라벨·필터·초기운용 목록을 `cmd_dictionary` 표 하나로 | 콤보에 보이는 순서·내용 | 화면 A/B, 콤보 항목 덤프 비교 |
| 2 | case 분할 (기계적) | 각 case를 `Form_X()`(UI)와 `Build_X()`(패킷 조립)로, 앱별 파일로 이동 | 조립 코드 본문(그대로 이동) | 골든 바이트 152개 동일 |
| 3 | 인코더 통일 | 변형 A–D를 `ccsds_cmd` 하나로 교체 | 바이트 (첫 전송 기준) | 골든 바이트 동일 |
| 4 | 송신 경로 통일 | 엔벨로프/로그/대기를 `svc_tx::SendTelecommand`로 | 송신 순서, 로그 | 골든 + ui_snapshot |
| 5 | ViewModel 도입 | 선택·필터·Generate를 VM으로, View는 ImGui만 | 화면 | ui_snapshot |
| 6 | 시퀀스 분리 (선택) | 207/208을 svc_procedure로 | 패킷 내용 (타이밍은 바뀜 → 동작 변경이라 별도 승인) | 골든 |

**골든 패킷 하네스**가 이 리팩토링의 안전장치다. 152개 명령을 실제로 조립해서 바이트를 파일로 남기고,
매 단계 후 다시 덤프해서 1바이트라도 다르면 실패시킨다. 화면 스냅샷으로는 확인할 수 없는
"위성에 나가는 패킷이 같은가"를 직접 확인한다.

### 5-3. 결정이 필요한 것

1. **D1(체크섬) 수정 순서**: 리팩토링 전에 별도 `fix:` 커밋으로 먼저 고칠지.
   3단계에서 인코더를 하나로 합치면 이 결함은 자연히 사라지므로, 그 전에 명시적으로 고쳐 두어야
   "리팩토링이 동작을 바꿨다"는 혼동이 없다. → **먼저 고치는 것을 권장.**
2. **D2(Schedule), D3(길이), D4(216 라벨)**: 위성 FSW 쪽 기대값을 아는 사람이 확인해야 하는 항목.
3. **COSMIC 명령의 위치**: BEE-1000 명령 사전과 분리된 미션 프로필로 둘지 (위성 전환 기능과 연결).
4. **위험 명령 확인 절차**: 분류만 메타데이터로 넣고 동작은 그대로 둘지 (확인 팝업은 동작 변경).
