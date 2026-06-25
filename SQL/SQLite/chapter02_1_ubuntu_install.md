# Chapter 1. Ubuntu 24.04에서 SQLite 설치 및 개발 환경 구성

## 학습 목표

본 실습을 완료하면 다음을 수행할 수 있다.

* SQLite3 설치
* SQLite 개발 라이브러리 설치
* SQLite 버전 확인
* SQLite 데이터베이스 생성
* 테이블 생성 및 데이터 조회
* C 언어에서 SQLite 라이브러리 사용
* SQLite 연동 프로그램 컴파일

---

# 1. SQLite란?

SQLite는 서버 설치 없이 파일 하나로 동작하는 경량 데이터베이스이다.

특징

* 설치가 간단함
* 별도의 DB 서버가 필요 없음
* 단일 파일로 데이터 저장
* 임베디드 시스템에서 많이 사용
* 모바일 앱(Android, iOS)에서 사용
* 스마트팩토리 장비 및 IoT 장비에서 사용

예)

```text
products.db
```

하나의 파일 안에 모든 데이터가 저장된다.

---

# 2. SQLite 설치

패키지 목록 갱신

```bash
sudo apt update
```

SQLite 설치

```bash
sudo apt install sqlite3
```

설치 확인

```bash
sqlite3 --version
```

실행 결과 예

```text
3.45.1 2024-01-30 ...
```

---

# 실습 1

SQLite 버전을 확인하시오.

```bash
sqlite3 --version
```

---

# 3. SQLite 개발 라이브러리 설치

C 언어에서 SQLite API를 사용하려면 개발 라이브러리가 필요하다.

설치

```bash
sudo apt install libsqlite3-dev
```

설치 확인

```bash
dpkg -L libsqlite3-dev
```

헤더 파일 확인

```bash
find /usr/include -name sqlite3.h
```

결과 예

```text
/usr/include/sqlite3.h
```

---

# 실습 2

SQLite 헤더 파일의 위치를 확인하시오.

---

# 4. SQLite 실행

데이터베이스 생성

```bash
sqlite3 products.db
```

실행 후

```text
SQLite version 3.xx
sqlite>
```

프롬프트가 나타난다.

---

# 실습 3

products.db 데이터베이스를 생성하시오.

---

# 5. 테이블 생성

제품 정보를 저장하는 테이블 생성

```sql
CREATE TABLE products(
    id INTEGER PRIMARY KEY,
    name TEXT,
    quantity INTEGER,
    price INTEGER
);
```

테이블 확인

```sql
.tables
```

결과

```text
products
```

---

# 실습 4

products 테이블을 생성하시오.

---

# 6. 데이터 입력

데이터 추가

```sql
INSERT INTO products
VALUES(1,'Motor',100,5000);

INSERT INTO products
VALUES(2,'Sensor',50,12000);

INSERT INTO products
VALUES(3,'PLC',10,300000);
```

---

# 실습 5

3개의 제품 정보를 입력하시오.

---

# 7. 데이터 조회

전체 조회

```sql
SELECT * FROM products;
```

결과 예

```text
1|Motor|100|5000
2|Sensor|50|12000
3|PLC|10|300000
```

가독성 향상

```sql
.headers on
.mode column
```

다시 조회

```sql
SELECT * FROM products;
```

결과

```text
id  name    quantity  price
--  ------  --------  ------
1   Motor   100       5000
2   Sensor  50        12000
3   PLC     10        300000
```

---

# 실습 6

컬럼 형식으로 출력하시오.

---

# 8. SQLite 종료

```sql
.quit
```

또는

```sql
.exit
```

---

# 9. C 언어와 SQLite 연동

예제 코드

```c
#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;

    if(sqlite3_open("products.db", &db) == SQLITE_OK)
    {
        printf("DB 연결 성공\n");
        sqlite3_close(db);
    }
    else
    {
        printf("DB 연결 실패\n");
    }

    return 0;
}
```

---

# 10. 컴파일

파일명

```text
main.c
```

컴파일

```bash
gcc main.c -o main -lsqlite3
```

실행

```bash
./main
```

결과

```text
DB 연결 성공
```

---

# 실습 7

다음을 수행하시오.

1. main.c 작성
2. gcc로 컴파일
3. 프로그램 실행
4. DB 연결 성공 메시지 확인

---

# 11. SQLite 버전 출력

예제 코드

```c
#include <stdio.h>
#include <sqlite3.h>

int main()
{
    printf("SQLite Version : %s\n",
           sqlite3_libversion());

    return 0;
}
```

컴파일

```bash
gcc version.c -o version -lsqlite3
```

실행

```bash
./version
```

결과

```text
SQLite Version : 3.45.1
```

---

# 실습 8

SQLite 라이브러리 버전을 출력하시오.

---

# 확인 문제

### 문제 1

SQLite 데이터는 어디에 저장되는가?

① 메모리
② 데이터베이스 서버
③ 파일
④ 웹브라우저

---

### 문제 2

SQLite 개발용 헤더 파일을 제공하는 패키지는?

① sqlite-dev
② sqlite3
③ libsqlite3-dev
④ gcc

---

### 문제 3

SQLite 종료 명령은?

① quit
② stop
③ .quit
④ end

---

### 문제 4

C 프로그램 컴파일 시 SQLite 라이브러리를 링크하는 옵션은?

```bash
gcc main.c -o main ?
```

---

# 정리

이번 실습에서는 다음을 수행하였다.

* SQLite 설치
* SQLite 개발 라이브러리 설치
* 데이터베이스 생성
* 테이블 생성
* 데이터 입력
* 데이터 조회
* C 언어와 SQLite 연동
* SQLite 프로그램 컴파일

다음 시간에는 SQL 문법(CREATE, INSERT, SELECT)을 이용하여 생산량 관리 데이터베이스를 만들어 본다.
