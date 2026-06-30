# Chapter. SQLite C API

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

- SQLite 라이브러리의 역할을 설명할 수 있다.
- SQLite 데이터베이스를 열고 닫을 수 있다.
- SQL을 실행하는 함수를 사용할 수 있다.
- 조회 결과를 출력하는 다양한 방법을 이해할 수 있다.
- Prepared Statement를 이용한 안전한 SQL 실행 방법을 이해할 수 있다.
- SQLite API를 이용하여 CRUD 프로그램을 작성할 수 있다.

---

# SQLite API란?

SQLite API(Application Programming Interface)는 C 프로그램에서 SQLite 데이터베이스를 제어하기 위한 함수들의 집합이다.

C 프로그램은 SQLite API를 호출하여

- 데이터베이스 열기
- 테이블 생성
- 데이터 입력
- 데이터 조회
- 데이터 수정
- 데이터 삭제

등의 작업을 수행한다.

구조는 다음과 같다.

```
C 프로그램
      │
      ▼
 SQLite API
      │
      ▼
 factory.db
```

---

# SQLite 프로그램의 기본 흐름

```
sqlite3_open()

        ↓

SQL 실행

        ↓

결과 출력

        ↓

sqlite3_close()
```

실제 프로그램은 대부분 위 순서를 따른다.

---

# sqlite3 자료형

SQLite 데이터베이스를 가리키는 구조체이다.

```c
sqlite3 *db;
```

- 데이터베이스 연결 객체
- 대부분의 함수가 이 포인터를 사용한다.

---

# sqlite3_open()

데이터베이스를 연다.

```c
sqlite3_open(
    "factory.db",
    &db
);
```

성공하면

```
factory.db
```

파일을 열고

```
db
```

에 연결 정보를 저장한다.

성공 시

```
SQLITE_OK
```

를 반환한다.

예제

```c
sqlite3 *db;

if(sqlite3_open("factory.db", &db)==SQLITE_OK)
{
    printf("DB Open Success\n");
}
```

---

# sqlite3_close()

데이터베이스 연결을 종료한다.

```c
sqlite3_close(db);
```

프로그램 종료 전 반드시 호출하는 것이 좋다.

---

# sqlite3_exec()

가장 간단하게 SQL을 실행하는 함수이다.

형식

```c
sqlite3_exec(
    db,
    sql,
    callback,
    data,
    &errMsg
);
```

매개변수

|매개변수|설명|
|--------|----|
|db|데이터베이스|
|sql|실행할 SQL|
|callback|조회 결과 처리 함수|
|data|사용자 데이터|
|errMsg|오류 메시지|

---

# CREATE TABLE 실행

```c
char *errMsg=NULL;

sqlite3_exec(
    db,
    "CREATE TABLE person("
    "id INTEGER PRIMARY KEY,"
    "name TEXT"
    ");",
    NULL,
    NULL,
    &errMsg
);
```

조회가 아니므로

```
callback=NULL
```

이다.

---

# INSERT 실행

```c
sqlite3_exec(
    db,
    "INSERT INTO person VALUES(1,'Hong');",
    NULL,
    NULL,
    NULL
);
```

---

# UPDATE 실행

```c
sqlite3_exec(
    db,
    "UPDATE person SET name='Kim' WHERE id=1;",
    NULL,
    NULL,
    NULL
);
```

---

# DELETE 실행

```c
sqlite3_exec(
    db,
    "DELETE FROM person WHERE id=1;",
    NULL,
    NULL,
    NULL
);
```

---

# Callback 함수

SELECT 문은 여러 개의 행이 반환된다.

SQLite는 한 행씩 Callback 함수를 호출한다.

형식

```c
int callback(
    void *data,
    int argc,
    char **argv,
    char **columnName
)
```

매개변수

|매개변수|설명|
|--------|----|
|argc|컬럼 개수|
|argv|컬럼 값|
|columnName|컬럼 이름|

---

# Callback 예제

```c
int callback(
    void *data,
    int argc,
    char **argv,
    char **columnName)
{
    for(int i=0;i<argc;i++)
    {
        printf("%s : %s\n",
               columnName[i],
               argv[i]);
    }

    printf("\n");

    return 0;
}
```

---

# SELECT 실행

```c
sqlite3_exec(
    db,
    "SELECT * FROM person;",
    callback,
    NULL,
    NULL
);
```

출력

```
id : 1
name : Hong

id : 2
name : Lee
```

---

# sqlite3_errmsg()

최근 오류 메시지를 반환한다.

```c
printf("%s\n",
       sqlite3_errmsg(db));
```

예

```
no such table
```

---

# sqlite3_prepare_v2()

Prepared Statement를 생성한다.

형식

```c
sqlite3_prepare_v2(
    db,
    sql,
    -1,
    &stmt,
    NULL
);
```

stmt는 SQL 실행 객체이다.

---

# sqlite3_step()

Prepared Statement를 실행한다.

```c
sqlite3_step(stmt);
```

반환값

|반환값|의미|
|-------|----|
|SQLITE_ROW|다음 행 존재|
|SQLITE_DONE|종료|

---

# sqlite3_column_int()

정수 값을 가져온다.

```c
int id =
sqlite3_column_int(stmt,0);
```

0은 첫 번째 컬럼이다.

---

# sqlite3_column_text()

문자열을 가져온다.

```c
const unsigned char *name;

name =
sqlite3_column_text(stmt,1);
```

---

# sqlite3_finalize()

Prepared Statement를 종료한다.

```c
sqlite3_finalize(stmt);
```

메모리를 반환한다.

---

# sqlite3_bind_int()

SQL의 ? 자리에 정수를 넣는다.

```c
sqlite3_bind_int(
    stmt,
    1,
    100
);
```

---

# sqlite3_bind_text()

문자열을 바인딩한다.

```c
sqlite3_bind_text(
    stmt,
    2,
    "Motor",
    -1,
    SQLITE_STATIC
);
```

---

# Prepared Statement 실행 순서

```
prepare

   ↓

bind

   ↓

step

   ↓

step

   ↓

...

   ↓

finalize
```

---

# Exec 방식과 Prepare 방식 비교

|항목|sqlite3_exec|Prepare|
|----|------------|--------|
|사용 난이도|쉬움|약간 어려움|
|속도|보통|빠름|
|반복 실행|비효율|효율적|
|SQL Injection|취약|안전|
|실무 사용|간단한 작업|대부분 사용|

---

# CRUD에서 사용하는 함수

|작업|주요 함수|
|----|---------|
|DB 열기|sqlite3_open()|
|생성(Create)|sqlite3_exec()|
|조회(Read)|sqlite3_prepare_v2(), sqlite3_step()|
|수정(Update)|sqlite3_exec() 또는 Prepare|
|삭제(Delete)|sqlite3_exec() 또는 Prepare|
|DB 닫기|sqlite3_close()|

---

# 자주 사용하는 SQLite API

|함수|설명|
|----|----|
|sqlite3_open()|DB 열기|
|sqlite3_close()|DB 닫기|
|sqlite3_exec()|SQL 실행|
|sqlite3_prepare_v2()|SQL 준비|
|sqlite3_step()|SQL 실행|
|sqlite3_finalize()|종료|
|sqlite3_column_int()|정수 읽기|
|sqlite3_column_text()|문자열 읽기|
|sqlite3_bind_int()|정수 바인딩|
|sqlite3_bind_text()|문자열 바인딩|
|sqlite3_errmsg()|오류 메시지|

---

# 실습 1

person.db를 연다.

```
person.db
```

---

# 실습 2

person 테이블을 생성한다.

```
id
name
phone
```

---

# 실습 3

3명의 데이터를 입력한다.

```
Hong
Lee
Kim
```

---

# 실습 4

Callback 함수를 이용하여

```
SELECT *
```

를 출력한다.

---

# 실습 5

Prepared Statement를 이용하여

```
SELECT
```

를 작성한다.

---

# 실습 6

사용자로부터 이름을 입력받아

```
WHERE name=?
```

형태로 조회한다.

---

# 실습 7

Prepared Statement를 이용하여

```
INSERT
```

를 구현한다.

---

# 실습 8

이름을 입력받아

```
UPDATE
```

를 수행한다.

---

# 실습 9

이름을 입력받아

```
DELETE
```

를 수행한다.

---

# 핵심 정리

- sqlite3_open() : 데이터베이스 연결
- sqlite3_close() : 데이터베이스 종료
- sqlite3_exec() : 간단한 SQL 실행
- Callback : SELECT 결과 처리
- sqlite3_prepare_v2() : SQL 준비
- sqlite3_step() : SQL 실행
- sqlite3_column_*() : 결과 읽기
- sqlite3_bind_*() : 값 전달
- sqlite3_finalize() : 메모리 해제
- 실무에서는 `sqlite3_prepare_v2()` + `sqlite3_bind_*()` + `sqlite3_step()` 조합을 가장 많이 사용한다.
