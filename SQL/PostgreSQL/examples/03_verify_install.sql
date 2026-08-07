-- 3장: PostgreSQL 설치 검증
-- 실행 예:
-- sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - \
--   < examples/03_verify_install.sql

\set ON_ERROR_STOP on

SELECT version();

SELECT current_database(), current_user;

SELECT current_setting('server_version') AS server_version,
       current_setting('port') AS port,
       current_setting('data_directory') AS data_directory,
       current_setting('config_file') AS config_file,
       current_setting('hba_file') AS hba_file,
       current_setting('TimeZone') AS timezone;
