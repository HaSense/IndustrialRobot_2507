# Part 05. 브랜치와 병합

> 목표: 기능별 브랜치를 만들고 병합하며 충돌을 직접 해결한다.

## 1. 브랜치 만들기

```bash
cd ~/workspace/git-practice
git pull --ff-only
git switch -c feature/greeting
git branch
```

`switch -c`는 브랜치를 만들면서 이동한다. 기존 방식인 `checkout -b`도 가능하지만 이 교재는 역할이 명확한 `switch`를 사용한다.

```bash
printf 'name = input("이름: ")\nprint(f"반갑습니다, {name}님")\n' > greeting.py
git add greeting.py
git commit -m "인사 기능 추가"
git log --oneline --graph --decorate --all
```

## 2. main에 병합

```bash
git switch main
git merge feature/greeting
git log --oneline --graph --decorate --all
git branch -d feature/greeting
git push
```

`-d`는 병합된 브랜치만 안전하게 삭제한다. 커밋은 `main`에 남는다.

## 3. 병렬 작업 모습 확인

```bash
git switch -c feature/menu
printf '\n## 메뉴\n- greeting.py\n' >> README.md
git add README.md
git commit -m "실행 메뉴 문서 추가"
git switch main
printf '\n문의: teacher@example.com\n' >> README.md
git add README.md
git commit -m "문의 정보 추가"
git log --oneline --graph --decorate --all
```

서로 다른 줄이면 Git이 자동 병합할 가능성이 높다.

```bash
git merge feature/menu
git branch -d feature/menu
```

## 4. 의도적으로 충돌 만들기

```bash
printf 'title=Git Practice\n' > config.txt
git add config.txt
git commit -m "기본 제목 설정"
git switch -c feature/korean-title
printf 'title=Git 실습 프로젝트\n' > config.txt
git add config.txt
git commit -m "한글 제목 적용"
git switch main
printf 'title=Git Training\n' > config.txt
git add config.txt
git commit -m "영문 제목 적용"
git merge feature/korean-title
```

`git status`와 파일을 확인한다.

```text
<<<<<<< HEAD
title=Git Training
=======
title=Git 실습 프로젝트
>>>>>>> feature/korean-title
```

이 표시는 오류 메시지가 아니라 선택해야 할 두 변경이다. 원하는 최종 내용만 남기고 표시선을 모두 지운다.

```text
title=Git Practice | Git 실습 프로젝트
```

```bash
git add config.txt
git commit -m "제목 충돌 해결"
git branch -d feature/korean-title
```

## 5. 병합 취소

충돌 해결을 시작하지 않고 병합 전으로 돌아가려면 다음을 사용한다.

```bash
git merge --abort
```

단, 병합 전부터 커밋하지 않은 변경이 섞여 있으면 복원이 어려울 수 있으므로 병합 전에 `git status`를 확인한다.

## 6. 원격 기능 브랜치

```bash
git switch -c feature/about
printf '# 소개\n' > ABOUT.md
git add ABOUT.md
git commit -m "소개 문서 추가"
git push -u origin feature/about
```

GitHub에서 브랜치를 확인한 뒤 Part 06에서 Pull Request를 만든다.

## 실습 과제

1. `feature/contact` 브랜치에서 `CONTACT.md`를 만든다.
2. 커밋하고 원격 브랜치에 push한다.
3. `main`과 기능 브랜치에서 같은 줄을 다르게 수정해 충돌을 만든다.
4. 두 내용을 보존하는 방식으로 해결한다.

## 참고

- [git switch](https://git-scm.com/docs/git-switch)
- [git merge](https://git-scm.com/docs/git-merge)
- [GitHub 병합 충돌 해결](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/addressing-merge-conflicts)

