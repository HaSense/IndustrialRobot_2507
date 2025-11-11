⚙️ BIOS에서 확인할 주요 옵션

WSL2를 위해 다음을 Enabled로 설정해야 합니다.

Intel Virtualization Technology (VT-x)

VT-d (입출력 가상화)



Secure Boot → Enabled 유지
🧩 1. WSL2 설치
# 1️⃣ WSL 및 Ubuntu 자동 설치
wsl --install Ubuntu

# 2️⃣ 설치 가능한 배포판 목록 확인
wsl --list --online

# 3️⃣ 특정 Ubuntu 버전 선택 (예: Ubuntu 24.04)
wsl --install Ubuntu

ex) wsl --install -d Ubuntu-24.04

# 4️⃣ 설치 확인
wsl --list --verbose

wsl -l -v


# 5️⃣ Ubuntu 첫 실행 및 사용자 계정 설정 후 버전 확인
wsl --version

uname -a


💡 참고

기본 설치 시 WSL2가 자동으로 적용됨

필요 시 수동으로 버전 지정 가능

wsl --set-default-version 2

⚙️ 2. Ubuntu 패키지 초기 설정
# Ubuntu 터미널에서 실행
sudo apt update && sudo apt upgrade -y
sudo apt install build-essential curl git -y

설치 후, WSL 확장(“Remote - WSL”) 설치 필요
VSCode 실행 후 확장창에서 아래 명령:

ext install ms-vscode-remote.remote-wsl

🧰 4. VS Code + WSL 통합 사용 확인
# Windows 터미널에서 Ubuntu 접속
wsl

# Ubuntu 터미널에서 VS Code 실행
code .

# 배포판 삭제방법 

wsl --unregister Ubuntu

# Ubuntu 24.04 mirror 사이트 카카오로 변경 방법

sudo nano /etc/apt/sources.list.d/ubuntu.sources

> URIs: http://ftp.daumkakao.com/ubuntu/ 으로 변경

# gcc, make, g++ 등 한번에 받기

sudo apt install build-essential



# 리눅스 명령어

sudo apt update  : repository(원격저장소) 갱신 - 카카오 미러 사이트로 갱신

cd ~/  : 여러분 계정의 Home 디렉토리로 가는 명령어

pwd : 현재 폴더의 위치를 알수 있는 명령어

ls : 디렉토리 검색 옵션이 있습니다. -l -a

mkdir : 디렉터리 만들기 (폴더 만든다)

## 여러분이 만들어야 할 디렉터리 구조 /home/robot/work/basic/

### 나노 에디터로 에디팅 하는 방법 
$> cd /home/robot/work/basic/

$> nano hello.c

코딩을 하세요.

### 컴파일 방법

$> gcc hello.c

$> gcc -c hello.c  

   오브젝트 파일이 만들어 진다.

$> gcc -o hello hello.c

hello.c 파일을 컴파일 하면서 바로 hello 실행 파일을 만든다.






