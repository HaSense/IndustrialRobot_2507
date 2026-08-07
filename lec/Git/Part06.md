# Part 06. GitHub 협업

> 목표: Issue, 기능 브랜치, Pull Request, 리뷰, Fork를 이용한 협업 흐름을 실습한다.

## 1. 권장 협업 흐름

`Issue → 브랜치 → 커밋 → push → Pull Request → 리뷰 → 병합`

`main`에 바로 작업하지 않고 기능 브랜치에서 작업하면 검토와 취소가 쉬워진다.

## 2. 협업자 초대

저장소 소유자는 GitHub의 `Settings → Collaborators`에서 학생을 초대한다. 초대받은 사람은 알림 또는 이메일에서 수락한다. 조직 저장소는 조직 정책과 역할에 따라 메뉴가 다를 수 있다.

## 3. Issue 만들기

GitHub 저장소의 Issues에서 다음 내용을 등록한다.

```text
제목: 소개 문서 추가

완료 조건
- ABOUT.md가 존재한다.
- 프로젝트 목적이 두 문장 이상 작성된다.
- Pull Request로 검토받는다.
```

Issue 번호가 `#1`이라고 가정한다.

## 4. Issue 기반 작업

```bash
git switch main
git pull --ff-only
git switch -c docs/issue-1-about
printf '# 프로젝트 소개\n\nGit과 GitHub 협업을 학습합니다.\n' > ABOUT.md
git add ABOUT.md
git commit -m "docs: 프로젝트 소개 추가 (#1)"
git push -u origin docs/issue-1-about
```

## 5. Pull Request 만들기

GitHub 웹의 `Compare & pull request`를 누르거나 CLI를 사용한다.

```bash
gh pr create --base main --head docs/issue-1-about \
  --title "프로젝트 소개 문서 추가" \
  --body "Closes #1"
```

`Closes #1`은 PR이 병합될 때 Issue를 자동으로 닫는다.

확인:

```bash
gh pr status
gh pr view --web
```

## 6. 리뷰 반영

리뷰어가 문장 추가를 요청했다고 가정한다.

```bash
printf '브랜치와 Pull Request를 실습합니다.\n' >> ABOUT.md
git add ABOUT.md
git commit -m "docs: 소개 설명 보완"
git push
```

같은 브랜치에 push하면 열려 있는 PR에 자동 반영된다. 리뷰를 통과하면 GitHub에서 병합한다.

```bash
git switch main
git pull --ff-only
git branch -d docs/issue-1-about
git fetch --prune
```

## 7. Fork 방식

원본 저장소에 쓰기 권한이 없을 때 Fork를 사용한다.

1. GitHub에서 원본 저장소의 **Fork**를 누른다.
2. 자신의 계정에 만들어진 Fork를 clone한다.
3. 원본을 `upstream`으로 등록한다.

```bash
git clone https://github.com/YOUR-ID/REPOSITORY.git
cd REPOSITORY
git remote add upstream https://github.com/ORIGINAL-OWNER/REPOSITORY.git
git remote -v
```

원본 동기화:

```bash
git switch main
git fetch upstream
git merge --ff-only upstream/main
git push origin main
```

또는 GitHub CLI를 사용한다.

```bash
gh repo sync YOUR-ID/REPOSITORY -b main
```

기능 브랜치를 자신의 Fork에 push한 뒤 원본 저장소로 PR을 만든다.

## 8. 2인 협업 실습

| 역할 | 작업 |
|---|---|
| 저장소 관리자 | 저장소 생성, Issue 2개, 리뷰 및 병합 |
| 개발자 | clone, 브랜치 작업, PR 생성, 리뷰 반영 |

교대하여 두 역할을 모두 수행한다. 동시에 같은 파일의 같은 줄을 수정하지 않도록 작업 범위를 Issue에 적는다.

## 완료 체크리스트

- [ ] 작업 전 `main`을 최신화했다.
- [ ] Issue 번호가 브랜치·커밋·PR에 연결되어 있다.
- [ ] 기능 브랜치를 원격에 올렸다.
- [ ] PR에서 변경 파일을 직접 확인했다.
- [ ] 리뷰 반영 후 병합했다.
- [ ] 로컬과 원격의 불필요한 브랜치를 정리했다.

## 참고

- [Pull Request 개요](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)
- [Fork 저장소](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/about-permissions-and-visibility-of-forks)
- [Fork 동기화](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/syncing-a-fork)
- [gh pr create](https://cli.github.com/manual/gh_pr_create)

