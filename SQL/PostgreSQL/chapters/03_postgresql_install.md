# 3장 PostgreSQL 설치와 실행

이 장에서는 Ubuntu 26.04 기본 저장소에서 PostgreSQL 18을 설치합니다. 설치 명령 하나로 끝내지 않고 어떤 패키지가 선택되는지 먼저 확인한 뒤, 서버 프로세스, 포트, 클러스터, 데이터 디렉터리와 설정 파일을 서로 연결해 살펴봅니다. 마지막에는 기본 관리자 역할로 접속해 서버가 실제로 SQL을 처리하는지 검증합니다.

> **버전 기준**  
> 2026년 8월 4일 확인 결과 Ubuntu 26.04(Resolute)의 기본 `postgresql` 패키지는 PostgreSQL 18을 설치합니다. amd64용 서버 패키지의 확인 당시 버전은 `18.4-0ubuntu0.26.04.1`입니다. 보안 업데이트 뒤에는 마이너 버전과 패키지 리비전이 달라질 수 있습니다. 이 책은 메이저 버전 18과 Ubuntu 기본 저장소를 기준으로 합니다.

## 이 장에서 배울 내용

- Ubuntu 패키지 목록에서 설치할 PostgreSQL 버전과 저장소를 확인한다.
- PostgreSQL 서버와 보조 패키지를 설치하고 버전을 확인한다.
- systemd로 서비스와 개별 클러스터를 시작, 중지하고 상태를 확인한다.
- 프로세스, 기본 포트와 준비 상태를 서로 다른 명령으로 확인한다.
- Ubuntu의 PostgreSQL 클러스터와 주요 파일 경로를 설명한다.
- `postgres` 운영체제 계정으로 `psql`에 처음 접속한다.

## 선행 지식

- 1장에서 서버, 데이터베이스, 스키마, 역할과 클라이언트를 구분했습니다.
- 2장에서 Ubuntu 26.04 WSL 2, Bash와 systemd를 준비했습니다.
- Ubuntu 사용자에게 `sudo` 권한이 있고 인터넷에 연결되어 있습니다.

다음 명령으로 환경을 다시 확인합니다.

**Ubuntu Bash**

```bash
cat /etc/os-release
ps -p 1 -o comm=
```

`VERSION_ID="26.04"`와 `systemd`가 확인되어야 이 장의 경로와 서비스 예시가 그대로 맞습니다.

## 1. 설치 전에 패키지를 이해하기

Ubuntu의 `apt`는 설정된 저장소에서 패키지 목록을 받고, 의존하는 다른 패키지와 함께 프로그램을 설치합니다. 이 책에서는 Ubuntu 기본 저장소와 PostgreSQL 공식 Apt 저장소(PGDG)를 섞지 않습니다.

Ubuntu 기본 저장소는 한 Ubuntu 릴리스에 정해진 PostgreSQL 메이저 버전을 그 Ubuntu의 수명 동안 유지·보수합니다. PGDG는 여러 PostgreSQL 메이저 버전을 선택할 수 있지만 별도 저장소와 키 관리가 필요합니다. 입문 실습의 재현성을 위해 기본 저장소를 사용합니다.

### 1.1 패키지 목록 갱신

```bash
sudo apt update
```

`apt update`는 PostgreSQL을 설치하지 않고 설치 가능한 패키지 목록만 갱신합니다.

### 1.2 후보 버전과 출처 확인

```bash
apt policy postgresql postgresql-18
```

Ubuntu 26.04 기본 저장소를 사용하는 경우 `postgresql`의 후보가 있고 `postgresql-18`의 18.x 패키지가 표시됩니다. 출력의 정확한 마이너 버전과 미러 주소는 시점과 지역에 따라 다릅니다.

```text
postgresql:
  Installed: (none)
  Candidate: ...
postgresql-18:
  Installed: (none)
  Candidate: 18.x-...
```

출처 경로에 `apt.postgresql.org`가 보이면 PGDG 저장소가 이미 추가된 환경입니다. 이 책과 같은 결과를 재현하려면 저장소 구성을 임의로 삭제하지 말고, 관리자와 상의하거나 깨끗한 Ubuntu 26.04 실습 배포판을 준비합니다.

### 1.3 메타 패키지와 버전 패키지

`postgresql`은 해당 Ubuntu 릴리스가 기본으로 정한 서버 버전을 의존성으로 설치하는 메타 패키지(meta package)입니다. 실제 서버 실행 파일과 파일은 `postgresql-18`, 클라이언트는 `postgresql-client-18`, Ubuntu식 클러스터 관리 도구는 `postgresql-common`에 들어 있습니다.

메타 패키지를 설치하면 기본 버전을 따라가겠다는 의도가 명확해집니다. 따라서 이 책의 기본 명령은 `postgresql`을 설치합니다.

## 2. PostgreSQL 설치

### 2.1 서버와 추가 모듈 설치

```bash
sudo apt install postgresql postgresql-contrib
```

`postgresql-contrib`는 PostgreSQL과 함께 배포되는 유용한 확장 모듈을 기본 버전에 맞춰 설치하는 메타 패키지입니다. 확장은 설치만으로 모든 데이터베이스에 자동 활성화되지 않으며, 필요한 데이터베이스에서 나중에 `CREATE EXTENSION`으로 선택합니다.

설치 확인 질문이 나오면 설치될 패키지와 디스크 사용량을 읽은 뒤 진행합니다. 네트워크와 컴퓨터 성능에 따라 시간이 걸릴 수 있습니다.

### 2.2 설치된 패키지 확인

```bash
dpkg -l 'postgresql*'
```

목록에서 상태가 `ii`인 `postgresql`, `postgresql-18`, `postgresql-client-18`과 `postgresql-common`을 확인합니다. `dpkg -l`의 목록이 길어도 패키지를 다시 설치할 필요는 없습니다.

### 2.3 클라이언트와 서버 버전 구분

```bash
psql --version
/usr/lib/postgresql/18/bin/postgres --version
```

예상 결과의 형식은 다음과 같습니다. 패치 버전은 다를 수 있습니다.

```text
psql (PostgreSQL) 18.4 (...)
postgres (PostgreSQL) 18.4 (...)
```

`psql --version`은 클라이언트 프로그램의 버전입니다. 서버가 실제로 실행 중인지는 증명하지 않습니다. 서버 버전은 접속 후 `SELECT version();`으로 다시 확인합니다.

## 3. Ubuntu의 PostgreSQL 클러스터

PostgreSQL에서 데이터베이스 클러스터(database cluster)는 한 서버 인스턴스가 관리하는 데이터베이스의 모음과 저장 영역입니다. Ubuntu 패키지는 한 컴퓨터에서 여러 메이저 버전과 여러 클러스터를 구분하기 위해 `버전/이름` 형식을 사용합니다.

기본 설치에서는 보통 PostgreSQL 18의 `main` 클러스터를 자동 생성합니다.

```bash
pg_lsclusters
```

예상 결과의 핵심 열은 다음과 같습니다.

```text
Ver Cluster Port Status Owner    Data directory              Log file
18  main    5432 online postgres /var/lib/postgresql/18/main /var/log/postgresql/postgresql-18-main.log
```

- `Ver`: PostgreSQL 메이저 버전
- `Cluster`: Ubuntu 관리 도구가 붙인 클러스터 이름
- `Port`: 서버가 기다리는 TCP 포트
- `Status`: `online`이면 실행 중, `down`이면 중지
- `Owner`: 서버 프로세스를 실행하는 Ubuntu 운영체제 계정
- `Data directory`: 실제 데이터 파일이 저장되는 디렉터리
- `Log file`: 기본 로그 파일

`main`은 특별한 SQL 키워드가 아니라 Ubuntu 패키지가 기본으로 쓰는 클러스터 이름입니다.

## 4. 서비스 상태와 제어

### 4.1 전체 서비스와 개별 클러스터 서비스

```bash
systemctl status postgresql --no-pager
systemctl status postgresql@18-main --no-pager
```

`postgresql.service`는 여러 PostgreSQL 클러스터를 묶는 상위 서비스입니다. 이 서비스가 `active (exited)`로 표시될 수 있는데, 시작 작업을 마친 상위 단위라는 뜻이며 서버가 죽었다는 뜻은 아닙니다. 실제 `18/main` 서버는 `postgresql@18-main.service`와 `pg_lsclusters`로 확인합니다.

상태 출력에서 다음 항목을 봅니다.

- `Loaded`: 서비스 정의를 읽었는가
- `Active`: 현재 동작 상태
- `Main PID`: 실제 서비스의 주 프로세스 번호
- 최근 로그: 시작 실패 원인이 있는가

### 4.2 시작, 중지와 재시작

실습 중에는 개별 클러스터를 명시하는 편이 분명합니다.

```bash
sudo systemctl stop postgresql@18-main
pg_lsclusters
sudo systemctl start postgresql@18-main
pg_lsclusters
```

첫 번째 확인에서는 `down`, 다시 시작한 뒤에는 `online`이 예상됩니다.

> **주의: 서비스 중지**  
> 중지는 모든 클라이언트 연결을 끝내므로 진행 중인 작업이 실패할 수 있습니다. 이 절에서는 아직 사용자 데이터가 없는 개인 실습 서버에서만 실행합니다. 운영 서버에서는 사용자 공지, 연결과 트랜잭션 확인, 백업 정책 없이 중지하지 않습니다.

설정 변경을 적용하거나 서버 상태를 초기화해야 할 때 재시작할 수 있습니다.

```bash
sudo systemctl restart postgresql@18-main
```

연결을 끊지 않고 적용 가능한 설정은 `reload`를 사용합니다. 모든 설정이 다시 읽기만으로 적용되는 것은 아닙니다.

```bash
sudo systemctl reload postgresql@18-main
```

어떤 설정이 재시작을 요구하는지는 25장에서 `pg_settings.context`와 함께 다룹니다.

### 4.3 부팅 시 자동 시작 확인

```bash
systemctl is-enabled postgresql.service
systemctl is-enabled postgresql@18-main.service
```

Ubuntu 패키지의 서비스 구성에 따라 상위 서비스와 인스턴스 단위의 출력 방식이 다를 수 있습니다. 핵심 검증은 WSL을 완전히 종료했다가 다시 시작한 뒤 `pg_lsclusters`가 `online`인지 확인하는 것입니다.

## 5. 프로세스와 포트 확인

서비스 관리자의 상태만 보지 않고 운영체제와 PostgreSQL 관점에서도 확인합니다.

### 5.1 서버 프로세스

```bash
ps -ef | grep '[p]ostgres'
```

서버가 실행 중이면 부모 `postgres` 프로세스와 체크포인터(checkpointer), 백그라운드 라이터(background writer), WAL 라이터 같은 보조 프로세스가 보입니다. 프로세스 번호와 세부 이름은 버전과 상태에 따라 다릅니다.

`grep '[p]ostgres'`처럼 쓰면 검색 명령 자체가 결과에 섞이는 일을 줄일 수 있습니다.

### 5.2 기본 포트

```bash
sudo ss -ltnp | grep ':5432'
```

옵션의 뜻은 다음과 같습니다.

- `-l`: 기다리는(listening) 소켓
- `-t`: TCP 소켓
- `-n`: 포트 번호를 숫자로 표시
- `-p`: 관련 프로세스 표시

기본 클러스터가 TCP 5432 포트에서 기다리면 해당 줄이 표시됩니다. PostgreSQL은 로컬 Unix 도메인 소켓도 사용할 수 있으므로 TCP 결과만으로 모든 접속 방식을 설명할 수는 없습니다.

### 5.3 PostgreSQL 준비 상태

```bash
pg_isready
pg_isready -h localhost -p 5432
```

성공하면 다음과 비슷한 결과가 나옵니다.

```text
/var/run/postgresql:5432 - accepting connections
localhost:5432 - accepting connections
```

첫 명령은 기본 로컬 소켓, 두 번째는 TCP의 `localhost`로 확인합니다. `accepting connections`는 서버가 요청을 받을 준비가 되었다는 뜻이지 현재 Ubuntu 사용자가 특정 데이터베이스에 로그인할 권한이 있다는 뜻은 아닙니다.

## 6. 데이터 디렉터리와 설정 파일

Ubuntu 패키지는 변경 가능한 설정과 실제 데이터 파일을 서로 다른 표준 경로에 둡니다.

| 목적 | PostgreSQL 18 기본 경로 |
|---|---|
| 데이터 파일 | `/var/lib/postgresql/18/main` |
| 주 설정 | `/etc/postgresql/18/main/postgresql.conf` |
| 클라이언트 인증 | `/etc/postgresql/18/main/pg_hba.conf` |
| 이름별 인증 설정 | `/etc/postgresql/18/main/pg_ident.conf` |
| Ubuntu 시작 옵션 | `/etc/postgresql/18/main/start.conf` |
| 로그 | `/var/log/postgresql/postgresql-18-main.log` |

실제 서버 설정값을 조회해 경로를 확인하는 것이 가장 안전합니다.

```bash
sudo -u postgres psql -X -d postgres -c "SHOW data_directory;"
sudo -u postgres psql -X -d postgres -c "SHOW config_file;"
sudo -u postgres psql -X -d postgres -c "SHOW hba_file;"
```

`-X`는 사용자의 `psql` 시작 파일을 읽지 않아 검증 결과가 개인 설정에 영향을 받지 않게 합니다. `-d postgres`는 접속할 기본 관리 데이터베이스를 명시하고 `-c`는 명령 하나를 실행한 뒤 종료합니다.

### 6.1 파일을 직접 바꾸기 전에 읽기

```bash
sudo sed -n '1,40p' /etc/postgresql/18/main/postgresql.conf
sudo sed -n '1,80p' /etc/postgresql/18/main/pg_hba.conf
```

지금은 내용을 읽기만 합니다. 설정 파일을 바꾸기 전에 원본을 백업하고, 변경 목적과 적용 방식이 `reload`인지 `restart`인지 확인해야 합니다.

### 6.2 데이터 파일을 직접 편집하지 않는다

`/var/lib/postgresql/18/main` 아래의 파일은 서버가 자체 형식으로 관리합니다. 텍스트 편집기로 열어 수정하거나 Windows 탐색기로 옮기지 않습니다. 데이터 변경은 SQL, 백업은 `pg_dump` 같은 PostgreSQL 도구로 수행합니다.

파일 단위 복사가 필요한 물리 백업은 서버 상태와 WAL을 일관되게 관리해야 하며 단순 폴더 복사와 다릅니다. 24장에서 안전한 백업과 복원을 배웁니다.

## 7. `postgres` 계정과 첫 접속

설치 직후에는 같은 `postgres`라는 이름이 두 계정 체계에 존재합니다.

| 이름 | 종류 | 역할 |
|---|---|---|
| Ubuntu의 `postgres` | 운영체제 계정 | PostgreSQL 서버 프로세스와 파일 소유 |
| PostgreSQL의 `postgres` | 데이터베이스 역할 | 초기 슈퍼사용자 역할 |

로컬 기본 인증은 운영체제 사용자 이름과 PostgreSQL 역할 이름을 대조하는 피어(peer) 방식을 사용합니다. 따라서 현재 일반 Ubuntu 사용자가 바로 `psql -U postgres`를 실행하면 피어 인증에 실패할 수 있습니다. `sudo`로 운영체제 사용자만 `postgres`로 바꿔 접속합니다.

```bash
sudo -u postgres psql -X -d postgres
```

성공하면 프롬프트가 다음과 비슷하게 바뀝니다.

```text
psql (18.x (...))
Type "help" for help.

postgres=#
```

`postgres=#`는 출력 예시인 프롬프트이므로 복사하지 않습니다. `#`는 현재 PostgreSQL 역할이 슈퍼사용자임을 나타냅니다.

### 7.1 서버 정보 조회

이제 **psql 안에서** 다음 SQL을 실행합니다.

```sql
SELECT version();

SELECT current_database(), current_user;

SHOW server_version;

SHOW port;

SHOW TimeZone;
```

핵심 예상값은 다음과 같습니다.

| 항목 | 예상값 |
|---|---|
| `current_database()` | `postgres` |
| `current_user` | `postgres` |
| `server_version` | `18.x` |
| `port` | `5432` |

`TimeZone`은 설치 환경에 따라 `Etc/UTC`, `Asia/Seoul` 등으로 다를 수 있습니다. 지금 임의 변경하지 않고 실제 값을 기록합니다. 시간대 처리는 11장에서 다룹니다.

### 7.2 데이터베이스 목록과 종료

다음은 SQL이 아니라 `psql` 메타 명령(meta-command)입니다. 역슬래시로 시작하고 세미콜론을 붙이지 않습니다.

```text
\l
\conninfo
\q
```

- `\l`: 데이터베이스 목록
- `\conninfo`: 현재 연결 정보
- `\q`: `psql` 종료

메타 명령의 자세한 사용법은 4장에서 배웁니다.

## 8. 검증 SQL 파일 실행

이 저장소의 `examples/03_verify_install.sql`은 앞에서 확인한 서버 정보를 한 번에 조회합니다. 저장소 루트에서 실행합니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/03_verify_install.sql
```

`-f -`는 현재 Bash 사용자가 연 파일을 표준 입력으로 전달합니다. 따라서 운영체제 사용자 `postgres`에게 학생 홈 디렉터리의 읽기 권한을 추가하지 않아도 됩니다. `ON_ERROR_STOP=1`은 SQL 오류가 발생했을 때 다음 문장으로 넘어가지 않고 실패 상태로 종료하게 합니다. 자동 검증에서 오류를 놓치지 않는 데 유용합니다.

## 9. WSL 재시작 뒤 서비스 확인

터미널만 닫는 것과 WSL 전체를 종료하는 것은 다릅니다. 저장 중인 작업이 없는지 확인한 뒤 PowerShell에서 다음 명령을 실행합니다.

> **주의: 실행 중 작업 중단**  
> `wsl --shutdown`은 모든 WSL 배포판을 종료합니다. 다른 WSL 터미널이나 프로그램이 작업 중이면 먼저 안전하게 종료합니다.

**Windows PowerShell**

```powershell
wsl --shutdown
wsl -d Ubuntu-26.04
```

다시 열린 **Ubuntu Bash**에서 확인합니다.

```bash
pg_lsclusters
pg_isready
systemctl status postgresql@18-main --no-pager
```

클러스터가 `online`이고 `accepting connections`가 나오면 systemd가 PostgreSQL을 다시 시작한 것입니다.

만약 `down`이면 로그를 확인한 뒤 시작을 시도합니다.

```bash
sudo journalctl -u postgresql@18-main -n 50 --no-pager
sudo tail -n 50 /var/log/postgresql/postgresql-18-main.log
sudo systemctl start postgresql@18-main
```

로그의 실제 오류를 읽기 전에 패키지 재설치나 데이터 디렉터리 삭제부터 하지 않습니다.

## 10. 원리 이해: 접속 한 번에 관여하는 구성 요소

`sudo -u postgres psql -d postgres` 명령에는 여러 층이 관여합니다.

```text
현재 Ubuntu 사용자
  └─ sudo: 운영체제 사용자 postgres로 명령 실행
      └─ psql: 로컬 소켓으로 서버에 연결
          └─ pg_hba.conf: peer 인증 규칙 적용
              └─ PostgreSQL 역할 postgres로 데이터베이스 postgres 접속
```

각 이름을 구분하면 오류 위치를 좁힐 수 있습니다.

- `sudo` 오류: Ubuntu 사용자 권한 또는 비밀번호 문제
- `psql: command not found`: 클라이언트 패키지나 실행 경로 문제
- `No such file or directory`: 서버가 꺼졌거나 소켓 경로가 다름
- `Peer authentication failed`: 운영체제 사용자와 PostgreSQL 역할의 대응 문제
- `database ... does not exist`: 서버 연결은 됐지만 지정 데이터베이스가 없음

## 11. 주의 및 오류 해결

### `Unable to locate package postgresql`

`sudo apt update`가 성공했는지, `/etc/os-release`가 지원 중인 Ubuntu인지 확인합니다. 저장소 주소를 인터넷 블로그의 값으로 덮어쓰지 않습니다. 네트워크 또는 저장소 오류 메시지를 먼저 해결합니다.

### 설치 후 `pg_lsclusters`가 비어 있음

패키지는 있지만 기본 클러스터 생성이 실패했을 수 있습니다. 먼저 설치 로그와 패키지 상태를 확인합니다.

```bash
dpkg -l 'postgresql*'
sudo journalctl -u postgresql --no-pager -n 100
```

`pg_createcluster`로 새 클러스터를 만들 수 있지만 데이터 경로와 포트에 영향을 줍니다. 원인을 확인하지 않은 채 실행하지 말고 설치 오류, 남은 기존 구성과 디스크 공간을 먼저 점검합니다.

### `postgresql.service`가 `active (exited)`

Ubuntu의 상위 서비스에서는 정상일 수 있습니다. `pg_lsclusters`와 `systemctl status postgresql@18-main`으로 실제 클러스터를 확인합니다.

### `connection to server ... failed: No such file or directory`

기본 로컬 소켓에서 서버를 찾지 못했다는 뜻일 가능성이 큽니다.

```bash
pg_lsclusters
systemctl status postgresql@18-main --no-pager
sudo journalctl -u postgresql@18-main -n 50 --no-pager
```

클러스터 이름이나 버전이 실제 출력과 다르면 명령의 `18-main`을 실제 값에 맞춥니다.

### `Peer authentication failed for user "postgres"`

일반 Ubuntu 사용자로 `psql -U postgres`를 실행했는지 확인합니다. 첫 접속은 다음 명령을 사용합니다.

```bash
sudo -u postgres psql -X -d postgres
```

인증 오류를 없애려고 `pg_hba.conf`를 무조건 `trust`로 바꾸지 않습니다. `trust`는 비밀번호 없이 접속을 허용하므로 보안상 위험합니다. 역할과 비밀번호 구성은 5장, 인증 원리는 22장에서 다룹니다.

### 포트 5432가 이미 사용 중

다른 PostgreSQL 클러스터나 프로그램이 사용 중인지 확인합니다.

```bash
sudo ss -ltnp | grep ':5432'
pg_lsclusters
```

프로세스를 강제로 종료하거나 데이터 파일을 삭제하지 않습니다. 어느 서비스가 포트를 소유하는지 확인한 뒤 포트 또는 클러스터 구성을 결정합니다.

### WSL 재시작 뒤 서버가 자동으로 시작하지 않음

PID 1이 systemd인지, 서비스와 클러스터 상태, 로그 순서로 확인합니다.

```bash
ps -p 1 -o comm=
systemctl is-enabled postgresql.service
pg_lsclusters
sudo journalctl -u postgresql@18-main -b --no-pager
```

systemd가 아니면 2장의 `/etc/wsl.conf` 설정을 확인합니다.

## 12. 초기화와 제거에 관한 주의

이 장에서는 데이터베이스 클러스터 삭제나 패키지 완전 제거를 실습하지 않습니다. `apt remove`, `apt purge`, `pg_dropcluster`는 범위에 따라 프로그램, 설정 또는 모든 데이터베이스를 잃게 할 수 있습니다.

설치를 처음부터 다시 해야 한다면 다음 순서를 지킵니다.

1. 필요한 데이터가 있는지 확인합니다.
2. 서버에 접속할 수 있으면 논리 백업을 만듭니다.
3. `pg_lsclusters`로 삭제 대상을 정확히 확인합니다.
4. 백업 복원 시험 뒤 제거 범위를 결정합니다.

구체적인 백업과 복원은 24장, 실습 환경 전체 초기화는 부록 G에서 다룹니다. 그 전에는 데이터 디렉터리를 직접 삭제하지 않습니다.

## 13. 실습 문제

### 기본 문제

1. `apt policy`로 `postgresql`과 `postgresql-18`의 설치 후보를 확인하세요.
2. 클라이언트 프로그램 버전과 서버 실행 파일 버전을 각각 확인하세요.
3. `pg_lsclusters` 결과에서 버전, 이름, 포트, 상태, 소유자와 데이터 디렉터리를 적으세요.
4. systemd, 프로세스, 포트와 `pg_isready` 네 관점에서 서버 상태를 확인하세요.
5. `postgres` 운영체제 계정을 통해 접속하여 현재 데이터베이스와 역할을 조회하세요.

### 응용 문제

1. `SHOW` 명령으로 실제 데이터 디렉터리, 설정 파일과 인증 파일 경로를 확인하세요.
2. 개인 실습 서버에서 클러스터를 중지하고 `pg_lsclusters`, `pg_isready`의 변화 확인 후 다시 시작하세요.
3. WSL을 안전하게 종료하고 다시 실행한 뒤 PostgreSQL의 자동 시작 여부를 확인하세요.
4. `examples/03_verify_install.sql`을 오류 중단 옵션과 함께 실행하고 각 결과의 의미를 설명하세요.

## 실습 문제 정답

### 기본 문제 정답

1. `apt policy postgresql postgresql-18`로 후보 버전과 저장소를 확인합니다.
2. `psql --version`과 `/usr/lib/postgresql/18/bin/postgres --version`으로 클라이언트와 서버 실행 파일을 따로 확인합니다.
3. `pg_lsclusters`의 버전, 이름, 포트, 상태, 소유자와 데이터 디렉터리 열을 읽습니다.
4. `systemctl status postgresql-main --no-pager`, `ps -ef`, `sudo ss -ltnp`, `pg_isready`로 각각 서비스·프로세스·포트·준비 상태를 확인합니다.
5. `sudo -u postgres psql -X -d postgres`로 접속해 `SELECT current_database(), current_user;`를 실행합니다. 핵심값은 모두 `postgres`입니다.

### 응용 문제 정답

1. 관리자 `psql`에서 `SHOW data_directory;`, `SHOW config_file;`, `SHOW hba_file;`을 실행합니다.
2. 개인 실습 환경에서 클러스터를 중지한 뒤 `pg_lsclusters`와 `pg_isready`를 확인하고 다시 시작한 뒤 `online`과 `accepting connections`를 확인합니다.
3. Windows PowerShell에서 `wsl --shutdown` 후 Ubuntu를 다시 열고 서비스 상태, 클러스터와 준비 상태를 확인합니다.
4. 저장소 루트에서 `sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/03_verify_install.sql`을 실행하고 버전, 역할, 경로, 포트와 시간대를 확인합니다.

## 14. 핵심 정리

- 이 책은 Ubuntu 26.04 기본 저장소의 PostgreSQL 18을 사용합니다.
- 설치 전에 `apt update`와 `apt policy`로 후보 버전과 출처를 확인합니다.
- `postgresql`은 Ubuntu 기본 서버 버전을 설치하는 메타 패키지입니다.
- `psql --version`은 클라이언트 버전이며 서버 실행 여부를 보장하지 않습니다.
- Ubuntu의 기본 클러스터는 보통 `18/main`, 포트는 `5432`입니다.
- 상위 `postgresql.service`와 실제 `postgresql@18-main.service`의 상태를 구분합니다.
- `pg_lsclusters`, `ps`, `ss`와 `pg_isready`는 서로 다른 관점의 상태를 보여 줍니다.
- 설정은 `/etc/postgresql/18/main`, 데이터는 `/var/lib/postgresql/18/main`에 둡니다.
- 운영체제 계정 `postgres`와 PostgreSQL 역할 `postgres`는 별개입니다.
- 데이터 파일을 직접 편집하거나 원인 확인 없이 클러스터를 삭제하지 않습니다.

## 15. 확인 문제

1. Ubuntu 26.04 기본 저장소가 제공하는 PostgreSQL 메이저 버전은 무엇입니까?
2. Ubuntu가 관리하는 PostgreSQL 클러스터 목록을 보는 명령은 무엇입니까?
3. `postgresql.service`가 `active (exited)`일 때 실제 서버 상태를 추가로 확인할 두 가지 방법을 적으세요.
4. 서버가 기본 로컬 연결 요청을 받을 준비가 되었는지 확인하는 명령은 무엇입니까?
5. 참 또는 거짓: `/var/lib/postgresql/18/main`의 파일은 서버를 중지하면 텍스트 편집기로 안전하게 수정할 수 있다.

정답: 1. 18, 2. `pg_lsclusters`, 3. 예: `pg_lsclusters`, `systemctl status postgresql@18-main`, 4. `pg_isready`, 5. 거짓

## 다음 장 안내

PostgreSQL 서버를 설치하고 첫 SQL까지 실행했습니다. 다음 장에서는 `psql`에 일반적인 방법으로 접속하고, SQL과 메타 명령의 차이, 객체 목록, 출력 형식, 파일 실행과 명령 기록을 집중적으로 익힙니다.
