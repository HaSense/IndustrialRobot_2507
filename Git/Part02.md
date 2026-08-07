# Part 02. 로컬 Git 저장소 사용하기

> 목표: 저장소를 만들고 파일의 상태를 확인하며 커밋·비교·복구를 실습한다.

## 1. 저장소 초기화

```bash
cd ~/workspace/git-practice
git init
git status
ls -la
```

`git init`은 현재 폴더에 `.git` 디렉터리를 만든다. 일반 프로젝트 파일을 삭제하지 않는다.

```bash
git branch --show-current
```

아직 첫 커밋 전이라 출력이 비어 있을 수 있다. `git status`에는 `On branch main`이 표시된다.

## 2. 첫 번째 커밋

```bash
git status
git add README.md hello.py
git status
git commit -m "프로젝트 시작"
git log --oneline
```

상태는 다음 세 구역으로 생각한다.

1. 작업 영역: 편집 중인 실제 파일
2. 스테이징 영역: 다음 커밋에 넣을 변경사항
3. 저장소: 커밋으로 확정된 이력

## 3. 수정과 비교

```bash
printf '\nGit 학습 프로젝트입니다.\n' >> README.md
git status --short
git diff
git add README.md
git diff --staged
git commit -m "README 설명 추가"
```

`git diff`는 아직 스테이징하지 않은 차이, `git diff --staged`는 다음 커밋에 들어갈 차이를 보여준다.

## 4. 여러 파일을 나누어 커밋하기

```bash
mkdir -p docs
printf '# 사용법\n\npython3 hello.py\n' > docs/usage.md
printf 'print("Git status practice")\n' >> hello.py
git status --short
```

문서와 프로그램 변경을 별도 커밋으로 나눈다.

```bash
git add docs/usage.md
git commit -m "사용법 문서 추가"
git add hello.py
git commit -m "실행 메시지 추가"
git log --oneline --graph --decorate
```

좋은 커밋은 하나의 목적만 설명하고, 다시 적용하거나 취소하기 쉬운 크기다.

## 5. 변경사항 되돌리기

### 커밋하지 않은 수정 복구

```bash
printf '잘못된 내용\n' >> README.md
git diff
git restore README.md
git status
```

### 스테이징 취소

```bash
printf '임시 내용\n' >> README.md
git add README.md
git restore --staged README.md
git status --short
git restore README.md
```

### 최근 커밋 메시지 수정

```bash
git commit --amend -m "새 커밋 메시지"
```

공유된 커밋을 `amend`하면 커밋 ID가 바뀐다. GitHub에 이미 올린 공용 이력에서는 주의한다.

## 6. 안전한 커밋 취소

실습용 커밋을 만든다.

```bash
printf 'temporary=true\n' > temp.conf
git add temp.conf
git commit -m "임시 설정 추가"
git log --oneline -3
```

그 커밋을 새 커밋으로 취소한다.

```bash
git revert HEAD
git log --oneline -3
```

`revert`는 공유 이력에 적합하다. `reset`은 브랜치 위치와 이력을 바꾸므로 초급 단계에서는 공용 브랜치에 사용하지 않는다.

## 7. .gitignore

```bash
printf '.venv/\n__pycache__/\n.env\n*.log\n.vscode/\n' > .gitignore
mkdir -p __pycache__
printf 'SECRET=do-not-commit\n' > .env
touch app.log
git status --short
git check-ignore -v .env app.log
git add .gitignore
git commit -m "Git 제외 규칙 추가"
```

이미 추적 중인 파일은 `.gitignore`에 적어도 자동으로 제외되지 않는다.

```bash
git rm --cached 파일명
```

## 8. 저장소 초기화 취소

학습용 복사본에서만 원리를 확인한다.

```bash
cd ~/workspace
cp -r git-practice git-practice-copy
cd git-practice-copy
rm -rf .git
git status
```

`.git` 삭제는 모든 로컬 이력을 제거한다. 프로젝트 파일은 남지만 복구가 어려우므로 실제 저장소에서는 신중해야 한다.

## 종합 실습

1. `student.md`에 이름과 학습 목표를 작성한다.
2. 첫 커밋을 만든다.
3. 내용을 수정하고 `git diff`를 확인한다.
4. 두 번째 커밋을 만든다.
5. `git log --oneline --graph` 결과를 캡처한다.

## 참고

- [git init](https://git-scm.com/docs/git-init)
- [git restore](https://git-scm.com/docs/git-restore)
- [git revert](https://git-scm.com/docs/git-revert)

