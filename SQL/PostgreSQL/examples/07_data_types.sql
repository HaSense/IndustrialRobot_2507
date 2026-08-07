-- 7장: PostgreSQL 데이터 타입 확인
-- 실행: psql -X -v ON_ERROR_STOP=1 -d task_management -f examples/07_data_types.sql
\set ON_ERROR_STOP on
\pset pager off

SELECT pg_typeof(42::integer) AS integer_type,
       pg_typeof(42::bigint) AS bigint_type,
       pg_typeof(19.90::numeric(10, 2)) AS numeric_type;

SELECT 0.1::double precision + 0.2::double precision AS approximate_sum,
       0.1::numeric + 0.2::numeric AS exact_sum;

SELECT char_length('PostgreSQL') AS character_count,
       octet_length('한글') AS byte_count;

SELECT DATE '2026-08-04' AS due_date,
       TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul' AS seoul_time,
       TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul' AT TIME ZONE 'UTC' AS utc_clock;

SELECT TRUE::boolean AS is_active,
       '550e8400-e29b-41d4-a716-446655440000'::uuid AS example_uuid;

SELECT '{"status":"todo","labels":["sql","beginner"]}'::jsonb AS details,
       ARRAY['sql', 'postgresql']::text[] AS tags;

SELECT '42'::integer AS cast_integer,
       CAST('2026-08-04' AS date) AS cast_date,
       42::text AS cast_text;

\pset pager on
