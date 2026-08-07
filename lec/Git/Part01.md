# Part 01. Git과 GitHub 시작하기

> 대상: Git을 처음 사용하는 학습자  
> 환경: Windows 11, WSL2, Ubuntu, VS Code  
> 목표: Git과 GitHub의 차이를 이해하고 WSL에 실습 환경을 구성한다.

## 1. Git과 GitHub는 무엇인가

- **Git**: 파일 변경 이력을 내 컴퓨터에 기록하는 버전 관리 프로그램
- **GitHub**: Git 저장소를 인터넷에서 보관하고 공유·협업하는 서비스
- **저장소(repository)**: 프로젝트 파일과 변경 이력을 함께 관리하는 공간
- **커밋(commit)**: 특정 시점의 변경사항에 설명을 붙여 저장한 기록

작업 흐름은 `파일 수정 → git add → git commit → git push` 순서로 진행된다. `commit`은 로컬 저장이고 `push`는 GitHub 전송이다.

## 2. WSL 환경 확인

PowerShell에서 확인한다.

```powershell
wsl --status
wsl --list --verbose
```

Ubuntu 터미널에서 다음을 실행한다.

```bash
uname -a
cat /etc/os-release
pwd
whoami
```

### 실습 폴더 위치

Git 프로젝트는 가능하면 WSL 홈 아래에 둔다. `/mnt/c`보다 Linux 파일 권한과 파일 감시가 자연스럽고 대체로 빠르다.

```bash
mkdir -p ~/workspace
cd ~/workspace
pwd
```

예상 결과는 `/home/사용자명/workspace` 형태다.

## 3. Git 설치

```bash
sudo apt update
sudo apt install -y git
git --version
```

도움말도 명령으로 확인할 수 있다.

```bash
git help init
git status --help
```

## 4. VS Code와 WSL 연결

Windows에 VS Code와 **WSL 확장**을 설치한 뒤 WSL 터미널에서 실행한다.

```bash
cd ~/workspace
code .
```

VS Code 왼쪽 아래에 `WSL: Ubuntu`가 표시되는지 확인한다.

## 5. Git 사용자 설정

아래 값은 자신의 정보로 바꾼다. GitHub 계정 이름과 `user.name`은 같지 않아도 되지만 이메일은 GitHub 계정에 등록된 이메일 또는 GitHub의 비공개 이메일을 사용하는 것이 좋다.

```bash
git config --global user.name "Student Name"
git config --global user.email "student@example.com"
git config --global init.defaultBranch main
git config --global core.autocrlf input
```

설정을 확인한다.

```bash
git config --global --list
git config --global --get user.name
git config --global --get user.email
```

특정 저장소에서만 다른 정보를 사용하려면 해당 폴더에서 `--global` 없이 설정한다.

```bash
git config user.name "Company Name"
git config user.email "company@example.com"
```

## 6. 첫 실습 프로젝트 준비

```bash
cd ~/workspace
mkdir git-practice
cd git-practice
printf '# Git Practice\n' > README.md
printf 'print("Hello Git")\n' > hello.py
ls -la
```

아직 Git 저장소가 아니므로 다음 명령은 오류가 난다.

```bash
git status
```

`fatal: not a git repository`는 고장이 아니라 현재 폴더에 `.git`이 없다는 뜻이다.

## 7. 점검 실습

1. Git과 GitHub의 차이를 한 문장씩 적는다.
2. `~/workspace/git-practice`를 VS Code로 연다.
3. Git 사용자 이름과 이메일을 확인한다.
4. `README.md`, `hello.py`가 있는지 확인한다.

```bash
pwd
git --version
git config --global --list
ls -la
```

## 핵심 정리

- WSL과 Windows의 Git 설정은 서로 별개일 수 있다.
- Git 저장소는 변경 이력을 관리하고 GitHub는 저장소 공유와 협업을 제공한다.
- `commit`과 `push`는 같은 작업이 아니다.

## 참고

- [Git 공식 문서](https://git-scm.com/docs)
- [GitHub의 Git 시작 안내](https://docs.github.com/en/get-started/using-git/about-git)

