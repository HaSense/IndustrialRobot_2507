# 부록. Git 명령어와 수업 운영 자료

## A. 핵심 명령어 표

| 목적 | 명령 |
|---|---|
| 저장소 생성 | `git init` |
| 저장소 복제 | `git clone URL` |
| 상태 확인 | `git status`, `git status -sb` |
| 차이 확인 | `git diff`, `git diff --staged` |
| 스테이징 | `git add 파일`, `git add .` |
| 커밋 | `git commit -m "메시지"` |
| 이력 | `git log --oneline --graph --decorate --all` |
| 브랜치 생성·이동 | `git switch -c 이름` |
| 브랜치 이동 | `git switch 이름` |
| 병합 | `git merge 브랜치` |
| 원격 확인 | `git remote -v` |
| 원격 추가 | `git remote add origin URL` |
| 원격 정보 갱신 | `git fetch origin` |
| 가져와 통합 | `git pull --ff-only` |
| 원격 전송 | `git push`, `git push -u origin 브랜치` |
| 작업 복구 | `git restore 파일` |
| 스테이징 취소 | `git restore --staged 파일` |
| 커밋 취소 기록 | `git revert 커밋` |
| 임시 보관 | `git stash`, `git stash pop` |

## B. 용어

| 용어 | 의미 |
|---|---|
| Working tree | 실제로 편집하는 파일 영역 |
| Index/Staging area | 다음 커밋에 넣을 변경사항 |
| Repository | 커밋과 객체가 보관되는 저장소 |
| HEAD | 현재 체크아웃한 커밋·브랜치 위치 |
| Branch | 독립적인 작업 이력의 이름 |
| Remote | 다른 위치의 Git 저장소 연결 이름 |
| origin | 복제하거나 처음 연결한 원격의 관례적 이름 |
| upstream | Fork가 참조하는 원본 저장소의 관례적 이름 |
| Pull Request | 변경사항의 검토와 병합을 제안하는 GitHub 기능 |
| Fork | 다른 계정의 저장소와 연결된 개인 사본 |

## C. 커밋 메시지 예시

```text
feat: 로그인 기능 추가
fix: 빈 이름 등록 오류 수정
docs: 설치 방법 보완
test: 회원가입 테스트 추가
refactor: 중복 검증 로직 분리
chore: 의존성 버전 갱신
```

첫 줄은 무엇을 했는지 명령형으로 간결하게 쓴다. 수업에서는 형식보다 **변경 목적을 설명하는 습관**이 우선이다.

## D. 수업 전 점검표

- [ ] WSL2와 Ubuntu가 실행된다.
- [ ] `git --version`, `gh --version`, `code --version`이 출력된다.
- [ ] GitHub 계정 이메일 인증을 완료했다.
- [ ] 브라우저에서 GitHub에 로그인할 수 있다.
- [ ] `git config --global user.name`과 이메일을 설정했다.
- [ ] 공용 PC라면 수업 후 로그아웃 방법을 안내했다.

## E. 실습 오류 제출 양식

```text
1. 실행한 명령:
2. 전체 오류 메시지:
3. pwd 결과:
4. git status 결과:
5. git branch -vv 결과:
6. git remote -v 결과(토큰은 제거):
7. 기대한 결과:
```

토큰, 비밀번호, 개인키, `.env` 내용은 제출하지 않는다.

## F. 저장소 상태 수집 명령

다음 결과만으로 많은 문제를 진단할 수 있다.

```bash
pwd
git status -sb
git branch -vv
git remote -v
git log --oneline --graph --decorate --all -10
```

## G. 전역 설정 정리

```bash
git config --global --list
git config --global --unset user.name
git config --global --unset user.email
gh auth logout
```

저장소 하나만 초기화하려면 해당 복사본의 `.git`을 제거한다. 실제 프로젝트에서는 이력이 사라지므로 백업과 대상 경로를 먼저 확인한다.

## H. 권장 수업 시간

| 차시 | 내용 | 실습 시간 |
|---|---|---:|
| 1 | Part 01 환경과 개념 | 2시간 |
| 2 | Part 02 로컬 저장소 | 3시간 |
| 3 | Part 03 인증과 첫 push | 3시간 |
| 4 | Part 04 clone과 동기화 | 2시간 |
| 5 | Part 05 브랜치와 충돌 | 3시간 |
| 6 | Part 06 Issue와 PR | 3시간 |
| 7 | Part 07 오류와 보안 | 2시간 |
| 8 | Part 08 종합 프로젝트 | 4~6시간 |

## I. 공식 참고자료

- [Git Reference](https://git-scm.com/docs)
- [Pro Git 한국어판](https://git-scm.com/book/ko/v2)
- [GitHub Docs](https://docs.github.com/)
- [GitHub CLI Manual](https://cli.github.com/manual/)

