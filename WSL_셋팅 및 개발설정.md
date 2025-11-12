# ⚙️ WSL2 + Ubuntu 설치 및 설정 가이드

---

## 🧩 BIOS 설정

WSL2를 사용하기 전에 BIOS에서 아래 항목을 **Enabled**로 설정하세요.

- Intel Virtualization Technology (VT-x)
- VT-d (입출력 가상화)
- Secure Boot → Enabled 유지

---

## 🚀 1. WSL2 설치

### ▪️ WSL 및 Ubuntu 자동 설치
```bash
wsl --install Ubuntu
```

### ▪️ 설치 가능한 배포판 목록 확인
```bash
wsl --list --online
```

### ▪️ 특정 Ubuntu 버전 선택 (예: Ubuntu 24.04)
```bash
wsl --install -d Ubuntu-24.04
```

### ▪️ 설치 확인
```bash
wsl --list --verbose
# 또는
wsl -l -v
```

### ▪️ Ubuntu 첫 실행 및 버전 확인
```bash
wsl --version
uname -a
```

💡 **참고:**  
기본 설치 시 WSL2가 자동 적용됩니다.  
필요 시 수동으로 버전 지정 가능:
```bash
wsl --set-default-version 2
```

---

## ⚙️ 2. Ubuntu 초기 설정

Ubuntu 터미널에서 다음 명령 실행:
build-essential은 우분투에서 c/c++ 개발도구 설치

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install build-essential -y
```

---

## 🧰 3. VS Code + WSL 통합 설정

### ▪️ VS Code 확장 설치
VS Code 실행 → 확장창에서 아래 명령 입력:
```
ext install ms-vscode-remote.remote-wsl
```

### ▪️ 통합 실행 확인
```bash
# Windows 터미널에서
wsl

# Ubuntu 터미널에서
code .
```

---

## 🧹 4. 배포판 관리

### ▪️ 배포판 삭제
```bash
wsl --unregister Ubuntu
```

---

## 🌏 5. Ubuntu 24.04 미러 사이트 변경 (카카오)

```bash
sudo nano /etc/apt/sources.list.d/ubuntu.sources
```

> URIs 항목을 아래처럼 변경  
> ```
> URIs: http://ftp.daumkakao.com/ubuntu/
> ```

---

## 🧱 6. 개발 도구 설치

```bash
sudo apt install build-essential
```

---

## 🐧 7. 기본 리눅스 명령어

| 명령어 | 설명 |
|--------|------|
| sudo apt update | 저장소 갱신 (카카오 미러 반영) |
| cd ~/ | 홈 디렉터리 이동 |
| pwd | 현재 경로 확인 |
| ls / ls -l / ls -a | 디렉터리 목록 보기 |
| mkdir | 새 디렉터리 생성 |

---

## 📁 디렉터리 구조 예시
```
/home/robot/work/basic/
```

---

## ✏️ Nano 에디터 사용 예시
```bash
cd /home/robot/work/basic/
nano hello.c
```

---

## ⚙️ 컴파일 방법
```bash
gcc hello.c                # 기본 컴파일
gcc -c hello.c             # 오브젝트 파일 생성
gcc -o hello hello.c       # 실행 파일 생성
```

---

## ✅ 실행
```bash
./hello
```

---

## 🧩 Visual Studio Code

**홈페이지:** [https://code.visualstudio.com/](https://code.visualstudio.com/)

---

### 🔌 Plug-in 

1. **Korean Language Pack**  
   → 영문이 편한 분은 설치하지 않아도 됩니다.

2. **C/C++**, **C/C++ Extension**  
   → *Microsoft* 제품을 설치하세요.

3. **WSL**  
   → WSL에 접속하기 위한 플러그인입니다.  
     이 역시 *Microsoft* 제품을 설치하세요

          


