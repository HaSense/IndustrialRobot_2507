# ⚙️ WSL2 + Ubuntu 설치 및 설정 가이드

---

## 🧩 1. BIOS 설정

WSL2를 사용하기 전에 BIOS에서 다음 항목을 **Enabled**로 설정하세요.

- **Intel Virtualization Technology (VT-x)**
- **VT-d (입출력 가상화)**  
- **Secure Boot** → Enabled 유지

---

## 🚀 2. WSL2 설치

### ① WSL 및 Ubuntu 자동 설치
```bash
wsl --install Ubuntu
② 설치 가능한 배포판 목록 확인
bash
코드 복사
wsl --list --online
③ 특정 Ubuntu 버전 설치 (예: Ubuntu 24.04)
bash
코드 복사
wsl --install -d Ubuntu-24.04
④ 설치 상태 확인
bash
코드 복사
wsl --list --verbose
# 또는
wsl -l -v
⑤ 버전 확인
bash
코드 복사
wsl --version
uname -a
💡 참고:
기본 설치 시 WSL2가 자동으로 적용됩니다.
필요 시 수동으로 버전 지정 가능:

bash
코드 복사
wsl --set-default-version 2
🛠️ 3. Ubuntu 초기 설정
Ubuntu 터미널에서 다음 명령을 실행합니다.

bash
코드 복사
sudo apt update && sudo apt upgrade -y
sudo apt install build-essential curl git -y
🧰 4. VS Code + WSL 통합 설정
(1) VS Code 확장 설치
VS Code 실행 → 확장(Extensions) 창에서 아래 명령 실행:

nginx
코드 복사
ext install ms-vscode-remote.remote-wsl
(2) 통합 실행 확인
Windows 터미널에서:

bash
코드 복사
wsl
Ubuntu 터미널에서:

bash
코드 복사
code .
🧹 5. 배포판 관리
배포판 삭제
bash
코드 복사
wsl --unregister Ubuntu
🌏 6. Ubuntu 24.04 미러사이트 변경 (카카오)
bash
코드 복사
sudo nano /etc/apt/sources.list.d/ubuntu.sources
URIs: 항목을 아래와 같이 수정

arduino
코드 복사
URIs: http://ftp.daumkakao.com/ubuntu/
🧱 7. 개발 도구 설치
bash
코드 복사
sudo apt install build-essential
🐧 8. 기본 리눅스 명령어 요약
명령어	설명
sudo apt update	저장소 정보 갱신 (카카오 미러 반영)
cd ~/	홈 디렉터리 이동
pwd	현재 폴더 위치 확인
ls, ls -l, ls -a	디렉터리 목록 표시
mkdir	새 폴더 생성

📁 디렉터리 구조 예시
swift
코드 복사
/home/robot/work/basic/
✏️ Nano 에디터 사용 예시
bash
코드 복사
cd /home/robot/work/basic/
nano hello.c
⚙️ 컴파일 방법
bash
코드 복사
gcc hello.c                # 기본 컴파일
gcc -c hello.c             # 오브젝트 파일 생성
gcc -o hello hello.c       # 실행 파일 생성
✅ 실행
bash
코드 복사
./hello
yaml
코드 복사

---

이 버전은 GitHub Markdown에서  
- 제목 크기 균형(2~3단계 헤더 중심)  
- 코드블록은 bash 구문 강조  
- 구분선(`---`)으로 섹션 분리  
- 표와 인라인 강조로 가독성 향상  
되어 있습니다.

원하신다면 “교육용 배포 PDF용 버전(가독성 높은 폰트/배경)”으로도 변환해드릴 수 있습니다.  
필요하신가요?
