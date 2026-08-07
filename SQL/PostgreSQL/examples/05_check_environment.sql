-- 5장: 업무 관리 시스템 환경 검증
-- 실행 예:
-- sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - \
--   < examples/05_check_environment.sql

\set ON_ERROR_STOP on
\pset pager off

SELECT rolname,
       rolcanlogin,
       rolsuper,
       rolcreatedb,
       rolcreaterole,
       rolreplication
FROM pg_roles
WHERE rolname IN ('task_admin', 'task_app')
ORDER BY rolname;

SELECT datname,
       pg_get_userbyid(datdba) AS owner,
       pg_encoding_to_char(encoding) AS encoding,
       datallowconn
FROM pg_database
WHERE datname = 'task_management';

SELECT datname,
       datacl
FROM pg_database
WHERE datname = 'task_management';

\connect task_management postgres

SELECT current_setting('TimeZone') AS timezone;

SELECT nspname,
       nspacl
FROM pg_namespace
WHERE nspname = 'public';

\pset pager on
