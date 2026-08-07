# 4장 psql 사용법

`psql`은 PostgreSQL 서버에 SQL을 보내고 결과를 확인하는 공식 명령줄 클라이언트입니다. 3장에서는 설치 확인을 위해 `psql`에 한 번 접속했습니다. 이 장에서는 접속 정보를 명시하는 방법부터 SQL과 메타 명령의 차이, 객체 확인, 출력 조절, 파일 실행, 명령 기록과 편집까지 일상적으로 반복해서 사용할 기능을 익힙니다.

> **버전 기준**  
> 이 장은 PostgreSQL 18의 `psql`을 기준으로 2026년 8월 4일 공식 문서와 대조했습니다. 클라이언트와 서버의 메이저 버전을 가능하면 같게 유지합니다. 이 장의 출력에서 버전 문자열, 소유자, 로캘과 실행 시간은 환경에 따라 달라질 수 있습니다.

## 이 장에서 배울 내용

- `psql`의 접속 옵션과 생략했을 때 적용되는 기본값을 설명한다.
- SQL 명령과 `psql` 메타 명령을 구분하고 입력 버퍼를 다룬다.
- 데이터베이스, 스키마, 테이블과 테이블 정의를 확인한다.
- 표, 확장 출력, 페이저와 실행 시간 표시를 조절한다.
- SQL 파일을 대화형·비대화형 방식으로 안전하게 실행한다.
- 명령 기록, 자동 완성, 외부 편집기, 도움말과 종료 기능을 사용한다.

## 선행 지식

- 1장에서 서버, 데이터베이스, 스키마, 테이블과 역할의 관계를 익혔습니다.
- 2장에서 Ubuntu Bash의 경로와 파일 사용법을 익혔습니다.
- 3장에서 PostgreSQL 18을 설치하고 `18/main` 클러스터를 실행했습니다.
- 저장소 루트에 `examples/04_psql_basics.sql`이 있습니다.

먼저 서버 상태와 클라이언트 버전을 확인합니다.

**Ubuntu Bash**

```bash
pg_isready
psql --version
```

다음과 같은 핵심 결과가 예상됩니다. 마이너 버전은 다를 수 있습니다.

```text
/var/run/postgresql:5432 - accepting connections
psql (PostgreSQL) 18.x (...)
```

서버가 준비되지 않았다면 3장의 서비스 상태와 로그 확인 절차를 먼저 수행합니다.

## 1. psql은 클라이언트다

`psql`은 터미널 기반 프런트엔드(front-end), 즉 클라이언트 프로그램입니다. 입력한 SQL을 서버에 전달하고 서버가 돌려준 결과를 화면에 표시합니다. `psql` 자체가 테이블 데이터를 저장하거나 SQL을 실행하는 서버는 아닙니다.

```text
Ubuntu Bash
  └─ psql 클라이언트
      └─ Unix 도메인 소켓 또는 TCP 연결
          └─ PostgreSQL 서버
              └─ 선택한 데이터베이스
```

이 구분은 오류를 해석할 때 중요합니다.

- `psql: command not found`: 클라이언트 설치 또는 실행 경로 문제
- `connection ... failed`: 클라이언트는 실행됐지만 서버 연결 문제
- `ERROR: relation ... does not exist`: 서버 연결은 됐지만 SQL 객체 문제

## 2. psql 접속

### 2.1 기본 관리자 접속

3장과 마찬가지로 Ubuntu 운영체제 사용자 `postgres`를 통해 PostgreSQL 역할 `postgres`로 접속합니다.

**Ubuntu Bash**

```bash
sudo -u postgres psql -X -d postgres
```

옵션의 뜻은 다음과 같습니다.

| 옵션 | 긴 이름 | 의미 |
|---|---|---|
| `-X` | `--no-psqlrc` | 개인·시스템 시작 설정을 읽지 않음 |
| `-d postgres` | `--dbname=postgres` | `postgres` 데이터베이스에 접속 |

`-X`는 개인 출력 설정의 영향을 배제해 책의 결과를 재현하기 쉽게 합니다. 평소 대화형 작업에서는 생략할 수 있지만 검증 명령과 자동 실행에서는 사용하는 편이 안전합니다.

접속하면 다음과 비슷한 화면이 나옵니다.

```text
psql (18.x (...))
Type "help" for help.

postgres=#
```

`postgres=#`는 입력하라는 표시인 **프롬프트(prompt)**입니다. 복사할 명령에는 포함하지 않습니다. 기본 프롬프트에서 앞부분은 현재 데이터베이스 이름이고, 끝의 `#`는 현재 세션 사용자가 슈퍼사용자임을 뜻합니다. 일반 역할이면 보통 `>`가 표시됩니다.

### 2.2 접속 정보 확인

`psql` 안에서 다음 메타 명령을 실행합니다.

```text
\conninfo
```

로컬 기본 접속에서는 데이터베이스, 역할, 포트와 Unix 도메인 소켓 디렉터리 정보가 표시됩니다. 이어서 SQL로도 확인합니다.

```sql
SELECT current_database(), current_user;
```

예상 핵심값은 모두 `postgres`입니다.

```text
 current_database | current_user
------------------+--------------
 postgres         | postgres
(1 row)
```

### 2.3 접속 옵션 전체 모양

일반적인 접속 형식은 다음과 같습니다.

```bash
psql -h host_name -p port_number -U role_name -d database_name
```

| 옵션 | 의미 | 이 책의 로컬 기본값 예 |
|---|---|---|
| `-h` | 서버 호스트 또는 소켓 디렉터리 | 생략하여 로컬 소켓 사용 |
| `-p` | 서버 포트 | `5432` |
| `-U` | PostgreSQL 역할 | 운영체제 사용자 이름을 기본값으로 사용 |
| `-d` | 데이터베이스 | 역할 이름을 기본값으로 사용 |

호스트를 생략한 Linux 접속은 기본적으로 로컬 Unix 도메인 소켓을 사용합니다. `-h localhost`를 지정하면 TCP 접속이 됩니다. 같은 컴퓨터라도 접속 방식이 달라지므로 `pg_hba.conf`의 다른 인증 규칙이 적용될 수 있습니다.

다음 TCP 명령은 설치 직후 비밀번호가 설정되지 않은 `postgres` 역할에 실패할 수 있습니다. 실패가 예상되는 비교용 명령이며, 인증을 약하게 바꾸지 않습니다.

```bash
psql -h localhost -p 5432 -U postgres -d postgres
```

역할과 비밀번호는 5장에서 만들고, 인증 규칙은 22장에서 자세히 다룹니다.

### 2.4 값을 생략할 때 주의할 점

현재 Ubuntu 사용자가 `student`라고 가정하고 아무 옵션 없이 실행하면 `psql`은 보통 PostgreSQL 역할 `student`와 데이터베이스 `student`를 찾습니다.

```bash
psql
```

해당 역할이나 데이터베이스를 아직 만들지 않았으므로 다음과 같은 오류가 날 수 있습니다.

```text
psql: error: connection to server ... failed: FATAL:  role "student" does not exist
```

이 오류는 서버 설치 실패가 아닙니다. 접속 기본값과 실제 역할이 맞지 않는 것입니다. 입문 단계에서는 `-d`와 필요한 역할을 명시해 대상이 분명한 명령을 사용합니다.

### 2.5 접속 중 데이터베이스 바꾸기

현재 세션에서 다른 데이터베이스로 다시 연결할 때 `\connect` 또는 줄임말 `\c`를 사용합니다.

```text
\c postgres
\conninfo
```

성공하면 새 연결 정보가 표시됩니다. PostgreSQL은 한 연결에서 하나의 데이터베이스를 사용하므로 다른 데이터베이스의 객체를 보려면 다시 연결해야 합니다.

## 3. SQL 명령과 메타 명령

`psql` 입력에는 서버가 처리하는 SQL 명령과 클라이언트가 처리하는 메타 명령(meta-command)이 함께 존재합니다.

| 구분 | 처리 주체 | 시작·종료 규칙 | 예 |
|---|---|---|---|
| SQL | PostgreSQL 서버 | 보통 세미콜론으로 종료 | `SELECT current_date;` |
| 메타 명령 | `psql` 클라이언트 | 역슬래시로 시작, 줄 끝에서 종료 | `\conninfo` |

### 3.1 SQL은 세미콜론까지 입력 버퍼에 쌓인다

다음 SQL을 여러 줄로 입력합니다.

```sql
SELECT current_database() AS database_name,
       current_user AS role_name,
       current_date AS today;
```

첫 줄에서 Enter를 눌러도 아직 서버로 보내지 않습니다. 세미콜론에 도달하면 전체 문장을 전송합니다. 입력 중 프롬프트가 `postgres-#`처럼 바뀌는 것은 문장이 아직 끝나지 않았다는 뜻입니다.

세미콜론 대신 `\g`로 현재 입력 버퍼를 실행할 수도 있습니다.

```text
SELECT 18 AS server_major
\g
```

위에서는 `SELECT` 줄 끝에 세미콜론을 붙이지 않습니다. 다음 줄의 `\g`가 전송합니다.

### 3.2 현재 입력 버퍼 확인과 취소

세미콜론 없이 다음을 입력합니다.

```text
SELECT '아직 실행하지 않음' AS message
```

현재 버퍼를 확인합니다.

```text
\p
```

실행하지 않고 버퍼를 지웁니다.

```text
\r
```

`Query buffer reset (cleared).`가 표시됩니다. 실수로 따옴표나 괄호를 닫지 않아 메타 명령도 정상 해석되지 않으면 `Ctrl+C`를 한 번 눌러 현재 입력을 취소합니다. `Ctrl+C`는 이 상황에서 서버 자체를 종료하는 명령이 아닙니다.

### 3.3 메타 명령에는 세미콜론을 붙이지 않는다

다음은 올바른 메타 명령입니다.

```text
\conninfo
\l
```

`\l;`처럼 세미콜론을 붙이면 세미콜론이 인자의 일부로 해석되어 예상과 다른 검색 결과나 오류가 생길 수 있습니다. 메타 명령은 줄바꿈으로 끝냅니다.

### 3.4 두 종류의 도움말

SQL 문법 도움말은 `\h`, `psql` 기능 도움말은 `\?`입니다.

```text
\h SELECT
\? commands
\? options
\? variables
```

화면이 길어 페이저(pager)가 열리면 방향키나 Space로 이동하고 `q`로 페이저만 닫습니다. `psql` 세션은 계속 유지됩니다.

## 4. 데이터베이스, 스키마와 테이블 확인

### 4.1 데이터베이스 목록

```text
\l
```

긴 이름인 `\list`도 같은 기능입니다. 기본 설치 직후에는 보통 `postgres`, `template0`, `template1`이 보입니다. `template0`과 `template1`은 새 데이터베이스를 만들 때 사용하는 템플릿이므로 실습 테이블을 만들지 않습니다.

### 4.2 스키마 목록

```text
\dn
```

기본 사용자 스키마인 `public`이 보입니다. 시스템 스키마까지 포함하려면 대문자 `S` 수정자를 붙입니다.

```text
\dnS
```

시스템 객체 목록은 길 수 있습니다. 지금은 `public`과 `pg_catalog`이 서로 다른 스키마라는 점만 확인합니다.

### 4.3 테이블 목록

```text
\dt
```

아직 영구 사용자 테이블을 만들지 않았다면 다음 메시지가 정상입니다.

```text
Did not find any relations.
```

`\dt`는 현재 데이터베이스에서 현재 검색 경로(search path)를 통해 보이는 사용자 테이블을 나열합니다. "서버에 테이블이 하나도 없다"는 뜻으로 단정하면 안 됩니다. 다른 데이터베이스나 스키마에 테이블이 있을 수 있습니다.

특정 스키마의 테이블은 패턴을 지정합니다.

```text
\dt public.*
```

### 4.4 임시 테이블로 구조 확인 연습

5장 이후에 사용할 영구 실습 환경을 미리 만들지 않기 위해 세션 종료 시 자동 삭제되는 임시 테이블(temporary table)을 사용합니다.

```sql
CREATE TEMP TABLE psql_task_demo (
    task_id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title text NOT NULL,
    status text NOT NULL CHECK (status IN ('todo', 'in_progress', 'done')),
    due_date date
);
```

예상 결과는 다음과 같습니다.

```text
CREATE TABLE
```

고정된 예제 데이터를 입력합니다.

```sql
INSERT INTO psql_task_demo (title, status, due_date)
VALUES
    ('요구사항 정리', 'done', DATE '2026-08-05'),
    ('화면 설계', 'in_progress', DATE '2026-08-08'),
    ('테스트 작성', 'todo', NULL);
```

예상 결과는 `INSERT 0 3`입니다. 이제 테이블 목록과 정의를 확인합니다.

```text
\dt
\d psql_task_demo
\d+ psql_task_demo
```

- `\dt`: 보이는 테이블 목록
- `\d 이름`: 열, 데이터 타입, NULL 허용 여부, 기본값, 인덱스와 제약조건
- `\d+ 이름`: 저장 방식, 크기 등 추가 정보

`\d` 계열의 `+`는 대체로 더 자세한 정보를 요청합니다. 대문자 `S`는 시스템 객체까지 포함한다는 뜻입니다.

### 4.5 SQL로 데이터 조회

```sql
SELECT task_id, title, status, due_date
FROM psql_task_demo
ORDER BY task_id;
```

예상 결과는 세 행입니다.

```text
 task_id |    title     |   status    |  due_date
---------+--------------+-------------+------------
       1 | 요구사항 정리 | done        | 2026-08-05
       2 | 화면 설계     | in_progress | 2026-08-08
       3 | 테스트 작성   | todo        |
(3 rows)
```

공백 너비는 글꼴과 로캘에 따라 달라질 수 있습니다. `due_date`의 빈 칸은 SQL의 `NULL`입니다.

## 5. 출력 형식과 실행 결과 조절

### 5.1 현재 출력 설정 확인

인자 없이 `\pset`을 실행하면 현재 출력 설정을 표시합니다.

```text
\pset
```

설정은 현재 `psql` 세션에만 적용됩니다. `-X`로 시작했다면 개인 `~/.psqlrc` 설정은 읽지 않습니다.

### 5.2 NULL을 눈에 보이게 표시

기본 표에서는 `NULL`이 빈 칸처럼 보일 수 있습니다. 학습 중에는 다음 설정이 구분에 유용합니다.

```text
\pset null '(null)'
```

다시 조회합니다.

```sql
SELECT task_id, title, due_date
FROM psql_task_demo
ORDER BY task_id;
```

세 번째 행의 마감일이 `(null)`로 보입니다. 이는 화면 표시만 바꾸며 데이터의 실제 값을 문자열 `'(null)'`로 변경하지 않습니다. 기본 빈 문자열 표시로 되돌리려면 작은따옴표 두 개를 사용합니다.

```text
\pset null ''
```

### 5.3 확장 출력

열이 많거나 값이 긴 결과는 한 행을 세로로 펼치면 읽기 쉽습니다.

```text
\x on
```

```sql
SELECT task_id, title, status, due_date
FROM psql_task_demo
ORDER BY task_id;
```

각 행이 `-[ RECORD 1 ]-` 같은 구분 아래 세로로 표시됩니다. 자동 모드는 화면 폭에 따라 표와 확장 출력을 선택합니다.

```text
\x auto
```

기본 표 형식으로 명확하게 돌아갑니다.

```text
\x off
```

현재 버퍼의 쿼리 한 번만 확장 출력하려면 세미콜론 대신 `\gx`를 사용할 수도 있습니다.

### 5.4 페이저 제어

결과가 화면보다 길면 `psql`은 `less` 같은 페이저를 사용할 수 있습니다. 짧은 실습에서 페이저를 끕니다.

```text
\pset pager off
```

다시 자동 사용하도록 돌립니다.

```text
\pset pager on
```

화면 아래에 `(END)`가 보이고 입력이 안 되는 것처럼 느껴지면 `psql`이 멈춘 것이 아니라 페이저 안에 있는 경우가 많습니다. `q`를 눌러 결과 보기만 종료합니다.

### 5.5 실행 시간 표시

```text
\timing on
```

이후 각 SQL 결과 아래에 클라이언트가 측정한 시간이 밀리초 단위로 표시됩니다.

```sql
SELECT count(*)
FROM psql_task_demo;
```

```text
 count
-------
     3
(1 row)

Time: ... ms
```

컴퓨터 상태에 따라 값이 달라지므로 책의 숫자와 비교해 성능을 판단하지 않습니다. 표시를 끕니다.

```text
\timing off
```

`\timing`은 전체 경과 시간을 간편하게 보는 기능입니다. 서버 내부 실행 계획과 실제 처리 시간은 26장에서 `EXPLAIN ANALYZE`로 분석합니다.

### 5.6 명령줄에서 CSV 출력

사람이 읽는 정렬 표 대신 다른 프로그램에 넘길 CSV가 필요할 때 `--csv`를 사용할 수 있습니다. 이 명령은 **Ubuntu Bash**에서 실행합니다.

```bash
sudo -u postgres psql -X --csv -d postgres -c "SELECT current_database() AS database_name, current_user AS role_name;"
```

예상 결과는 다음과 같습니다.

```text
database_name,role_name
postgres,postgres
```

`--csv`는 CSV 인용 규칙을 처리하므로 화면의 정렬 표를 직접 쉼표로 바꾸는 것보다 안전합니다.

## 6. SQL 파일 실행

반복할 명령은 터미널에 다시 입력하지 않고 파일로 보관합니다. 파일은 검토, 버전 관리와 재실행이 쉽고 오류가 발생한 줄도 찾기 쉽습니다.

### 6.1 예제 파일 내용 확인

먼저 `psql`을 종료합니다.

```text
\q
```

저장소 루트의 **Ubuntu Bash**에서 파일을 읽습니다.

```bash
sed -n '1,240p' examples/04_psql_basics.sql
```

예제는 다음 원칙으로 작성되어 있습니다.

- `\set ON_ERROR_STOP on`으로 첫 오류에서 중단합니다.
- 현재 연결 정보를 출력합니다.
- 임시 테이블과 고정 데이터를 사용합니다.
- SQL과 메타 명령을 함께 실행합니다.
- 세션 종료 시 임시 테이블이 자동 삭제되어 다시 실행할 수 있습니다.

### 6.2 Bash에서 파일 실행

저장소가 일반 사용자의 홈 아래에 있으면 Ubuntu 사용자 `postgres`가 디렉터리를 읽지 못할 수 있습니다. 현재 Bash 사용자가 파일을 열고 표준 입력으로 넘기면 저장소 권한을 넓힐 필요가 없습니다.

```bash
sudo -u postgres psql -X -d postgres -f - < examples/04_psql_basics.sql
```

`-f -`의 하이픈은 표준 입력에서 명령을 읽으라는 뜻입니다. 실행이 끝나면 `psql`도 종료되고 임시 테이블도 사라집니다.

자동 실행에서는 시작 파일의 영향과 조용한 실패를 피하도록 다음 요소를 습관화합니다.

```text
-X                      개인 psql 시작 설정을 읽지 않음
ON_ERROR_STOP=1         SQL·메타 명령 오류에서 즉시 중단
-f file 또는 -f -       파일 모드로 실행해 오류 위치를 확인
```

예제 파일 내부 설정에 의존하지 않고 명령줄에서 강제할 수도 있습니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/04_psql_basics.sql
```

오류가 나면 셸 종료 상태도 실패가 됩니다. 스크립트와 자동화 도구가 이를 감지할 수 있습니다.

### 6.3 한 트랜잭션으로 실행

파일의 변경이 전부 성공하거나 전부 취소되어야 하고 모든 문장이 트랜잭션 안에서 실행 가능한 경우 `-1` 또는 `--single-transaction`을 사용합니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -1 -d postgres -f - < examples/04_psql_basics.sql
```

`-1`은 첫 명령 전에 `BEGIN`, 마지막에 `COMMIT`을 보내며 오류가 나면 롤백하도록 돕습니다. 다음 경우에는 무조건 붙이지 않습니다.

- 파일이 자체적으로 `BEGIN`과 `COMMIT`을 관리하는 경우
- 트랜잭션 블록 안에서 실행할 수 없는 명령이 있는 경우
- 매우 큰 작업을 하나의 긴 트랜잭션으로 묶는 것이 부적절한 경우

트랜잭션의 원리는 18장에서 자세히 배웁니다.

### 6.4 접속한 상태에서 파일 실행

다시 접속합니다.

```bash
sudo -u postgres psql -X -d postgres
```

`psql`의 현재 작업 디렉터리를 확인합니다.

```text
\! pwd
```

저장소 루트가 아니라면 `\cd`로 이동합니다. `<ubuntu_user>`는 실제 경로로 바꿉니다.

```text
\cd /home/<ubuntu_user>/work/postgresql-book
```

단, 이 세션의 운영체제 사용자는 `postgres`이므로 해당 경로를 읽을 권한이 있어야 합니다. 권한을 넓히지 않으려면 앞 절의 Bash 리디렉션 방식을 사용합니다. 읽을 수 있는 위치라면 다음 명령으로 파일을 실행합니다.

```text
\i examples/04_psql_basics.sql
```

`\i`는 현재 `psql` 세션에서 파일을 읽기 때문에 파일 실행 뒤에도 임시 테이블이 남습니다. 같은 세션에서 `\dt`와 `SELECT`로 결과를 더 살펴볼 수 있습니다.

### 6.5 `-c`로 한 명령 실행

간단한 확인은 접속 화면에 들어가지 않고 실행할 수 있습니다.

```bash
sudo -u postgres psql -X -d postgres -c "SELECT current_database(), current_user;"
```

`-c` 한 번의 인자는 완전한 SQL 문자열 또는 메타 명령 하나여야 합니다. 한 `-c` 안에 SQL과 `\dt`를 섞지 않습니다. 필요하면 `-c`를 반복합니다.

```bash
sudo -u postgres psql -X -d postgres -c "SELECT current_database();" -c "\conninfo"
```

셸에서 SQL을 큰따옴표로 감싸면 `$`, 역따옴표와 명령 치환 같은 Bash 해석이 개입할 수 있습니다. 복잡한 SQL은 `-c`에 억지로 넣지 말고 검토 가능한 파일로 작성합니다.

## 7. 명령 기록, 자동 완성과 편집

### 7.1 이전 명령 다시 찾기

대화형 `psql`은 Readline 또는 libedit를 사용할 수 있으며 위·아래 방향키로 이전 명령을 찾습니다.

- 위 방향키 또는 `Ctrl+P`: 이전 입력
- 아래 방향키 또는 `Ctrl+N`: 다음 입력
- `Ctrl+A`: 줄의 시작
- `Ctrl+E`: 줄의 끝
- `Ctrl+R`: 기록 역방향 검색

키 동작은 터미널과 편집 라이브러리 설정에 따라 조금 다를 수 있습니다.

기본 명령 기록 파일은 일반적으로 `~/.psql_history`입니다. `sudo -u postgres`로 실행한 세션의 `~`는 학생 계정 홈이 아니라 대상 운영체제 사용자의 홈을 가리킬 수 있습니다. `HISTFILE` 변수를 명시적으로 설정했는지는 다음 명령으로 확인합니다.

```text
\echo :{?HISTFILE}
```

`FALSE`이면 기본 기록 경로를 사용하고, `TRUE`이면 `HISTFILE`에 별도 경로가 설정된 상태입니다. 기본 경로는 대상 운영체제 사용자의 홈 아래 `.psql_history`입니다.

> **보안 주의**  
> 대화형으로 입력한 SQL은 기록 파일에 남을 수 있습니다. 실제 비밀번호, 토큰과 개인정보를 SQL 문자열에 직접 쓰지 않습니다. PostgreSQL 역할 비밀번호 변경에는 평문을 명령 기록에 남기지 않는 `\password`를 사용합니다. 비밀번호 관리는 22장에서 다룹니다.

### 7.2 Tab 자동 완성

SQL 키워드나 객체 이름의 일부를 입력하고 Tab을 누르면 가능한 값을 완성하거나 후보를 보여 줍니다.

```text
sel<Tab>
```

환경에 따라 한 번 더 Tab을 눌러야 후보가 보일 수 있습니다. 자동 완성은 객체 이름을 찾기 위해 서버에 조회를 보낼 수 있으므로 트랜잭션의 매우 민감한 단계에서는 불필요한 Tab 입력을 피합니다.

### 7.3 외부 편집기로 쿼리 수정

`\e`는 현재 입력 버퍼 또는 직전 쿼리를 외부 편집기에서 엽니다.

```text
\e
```

Ubuntu에서는 설정이 없으면 보통 `vi`가 열립니다. `nano`를 쓰고 싶다면 `psql`을 시작하기 전에 환경변수를 지정합니다.

```bash
sudo -u postgres env PSQL_EDITOR=nano psql -X -d postgres
```

편집한 내용이 완전한 SQL이고 세미콜론으로 끝나면 편집기를 닫은 직후 실행될 수 있습니다. `UPDATE`, `DELETE`, `DROP` 같은 변경문을 편집할 때는 저장 전에 대상과 조건을 다시 확인합니다.

### 7.4 개인 시작 파일

`psql`은 기본적으로 시스템 `psqlrc`와 사용자의 `~/.psqlrc`를 읽어 출력 형식이나 변수를 설정할 수 있습니다. 예를 들어 학습 중 NULL 표시를 고정할 수 있습니다.

```text
\pset null '(null)'
\set HISTCONTROL ignoreboth
```

하지만 개인 설정은 책의 예상 출력이나 자동 스크립트를 바꿀 수 있습니다. 다음 원칙을 사용합니다.

- 개인 대화형 작업: 필요하면 `~/.psqlrc` 사용
- 교재 검증과 자동화: `-X`로 시작 파일 무시
- 팀 스크립트: 필요한 변수를 파일이나 명령줄에 명시

지금은 `postgres` 운영체제 계정의 시작 파일을 수정하지 않습니다.

## 8. 종료와 세션 경계

정상 종료는 다음 메타 명령을 사용합니다.

```text
\q
```

대화형 입력에서 `Ctrl+D`로 파일 끝(EOF)을 보내도 종료할 수 있지만, 초보자는 의도가 분명한 `\q`를 권장합니다.

`psql`을 종료하면 다음이 일어납니다.

- 현재 서버 연결이 닫힙니다.
- 커밋하지 않은 열린 트랜잭션은 서버에서 롤백됩니다.
- 이 장의 `psql_task_demo` 같은 임시 테이블은 삭제됩니다.
- PostgreSQL 서버 서비스 자체는 계속 실행됩니다.

즉, `\q`는 클라이언트 세션 종료이지 서버 중지가 아닙니다.

## 9. 원리 이해: 입력에서 결과까지

대화형 `psql`에서 한 문장이 처리되는 흐름을 정리하면 다음과 같습니다.

```text
키보드 입력
  ├─ 역슬래시로 시작 → psql이 메타 명령 처리
  └─ SQL 텍스트 → 입력 버퍼에 저장
       └─ 세미콜론 또는 \g
            └─ 서버로 전송
                 ├─ 성공 → 행 또는 명령 태그 반환
                 └─ 실패 → ERROR와 SQLSTATE 반환
                      └─ psql이 설정된 형식으로 표시
```

메타 명령 가운데 `\d`처럼 서버의 시스템 카탈로그를 조회하는 명령도 있습니다. 사용자는 역슬래시 명령을 입력하지만 `psql`이 내부 SQL을 만들어 서버에 보냅니다. 다음 설정을 켜면 숨겨진 내부 SQL을 학습 목적으로 볼 수 있습니다.

```text
\set ECHO_HIDDEN on
\dt
\set ECHO_HIDDEN off
```

내부 쿼리는 버전에 따라 달라질 수 있으므로 애플리케이션 코드가 그대로 의존해서는 안 됩니다.

비대화형 실행에서는 오류 처리 설정이 특히 중요합니다. 기본 `psql`은 파일 중간의 SQL 오류 뒤에도 다음 명령을 계속할 수 있습니다. `ON_ERROR_STOP`을 켜고 셸 종료 상태를 확인해야 자동화가 실패를 놓치지 않습니다.

## 10. 자주 사용하는 psql 명령

| 명령 | 기능 |
|---|---|
| `\conninfo` | 현재 연결 정보 표시 |
| `\c database_name` | 다른 데이터베이스로 다시 연결 |
| `\l` | 데이터베이스 목록 |
| `\dn` | 스키마 목록 |
| `\dt` | 테이블 목록 |
| `\d object_name` | 테이블 등 객체 정의 |
| `\du` | PostgreSQL 역할 목록 |
| `\h SQL_COMMAND` | SQL 문법 도움말 |
| `\?` | 메타 명령 도움말 |
| `\p` | 현재 입력 버퍼 표시 |
| `\r` | 현재 입력 버퍼 삭제 |
| `\g` | 현재 입력 버퍼 실행 |
| `\x on\|off\|auto` | 확장 출력 설정 |
| `\pset` | 출력 설정 확인·변경 |
| `\timing on\|off` | 실행 시간 표시 설정 |
| `\i filename` | 현재 세션에서 파일 실행 |
| `\e` | 입력 버퍼를 외부 편집기로 편집 |
| `\q` | `psql` 종료 |

명령 전체를 외우기보다 `\?`와 Tab 자동 완성을 사용해 필요한 기능을 찾아가는 습관이 중요합니다.

## 11. 주의 및 오류 해결

### `psql: command not found`

클라이언트 설치와 실행 경로를 확인합니다.

```bash
dpkg -l postgresql-client-18
command -v psql
```

패키지가 없으면 3장의 Ubuntu 기본 저장소 설치 절차를 다시 확인합니다.

### `Peer authentication failed`

일반 Ubuntu 사용자와 PostgreSQL 역할 `postgres`를 억지로 같게 인증하려 한 경우가 많습니다. 설치 직후 관리자 접속은 다음 명령을 사용합니다.

```bash
sudo -u postgres psql -X -d postgres
```

오류를 없애려고 `pg_hba.conf`를 `trust`로 바꾸지 않습니다.

### `database ... does not exist` 또는 `role ... does not exist`

옵션을 생략하면 운영체제 사용자 이름이 역할과 데이터베이스 기본값에 사용될 수 있습니다. `-U`와 `-d`로 의도한 대상을 확인합니다. 설치 직후에는 다음 명령이 기준입니다.

```bash
sudo -u postgres psql -X -d postgres
```

### 프롬프트가 `postgres-#`, `postgres'#`처럼 바뀜

세미콜론, 따옴표, 괄호 또는 주석이 닫히지 않아 추가 입력을 기다리는 상태입니다. 잘못 입력했다면 `Ctrl+C`로 현재 버퍼를 취소하고 문장을 처음부터 확인합니다. 무작정 세미콜론을 추가하면 의도하지 않은 SQL이 실행될 수 있습니다.

### `Did not find any relations.`

연결 실패가 아니라 현재 조건에 맞는 테이블이 없다는 뜻입니다. 다음 순서로 확인합니다.

```text
\conninfo
\dn
\dt *.*
```

현재 데이터베이스, 스키마와 검색 패턴이 맞는지 봅니다. 시스템 테이블까지 모두 출력하면 목록이 매우 길 수 있습니다.

### 결과 아래 `(END)`가 표시되고 입력이 안 됨

페이저가 열린 상태입니다. `q`로 페이저를 닫습니다. 짧은 실습에서는 `\pset pager off`로 끌 수 있습니다.

### SQL 파일을 읽을 권한이 없음

`sudo -u postgres`로 실행한 프로세스는 학생 홈 디렉터리를 통과하지 못할 수 있습니다. 저장소 권한을 넓히거나 파일 소유자를 바꾸지 말고 현재 Bash 사용자가 파일을 열어 표준 입력으로 전달합니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/04_psql_basics.sql
```

### 파일 중간에 오류가 났는데 다음 명령이 실행됨

비대화형 파일 실행의 기본 오류 처리에 의존하지 않습니다. `\set ON_ERROR_STOP on`을 파일 앞에 넣거나 `-v ON_ERROR_STOP=1`을 명령줄에 지정합니다. 전체 변경의 원자성도 필요하면 파일 내용을 검토한 뒤 `-1`을 함께 사용합니다.

### 한글이 깨짐

Ubuntu 터미널 로캘과 클라이언트 인코딩을 확인합니다.

```bash
locale
```

`psql` 안에서는 다음 값을 확인합니다.

```sql
SHOW client_encoding;
SHOW server_encoding;
```

일반적인 UTF-8 환경에서는 둘 다 `UTF8` 계열이 예상됩니다. 원인을 확인하지 않고 데이터베이스 인코딩을 변경하지 않습니다.

## 12. 실습 문제

### 기본 문제

1. `postgres` 데이터베이스에 개인 시작 파일을 읽지 않고 접속한 뒤 현재 연결 정보를 확인하세요.
2. SQL 도움말과 메타 명령 도움말을 각각 여는 명령을 실행하세요.
3. 데이터베이스, 스키마와 테이블 목록을 차례로 확인하세요.
4. 임시 테이블 `psql_task_demo`를 만든 뒤 열과 제약조건을 확인하세요.
5. NULL 표시를 `(null)`로 바꾸고 다시 기본 빈 표시로 되돌리세요.
6. 확장 출력과 실행 시간 표시를 켜고 조회한 뒤 모두 끄세요.
7. `examples/04_psql_basics.sql`을 첫 오류에서 중단하도록 실행하세요.

### 응용 문제

1. SQL을 세미콜론 없이 입력하고 `\p`로 확인한 뒤 `\r`로 취소하세요.
2. 예제 파일을 한 트랜잭션 옵션과 함께 실행하고 셸에서 성공 여부를 확인하세요.
3. `-c`를 두 번 사용해 현재 데이터베이스 조회와 `\conninfo`를 한 번의 `psql` 실행에서 처리하세요.
4. 임시 테이블의 한 행을 `\gx`로 세로 출력하세요.
5. `ECHO_HIDDEN`을 잠시 켜고 `\dt`가 실행하는 내부 SQL을 관찰한 뒤 끄세요.

## 13. 실습 문제 정답

실행 결과의 버전, 시간과 경로는 환경에 따라 다릅니다. 아래는 핵심 명령 예시입니다.

### 기본 문제 정답

1. 접속과 연결 정보:

   ```bash
   sudo -u postgres psql -X -d postgres
   ```

   ```text
   \conninfo
   ```

2. 두 종류의 도움말:

   ```text
   \h SELECT
   \?
   ```

3. 객체 목록:

   ```text
   \l
   \dn
   \dt
   ```

4. 본문 4.4절의 `CREATE TEMP TABLE`을 실행한 뒤 다음 명령을 사용합니다.

   ```text
   \d psql_task_demo
   ```

5. NULL 표시 변경과 복원:

   ```text
   \pset null '(null)'
   \pset null ''
   ```

6. 출력과 시간 표시:

   ```text
   \x on
   \timing on
   SELECT * FROM psql_task_demo;
   \x off
   \timing off
   ```

7. Bash에서 안전한 파일 실행:

   ```bash
   sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/04_psql_basics.sql
   ```

### 응용 문제 정답

1. 입력 버퍼 확인과 취소:

   ```text
   SELECT current_date
   \p
   \r
   ```

2. 한 트랜잭션으로 실행한 직후 Bash의 `$?`를 확인합니다. `0`이면 성공입니다.

   ```bash
   sudo -u postgres psql -X -v ON_ERROR_STOP=1 -1 -d postgres -f - < examples/04_psql_basics.sql
   echo $?
   ```

3. 반복 `-c` 사용:

   ```bash
   sudo -u postgres psql -X -d postgres -c "SELECT current_database();" -c "\conninfo"
   ```

4. 세미콜론 대신 `\gx`를 사용합니다.

   ```text
   SELECT *
   FROM psql_task_demo
   WHERE task_id = 1
   \gx
   ```

5. 숨은 SQL 확인과 복원:

   ```text
   \set ECHO_HIDDEN on
   \dt
   \set ECHO_HIDDEN off
   ```

## 14. 핵심 정리

- `psql`은 PostgreSQL 서버에 접속하는 공식 명령줄 클라이언트입니다.
- `-d`, `-h`, `-p`, `-U`는 데이터베이스, 호스트, 포트와 역할을 지정합니다.
- Linux에서 호스트를 생략하면 보통 로컬 Unix 도메인 소켓을 사용합니다.
- SQL은 서버가 처리하고 보통 세미콜론으로 끝납니다.
- 메타 명령은 `psql`이 처리하며 역슬래시로 시작하고 세미콜론을 붙이지 않습니다.
- `\l`, `\dn`, `\dt`와 `\d`로 주요 객체와 정의를 확인합니다.
- `\pset`, `\x`와 `\timing`으로 결과 표시를 조절합니다.
- 반복 가능한 작업은 파일로 저장하고 `-f` 또는 `\i`로 실행합니다.
- 자동 실행에서는 `-X`와 `ON_ERROR_STOP`을 명시하고 종료 상태를 확인합니다.
- `\q`는 클라이언트 연결을 닫을 뿐 PostgreSQL 서버를 중지하지 않습니다.

## 15. 확인 문제

1. `psql`에서 데이터베이스 이름을 지정하는 옵션은 무엇입니까?
2. SQL 명령과 메타 명령은 각각 무엇으로 끝납니까?
3. 현재 연결 정보를 표시하는 메타 명령은 무엇입니까?
4. 테이블의 열과 제약조건을 확인하는 메타 명령은 무엇입니까?
5. 긴 행을 세로로 펼치는 출력 모드를 켜는 명령은 무엇입니까?
6. 파일 실행 중 첫 오류에서 중단하게 하는 변수는 무엇입니까?
7. 참 또는 거짓: `\q`를 실행하면 PostgreSQL systemd 서비스가 중지된다.

정답: 1. `-d` 또는 `--dbname`, 2. SQL은 보통 세미콜론, 메타 명령은 줄바꿈, 3. `\conninfo`, 4. `\d table_name`, 5. `\x on`, 6. `ON_ERROR_STOP`, 7. 거짓

## 참고한 공식 문서

- [PostgreSQL 18: psql](https://www.postgresql.org/docs/18/app-psql.html)
- [PostgreSQL 18: 데이터베이스 연결 제어 함수](https://www.postgresql.org/docs/18/libpq-connect.html)
- [PostgreSQL 18: 스키마](https://www.postgresql.org/docs/18/ddl-schemas.html)

## 다음 장 안내

이 장에서는 기본 관리자 역할로 `psql`을 조작하는 방법을 익혔습니다. 다음 장에서는 일상 실습에 슈퍼사용자를 계속 쓰지 않도록 업무 관리 시스템용 PostgreSQL 역할과 데이터베이스를 만들고, 소유자와 접속 권한을 분리합니다.
