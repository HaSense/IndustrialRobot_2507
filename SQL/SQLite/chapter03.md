# Chapter 3. SQLite 실행

## 학습목표

- SQLite 데이터베이스를 생성할 수 있다.
- SQLite 데이터베이스를 열 수 있다.
- SQLite 프로그램을 종료할 수 있다.
- SQLite 명령어와 프롬프트를 이해할 수 있다.

---

# 3.1 SQLite 실행하기

SQLite는 별도의 서버 설치 없이 실행 가능한 데이터베이스입니다.

터미널에서 다음 명령을 입력하여 SQLite를 실행합니다.

```bash
sqlite3
```

실행 결과

```text
SQLite version 3.x.x
Enter ".help" for usage hints.
sqlite>
```

`sqlite>` 프롬프트가 나타나면 SQLite가 정상적으로 실행된 것입니다.

---

# 3.2 데이터베이스 생성

SQLite는 데이터베이스 파일이 존재하지 않으면 자동으로 생성합니다.

## 문법

```bash
sqlite3 데이터베이스명.db
```

## 예제

```bash
sqlite3 factory.db
```

실행 후

```text
SQLite version 3.x.x
sqlite>
```

현재 폴더에 `factory.db` 파일이 생성됩니다.

---

# 3.3 데이터베이스 열기

이미 존재하는 데이터베이스 파일도 동일한 방법으로 열 수 있습니다.

## 문법

```bash
sqlite3 데이터베이스명.db
```

## 예제

```bash
sqlite3 factory.db
```

SQLite가 실행되면서 기존 데이터베이스가 열립니다.

---

# 3.4 현재 데이터베이스 확인

현재 연결된 데이터베이스를 확인합니다.

## 명령어

```sql
.databases
```

## 실행 예

```sql
sqlite> .databases
```

결과

```text
main: /home/user/factory.db
```

---

# 3.5 도움말 보기

SQLite에서 사용할 수 있는 명령어 목록을 확인합니다.

## 명령어

```sql
.help
```

## 실행 예

```sql
sqlite> .help
```

---

# 3.6 테이블 목록 확인

현재 데이터베이스에 존재하는 테이블을 확인합니다.

## 명령어

```sql
.tables
```

## 실행 예

```sql
sqlite> .tables
```

테이블이 없으면 아무것도 출력되지 않습니다.

---

# 3.7 종료하기

SQLite를 종료합니다.

## 명령어

```sql
.quit
```

또는

```sql
.exit
```

## 실행 예

```sql
sqlite> .quit
```

---

# 실습 1. SQLite 실행

터미널에서 SQLite를 실행하시오.

```bash
sqlite3
```

---

# 실습 2. 데이터베이스 생성

`factory.db` 데이터베이스를 생성하시오.

```bash
sqlite3 factory.db
```

생성 여부 확인

```bash
ls
```

결과 예

```text
factory.db
```

---

# 실습 3. 데이터베이스 확인

SQLite에서 현재 연결된 데이터베이스를 확인하시오.

```sql
.databases
```

---

# 실습 4. 도움말 확인

SQLite 명령어 목록을 확인하시오.

```sql
.help
```

---

# 실습 5. 종료

SQLite를 종료하시오.

```sql
.quit
```

---

# 핵심 정리

| 명령어 | 설명 |
|----------|----------|
| sqlite3 | SQLite 실행 |
| sqlite3 factory.db | 데이터베이스 생성 또는 열기 |
| .databases | 현재 데이터베이스 확인 |
| .tables | 테이블 목록 확인 |
| .help | 도움말 보기 |
| .quit | SQLite 종료 |

---

# 확인 문제

### 문제 1

SQLite를 실행하는 명령어는?

### 문제 2

`factory.db` 데이터베이스를 생성하는 명령어는?

### 문제 3

현재 연결된 데이터베이스를 확인하는 명령어는?

### 문제 4

SQLite를 종료하는 명령어는?

### 문제 5

`.help` 명령어의 역할은 무엇인가?
