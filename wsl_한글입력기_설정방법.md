# WSL Ubuntu 26.04 한글 폰트 설치

## 개요

WSL Ubuntu 26.04에서는 기본적으로 한글 폰트가 설치되어 있지 않아 GUI 프로그램에서 한글이 깨져 보일 수 있습니다.

일반적으로 **Nanum 폰트**와 **Noto CJK 폰트**를 설치하면 대부분의 프로그램에서 한글이 정상적으로 표시됩니다.

---

# 1. 패키지 목록 갱신

```bash
sudo apt update
```

---

# 2. Nanum 한글 폰트 설치

```bash
sudo apt install -y fonts-nanum fonts-nanum-extra
```

설치되는 대표적인 폰트

- NanumGothic
- NanumMyeongjo
- NanumBarunGothic

---

# 3. Noto CJK 폰트 설치

많은 Linux 프로그램에서는 기본적으로 Noto 폰트를 사용합니다.

```bash
sudo apt install -y fonts-noto-cjk
```

또는 전체 Noto 폰트를 설치하려면

```bash
sudo apt install -y fonts-noto
```

---

# 4. 개발자를 위한 D2Coding 폰트 설치 (권장)

코드를 작성하거나 터미널을 사용할 경우 D2Coding 폰트가 가독성이 좋습니다.

```bash
sudo apt install -y fonts-naver-d2coding
```

---

# 5. 폰트 캐시 갱신

폰트를 설치한 후 캐시를 갱신합니다.

```bash
fc-cache -fv
```

---

# 설치 확인

## Nanum 폰트 확인

```bash
fc-list | grep -i nanum
```

예시

```
NanumGothic.ttf
NanumMyeongjo.ttf
NanumBarunGothic.ttf
```

---

## D2Coding 확인

```bash
fc-list | grep D2Coding
```

---

## 한글 폰트 목록 확인

```bash
fc-list :lang=ko
```

---

# 기본 폰트 확인

현재 시스템에서 선택되는 한글 기본 폰트를 확인할 수 있습니다.

Nanum Gothic

```bash
fc-match "NanumGothic"
```

Noto Sans CJK

```bash
fc-match "Noto Sans CJK KR"
```

예시 출력

```
NanumGothic.ttf
```

또는

```
NotoSansCJK-Regular.ttc
```

---

# GUI 프로그램에서 사용

설치 후 대부분의 GUI 프로그램에서 별도 설정 없이 한글이 표시됩니다.

- Firefox
- Chromium
- VS Code(Remote WSL)
- GTK 프로그램
- Qt 프로그램
- Xfce Terminal
- GNOME Terminal

---

# 터미널 글꼴이 깨질 경우

WSL에서는 Ubuntu보다 Windows Terminal의 폰트 설정이 더 중요한 경우가 많습니다.

다음 폰트를 사용하는 것을 권장합니다.

- D2Coding
- Cascadia Mono
- Noto Sans Mono CJK KR
- NanumGothicCoding

---

# 추천 설치 명령

아래 명령 하나만 실행해도 대부분의 개발 환경에서 문제가 없습니다.

```bash
sudo apt update

sudo apt install -y \
    fonts-nanum \
    fonts-nanum-extra \
    fonts-noto-cjk \
    fonts-naver-d2coding

fc-cache -fv
```

---

# 마무리

위 패키지를 설치하면 다음과 같은 환경에서 한글 표시가 정상적으로 동작합니다.

- WSLg
- X11
- Xorg
- xrdp
- XFCE Desktop
- VS Code Remote WSL
- Firefox
- Chromium
- GTK 응용프로그램
- Qt 응용프로그램

또한 `fcitx5`와 함께 사용할 경우 한글 입력 환경도 안정적으로 구성할 수 있습니다.


# WSL(Ubuntu 26.04)에서 한글 입력기 설정하기

WSLg 환경(X11/Wayland 프록시)에서 `gedit` 같은 X 프로그램을 띄울 때 한글 입력이 안 되는 문제를 해결하는 방법입니다. `fcitx5` + `fcitx5-hangul` 조합을 사용합니다.

## 1. 패키지 설치

```bash
sudo apt update
sudo apt install fcitx5 fcitx5-hangul fcitx5-config-qt dbus-x11
```

## 2. 환경변수 설정

`~/.bashrc` (또는 `~/.profile`, `~/.zshrc`)에 아래 내용을 추가합니다.

```bash
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS=@im=fcitx
export SDL_IM_MODULE=fcitx
export GLFW_IM_MODULE=ibus
```

X 프로그램(gedit 등)이 이 값을 읽으려면 fcitx5 데몬이 GUI 앱보다 먼저 떠 있어야 합니다.

## 3. fcitx5 자동 실행

WSLg는 별도의 세션 매니저가 없으므로, 셸 시작 시 데몬을 띄워주는 방식이 편합니다. `~/.bashrc`에 아래 내용을 추가합니다.

```bash
if ! pgrep -x fcitx5 > /dev/null; then
    fcitx5 -d --replace &> /dev/null &
fi
```

또는 WSL 자체의 `/etc/wsl.conf`에 boot 커맨드를 걸어도 됩니다.

```ini
[boot]
command="su - $USER -c 'fcitx5 -d --replace &> /dev/null &'"
```

## 4. 한글 입력기 등록

```bash
fcitx5-configtool
```

GUI가 뜨면 왼쪽 "사용 가능한 입력기" 목록에서 **Hangul**을 찾아 오른쪽 "현재 입력기" 목록에 추가합니다. (검색창에 `hangul` 입력하면 빨리 찾을 수 있습니다.)

## 5. WSL 재시작 후 확인

Windows PowerShell에서 아래 명령으로 WSL을 완전히 재시작합니다.

```powershell
wsl --shutdown
```

다시 Ubuntu 터미널을 열고 `gedit` 같은 프로그램을 실행해서 `Ctrl+Space`(fcitx5 기본 토글 단축키)로 한/영 전환이 되는지 확인합니다.

## 자주 걸리는 문제

- **전환 단축키가 안 먹음**: `fcitx5-configtool` → 전역 설정(Global Options)에서 "입력기 전환" 단축키를 확인합니다. 기본은 `Ctrl+Space`인데 시스템 단축키와 충돌할 수 있습니다.
- **fcitx5는 뜨는데 입력이 안 됨**: `GTK_IM_MODULE` 등 환경변수가 실제로 gedit 프로세스에 전달됐는지 `echo $GTK_IM_MODULE`로 확인합니다. 터미널을 새로 열어야 반영됩니다.
- **GTK4 앱(최신 gedit 등)**: GTK4는 `GTK_IM_MODULE` 변수를 무시하고 자체적으로 IM을 찾는 경우가 있어서, `im-config`로 시스템 기본 IM을 fcitx로 지정해주는 게 도움이 될 수 있습니다.

```bash
sudo apt install im-config
im-config -n fcitx5
```
