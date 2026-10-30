# 지상국 코어(Service) 계층 리팩토링 계획

> 원칙: **현재 기능을 빠짐없이 유지한다.** 이 계획의 모든 단계는 구조만 바꾸는 단계이며,
> 동작 변경(버그 수정 포함)은 하지 않는다. 발견한 결함은 `CLAUDE.md`의 "Known defects"에 적고
> 별도 커밋에서 다룬다.

## 1. 왜 Service 계층부터인가

현재 의존 방향은 **UI → 백엔드 한쪽으로만** 엉켜 있다.

| `miman_imgui.cpp` 안의 백엔드 코드 | 개수 (리팩토링 전) |
|---|---|
| `pthread_create` | 188 |
| `pthread_join` | 약 170 |
| `State.` 직접 접근 | 232 |
| `setup->` 직접 접근 | 65 |
| `csp_*` 직접 호출 | 11 |

백엔드 모듈(`coms`, `orbital`, `csp`, `autopilot`, `radial`)에는 `ImGui::` 호출이 0개다.
그래서 백엔드는 그대로 두고, **UI가 직접 하던 "스레드 기동/정지 + 상태 플래그 조작"을
Service 함수로 옮기는 것**만으로 경계를 세울 수 있다.

목표 구조:

```
View (ImGui 호출만)
   │  호출
   ▼
ViewModel (창별 상태/조건 판단)        ← 이후 단계 (CLAUDE.md의 창별 MVVM 순서)
   │  호출
   ▼
Service  (src/service/)               ← 이번 리팩토링
   │  - 스레드 슬롯 관리 (p_thread[] 를 아는 유일한 계층)
   │  - 시작/정지 시퀀스 (State 플래그 + 스레드)
   ▼
Model / Backend (miman_coms, miman_orbital, miman_radial, miman_csp, ...)
```

## 2. Service 모듈 구성

모든 Service는 `mim::svc` 네임스페이스의 자유 함수다. (`State`/`setup`가 아직 전역이므로
지금 클래스로 만들면 전역을 감싼 껍데기에 불과하다. `StateCheckUnit` 분해 단계에서 클래스화한다.)

| 파일 | 담당 | 대표 함수 |
|---|---|---|
| `svc_thread.{h,cpp}` | 스레드 슬롯 enum, 시작/대기 | `StartThread(Slot, fn, arg)`, `JoinThread(Slot)` |
| `svc_tx.{h,cpp}` | TX 슬롯(4) 작업: 업링크 패킷, 핑, 쉘 | `SendUplink(pkt)`, `WaitTx()`, `PingScan()`, ... |
| `svc_tracking.{h,cpp}` | 로테이터 추적, 로테이터 읽기, 도플러, 오토파일럿, TLE 로딩 | `StartEngage()`, `StopEngage()`, `StartDoppler()`, ... |
| `svc_radio.{h,cpp}` | GS100 파라미터 테이블, 캘리브레이션, 신호 테스트, AMP 타이머 | `LoadParamTable(n)`, `SaveParamTable(n)`, ... |
| `svc_station.{h,cpp}` | 하드웨어 초기화 / 종료 시퀀스 (main.cpp에서 이동) | `InitializeHardware()`, `StopAllThreads()` |

스레드 슬롯 번호는 `main.cpp`의 기존 주석과 **완전히 동일한 인덱스**를 쓴다.

## 3. 단계 (각 단계 = 1 커밋, 매 단계 빌드 + 화면 A/B 확인)

| # | 단계 | 바뀌는 것 | 바뀌지 않는 것 |
|---|---|---|---|
| 0 | 검증 도구 | `tools/ui_snapshot.sh` 추가 | 소스 코드 전부 |
| 1 | 스레드 슬롯 계층 | `svc_thread` 추가, CMake에 등록 | 기존 호출부 (아직 사용 안 함) |
| 2 | TX 슬롯 Service | 업링크/핑/쉘 호출을 `svc_tx`로 치환 | 패킷 생성 코드, 호출 순서, 로그 |
| 3 | 추적/로테이터 Service | Engage/Rotator/Doppler/AutoPilot/TLE 시작·정지를 `svc_tracking`으로 | 버튼 배치, 로그 문구, 플래그 순서 |
| 4 | 라디오 Service | 파라미터 테이블/캘리브레이션/신호테스트/AMP를 `svc_radio`로 | 팝업 UI, 로그 |
| 5 | 스테이션 Service | `main.cpp`의 초기화·종료 블록을 `svc_station`으로 | 초기화 순서, 로그, 종료 순서 |
| 6 | 백엔드 파일 정리 | 백엔드 안의 `pthread_create(&p_thread[..])`도 `svc_thread` 사용 | 백엔드 로직 |

치환 규칙 (동작 동일성을 지키는 방법):

- `pthread_join(p_thread[4]); pthread_create(&p_thread[4], ..., task_uplink_onorbit, X);`
  → `mim::svc::WaitTx(); mim::svc::SendUplink(X);` (join 과 create 를 **합치지 않는다** —
  join 없이 create 만 하던 곳도 있으므로 그대로 보존)
- 시작/정지 시퀀스는 **원래 줄 순서 그대로** 함수로 옮긴다. (플래그 설정 → 스레드 → 로그)
- 동일해 보여도 호출부마다 미세하게 다른 곳(예: Stop Engage가 `State.Doppler`도 끔)은
  각자 별도 함수로 둔다.

## 3-1. 진행 결과 (1차: 0~6단계 완료)

| 지표 | 리팩토링 전 | 후 |
|---|---|---|
| `miman_imgui.cpp` 의 `pthread_create/join` (주석 제외) | 358 | **0** |
| `miman_imgui.cpp` 의 `p_thread` / `ftp_start_worker` extern | 있음 | **없음** (UI 가 슬롯에 접근하면 컴파일 에러) |
| `main.cpp` 의 스레드 생성/조인 | 21 | **0** (`p_thread` 정의만 남음) |
| `p_thread[]` 를 직접 만지는 빌드 대상 파일 | main, imgui, autopilot (+extern 4개) | **`service/svc_thread.cpp` 하나** |
| UI 안의 `fork/execl` (앰프) | 6곳 | **0** (`svc_radio` AmpOn/AmpOff) |

Service 모듈: `svc_thread`, `svc_tx`, `svc_tracking`, `svc_radio`, `svc_ftp`, `svc_station`.

검증 (단계마다, 원본 커밋 `60a2c55` 과 비교):
- `tools/ui_snapshot.sh` 화면 6장 (Setup, Pre-Pass, Rotator On/OFF, Pass Operation, FTP Upload): 콘솔 위 영역 픽셀 차이 0
- 콘솔 로그(정렬 비교) / 표준출력 동일, SIGINT 종료 동일, Setup Quit 종료 경로 동일

시나리오에서 누르지 않은 버튼(원본도 TLE 없이 누르면 죽는 Engage/Doppler, 실제 장비가 필요한
명령 전송 등)은 치환이 1:1 기계적이라는 점(diff 줄 단위 검토)으로 확인했다.

## 4. 이후 단계 (이번 범위 밖)

- UI 안의 CCSDS 명령 패킷 생성 코드 → `CommandBuilder` (Model)
- UI에서 직접 하는 `csp_transaction`/`csp_reboot` → Service (현재 UI 스레드에서 블로킹됨: 동작 변경이므로 별도)
- 창별 ViewModel 도입 (CLAUDE.md 순서: Rotator → Track → Beacon → Control → Command)
- `StateCheckUnit` 분해, Service 클래스화
- TX 슬롯을 큐 기반 워커로 교체 (동작 변경이므로 별도)

## 5. 검증 방법

```bash
tools/ui_snapshot.sh /tmp/snap_after
```

1. 빌드 성공, 경고 수 증가 없음
2. Setup → Start → Pre-Pass → Pass Operation 시나리오에서 앱이 살아 있음
3. 콘솔 로그 순서/문구 동일 (`run.log`, `applogs/`)
4. 스크린샷 비교 (시간 표시 영역 제외 동일)
5. 단계별 불변식: `grep -c pthread_ src/miman_imgui.cpp` 가 단계마다 줄어 최종 0
