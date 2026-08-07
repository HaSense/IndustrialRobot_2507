-- 11장: PostgreSQL 함수와 표현식
\set ON_ERROR_STOP on
\pset pager off

SELECT name,
       upper(name) AS upper_name,
       char_length(name) AS character_count
FROM department
ORDER BY department_code;

SELECT round(1234.567::numeric, 2) AS rounded,
       abs(-15) AS absolute_value,
       ceil(3.2::numeric) AS ceiling,
       floor(3.8::numeric) AS floor_value;

SELECT title,
       due_date,
       due_date - DATE '2026-08-04' AS days_from_base
FROM task
WHERE due_date IS NOT NULL
ORDER BY due_date, task_id;

SELECT title,
       CASE priority
           WHEN 'urgent' THEN '긴급'
           WHEN 'high' THEN '높음'
           WHEN 'medium' THEN '보통'
           WHEN 'low' THEN '낮음'
           ELSE '알 수 없음'
       END AS priority_name
FROM task
ORDER BY task_id;

SELECT title,
       coalesce(description, '설명 없음') AS description,
       coalesce(due_date::text, '미정') AS due_date_text
FROM task
ORDER BY task_id;

SELECT event_type,
       details ->> 'status' AS initial_status
FROM task_history
ORDER BY history_id;

SELECT TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul'
           AT TIME ZONE 'Asia/Seoul' AS seoul_clock,
       TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul'
           AT TIME ZONE 'UTC' AS utc_clock;

\pset pager on
