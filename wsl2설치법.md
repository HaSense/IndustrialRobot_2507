⚙️ BIOS에서 확인할 주요 옵션

WSL2를 위해 다음을 Enabled로 설정해야 합니다.

Intel Virtualization Technology (VT-x)

VT-d (입출력 가상화)



Secure Boot → Enabled 유지
🧩 1. WSL2 설치
# 1️⃣ WSL 및 Ubuntu 자동 설치
wsl --install

# 2️⃣ 설치 가능한 배포판 목록 확인
wsl --list --online

# 3️⃣ 특정 Ubuntu 버전 선택 (예: Ubuntu 24.04)
wsl --install -d Ubuntu-24.04

# 4️⃣ 설치 확인
wsl --list --verbose

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

💻 3. Visual Studio Code 설치
# 1️⃣ Microsoft 공식 저장소 등록
winget search vscode        # 설치 가능 여부 확인
winget install --id Microsoft.VisualStudioCode -e


설치 후, WSL 확장(“Remote - WSL”) 설치 필요
VSCode 실행 후 확장창에서 아래 명령:

ext install ms-vscode-remote.remote-wsl

🧰 4. VS Code + WSL 통합 사용 확인
# Windows 터미널에서 Ubuntu 접속
wsl

# Ubuntu 터미널에서 VS Code 실행
code .


💡 “code .” 명령이 동작하지 않을 경우:

sudo apt install code

🔍 5. (선택) 추가 도구 설치
# Node.js (nvm 방식 권장)
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/master/install.sh | bash
source ~/.bashrc
nvm install --lts

# Python3 + pip
sudo apt install python3 python3-pip -y

# Git 설정
git config --global user.name "YourName"
git config --global user.email "you@example.com"

✅ 설치 확인 요약
항목	확인 명령어	예시 결과
WSL 버전	wsl --version	WSL version: 2.x
Ubuntu 실행	wsl	Linux 프롬프트
VS Code 실행	code .	VSCode GUI 실행
Node 버전	node -v	v20.x
Python 버전	python3 --version	3.12.x
