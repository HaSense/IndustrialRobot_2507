-- 5장: 업무 관리 시스템 역할과 데이터베이스 생성
-- 주의: 최초 1회 실행용이며 같은 이름의 객체가 있으면 중단됩니다.
-- 실행 예:
-- sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - \
--   < examples/05_create_environment.sql

\set ON_ERROR_STOP on
\echo '== 로그인 역할 생성 =='

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

\echo '== 데이터베이스 생성 =='

CREATE DATABASE task_management
    OWNER task_admin
    TEMPLATE template0
    ENCODING 'UTF8';

COMMENT ON DATABASE task_management
    IS 'PostgreSQL 교재용 업무 관리 시스템';

ALTER DATABASE task_management
    SET timezone TO 'Asia/Seoul';

REVOKE CONNECT ON DATABASE task_management FROM PUBLIC;
GRANT CONNECT ON DATABASE task_management TO task_admin, task_app;

\echo '== public 스키마 기본 권한 정리 =='
\connect task_management postgres

REVOKE CREATE ON SCHEMA public FROM PUBLIC;
GRANT USAGE ON SCHEMA public TO task_app;

\echo '== 생성 결과 =='
\du task_admin
\du task_app
\l+ task_management
\dn+

\echo '== 완료: 비밀번호는 대화형 psql에서 \password로 별도 설정하세요. =='
