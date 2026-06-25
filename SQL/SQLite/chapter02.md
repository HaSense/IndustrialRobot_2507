# Chapter 2. SQLite 소개

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

* SQLite가 무엇인지 설명할 수 있다.
* SQLite의 특징을 이해할 수 있다.
* 서버형 DB와 서버리스 DB의 차이를 설명할 수 있다.
* SQLite와 MySQL의 차이점을 비교할 수 있다.
* SQLite를 설치하고 버전을 확인할 수 있다.

---

# 1. SQLite란?

SQLite는 가볍고 빠른 관계형 데이터베이스(RDBMS)이다.

일반적인 데이터베이스와 달리 별도의 서버 프로그램이 필요하지 않다.

---

## SQLite의 의미

SQLite는 다음과 같은 특징을 가진다.

* 무료(Open Source)
* 설치가 간단함
* 파일 하나로 데이터 관리
* SQL 지원
* C/C++, Python, Java 등 다양한 언어 지원

---

## SQLite 파일 예시

```text
factory.db
```

위 파일 하나가 데이터베이스이다.

---

## SQLite 사용 사례

* 모바일 앱(Android)
* IoT 장비
* 임베디드 시스템
* 소규모 웹 서비스
* 스마트팩토리 설비 데이터 저장
* 제조 데이터 수집 프로그램

---

# 2. SQLite 특징

---

## 특징 1. 서버가 필요 없다

MySQL이나 Oracle은 별도의 서버 프로그램이 필요하다.

SQLite는 데이터베이스 파일만 있으면 된다.

```text
factory.db
```

---

## 특징 2. 설치가 매우 간단하다

SQLite는 실행 파일 하나로 동작한다.

```bash
sqlite3
```

만 있으면 사용할 수 있다.

---

## 특징 3. 매우 가볍다

SQLite 라이브러리 크기

```text
약 수 MB 이하
```

임베디드 장비에도 탑재 가능하다.

---

## 특징 4. SQL 지원

다음과 같은 SQL을 사용할 수 있다.

```sql
CREATE TABLE products(
    id INTEGER,
    name TEXT
);
```

```sql
INSERT INTO products VALUES(1,'Motor');
```

```sql
SELECT * FROM products;
```

---

## 특징 5. 하나의 파일로 관리

```text
factory.db
```

파일을 복사하면 백업이 된다.

```bash
cp factory.db backup.db
```

---

## 특징 6. 다양한 언어 지원

SQLite는 대부분의 프로그래밍 언어에서 사용할 수 있다.

* C
* C++
* Python
* Java
* C#
* Go
* Rust

---

# 3. 서버가 없는 DB

SQLite를 이해하기 위해서는 서버형 DB와 비교해야 한다.

---

## 서버형 데이터베이스 구조

```text
프로그램
    ↓
MySQL Server
    ↓
Database
```

예시

* MySQL
* MariaDB
* PostgreSQL
* Oracle
* SQL Server

---

## SQLite 구조

```text
프로그램
    ↓
SQLite Library
    ↓
factory.db
```

중간 서버가 없다.

---

## 서버형 DB의 장점

* 수백~수천 명 동시 접속
* 대규모 서비스 가능
* 강력한 권한 관리

---

## 서버형 DB의 단점

* 설치 복잡
* 관리 필요
* 서버 자원 사용

---

## SQLite의 장점

* 설치 쉬움
* 관리 쉬움
* 빠른 개발 가능
* 파일 하나로 배포 가능

---

## SQLite의 단점

* 대규모 동시 접속에 부적합
* 분산 시스템 구축 어려움
* 초대형 서비스에는 한계

---

# 4. SQLite와 MySQL 차이

---

## 구조 비교

### SQLite

```text
프로그램
    ↓
factory.db
```

### MySQL

```text
프로그램
    ↓
MySQL Server
    ↓
Database
```

---

## 기능 비교

| 항목     | SQLite    | MySQL  |
| ------ | --------- | ------ |
| 서버 설치  | 불필요       | 필요     |
| 데이터 저장 | 파일        | 서버     |
| 동시 사용자 | 적음        | 많음     |
| 설치 난이도 | 매우 쉬움     | 보통     |
| 성능     | 소규모 우수    | 대규모 우수 |
| 관리 기능  | 단순        | 강력     |
| 사용 분야  | 임베디드, 모바일 | 웹 서비스  |

---

## 제조AI 관점 비교

### SQLite 적합

* 생산량 기록
* 설비 로그 저장
* 센서 데이터 저장
* PoC 개발
* 교육용 프로젝트

---

### MySQL 적합

* MES
* ERP
* 웹 기반 생산관리
* 다수 사용자 환경

---

# SQLite를 배우는 이유

SQLite는 SQL 학습에 가장 적합하다.

---

## SQL 학습

```sql
CREATE TABLE products(
    id INTEGER,
    name TEXT
);
```

---

## 데이터 입력

```sql
INSERT INTO products
VALUES(1,'Motor');
```

---

## 데이터 조회

```sql
SELECT * FROM products;
```

---

## 데이터 수정

```sql
UPDATE products
SET name='Sensor'
WHERE id=1;
```

---

## 데이터 삭제

```sql
DELETE FROM products
WHERE id=1;
```

---

# 실습 1. SQLite 설치

---

## Ubuntu 설치

```bash
sudo apt update
sudo apt install sqlite3
```

---

## 설치 확인

```bash
which sqlite3
```

예시

```text
/usr/bin/sqlite3
```

---

## Windows 설치

1. SQLite 다운로드
2. 압축 해제
3. sqlite3.exe 실행

---

# 실습 2. 버전 확인

---

## 버전 확인 명령

```bash
sqlite3 --version
```

---

## 실행 예시

```bash
$ sqlite3 --version
3.46.0
```

버전 번호가 출력되면 설치가 완료된 것이다.

---

# 실습 3. SQLite 실행

---

## SQLite 시작

```bash
sqlite3
```

결과

```text
SQLite version 3.x.x
Enter ".help" for usage hints.
sqlite>
```

---

## 종료

```sql
.quit
```

또는

```sql
.exit
```

---

# 실습 4. 도움말 확인

```sql
.help
```

SQLite에서 사용할 수 있는 명령 목록이 출력된다.

---

# 핵심 정리

* SQLite는 가벼운 관계형 데이터베이스이다.
* 서버 설치가 필요 없는 Serverless DB이다.
* 데이터베이스 전체가 하나의 파일로 저장된다.
* SQL을 사용할 수 있다.
* SQLite는 교육, IoT, 임베디드, 스마트팩토리 PoC에 적합하다.
* MySQL은 다수 사용자가 사용하는 대규모 서비스에 적합하다.
* SQLite는 SQL을 배우기 위한 최고의 입문용 DB이다.

---

# 다음 시간

Chapter 3. SQLite 시작하기

* 데이터베이스 생성
* SQLite CLI 사용
* 테이블 생성
* 첫 번째 SQL 실행
