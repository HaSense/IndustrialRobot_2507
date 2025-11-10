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
