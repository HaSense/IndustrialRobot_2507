# Part 07. 문제 해결과 실무 활용

> 목표: 오류를 원인별로 진단하고 여러 계정과 민감정보를 안전하게 관리한다.

## 1. 진단 순서

오류가 나면 명령을 반복하기 전에 다음을 확인한다.

```bash
pwd
git status
git branch -vv
git remote -v
git log --oneline --graph --decorate --all -10
gh auth status
```

오류 문장의 첫 줄과 마지막 줄을 읽고 **폴더, 브랜치, 원격 주소, 인증, 권한, 이력** 중 어디가 문제인지 분류한다.

## 2. 대표 오류

### not a git repository

```bash
pwd
ls -la
```

저장소 폴더로 이동하거나 새 프로젝트라면 `git init`한다. 무조건 `git init`하면 중첩 저장소가 생길 수 있다.

### remote origin already exists

```bash
git remote -v
git remote set-url origin 올바른주소
```

### src refspec main does not match any

```bash
git branch --show-current
git log --oneline
git branch -M main
```

첫 커밋이 없는 경우 파일을 커밋한 뒤 push한다.

### non-fast-forward / failed to push some refs

```bash
git fetch origin
git log --oneline --graph --decorate --all
git pull --rebase
git push
```

원인을 모르는 상태에서 `push --force`를 사용하지 않는다.

### Authentication failed / 403

```bash
gh auth status
gh auth switch --user YOUR-ID
git remote -v
```

계정, 저장소 권한, URL, 토큰 범위와 만료, 조직 SSO 정책을 확인한다.

### Repository not found

저장소가 실제로 존재하는지, 대소문자와 소유자명이 맞는지, 비공개 저장소 접근 권한이 있는지 확인한다.

## 3. 여러 GitHub 계정: GitHub CLI

```bash
gh auth login
gh auth status
gh auth switch --user ACCOUNT-NAME
```

CLI의 활성 계정과 커밋 작성자 정보는 별개다. 저장소별 작성자도 설정한다.

```bash
git config user.name "Work Name"
git config user.email "work@example.com"
git config --local --list
```

## 4. 여러 계정: SSH

계정별 키를 만든다.

```bash
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519_personal -C "personal@example.com"
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519_work -C "work@example.com"
```

`~/.ssh/config` 예시:

```sshconfig
Host github-personal
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519_personal
    IdentitiesOnly yes

Host github-work
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519_work
    IdentitiesOnly yes
```

각 공개키를 해당 GitHub 계정에 등록한 뒤 확인한다.

```bash
ssh -T git@github-personal
ssh -T git@github-work
git remote set-url origin git@github-work:WORK-ID/REPOSITORY.git
```

## 5. 민감정보 보호

`.gitignore` 예시:

```gitignore
.env
.env.*
!.env.example
*.pem
*.key
credentials.json
```

예시 환경 파일에는 실제 값을 넣지 않는다.

```dotenv
DATABASE_URL=postgresql://USER:PASSWORD@HOST/DB
API_KEY=replace-me
```

비밀값을 커밋했다면 파일만 삭제하는 것으로 끝나지 않는다.

1. 키·토큰을 즉시 폐기하고 새로 발급한다.
2. 저장소 접근 기록과 사용 내역을 점검한다.
3. 필요하면 `git filter-repo` 등으로 이력을 정리하고 협업자와 재동기화한다.

## 6. 공용·교육용 PC 정리

```bash
gh auth logout
git config --global --unset user.name
git config --global --unset user.email
```

자신이 만든 SSH 키가 맞는지 정확히 확인한 뒤 제거한다. 저장소를 지워도 GitHub 로그인이나 WSL의 자격 증명이 자동으로 삭제되지는 않는다.

## 7. 실무 규칙

- 작업 전 `git status`와 최신 원격 상태를 확인한다.
- 한 커밋에는 하나의 목적을 담는다.
- `main` 직접 작업을 줄이고 Pull Request로 검토한다.
- `--force` 대신 원인과 이력 그래프를 먼저 확인한다.
- 생성물, 가상환경, 비밀값을 `.gitignore`로 관리한다.
- 중요한 배포 지점에는 태그를 사용한다.

```bash
git tag -a v1.0.0 -m "첫 번째 실습 버전"
git push origin v1.0.0
```

## 참고

- [GitHub 인증 문제 해결](https://docs.github.com/en/authentication/troubleshooting-ssh)
- [원격 저장소 문제 해결](https://docs.github.com/en/get-started/git-basics/managing-remote-repositories)
- [민감정보 제거](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/removing-sensitive-data-from-a-repository)
- [gh auth switch](https://cli.github.com/manual/gh_auth_switch)

