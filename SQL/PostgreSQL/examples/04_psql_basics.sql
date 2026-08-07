-- 4장: psql 기본 사용법
-- 실행 예:
-- sudo -u postgres psql -X -v ON_ERROR_STOP=1 -d postgres -f - \
--   < examples/04_psql_basics.sql

\set ON_ERROR_STOP on
\pset pager off
\pset null '(null)'

\echo '== 현재 연결 =='
\conninfo

SELECT current_database() AS database_name,
       current_user AS role_name,
       current_setting('server_version') AS server_version;

\echo '== 재실행 가능한 임시 테이블 생성 =='
CREATE TEMP TABLE psql_task_demo (
    task_id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title text NOT NULL,
    status text NOT NULL CHECK (status IN ('todo', 'in_progress', 'done')),
    due_date date
);

INSERT INTO psql_task_demo (title, status, due_date)
VALUES
    ('요구사항 정리', 'done', DATE '2026-08-05'),
    ('화면 설계', 'in_progress', DATE '2026-08-08'),
    ('테스트 작성', 'todo', NULL);

\echo '== 임시 테이블 목록과 정의 =='
\dt
\d psql_task_demo

\echo '== 표 형식 조회 =='
SELECT task_id, title, status, due_date
FROM psql_task_demo
ORDER BY task_id;

\echo '== 한 행 확장 출력 =='
SELECT task_id, title, status, due_date
FROM psql_task_demo
WHERE task_id = 1
\gx

\echo '== 상태별 건수 =='
SELECT status, count(*) AS task_count
FROM psql_task_demo
GROUP BY status
ORDER BY status;

\pset null ''
\pset pager on
\echo '== 완료: 세션이 끝나면 임시 테이블이 자동 삭제됩니다. =='
