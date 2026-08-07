-- 10장: SELECT 기본 조회
\set ON_ERROR_STOP on
\pset pager off

SELECT task_id, title, status, priority
FROM task
ORDER BY task_id;

SELECT title AS task_title,
       due_date - start_date AS planned_days
FROM task
WHERE start_date IS NOT NULL
  AND due_date IS NOT NULL
ORDER BY planned_days DESC, task_id;

SELECT DISTINCT status
FROM task
ORDER BY status;

SELECT task_id, title, status, priority
FROM task
WHERE status IN ('todo', 'in_progress')
  AND priority IN ('high', 'urgent')
ORDER BY priority, task_id;

SELECT task_id, title, due_date
FROM task
WHERE due_date BETWEEN DATE '2026-08-01' AND DATE '2026-08-31'
ORDER BY due_date, task_id;

SELECT task_id, title
FROM task
WHERE assignee_id IS NULL
ORDER BY task_id;

SELECT task_id, title
FROM task
WHERE title ILIKE '%설계%'
ORDER BY task_id;

SELECT task_id, title, due_date
FROM task
ORDER BY due_date ASC NULLS LAST, task_id
LIMIT 5;

\pset pager on
