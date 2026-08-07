# 2장 WSL과 Ubuntu 실습 환경 준비

PostgreSQL은 Windows에도 설치할 수 있지만 이 책은 WSL 2 안의 Ubuntu 26.04를 실습 서버로 사용합니다. Windows를 그대로 쓰면서 실제 Linux 서버와 가까운 명령, 경로와 서비스 관리 방식을 익힐 수 있기 때문입니다. 이 장에서는 설치 위치를 헷갈리지 않도록 Windows와 Ubuntu의 경계를 먼저 세웁니다.

> **버전 기준**  
> 이 장은 2026년 8월 4일 기준 Windows 11, WSL 2와 Ubuntu 26.04 LTS를 대상으로 합니다. `wsl --install`의 배포판 목록은 Microsoft Store와 WSL 버전에 따라 달라질 수 있으므로 설치 전에 반드시 `wsl --list --online`의 실제 이름을 확인합니다.

## 이 장에서 배울 내용

- WSL과 WSL 2가 Windows에서 Linux를 실행하는 방식을 설명한다.
- Windows 11에 WSL과 Ubuntu 26.04를 설치하고 WSL 2 사용 여부를 확인한다.
- Ubuntu의 사용자, 패키지와 기본 디렉터리를 확인한다.
- Bash에서 디렉터리 이동, 파일 확인과 도움말 사용을 연습한다.
- Windows와 Linux 파일 시스템의 경로와 성능 차이를 이해한다.
- Visual Studio Code로 WSL 안의 작업 디렉터리를 연다.

## 선행 지식

- 1장의 클라이언트·서버 구조를 읽었습니다.
- Windows 11에 로그인할 수 있습니다.
- WSL 설치를 위해 Windows 관리자 권한과 인터넷 연결을 사용할 수 있습니다.

회사나 학교에서 관리하는 컴퓨터는 가상화 기능이나 Microsoft Store가 정책으로 제한될 수 있습니다. 이 경우 임의로 보안 정책을 우회하지 말고 관리자에게 WSL 2 사용 가능 여부를 확인합니다.

## 1. WSL과 WSL 2

WSL(Windows Subsystem for Linux)은 Windows에서 Linux 배포판과 명령줄 도구를 실행하는 기능입니다. Ubuntu는 Linux 배포판(distribution) 가운데 하나입니다. WSL과 Ubuntu는 같은 말이 아닙니다.

```text
Windows 11
├── PowerShell·Windows Terminal·VS Code(Windows 프로그램)
└── WSL 2의 관리형 가상 머신
    └── Ubuntu 26.04
        ├── Bash와 Linux 명령
        └── PostgreSQL 서버와 psql
```

WSL 1은 Linux 시스템 호출을 변환하는 방식이고, WSL 2는 실제 Linux 커널이 들어 있는 가벼운 관리형 가상 머신을 사용합니다. PostgreSQL 실습에서는 Linux 호환성과 `systemd` 서비스 관리를 지원하는 WSL 2를 사용합니다.

WSL 2가 일반 가상 머신과 비슷한 부분은 있지만 사용자가 가상 디스크와 네트워크 장치를 일일이 구성하지 않아도 Windows가 통합 관리한다는 차이가 있습니다. Windows 드라이브 접근, 명령 실행과 파일 탐색기 연동도 기본으로 제공합니다.

## 2. 설치 전 확인

### 2.1 가상화 확인

작업 관리자에서 **성능 → CPU → 가상화**가 `사용`인지 확인합니다. `사용 안 함`이면 UEFI/BIOS 설정 변경이 필요할 수 있습니다. 제조사마다 메뉴 이름이 다르므로 컴퓨터 제조사 안내를 따릅니다.

### 2.2 PowerShell과 Ubuntu Bash 구분

앞으로 명령 위에 실행 위치를 표시합니다.

**Windows PowerShell**

```powershell
wsl --status
```

**Ubuntu Bash**

```bash
uname -a
```

`wsl.exe`는 Windows 프로그램이므로 설치와 배포판 관리는 PowerShell에서 합니다. `apt`, `ls`, `sudo`는 Ubuntu 안의 명령이므로 Ubuntu Bash에서 실행합니다.

## 3. WSL 설치

### 3.1 기본 설치

Windows Terminal 또는 PowerShell을 **관리자 권한으로 실행**한 뒤 다음 명령을 입력합니다.

**Windows PowerShell**

```powershell
wsl --install
```

이 명령은 필요한 Windows 기능을 활성화하고 기본 Linux 배포판 설치를 시도합니다. 출력에서 다시 시작을 요구하면 작업을 저장하고 Windows를 재부팅합니다.

> `wsl --install`이 이미 설치되어 있다는 도움말을 표시해도 오류가 아닐 수 있습니다. 다음 절의 상태 확인으로 진행합니다.

### 3.2 WSL 업데이트와 기본 버전 지정

**Windows PowerShell**

```powershell
wsl --update
wsl --set-default-version 2
wsl --status
```

예상 결과에는 기본 버전이 `2`라고 표시됩니다. 문구는 WSL 언어와 버전에 따라 다릅니다.

```text
Default Version: 2
```

`wsl --update`가 권한 오류를 내면 관리자 PowerShell인지 확인합니다. 조직 정책으로 Store 접근이 제한된 환경은 시스템 관리자에게 업데이트 방법을 문의합니다.

## 4. Ubuntu 26.04 설치

### 4.1 온라인 배포판 이름 확인

배포판 이름을 추측하지 말고 현재 제공 목록을 확인합니다.

**Windows PowerShell**

```powershell
wsl --list --online
```

출력의 `NAME` 열에서 Ubuntu 26.04에 해당하는 정확한 이름을 찾습니다. 다음은 형식을 보여 주기 위한 예시이며 실제 목록이 기준입니다.

```text
NAME             FRIENDLY NAME
Ubuntu           Ubuntu
Ubuntu-26.04     Ubuntu 26.04 LTS
```

목록에 `Ubuntu-26.04`가 있으면 다음처럼 설치합니다.

**Windows PowerShell**

```powershell
wsl --install Ubuntu-26.04
```

처음 실행하면 Ubuntu 사용자 이름과 비밀번호를 만듭니다. 이 계정은 Ubuntu 운영체제 계정이며 3장에서 만날 PostgreSQL 역할과는 별개입니다.

- 사용자 이름은 영문 소문자와 숫자를 중심으로 정합니다.
- 입력 중 비밀번호가 화면에 보이지 않는 것은 정상입니다.
- 책의 예제 비밀번호를 그대로 쓰지 말고 개인 실습용 비밀번호를 만듭니다.

### 4.2 온라인 목록에 없을 때 공식 WSL 이미지 사용

Ubuntu 공식 배포 페이지에는 Ubuntu 26.04 WSL 이미지가 제공됩니다. 브라우저에서 다음 페이지를 열고 `.wsl` 파일과 `SHA256SUMS`를 같은 폴더에 내려받습니다.

<https://releases.ubuntu.com/26.04/>

다운로드 폴더에서 파일 이름을 확인한 뒤 체크섬을 검증합니다. `<파일명>`은 실제 이름으로 바꿉니다.

**Windows PowerShell**

```powershell
Get-FileHash .\ubuntu-26.04-wsl-amd64.wsl -Algorithm SHA256
Get-Content .\SHA256SUMS
```

두 출력의 SHA-256 값이 같을 때만 설치합니다. 다르면 파일을 삭제하고 공식 페이지에서 다시 받습니다.

**Windows PowerShell**

```powershell
wsl --install --from-file .\ubuntu-26.04-wsl-amd64.wsl
```

정확한 이미지 파일명과 지원 옵션은 다운로드 페이지 및 `wsl --help`의 현재 출력을 우선합니다.

## 5. 설치 결과 확인

### 5.1 배포판과 WSL 버전 확인

**Windows PowerShell**

```powershell
wsl --list --verbose
```

예상 결과의 `VERSION` 열이 `2`여야 합니다.

```text
  NAME             STATE           VERSION
* Ubuntu-26.04     Running         2
```

이름은 설치 방식에 따라 다를 수 있습니다. 버전이 `1`이면 실제 배포판 이름을 사용해 변환합니다.

**Windows PowerShell**

```powershell
wsl --set-version Ubuntu-26.04 2
```

변환 중에는 배포판을 사용하지 말고 완료될 때까지 기다립니다.

### 5.2 Ubuntu 버전 확인

시작 메뉴에서 설치한 Ubuntu를 열거나 PowerShell에서 실제 배포판 이름을 지정합니다.

**Windows PowerShell**

```powershell
wsl -d Ubuntu-26.04
```

이제 프롬프트가 Ubuntu Bash로 바뀝니다. 다음 명령은 배포판 정보를 읽습니다.

**Ubuntu Bash**

```bash
cat /etc/os-release
```

핵심 예상 결과는 다음과 같습니다. 점 릴리스나 표시 문구는 업데이트에 따라 달라질 수 있습니다.

```text
PRETTY_NAME="Ubuntu 26.04 LTS"
VERSION_ID="26.04"
VERSION_CODENAME=resolute
```

`VERSION_ID`가 `26.04`가 아니면 다른 배포판을 연 것입니다. Bash에서 `exit`로 나온 뒤 `wsl --list --verbose`의 이름을 다시 확인합니다.

### 5.3 systemd 확인

`systemd`는 Ubuntu에서 서비스를 시작하고 상태를 관리하는 시스템 및 서비스 관리자입니다. 최근 Ubuntu WSL 이미지에서는 기본으로 활성화되지만 실제 상태를 확인합니다.

**Ubuntu Bash**

```bash
ps -p 1 -o comm=
systemctl is-system-running
```

첫 명령의 예상 결과는 `systemd`입니다. 두 번째 명령은 초기화 중이면 잠시 `starting`, 정상 상태면 `running`을 표시할 수 있습니다. 일부 선택 서비스의 실패 때문에 `degraded`가 표시될 수도 있으므로 그때는 다음 명령으로 실패 항목을 확인합니다.

```bash
systemctl --failed
```

PID 1이 `systemd`가 아니라면 `/etc/wsl.conf`를 확인합니다.

```bash
sudo nano /etc/wsl.conf
```

다음 설정을 추가하거나 바로잡습니다.

```ini
[boot]
systemd=true
```

저장한 뒤 Ubuntu 창을 닫는 것만으로는 충분하지 않습니다. PowerShell에서 모든 WSL 인스턴스를 종료하고 다시 시작합니다.

**Windows PowerShell**

```powershell
wsl --shutdown
wsl -d Ubuntu-26.04
```

## 6. Ubuntu 초기 설정

### 6.1 현재 사용자와 관리자 권한

**Ubuntu Bash**

```bash
whoami
id
pwd
```

`whoami`는 현재 Ubuntu 사용자 이름, `id`는 사용자와 그룹 정보, `pwd`는 현재 디렉터리를 표시합니다. 처음 시작한 경우 보통 `/home/<ubuntu_user>`에 있습니다.

`sudo`는 허가된 사용자가 명령 하나를 관리자 권한으로 실행하게 합니다. 다음 명령은 권한을 확인한 뒤 관리자 세션을 계속 열어 두지 않고 즉시 끝냅니다.

```bash
sudo -v
```

비밀번호 입력 중 글자가 보이지 않는 것은 정상입니다. `sudo`는 필요한 관리 명령에만 붙입니다. 일반 문서와 프로그램 파일을 `sudo`로 만들면 소유권 문제를 일으킬 수 있습니다.

### 6.2 패키지 목록과 설치 패키지 업데이트

`apt`는 Ubuntu의 패키지 관리 도구입니다. `update`는 설치 가능한 패키지 목록을 새로 받고, `upgrade`는 설치된 패키지를 갱신합니다.

```bash
sudo apt update
sudo apt upgrade
```

목록을 받지 못하면 먼저 Windows의 인터넷 연결과 시간 설정을 확인합니다. 프록시를 사용하는 조직 환경에서는 관리자에게 Ubuntu의 프록시 설정을 문의합니다.

업데이트 후 배포판과 커널 정보를 기록합니다.

```bash
uname -a
cat /etc/os-release
```

커널 문자열에 `microsoft` 또는 `WSL`이 포함되는지는 WSL 릴리스에 따라 표현이 달라질 수 있습니다. 배포판 버전은 `/etc/os-release`를 기준으로 판단합니다.

## 7. Linux 터미널 기본 명령

### 7.1 현재 위치와 목록

```bash
pwd
ls
ls -la
```

- `pwd`: 현재 작업 디렉터리의 절대 경로를 표시합니다.
- `ls`: 디렉터리 내용을 표시합니다.
- `ls -la`: 숨김 파일을 포함해 자세히 표시합니다.

Linux 경로는 대소문자를 구분합니다. `Documents`와 `documents`는 다른 이름입니다.

### 7.2 디렉터리 만들고 이동하기

교재 실습은 Linux 홈 디렉터리 아래에 둡니다.

```bash
mkdir -p ~/work/postgresql-book
cd ~/work/postgresql-book
pwd
```

예상 결과에서 `<ubuntu_user>`는 자신의 사용자 이름으로 바뀝니다.

```text
/home/<ubuntu_user>/work/postgresql-book
```

`~`는 현재 사용자의 홈 디렉터리, `.`은 현재 디렉터리, `..`은 상위 디렉터리를 뜻합니다.

```bash
cd ..
cd -
```

`cd -`는 직전에 있던 디렉터리로 돌아갑니다.

### 7.3 파일 만들고 읽고 복사하기

```bash
touch environment.txt
ls -l environment.txt
cp environment.txt environment-copy.txt
cat environment-copy.txt
```

빈 파일이므로 `cat`은 내용을 출력하지 않습니다. 파일을 삭제하는 `rm`은 휴지통을 거치지 않을 수 있습니다.

> **주의: 삭제 명령**  
> 다음 명령은 이 절에서 방금 만든 빈 복사본 하나만 삭제합니다. 경로를 확인한 뒤 실행합니다.

```bash
rm environment-copy.txt
```

잘못 지운 파일을 자동 복구하는 명령은 없습니다. 중요한 파일은 Git이나 별도 백업으로 보호합니다.

### 7.4 도움말 보기

```bash
ls --help
man ls
```

`man` 화면에서는 방향키로 이동하고 `q`로 종료합니다. 명령의 옵션을 기억하지 못할 때 추측하지 말고 도움말을 확인하는 습관을 들입니다.

## 8. Windows와 WSL 파일 시스템

Windows의 `C:` 드라이브는 Ubuntu에서 보통 `/mnt/c`에 연결됩니다.

| Windows 경로 | Ubuntu에서 본 경로 |
|---|---|
| `C:\Users\winuser\Downloads` | `/mnt/c/Users/winuser/Downloads` |
| `C:\work\sql` | `/mnt/c/work/sql` |
| 해당 없음 | `/home/ubuntu_user/work` |

반대로 Windows 파일 탐색기의 주소 표시줄에서 `\\wsl$`을 입력하면 실행 중인 WSL 배포판의 Linux 파일을 볼 수 있습니다. 배포판 이름을 포함한 경로 형식은 `\\wsl$\Ubuntu-26.04\home\<ubuntu_user>`와 같습니다.

### 8.1 프로젝트는 Linux 파일 시스템에 둔다

PostgreSQL 데이터 디렉터리와 Linux 도구를 사용하는 소스 코드는 `/home/<ubuntu_user>` 아래에 두는 것을 기본으로 합니다. `/mnt/c`는 Windows와 파일을 주고받기 편하지만 파일 권한의 의미가 다르고 많은 작은 파일을 다루는 Linux 작업이 느릴 수 있습니다.

```text
/home/<ubuntu_user>/work/postgresql-book   권장: 원고, SQL, Python 코드
/mnt/c/Users/<winuser>/Downloads           교환: 내려받은 파일, 백업 복사본
```

PostgreSQL 데이터 디렉터리를 `/mnt/c`에 임의로 만들지 않습니다. Ubuntu 패키지가 정한 Linux 경로와 권한을 그대로 사용합니다.

### 8.2 경로 변환

```bash
wslpath 'C:\Users\winuser\Downloads'
wslpath -w ~/work/postgresql-book
```

첫 번째 명령의 `winuser`는 실제 Windows 사용자 이름으로 바꿔야 합니다. 공백이 있는 Windows 경로는 작은따옴표로 묶습니다.

## 9. 작업 디렉터리 구성

이 책을 직접 작성하는 경우 다음 구조를 사용합니다. 책을 읽기만 한다면 제공된 저장소를 Linux 홈 아래에 내려받고 구조를 확인합니다.

```text
postgresql-book/
├── chapters/
├── examples/
├── exercises/
├── images/
├── references/
└── scripts/
```

빈 연습 디렉터리가 필요하면 다음 명령을 사용합니다.

```bash
cd ~/work/postgresql-book
mkdir -p examples exercises scripts
```

## 10. Visual Studio Code와 WSL 연동

1. Windows에 Visual Studio Code를 설치합니다.
2. Visual Studio Code의 확장 보기에서 Microsoft의 WSL 확장을 설치합니다.
3. Ubuntu Bash에서 프로젝트 디렉터리로 이동합니다.
4. 다음 명령을 실행합니다.

```bash
cd ~/work/postgresql-book
code .
```

처음에는 WSL용 VS Code Server 구성 요소를 설치하느라 시간이 걸릴 수 있습니다. 열린 창의 왼쪽 아래 원격 표시가 WSL 배포판을 가리키는지 확인합니다. 통합 터미널에서 다음 명령을 실행했을 때 Linux 경로가 나와야 합니다.

```bash
pwd
```

`code: command not found`가 나오면 VS Code와 WSL 확장을 설치했는지 확인하고 Ubuntu 터미널을 다시 엽니다. 그래도 안 되면 VS Code에서 명령 팔레트를 열어 **WSL: Connect to WSL**을 실행한 뒤 폴더를 엽니다.

## 11. 원리 이해: WSL을 껐다는 뜻

Ubuntu 터미널 창을 닫는 것은 셸 하나를 종료하는 일입니다. WSL 가상 머신이나 다른 터미널, PostgreSQL 서비스가 즉시 모두 종료된다는 뜻은 아닙니다.

**Windows PowerShell**

```powershell
wsl --list --running
```

특정 배포판만 종료하려면 다음 명령을 사용합니다.

```powershell
wsl --terminate Ubuntu-26.04
```

모든 WSL 배포판과 WSL 2 가상 머신을 종료하려면 다음 명령을 사용합니다.

```powershell
wsl --shutdown
```

`wsl --shutdown`은 실행 중인 작업을 중단할 수 있습니다. 데이터베이스 변경이나 파일 저장이 진행 중일 때 실행하지 않습니다.

## 12. 주의 및 오류 해결

### `wsl` 명령을 찾을 수 없음

PowerShell에서 실행했는지 확인합니다. Windows 업데이트를 적용하고 관리자 PowerShell에서 `wsl --install`을 다시 실행한 뒤 재부팅합니다. 회사 정책 오류가 나오면 관리자에게 문의합니다.

### `0x80370102` 또는 가상화 관련 오류

UEFI/BIOS의 CPU 가상화와 Windows의 Virtual Machine Platform이 활성화되어 있는지 확인합니다. Windows 자체가 가상 머신 안에서 실행 중이라면 중첩 가상화 지원도 필요합니다.

### 설치한 배포판이 WSL 1로 표시됨

`wsl --set-default-version 2`는 앞으로 설치할 배포판의 기본값입니다. 이미 설치한 배포판은 `wsl --set-version <이름> 2`로 별도 변환합니다.

### Ubuntu 비밀번호를 잊음

일반 사용자의 비밀번호와 PostgreSQL 비밀번호를 혼동하지 않습니다. Ubuntu 계정 복구는 Windows에서 해당 배포판을 `root` 사용자로 시작해 처리할 수 있지만, 구체적인 배포판 이름과 계정 소유권을 확인해야 합니다. 중요한 파일을 먼저 백업하고 Microsoft WSL 공식 문제 해결 절차를 따릅니다.

### Windows와 Ubuntu에서 파일 소유자가 이상함

Windows 탐색기나 관리자 권한 명령으로 Linux 시스템 파일을 직접 수정하지 않습니다. 프로젝트는 Ubuntu 사용자 홈에 두고 VS Code의 WSL 연결로 편집합니다. `ls -l`로 소유자를 확인합니다.

### `systemctl`이 systemd로 부팅되지 않았다고 함

`ps -p 1 -o comm=` 결과를 확인하고 앞의 `/etc/wsl.conf` 설정과 WSL 재시작 절차를 적용합니다. 최신 WSL로 업데이트했는지도 확인합니다.

## 13. 실습 문제

### 기본 문제

1. PowerShell에서 설치된 배포판과 WSL 버전을 확인하세요.
2. Ubuntu Bash에서 배포판 버전, 현재 사용자와 현재 디렉터리를 확인하세요.
3. 홈 아래에 `~/work/postgresql-practice`를 만들고 이동하세요.
4. `touch`, `cp`, `ls -l`을 사용해 빈 파일과 복사본을 확인하세요.

### 응용 문제

1. Windows 다운로드 폴더의 Ubuntu 경로와 Ubuntu 홈의 Windows 탐색기 경로를 적어 보세요.
2. PID 1 프로세스와 systemd 상태를 확인하고 결과의 의미를 설명하세요.
3. VS Code를 WSL에 연결한 뒤 통합 터미널의 `pwd`가 Linux 경로인지 확인하세요.

## 실습 문제 정답

### 기본 문제 정답

1. Windows PowerShell에서 `wsl --list --verbose`를 실행하고 대상 배포판의 `VERSION`이 `2`인지 확인합니다.
2. Ubuntu Bash에서 `cat /etc/os-release`, `whoami`, `pwd`를 차례로 실행합니다.
3. `mkdir -p ~/work/postgresql-practice`, `cd ~/work/postgresql-practice`, `pwd`를 실행합니다.
4. `touch original.txt`, `cp original.txt copy.txt`, `ls -l original.txt copy.txt`를 실행합니다.

### 응용 문제 정답

1. Windows 다운로드 폴더는 보통 `/mnt/c/Users/<Windows사용자>/Downloads`입니다. Ubuntu 홈은 탐색기의 `\\wsl.localhost\<배포판이름>\home\<Ubuntu사용자>` 아래에 있습니다.
2. `ps -p 1 -o comm=`에서 PID 1이 `systemd`인지 보고 `systemctl is-system-running`으로 상태를 확인합니다.
3. VS Code의 WSL 연결 표시를 확인한 뒤 통합 터미널의 `pwd`가 `/home/<Ubuntu사용자>/...` 형태인지 확인합니다.

## 14. 핵심 정리

- WSL은 Windows에서 Linux 환경을 실행하는 기능이고 Ubuntu는 Linux 배포판입니다.
- 이 책은 실제 Linux 커널과 systemd를 지원하는 WSL 2를 사용합니다.
- WSL 관리 명령은 PowerShell, Linux 명령은 Ubuntu Bash에서 실행합니다.
- 설치할 배포판의 정확한 이름은 `wsl --list --online`으로 확인합니다.
- `/etc/os-release`에서 Ubuntu 26.04 사용 여부를 확인합니다.
- Ubuntu 사용자와 PostgreSQL 역할은 별도 계정입니다.
- Linux 홈은 `/home/<사용자>`, Windows `C:` 드라이브는 보통 `/mnt/c`입니다.
- Linux 프로젝트와 데이터베이스 파일은 Linux 파일 시스템에 두는 것이 기본입니다.
- VS Code의 WSL 연결을 사용하면 Windows UI로 Linux 파일을 안전하게 편집할 수 있습니다.
- 터미널 창 닫기, 배포판 종료와 `wsl --shutdown`은 서로 다른 동작입니다.

## 15. 확인 문제

1. WSL 2 사용 여부를 표시하는 PowerShell 명령은 무엇입니까?
2. Ubuntu 배포판 버전을 확인하는 파일은 무엇입니까?
3. Windows `C:\work`는 Ubuntu에서 보통 어떤 경로입니까?
4. `~`는 무엇을 뜻합니까?
5. 참 또는 거짓: Ubuntu 창을 하나 닫으면 모든 WSL 배포판이 즉시 종료된다.

정답: 1. `wsl --list --verbose`, 2. `/etc/os-release`, 3. `/mnt/c/work`, 4. 현재 Ubuntu 사용자의 홈 디렉터리, 5. 거짓

## 다음 장 안내

이제 Ubuntu 26.04와 Bash, 파일 시스템과 systemd의 기본을 준비했습니다. 다음 장에서는 Ubuntu 기본 저장소에서 PostgreSQL 18을 설치하고 서버 상태, 포트, 클러스터, 설정 파일과 첫 접속을 차례로 확인합니다.
