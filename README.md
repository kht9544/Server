# Game Server & Core Library

## 개요
이 프로젝트는 **게임 서버와 클라이언트**, 그리고 **멀티스레딩 및 메모리 관리 기능을 제공하는 Core Library**를 포함하는 시스템입니다.  
주요 기능은 다음과 같습니다.

- **Core Library**
  - 멀티스레딩 관리 (`ThreadManager`)
  - 동기화 및 락 시스템 (`Lock`, `DeadLockProfiler`)
  - 메모리 할당기 (`Allocator`, `MemoryPool`)
  - 데이터 관리 (`RefCounting`, `MyContainer`)

- **Game Server & Client**
  - **UDP 기반 게임 서버**
  - **UDP 클라이언트**
  - 사용자 계정 및 데이터 관리 (`AccountManager`, `UserManager`)

---

## 구성 요소

### 1. Core Library
#### (1) CoreGlobal (핵심 매니저)
- 싱글톤 패턴을 이용하여 `ThreadManager`, `DeadLockProfiler`, `Memory` 인스턴스를 관리
- `CoreGlobal::Instance()`를 통해 접근 가능

#### (2) Allocator (메모리 할당기)
- **BaseAllocator**: 기본 메모리 할당 및 해제
- **StompAllocator**: 페이지 단위 할당으로 버퍼 오버플로우 감지
- **PoolAllocator**: 메모리 풀 활용

#### (3) Memory & MemoryPool (메모리 관리)
- 미리 할당된 블록을 활용하여 성능 최적화
- `xalloc(size)` 및 `xrelease(ptr)`을 통한 메모리 할당

#### (4) 동기화 및 동시성 제어
- **락 시스템 (`Lock`)**
  - Read/Write Lock 지원
  - `_DEBUG` 모드에서는 `DeadLockProfiler`와 연동하여 데드락 탐지
- **락 기반 컨테이너**
  - `LockBasedQueue` / `LockBasedStack`
  - `ConditionVariable`을 이용한 동기화 기능
- **스레드 관리 (`ThreadManager`)**
  - `Launch()`로 스레드 실행
  - `Join()`으로 종료 시 동기화

#### (5) DeadLockProfiler (데드락 감지)
- 락 획득 및 해제를 추적하여 데드락 탐지
- DFS(깊이 우선 탐색) 방식으로 사이클 검출

#### (6) 자료구조 및 유틸리티
- **MyContainer**
  - `STLAllocator` 기반으로 STL 컨테이너 최적화 (`Vector`, `List`, `Map` 등)
- **RefCounting**
  - `TSharedPtr` 기반 스마트 포인터 사용

---

### 2. Game Server
#### (1) 서버 구조
- UDP 기반 서버 소켓을 사용하여 클라이언트와 통신
- 클라이언트로부터 메시지를 수신 후 응답을 보내는 구조

#### (2) 주요 기능
- **GameServer.cpp**
  - `recvfrom()`을 이용한 비연결형 UDP 서버 구현
  - 클라이언트의 요청을 처리하고 응답 전송
- **AccountManager**
  - 사용자 로그인 관리 (`Login()`)
  - 계정 정보 조회 (`GetAccount()`)
- **UserManager**
  - 사용자 데이터 관리 (`Save()`)
  - 계정과 연결하여 데이터 저장

---

### 3. Client
#### (1) 클라이언트 구조
- 서버로 패킷을 전송하고 응답을 수신하는 UDP 클라이언트
- `sendto()` 및 `recvfrom()`을 이용한 데이터 전송

#### (2) 주요 기능
- **Client.cpp**
  - 서버로 메시지 전송 (`sendto()`)
  - 응답 메시지 수신 (`recvfrom()`)

---
