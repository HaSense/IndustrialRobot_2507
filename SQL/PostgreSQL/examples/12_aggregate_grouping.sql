-- 12장: 집계와 그룹화
\set ON_ERROR_STOP on
\pset pager off

SELECT count(*) AS total_tasks,
       count(assignee_id) AS assigned_tasks,
       count(*) - count(assignee_id) AS unassigned_tasks,
       min(due_date) AS first_due_date,
       max(due_date) AS last_due_date
FROM task;

SELECT status, count(*) AS task_count
FROM task
GROUP BY status
ORDER BY status;

SELECT project_id,
       count(*) AS task_count,
       round(avg(due_date - start_date), 1) AS avg_planned_days
FROM task
GROUP BY project_id
ORDER BY project_id;

SELECT project_id, count(*) AS task_count
FROM task
GROUP BY project_id
HAVING count(*) >= 3
ORDER BY project_id;

SELECT count(*) FILTER (WHERE status = 'done') AS done_count,
       count(*) FILTER (WHERE status IN ('todo', 'in_progress', 'blocked'))
           AS open_count,
       count(*) FILTER (WHERE assignee_id IS NULL) AS unassigned_count
FROM task;

SELECT project_id,
       status,
       count(*) AS task_count,
       GROUPING(project_id, status) AS grouping_code
FROM task
GROUP BY ROLLUP (project_id, status)
ORDER BY project_id NULLS LAST, status NULLS LAST;

\pset pager on
