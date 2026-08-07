# Part 08. 종합 실습 프로젝트

> 목표: 개인 게시와 2인 협업을 처음부터 끝까지 수행하고 결과를 평가한다.

## 프로젝트 A. 개인 프로젝트 게시

### 요구사항

Python 명령행 프로그램 `task-note`를 만든다.

- `app.py`: 할 일 입력과 출력
- `README.md`: 설치·실행 방법
- `.gitignore`: 가상환경, 캐시, 환경변수 제외
- 최소 4개의 의미 있는 커밋
- `v1.0.0` 태그

### 1단계: 시작

```bash
cd ~/workspace
mkdir task-note && cd task-note
git init
printf '# Task Note\n' > README.md
printf '.venv/\n__pycache__/\n.env\n' > .gitignore
git add README.md .gitignore
git commit -m "chore: 프로젝트 초기화"
```

### 2단계: 기능 구현

```python
# app.py
tasks = []

while True:
    command = input("add/list/quit> ").strip()
    if command == "add":
        tasks.append(input("할 일: ").strip())
    elif command == "list":
        for index, task in enumerate(tasks, start=1):
            print(f"{index}. {task}")
    elif command == "quit":
        break
```

```bash
python3 app.py
git add app.py
git commit -m "feat: 할 일 등록과 조회 추가"
```

README 실행법과 사용 예시를 추가하고 별도 커밋한다.

### 3단계: GitHub 게시

```bash
gh repo create task-note --public --source=. --remote=origin --push
git tag -a v1.0.0 -m "첫 공개 버전"
git push origin v1.0.0
```

### 개인 프로젝트 평가

| 항목 | 점수 |
|---|---:|
| 저장소와 파일 구조 | 15 |
| 커밋 분리와 메시지 | 20 |
| README 완성도 | 20 |
| `.gitignore`와 보안 | 15 |
| GitHub push와 태그 | 20 |
| 실행 확인 | 10 |

## 프로젝트 B. 2인 협업

### 역할과 요구사항

- 관리자: 저장소, Issue 2개, 리뷰, 병합
- 개발자: clone, 기능 브랜치, PR, 리뷰 반영
- 두 번째 라운드에서 역할 교대

### 작업 1: 저장소 준비

```bash
mkdir team-note && cd team-note
git init
printf '# Team Note\n' > README.md
git add README.md
git commit -m "프로젝트 시작"
gh repo create team-note --private --source=. --remote=origin --push
```

관리자는 협업자를 초대하고 다음 Issue를 만든다.

- Issue 1: 메모 추가 기능
- Issue 2: 메모 목록 기능

### 작업 2: 기능 브랜치

```bash
git switch main
git pull --ff-only
git switch -c feature/issue-1-add-note
```

기능을 구현하고 테스트한 뒤:

```bash
git add .
git commit -m "feat: 메모 추가 기능 (#1)"
git push -u origin feature/issue-1-add-note
gh pr create --base main --title "메모 추가 기능" --body "Closes #1"
```

### 작업 3: 리뷰와 병합

리뷰어는 다음을 확인한다.

- 요구사항 충족 여부
- 불필요한 파일 포함 여부
- 실행 오류 여부
- 이름과 메시지의 이해 가능성

수정 요청을 반영해 같은 브랜치에 push하고 승인 후 병합한다.

### 작업 4: 동기화와 정리

```bash
git switch main
git pull --ff-only
git branch -d feature/issue-1-add-note
git fetch --prune
```

## 프로젝트 C. 충돌 해결 평가

두 학습자가 README의 같은 제목 줄을 서로 다르게 수정한다. 한 명의 PR을 먼저 병합한 뒤 다른 학습자는 다음 순서로 해결한다.

```bash
git fetch origin
git rebase origin/main
# 파일의 충돌 표시를 편집한다.
git add README.md
git rebase --continue
git push --force-with-lease
```

`--force-with-lease`는 원격 브랜치가 예상과 다르게 바뀌었으면 강제 push를 거부한다. 팀의 기능 브랜치에서만 합의 후 사용하며 `main`에는 사용하지 않는다.

## 최종 제출물

1. GitHub 저장소 주소
2. 커밋 그래프 캡처
3. Issue와 Pull Request 주소
4. 충돌 원인과 해결 방법 5줄
5. 본인이 사용한 명령어 10개와 의미

## 최종 점검 명령

```bash
git status
git remote -v
git branch -vv
git log --oneline --graph --decorate --all -15
git tag
gh pr list --state all
gh issue list --state all
```

## 참고

- [GitHub 기본 Git 작업](https://docs.github.com/en/get-started/git-basics)
- [GitHub Issues](https://docs.github.com/en/issues/tracking-your-work-with-issues/about-issues)
- [Pull Request](https://docs.github.com/en/pull-requests)

