# 5장 데이터베이스와 사용자 만들기

설치 직후의 `postgres` 역할은 서버 전체를 제어할 수 있는 슈퍼사용자입니다. 매일 하는 실습과 애플리케이션 접속에 이 역할을 계속 사용하면 작은 실수가 서버 전체 변경으로 이어질 수 있습니다. 이 장에서는 업무 관리 시스템 전용 역할과 데이터베이스를 만들고, 소유자·접속 역할·비밀번호를 분리해 다음 장부터 사용할 안전한 출발점을 준비합니다.

> **버전 기준**  
> 이 장은 PostgreSQL 18을 기준으로 2026년 8월 4일 공식 문서와 대조했습니다. 역할은 특정 데이터베이스가 아니라 PostgreSQL 클러스터 전체에 생성되며, 데이터베이스의 `CONNECT` 권한은 `pg_hba.conf` 인증과 별도로 검사됩니다.

## 이 장에서 배울 내용

- PostgreSQL 역할과 로그인 사용자의 관계를 설명한다.
- 최소한의 속성만 가진 로그인 역할을 생성하고 안전하게 비밀번호를 설정한다.
- 소유자를 지정해 업무 관리 시스템 데이터베이스를 생성한다.
- `PUBLIC`의 기본 접속 권한을 회수하고 필요한 역할에만 접속을 허용한다.
- 식별자와 객체 이름 규칙을 적용한다.
- 실습 환경을 검증하고 필요할 때 안전한 순서로 초기화한다.

## 선행 지식

- 3장에서 PostgreSQL 18 서버와 기본 `18/main` 클러스터를 준비했습니다.
- 4장에서 `psql`, SQL, 메타 명령과 파일 실행 방법을 익혔습니다.
- 저장소 루트에 `examples/05_create_environment.sql`과 `examples/05_check_environment.sql`이 있습니다.

먼저 **Ubuntu Bash**에서 서버 상태를 확인합니다.

```bash
pg_isready
```

`accepting connections`가 표시되어야 합니다. 현재 집필 환경에는 PostgreSQL이 설치되어 있지 않으므로 이 장의 실행 결과는 Ubuntu 26.04 WSL 실환경에서 추가 검증해야 합니다.

## 1. PostgreSQL의 역할과 사용자

PostgreSQL은 접근 주체를 모두 **역할(role)**이라는 하나의 개념으로 관리합니다. 역할은 데이터베이스 객체를 소유하거나 권한을 받을 수 있고, 다른 역할을 구성원으로 포함할 수도 있습니다.

`LOGIN` 속성이 있는 역할은 접속할 수 있으므로 일상적으로 **사용자(user)**라고 부릅니다. `LOGIN`이 없는 역할은 여러 권한을 묶는 그룹 역할로 사용할 수 있습니다.

```text
역할 role
├── LOGIN 있음  → 접속 가능한 사용자 역할
└── LOGIN 없음  → 권한 묶음·소유 전용 역할로 사용 가능
```

PostgreSQL 8.1 이후에는 사용자와 그룹이 별도 객체가 아니라 모두 역할입니다. `CREATE USER`는 `LOGIN`을 기본으로 포함한 `CREATE ROLE`의 다른 표기입니다. 이 책에서는 속성을 분명히 보이기 위해 `CREATE ROLE ... LOGIN`을 사용합니다.

### 1.1 역할은 클러스터 전체에 존재한다

테이블과 스키마는 데이터베이스 안에 있지만 역할은 한 PostgreSQL 클러스터의 모든 데이터베이스에서 공유됩니다.

```text
PostgreSQL 18/main 클러스터
├── 역할 postgres
├── 역할 task_admin
├── 역할 task_app
├── 데이터베이스 postgres
└── 데이터베이스 task_management
```

따라서 `task_admin`을 `postgres` 데이터베이스에 연결한 상태에서 만들어도 `task_management`에서 같은 역할을 사용할 수 있습니다. 반대로 같은 클러스터에 데이터베이스별로 이름이 같은 서로 다른 역할을 만들 수는 없습니다.

### 1.2 운영체제 사용자와 다시 구분하기

| 이름 | 계정 체계 | 용도 |
|---|---|---|
| Ubuntu 사용자 `postgres` | Linux 운영체제 | 서버 프로세스와 데이터 파일 소유 |
| PostgreSQL 역할 `postgres` | 데이터베이스 클러스터 | 초기 슈퍼사용자 |
| PostgreSQL 역할 `task_admin` | 데이터베이스 클러스터 | 교재 테이블의 소유·관리 |
| PostgreSQL 역할 `task_app` | 데이터베이스 클러스터 | 나중에 Python 애플리케이션이 사용 |

`sudo -u postgres`는 운영체제 사용자를 바꾸고, `psql -U task_admin`의 `-U`는 데이터베이스 역할을 선택합니다.

## 2. 실습 역할 설계

이 책에서는 다음 두 로그인 역할을 사용합니다.

| 역할 | 목적 | 슈퍼사용자 | DB·역할 생성 | 객체 소유 |
|---|---|---:|---:|---:|
| `task_admin` | 교재의 DDL과 관리 실습 | 아니요 | 아니요 | 예 |
| `task_app` | 애플리케이션 접속 | 아니요 | 아니요 | 아니요 |

`task_admin`은 `task_management` 데이터베이스의 소유자입니다. 6~21장의 설계와 SQL 실습은 주로 이 역할로 수행합니다. `task_app`은 접속만 먼저 허용하고 테이블 권한은 필요한 장에서 최소 범위로 추가합니다.

두 역할 모두 다음 강력한 속성을 주지 않습니다.

- `SUPERUSER`: 모든 접근 제한 우회
- `CREATEDB`: 새 데이터베이스 생성
- `CREATEROLE`: 다른 역할 관리
- `REPLICATION`: 복제 연결과 슬롯 관리
- `BYPASSRLS`: 행 수준 보안 우회

기본값도 대부분 제한적이지만 예제 파일에서는 의도를 명확히 보이도록 `NO...` 속성을 적습니다.

## 3. 기존 객체 사전 확인

생성 스크립트는 최초 한 번 실행하도록 설계했습니다. 같은 이름이 이미 있으면 덮어쓰지 않고 중단됩니다. 실행 전에 기본 관리자 세션을 엽니다.

**Ubuntu Bash**

```bash
sudo -u postgres psql -X -d postgres
```

역할을 확인합니다.

```sql
SELECT rolname, rolcanlogin
FROM pg_roles
WHERE rolname IN ('task_admin', 'task_app')
ORDER BY rolname;
```

데이터베이스를 확인합니다.

```sql
SELECT datname, pg_get_userbyid(datdba) AS owner
FROM pg_database
WHERE datname = 'task_management';
```

처음 실행하는 환경이면 두 쿼리 모두 0행이 예상됩니다. 객체가 하나라도 나오면 생성 스크립트를 바로 실행하지 않습니다. 이전 실습을 계속 사용할지, 이 장 뒤쪽의 초기화 절차로 비울지 먼저 결정합니다.

```text
\q
```

## 4. 역할 만들기

기본 관리자 역할로 접속한 상태에서 다음 SQL을 실행할 수 있습니다.

```sql
CREATE ROLE task_admin
    LOGIN
    NOSUPERUSER
    NOCREATEDB
    NOCREATEROLE
    NOREPLICATION
    NOBYPASSRLS;

CREATE ROLE task_app
    LOGIN
    NOSUPERUSER
    NOCREATEDB
    NOCREATEROLE
    NOREPLICATION
    NOBYPASSRLS;
```

예상 결과는 각각 `CREATE ROLE`입니다. `PASSWORD`를 넣지 않았으므로 비밀번호 인증은 아직 실패합니다. 이 상태는 비밀번호가 빈 문자열이라는 뜻이 아니라 비밀번호가 `NULL`이라서 비밀번호 인증을 사용할 수 없다는 뜻입니다.

### 4.1 왜 SQL에 비밀번호를 넣지 않는가

다음과 같은 문장은 문법상 가능하지만 교재의 권장 방식이 아닙니다.

```sql
-- 나쁜 예: 실제로 실행하지 않습니다.
CREATE ROLE unsafe_example LOGIN PASSWORD 'plain_text_password';
```

평문 비밀번호가 다음 위치에 남을 수 있기 때문입니다.

- SQL 파일과 Git 이력
- `psql` 명령 기록
- 터미널 캡처와 공유 문서
- 설정에 따른 서버 로그

예제에도 실제 비밀번호를 넣지 않습니다. 역할만 만든 뒤 `psql`의 `\password` 메타 명령으로 대화형 설정합니다.

### 4.2 역할 확인

```text
\du task_admin
\du task_app
```

`List of roles`에서 두 역할이 보이고 슈퍼사용자, 데이터베이스 생성, 역할 생성 속성이 없어야 합니다. 시스템 카탈로그로 명시적으로 확인할 수도 있습니다.

```sql
SELECT rolname,
       rolcanlogin,
       rolsuper,
       rolcreatedb,
       rolcreaterole,
       rolreplication
FROM pg_roles
WHERE rolname IN ('task_admin', 'task_app')
ORDER BY rolname;
```

`rolcanlogin`만 `t`, 나머지 확인 열은 `f`가 예상됩니다.

## 5. 비밀번호를 안전하게 설정하기

기본 관리자 `psql` 세션에서 다음 메타 명령을 한 줄씩 실행합니다.

```text
\password task_admin
\password task_app
```

각 명령은 새 비밀번호를 두 번 묻습니다. 입력 문자는 화면에 나타나지 않습니다. 두 역할에는 서로 다른 충분히 긴 개인 실습용 비밀번호를 사용합니다.

```text
Enter new password for user "task_admin":
Enter it again:
```

`\password`는 입력한 평문을 SQL 명령 기록에 남기지 않고 `ALTER ROLE`을 전송합니다. 비밀번호 해시 자체는 서버가 관리합니다.

다음 값은 사용하지 않습니다.

- 책이나 저장소에 적힌 예시 문자열
- 사용자 이름과 같은 값
- 다른 서비스에서 사용하는 실제 비밀번호
- 빈 비밀번호

비밀번호를 잊으면 슈퍼사용자로 다시 접속해 같은 `\password role_name` 명령으로 새로 설정할 수 있습니다. 기존 비밀번호를 조회하는 기능은 제공하지 않습니다.

## 6. 데이터베이스 만들기

### 6.1 소유자를 지정해 생성

기본 관리자 역할 `postgres`로 다음 문장을 실행합니다.

```sql
CREATE DATABASE task_management
    OWNER task_admin
    TEMPLATE template0
    ENCODING 'UTF8';
```

예상 결과는 `CREATE DATABASE`입니다.

- `task_management`: 교재에서 사용할 데이터베이스 이름
- `OWNER task_admin`: 객체 관리의 기준 소유자
- `TEMPLATE template0`: 사용자 정의 객체가 없는 깨끗한 시스템 템플릿
- `ENCODING 'UTF8'`: 한글을 포함할 수 있는 UTF-8 인코딩

`CREATE DATABASE`는 트랜잭션 블록 안에서 실행할 수 없습니다. 따라서 다음처럼 `BEGIN`과 `COMMIT` 사이에 넣지 않습니다.

```sql
-- 잘못된 예: 실행하지 않습니다.
BEGIN;
CREATE DATABASE task_management OWNER task_admin;
COMMIT;
```

### 6.2 설명과 기본 시간대

```sql
COMMENT ON DATABASE task_management
    IS 'PostgreSQL 교재용 업무 관리 시스템';

ALTER DATABASE task_management
    SET timezone TO 'Asia/Seoul';
```

`ALTER DATABASE ... SET`은 이 데이터베이스에 새로 접속하는 세션의 기본값을 정합니다. 현재 열려 있는 `postgres` 데이터베이스 세션의 시간대가 즉시 바뀌는 것은 아닙니다.

### 6.3 목록과 소유자 확인

```text
\l+ task_management
```

목록에서 이름, 소유자 `task_admin`, 인코딩 `UTF8`과 설명을 확인합니다. SQL로도 확인합니다.

```sql
SELECT datname,
       pg_get_userbyid(datdba) AS owner,
       pg_encoding_to_char(encoding) AS encoding,
       datallowconn
FROM pg_database
WHERE datname = 'task_management';
```

예상 핵심 결과는 다음과 같습니다.

```text
     datname     |   owner    | encoding | datallowconn
-----------------+------------+----------+--------------
 task_management | task_admin | UTF8     | t
(1 row)
```

## 7. 접속 권한 구성

### 7.1 PUBLIC의 의미

`PUBLIC`은 새 역할을 하나 만든 것이 아니라 모든 역할을 뜻하는 특수한 이름입니다. PostgreSQL은 새 데이터베이스에 `PUBLIC`의 `CONNECT`와 `TEMPORARY` 권한을 기본으로 부여합니다.

인증에 성공한 임의의 역할이 업무 데이터베이스에 접속하지 않도록 기본 접속 권한을 회수하고 두 역할에 명시적으로 부여합니다.

```sql
REVOKE CONNECT ON DATABASE task_management FROM PUBLIC;

GRANT CONNECT ON DATABASE task_management
TO task_admin, task_app;
```

`CONNECT`는 접속 시작 시 확인됩니다. 이 권한이 있어도 `pg_hba.conf`의 인증을 통과해야 하며, 반대로 인증에 성공해도 `CONNECT`가 없으면 데이터베이스에 들어갈 수 없습니다.

### 7.2 데이터베이스 권한과 테이블 권한은 다르다

`task_app`에 `CONNECT`를 주었다고 아직 만들어지지도 않은 테이블을 읽고 쓸 수 있는 것은 아닙니다.

```text
서버 인증 통과
  └─ 데이터베이스 CONNECT 권한
      └─ 스키마 USAGE 권한
          └─ 테이블 SELECT·INSERT·UPDATE·DELETE 권한
```

이 장에서는 접속과 스키마 사용의 바닥만 준비합니다. 테이블 권한과 기본 권한은 22장에서 최소 권한 원칙에 맞춰 완성합니다.

### 7.3 public 스키마 권한 정리

대상 데이터베이스로 연결을 바꿉니다.

```text
\connect task_management postgres
```

임의 역할의 객체 생성을 막는 의도를 분명히 합니다.

```sql
REVOKE CREATE ON SCHEMA public FROM PUBLIC;
GRANT USAGE ON SCHEMA public TO task_app;
```

PostgreSQL 15 이후 기본 구성에서는 `public` 스키마의 `CREATE` 권한이 이미 `PUBLIC`에 없을 수 있습니다. `REVOKE`는 해당 권한이 없어도 안전하게 의도를 명시합니다. 데이터베이스 소유자 `task_admin`은 `pg_database_owner`를 통해 기본 `public` 스키마를 관리할 수 있습니다.

권한을 확인합니다.

```text
\dn+
\l+ task_management
```

권한 표기의 한 글자 약어는 처음에는 낯설 수 있습니다. 데이터베이스에서 `c`는 `CONNECT`, `T`는 임시 테이블, 스키마에서 `U`는 `USAGE`, `C`는 `CREATE`를 뜻합니다.

## 8. 전체 환경을 예제 파일로 만들기

앞의 명령을 직접 실행하지 않았다면 저장소 루트의 **Ubuntu Bash**에서 최초 1회 생성 스크립트를 실행합니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/05_create_environment.sql
```

이 파일은 역할과 데이터베이스 생성, 시간대와 권한 설정까지 수행하지만 비밀번호는 포함하지 않습니다. 실행 뒤 대화형 관리자 세션에서 비밀번호를 설정합니다.

```bash
sudo -u postgres psql -X -d postgres
```

```text
\password task_admin
\password task_app
\q
```

> **재실행 주의**  
> 생성 파일은 멱등성(idempotency)을 가장하기 위해 기존 객체를 자동 삭제하지 않습니다. 같은 이름이 있으면 `ON_ERROR_STOP`에 의해 중단됩니다. 이미 올바르게 만들어졌다면 검증 파일만 실행하고, 잘못 만든 빈 실습 환경이라면 12절의 초기화 절차를 검토합니다.

## 9. 환경 검증

관리자 관점에서 역할, 데이터베이스, 시간대와 권한을 한 번에 확인합니다.

```bash
sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/05_check_environment.sql
```

다음을 만족해야 합니다.

- 정확히 `task_admin`, `task_app` 두 행이 조회된다.
- 두 역할은 로그인 가능하지만 슈퍼사용자, DB 생성, 역할 생성, 복제 권한이 없다.
- `task_management`의 소유자는 `task_admin`이다.
- 인코딩은 `UTF8`이고 연결 허용 상태다.
- 새 `task_management` 세션의 `TimeZone`은 `Asia/Seoul`이다.
- `PUBLIC`의 데이터베이스 `CONNECT` 권한이 없고 두 역할에 명시적으로 부여되어 있다.

`datacl`과 `nspacl`은 접근 제어 목록(Access Control List, ACL)의 내부 표현이라 처음에는 복잡해 보입니다. 이 장에서는 `\l+`, `\dn+` 출력과 함께 원하는 역할 이름이 있는지만 확인하고 22장에서 정확히 해석합니다.

## 10. 새 역할로 접속하기

비밀번호 인증을 실제로 사용하기 위해 TCP의 `localhost`로 접속합니다.

```bash
psql -X -h localhost -p 5432 -U task_admin -d task_management -W
```

`-W`는 연결 전에 비밀번호 입력을 요청합니다. 성공하면 다음을 확인합니다.

```text
\conninfo
```

```sql
SELECT current_database(), current_user;
SHOW TimeZone;
```

핵심 예상값은 `task_management`, `task_admin`, `Asia/Seoul`입니다. 프롬프트 끝은 슈퍼사용자가 아닌 일반 역할이므로 보통 `>`입니다.

```text
task_management=>
```

4장에서 만든 임시 테이블과 달리 앞으로 만드는 테이블은 이 데이터베이스에 영구 저장됩니다.

접속을 종료합니다.

```text
\q
```

`task_app`도 같은 방식으로 접속만 확인합니다.

```bash
psql -X -h localhost -p 5432 -U task_app -d task_management -W
```

아직 테이블 생성 권한을 주지 않았으므로 이 역할로 DDL을 실행하지 않습니다.

## 11. 이름과 식별자 규칙

SQL에서 테이블, 열, 역할과 데이터베이스의 이름을 **식별자(identifier)**라고 합니다. 일관된 규칙은 따옴표와 대소문자 문제를 줄입니다.

이 책의 규칙은 다음과 같습니다.

- 영문 소문자 `snake_case`를 사용한다.
- 역할은 목적을 드러내는 `task_admin`, `task_app`처럼 짓는다.
- 데이터베이스는 도메인을 드러내는 `task_management`를 사용한다.
- 공백, 하이픈, 한글과 달러 기호를 객체 이름에 사용하지 않는다.
- SQL 키워드와 충돌하는 이름을 피한다.
- 기본 설정의 식별자 한계인 63바이트보다 충분히 짧게 짓는다.

따옴표 없는 식별자는 PostgreSQL에서 소문자로 접힙니다.

```sql
SELECT current_user AS TaskAdmin;
```

별칭은 실제로 `taskadmin`으로 보입니다. 큰따옴표를 사용하면 대소문자가 보존됩니다.

```sql
SELECT current_user AS "TaskAdmin";
```

하지만 이후에도 매번 정확한 큰따옴표와 대소문자를 써야 하므로 다음과 같은 객체 이름은 만들지 않습니다.

```sql
-- 나쁜 예: 실행하지 않습니다.
CREATE ROLE "Task Admin" LOGIN;
CREATE DATABASE "Task-Management";
```

SQL 문자열 값에는 작은따옴표, 식별자를 강제로 인용할 때는 큰따옴표를 쓴다는 차이도 기억합니다.

## 12. 실습 환경 초기화와 객체 삭제

> **위험: 이후 장의 모든 데이터 삭제**  
> 다음 절차는 `task_management` 데이터베이스와 그 안의 모든 테이블·데이터를 삭제합니다. 6장 이후에 데이터를 만들었다면 먼저 24장의 백업·복원 절차를 적용해야 합니다. 이 장을 처음 연습하는 빈 개인 환경에서만 사용합니다.

다른 `psql`과 DBeaver 연결을 모두 종료합니다. 기본 관리자 데이터베이스에 접속합니다.

```bash
sudo -u postgres psql -X -d postgres
```

현재 연결을 확인합니다.

```sql
SELECT datname, usename, application_name, client_addr
FROM pg_stat_activity
WHERE datname = 'task_management';
```

행이 있으면 해당 클라이언트에서 정상 종료합니다. 강제로 세션을 끊지 않습니다. 0행이 된 것을 확인한 뒤에만 다음 삭제를 실행합니다.

```sql
DROP DATABASE task_management;
DROP ROLE task_app;
DROP ROLE task_admin;
```

순서가 중요합니다. 데이터베이스를 먼저 삭제해 `task_admin`이 소유한 객체와 `task_app`에 부여된 데이터베이스 내부 권한을 제거한 뒤 역할을 삭제합니다.

삭제 결과를 확인합니다.

```sql
SELECT datname
FROM pg_database
WHERE datname = 'task_management';

SELECT rolname
FROM pg_roles
WHERE rolname IN ('task_admin', 'task_app');
```

두 쿼리 모두 0행이면 초기 상태입니다. 다시 실습하려면 8절의 생성 파일과 비밀번호 설정을 순서대로 수행합니다.

## 13. 원리 이해: 인증과 권한은 여러 문을 통과한다

`task_app`이 접속하고 테이블을 조회하기까지는 한 가지 설정만 확인하지 않습니다.

```text
1. 서버 주소·포트에 도달
   ↓
2. pg_hba.conf가 인증 방식 선택
   ↓
3. 역할의 LOGIN과 비밀번호 확인
   ↓
4. 데이터베이스 CONNECT 권한 확인
   ↓
5. 스키마 USAGE 권한 확인
   ↓
6. 테이블 SELECT 등 객체 권한 확인
```

따라서 "비밀번호가 맞는데 접속할 수 없다"거나 "접속했는데 테이블을 읽을 수 없다"는 상황이 가능합니다. 오류가 난 단계를 구분해야 합니다.

소유자(owner)는 해당 객체를 변경하고 삭제할 수 있는 특별한 위치에 있습니다. 객체 권한은 소유자가 다른 역할에 나누어 줄 수 있지만, 소유권 자체는 일반 권한과 다릅니다. 이 책에서는 `task_admin`이 구조를 소유하고 `task_app`은 필요한 동작 권한만 받는 방향을 유지합니다.

## 14. 주의 및 오류 해결

### `ERROR: role "task_admin" already exists`

생성 스크립트를 두 번 실행했거나 이전 실행이 일부 완료됐습니다. 자동으로 역할을 삭제하지 말고 다음을 확인합니다.

```sql
SELECT rolname, rolcanlogin, rolsuper, rolcreatedb, rolcreaterole
FROM pg_roles
WHERE rolname IN ('task_admin', 'task_app');
```

올바르면 검증 단계로 넘어갑니다. 잘못된 빈 환경일 때만 12절을 사용합니다.

### `ERROR: database "task_management" already exists`

소유자와 권한을 확인합니다.

```text
\l+ task_management
```

기존 데이터가 있다면 삭제하지 않습니다. 이전 학습을 이어갈지 백업할지 결정합니다.

### `CREATE DATABASE cannot run inside a transaction block`

`CREATE DATABASE`를 `BEGIN`과 `COMMIT` 사이 또는 `psql -1` 파일에서 실행했습니다. 생성 파일에는 `-1` 옵션을 사용하지 않습니다. 역할 생성이 먼저 성공했다면 상태를 확인하고 환경을 초기화한 뒤 다시 순서대로 실행합니다.

### `password authentication failed for user "task_admin"`

역할 이름과 비밀번호를 확인하고, 관리자 세션에서 비밀번호를 다시 설정합니다.

```text
\password task_admin
```

`pg_hba.conf`를 `trust`로 낮추지 않습니다. TCP와 소켓 중 어느 방식으로 접속했는지도 `-h` 옵션으로 확인합니다.

### `permission denied for database task_management`

역할에 `CONNECT`가 있는지 `\l+ task_management`로 확인합니다. 인증 규칙과 `CONNECT` 권한은 별개입니다.

```sql
GRANT CONNECT ON DATABASE task_management TO task_admin;
```

권한을 추가하기 전에 역할 이름과 목적이 맞는지 확인합니다.

### `database ... is being accessed by other users`

삭제하려는 데이터베이스에 다른 세션이 있습니다. DBeaver, 다른 터미널과 애플리케이션을 정상 종료합니다. `pg_stat_activity`에서 0행인지 확인한 뒤 다시 시도합니다. 입문 실습에서는 `WITH (FORCE)`나 `pg_terminate_backend`로 강제 종료하지 않습니다.

### 예상과 다른 역할로 접속됨

```sql
SELECT session_user, current_user;
```

초기 접속 역할과 현재 권한 역할을 확인합니다. 셸의 `PGUSER`, `PGDATABASE` 같은 환경변수가 기본값을 바꿀 수 있으므로 명령에서 `-U`, `-d`, `-h`를 명시합니다.

## 15. 실습 문제

### 기본 문제

1. 역할과 `LOGIN`이 있는 사용자 역할의 차이를 설명하세요.
2. `task_admin`과 `task_app`이 슈퍼사용자가 아닌지 확인하세요.
3. 평문 비밀번호를 SQL 파일에 쓰지 않고 두 역할의 비밀번호를 설정하세요.
4. `task_management`의 소유자, 인코딩과 연결 허용 상태를 확인하세요.
5. `PUBLIC`의 `CONNECT`를 회수하고 두 역할에만 부여하세요.
6. `task_admin`으로 TCP 접속해 데이터베이스, 역할과 시간대를 확인하세요.

### 응용 문제

1. `examples/05_check_environment.sql`을 오류 중단 옵션과 함께 실행하세요.
2. `task_app`의 데이터베이스 접속과 스키마 `USAGE`는 있지만 테이블 권한은 아직 별도라는 사실을 권한 계층으로 설명하세요.
3. 따옴표 없는 별칭 `TaskAdmin`과 큰따옴표 별칭 `"TaskAdmin"`의 출력 차이를 확인하세요.
4. 빈 개인 실습 환경에서 활성 연결이 없는지 확인한 뒤 데이터베이스와 역할을 안전한 순서로 초기화하고 다시 생성하세요.

## 16. 실습 문제 정답

### 기본 문제 정답

1. 모든 접근 주체는 역할이며, 그중 `LOGIN` 속성이 있는 역할이 초기 세션 사용자로 접속할 수 있습니다.

2. 관리자 세션에서 다음을 실행합니다.

   ```sql
   SELECT rolname, rolcanlogin, rolsuper, rolcreatedb, rolcreaterole
   FROM pg_roles
   WHERE rolname IN ('task_admin', 'task_app')
   ORDER BY rolname;
   ```

   `rolcanlogin`은 `t`, 나머지 세 권한 열은 `f`여야 합니다.

3. 대화형 `psql`에서 다음을 사용합니다.

   ```text
   \password task_admin
   \password task_app
   ```

4. 다음 SQL 또는 `\l+ task_management`를 사용합니다.

   ```sql
   SELECT datname,
          pg_get_userbyid(datdba) AS owner,
          pg_encoding_to_char(encoding) AS encoding,
          datallowconn
   FROM pg_database
   WHERE datname = 'task_management';
   ```

5. 관리자 세션에서 실행합니다.

   ```sql
   REVOKE CONNECT ON DATABASE task_management FROM PUBLIC;
   GRANT CONNECT ON DATABASE task_management TO task_admin, task_app;
   ```

6. Bash와 접속한 `psql`에서 순서대로 실행합니다.

   ```bash
   psql -X -h localhost -p 5432 -U task_admin -d task_management -W
   ```

   ```sql
   SELECT current_database(), current_user;
   SHOW TimeZone;
   ```

### 응용 문제 정답

1. 다음 명령을 사용합니다.

   ```bash
   sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - < examples/05_check_environment.sql
   ```

2. 인증 뒤 `CONNECT`는 데이터베이스 진입, `USAGE`는 스키마 안의 객체 이름 접근, `SELECT`·`INSERT` 등은 실제 테이블 동작을 허용합니다. 앞 단계의 성공이 뒤 단계 권한을 자동으로 주지 않습니다.

3. 다음 두 쿼리를 비교합니다.

   ```sql
   SELECT current_user AS TaskAdmin;
   SELECT current_user AS "TaskAdmin";
   ```

   첫 열 이름은 `taskadmin`, 두 번째는 `TaskAdmin`으로 표시됩니다.

4. `pg_stat_activity`로 활성 연결이 0행인지 확인하고 `postgres` 데이터베이스에서 다음 순서로 실행합니다. 데이터가 있는 환경에서는 실행하지 않습니다.

   ```sql
   DROP DATABASE task_management;
   DROP ROLE task_app;
   DROP ROLE task_admin;
   ```

   이후 생성 파일을 실행하고 `\password`로 두 비밀번호를 다시 설정합니다.

## 17. 핵심 정리

- PostgreSQL은 사용자와 그룹을 모두 역할이라는 개념으로 관리합니다.
- `LOGIN` 속성이 있는 역할만 초기 세션 사용자로 접속할 수 있습니다.
- 역할은 데이터베이스가 아니라 PostgreSQL 클러스터 전체에 존재합니다.
- `task_admin`은 구조 소유·관리, `task_app`은 애플리케이션 접속 역할입니다.
- 일상 역할에는 슈퍼사용자와 역할·데이터베이스 생성 권한을 주지 않습니다.
- 비밀번호는 SQL 파일에 쓰지 않고 대화형 `\password`로 설정합니다.
- `task_management`의 소유자는 `task_admin`, 기본 시간대는 `Asia/Seoul`입니다.
- `PUBLIC`은 모든 역할을 뜻하며 새 데이터베이스에는 기본 `CONNECT`가 있습니다.
- 인증, 데이터베이스 접속, 스키마와 테이블 권한은 서로 다른 단계입니다.
- `CREATE DATABASE`는 트랜잭션 블록 안에서 실행할 수 없습니다.
- 파괴적인 초기화는 활성 연결과 데이터 유무를 확인한 뒤 정확한 순서로 수행합니다.

## 18. 확인 문제

1. PostgreSQL에서 접속 가능한 사용자 역할을 만드는 속성은 무엇입니까?
2. 역할은 특정 데이터베이스에만 존재합니까, 클러스터 전체에 존재합니까?
3. SQL 기록에 평문 비밀번호를 남기지 않는 `psql` 명령은 무엇입니까?
4. 데이터베이스 소유자를 지정하는 `CREATE DATABASE` 절은 무엇입니까?
5. 모든 역할을 뜻하는 특수 이름은 무엇입니까?
6. 데이터베이스에 들어갈 수 있게 하는 권한은 무엇입니까?
7. 참 또는 거짓: `CONNECT` 권한이 있으면 모든 테이블을 조회할 수 있다.
8. 참 또는 거짓: `CREATE DATABASE`는 `BEGIN`과 `COMMIT` 사이에서 실행할 수 있다.

정답: 1. `LOGIN`, 2. 클러스터 전체, 3. `\password`, 4. `OWNER`, 5. `PUBLIC`, 6. `CONNECT`, 7. 거짓, 8. 거짓

## 참고한 공식 문서

- [PostgreSQL 18: Database Roles](https://www.postgresql.org/docs/18/user-manag.html)
- [PostgreSQL 18: CREATE ROLE](https://www.postgresql.org/docs/18/sql-createrole.html)
- [PostgreSQL 18: CREATE DATABASE](https://www.postgresql.org/docs/18/sql-createdatabase.html)
- [PostgreSQL 18: Privileges](https://www.postgresql.org/docs/18/ddl-priv.html)
- [PostgreSQL 18: Lexical Structure](https://www.postgresql.org/docs/18/sql-syntax-lexical.html)

## 다음 장 안내

업무 관리 시스템 전용 역할과 빈 데이터베이스를 준비했습니다. 다음 장에서는 바로 테이블을 만들기 전에 요구사항에서 엔터티를 찾고, 기본 키·외래 키·관계·NULL과 정규화를 이용해 데이터 모델과 ERD를 확정합니다.
