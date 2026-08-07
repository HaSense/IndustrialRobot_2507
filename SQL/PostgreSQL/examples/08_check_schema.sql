-- 8장: 공통 스키마 구조 확인
\set ON_ERROR_STOP on
\pset pager off

SELECT table_name
FROM information_schema.tables
WHERE table_schema = 'public'
  AND table_type = 'BASE TABLE'
ORDER BY table_name;

SELECT table_name, constraint_type, count(*) AS constraint_count
FROM information_schema.table_constraints
WHERE table_schema = 'public'
GROUP BY table_name, constraint_type
ORDER BY table_name, constraint_type;

SELECT table_name, column_name, data_type, is_nullable, column_default
FROM information_schema.columns
WHERE table_schema = 'public'
ORDER BY table_name, ordinal_position;

\pset pager on
