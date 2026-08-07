# Part 03. GitHub 연결과 인증

> 목표: GitHub 저장소를 만들고 WSL의 로컬 저장소를 안전하게 연결한다.

## 1. 인증 방법 선택

GitHub는 Git 작업에서 계정 비밀번호 인증을 지원하지 않는다. 이 교재의 권장 순서는 다음과 같다.

1. **GitHub CLI**: 수업에서 가장 간단한 기본 방법
2. **SSH 키**: 개인 개발 PC에서 편리한 방법
3. **PAT**: HTTPS 비밀번호 입력란에 토큰을 사용하는 방법

토큰이나 개인키를 채팅, 문서, 저장소에 기록하지 않는다.

## 2. GitHub CLI 설치

Ubuntu 패키지에 `gh`가 제공되는 경우 다음을 사용한다.

```bash
sudo apt update
sudo apt install -y gh
gh --version
```

패키지가 없거나 너무 오래된 경우 [GitHub CLI 공식 설치 안내](https://github.com/cli/cli/blob/trunk/docs/install_linux.md)를 따른다.

## 3. 로그인

```bash
gh auth login
```

화면에서 일반적으로 다음을 선택한다.

- GitHub.com
- HTTPS
- 브라우저 로그인
- Git 자격 증명 설정: Yes

브라우저가 자동으로 열리지 않으면 표시된 일회용 코드를 Windows 브라우저의 안내 주소에 입력한다.

```bash
gh auth status
gh auth setup-git
```

## 4. GitHub 저장소 만들기

웹에서 `git-practice`라는 **빈 저장소**를 만든다. 로컬에 이미 README가 있으므로 GitHub에서 README, `.gitignore`, License 초기화를 선택하지 않는다.

사용자명을 확인한다.

```bash
gh api user --jq .login
```

이하 `YOUR-ID`는 자신의 GitHub 사용자명으로 바꾼다.

## 5. 원격 저장소 연결

```bash
cd ~/workspace/git-practice
git remote add origin https://github.com/YOUR-ID/git-practice.git
git remote -v
git push -u origin main
```

`origin`은 원격 저장소의 관례적 별칭이다. `-u`는 로컬 `main`이 `origin/main`을 추적하게 한다. 이후에는 다음만 실행해도 된다.

```bash
git push
```

## 6. CLI로 저장소 생성과 게시

웹 대신 현재 폴더에서 한 번에 만들 수도 있다. 이미 원격 저장소를 연결했다면 실행하지 않는다.

```bash
gh repo create git-practice --private --source=. --remote=origin --push
```

## 7. SSH 방식

기존 키를 확인한다.

```bash
ls -la ~/.ssh
```

새 키를 만든다. 이메일은 자신의 값으로 바꾼다.

```bash
ssh-keygen -t ed25519 -C "student@example.com"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
cat ~/.ssh/id_ed25519.pub
```

출력된 **공개키**를 GitHub의 `Settings → SSH and GPG keys → New SSH key`에 등록한다. `id_ed25519` 개인키 내용은 절대 복사하거나 공유하지 않는다.

```bash
ssh -T git@github.com
git remote set-url origin git@github.com:YOUR-ID/git-practice.git
git remote -v
git push
```

첫 연결의 호스트 지문은 GitHub 공식 문서와 대조한 후 승인한다.

## 8. PAT 방식

GitHub 설정에서 fine-grained PAT를 만들고 대상 저장소와 필요한 최소 권한만 부여한다. HTTPS 인증 창의 사용자명에는 GitHub ID, 비밀번호에는 PAT를 입력한다. 일반 비밀번호는 사용할 수 없다.

토큰을 URL에 넣거나 셸 기록에 남기지 않는다.

## 9. 원격 연결 오류 해결

```bash
git remote -v
git branch --show-current
gh auth status
```

### origin이 이미 존재함

```bash
git remote set-url origin https://github.com/YOUR-ID/git-practice.git
```

### 잘못된 origin 삭제

```bash
git remote remove origin
```

### refspec main 오류

첫 커밋과 브랜치를 확인한다.

```bash
git log --oneline
git branch -M main
```

### 403 오류

- 로그인 계정이 저장소 소유자 또는 협업자인지 확인한다.
- 원격 URL의 사용자명·저장소명이 정확한지 확인한다.
- PAT 대상 저장소와 Contents 쓰기 권한을 확인한다.
- 조직 저장소라면 SSO 승인 정책을 확인한다.

## 실습 완료 기준

```bash
git status
git remote -v
git branch -vv
gh auth status
```

GitHub 웹 화면에서 모든 커밋과 파일이 보이면 완료다.

## 참고

- [GitHub 인증 개요](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/about-authentication-to-github)
- [Personal Access Token 관리](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens)
- [SSH로 GitHub 연결](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)
- [gh auth login](https://cli.github.com/manual/gh_auth_login)

