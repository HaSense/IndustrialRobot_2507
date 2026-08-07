# Part 04. 원격 저장소 활용하기

> 목표: 저장소를 복제하고 `fetch`, `pull`, `push`, `remote`를 구분해 사용한다.

## 1. clone 실습

기존 폴더와 충돌하지 않도록 새 위치에서 복제한다.

```bash
cd ~/workspace
mkdir -p clone-lab
cd clone-lab
git clone https://github.com/YOUR-ID/git-practice.git
cd git-practice
git remote -v
git branch -vv
git log --oneline --graph --all
```

`clone`은 파일뿐 아니라 커밋 이력과 원격 추적 브랜치 설정도 가져온다. ZIP 다운로드에는 이 정보가 없다.

## 2. 다른 컴퓨터에서 이어서 작업하는 흐름

작업 전:

```bash
git status
git pull --ff-only
```

작업 후:

```bash
git add .
git commit -m "학습 기록 추가"
git push
```

`--ff-only`는 로컬과 원격 이력이 갈라졌을 때 자동 병합하지 않고 멈추므로 초급자가 상황을 확인하기 좋다.

## 3. fetch와 pull 비교

GitHub 웹에서 `remote-note.md`를 만들고 커밋한 후 WSL에서 실행한다.

```bash
git fetch origin
git status
git log --oneline --graph --decorate --all
git diff main..origin/main
```

`fetch`는 원격 정보를 갱신하지만 현재 파일을 바꾸지 않는다. 확인 후 병합한다.

```bash
git merge --ff-only origin/main
```

다음 명령은 가져오기와 현재 브랜치 통합을 연속 수행한다.

```bash
git pull --ff-only
```

## 4. push가 거부되는 상황

GitHub에 로컬에 없는 커밋이 있으면 먼저 가져와야 한다.

```bash
git fetch origin
git log --oneline --graph --decorate --all
git pull --rebase
git push
```

`pull --rebase`는 자신의 아직 공유되지 않은 로컬 커밋을 최신 원격 커밋 위에 다시 놓는다. 충돌이 발생하면 Part 05의 절차로 해결한다.

## 5. remote 관리

```bash
git remote
git remote -v
git remote get-url origin
git remote show origin
```

주소 변경:

```bash
git remote set-url origin git@github.com:YOUR-ID/git-practice.git
```

이름 변경과 삭제:

```bash
git remote rename origin github
git remote rename github origin
```

```bash
git remote remove 잘못된이름
```

`remote remove`는 GitHub 저장소를 삭제하지 않고 로컬 연결 정보만 지운다.

## 6. 추적 브랜치 이해

```bash
git branch -vv
git status -sb
git branch --remotes
```

- `main`: 로컬 브랜치
- `origin/main`: 마지막으로 확인한 원격 `main` 상태
- `upstream`: Fork 협업에서 원본 저장소에 흔히 붙이는 이름

추적 설정:

```bash
git branch --set-upstream-to=origin/main main
```

## 7. 안전한 동기화 체크리스트

```bash
git status
git fetch --prune
git log --oneline --graph --decorate --all -10
git pull --ff-only
```

작업 파일이 남아 있으면 먼저 커밋하거나 `git stash`로 임시 보관한다.

```bash
git stash push -m "작업 중 임시 보관"
git pull --ff-only
git stash pop
```

## 종합 실습

1. 저장소를 다른 폴더에 clone한다.
2. GitHub 웹에서 파일을 수정한다.
3. `fetch` 후 차이를 확인한다.
4. `merge --ff-only`로 반영한다.
5. WSL에서 새 커밋을 만들고 push한다.

## 참고

- [git clone](https://git-scm.com/docs/git-clone)
- [git fetch](https://git-scm.com/docs/git-fetch)
- [git pull](https://git-scm.com/docs/git-pull)
- [GitHub 원격 저장소 안내](https://docs.github.com/en/get-started/git-basics/about-remote-repositories)

