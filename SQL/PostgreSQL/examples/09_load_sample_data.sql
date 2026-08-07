-- 9장 이후 공통 샘플 데이터 초기화
-- 경고: 7개 업무 테이블의 기존 데이터를 모두 삭제하고 다시 입력합니다.
-- 개인 실습 데이터베이스 task_management에서만 실행하십시오.
\set ON_ERROR_STOP on

BEGIN;

TRUNCATE TABLE
    task_history,
    task_comment,
    task,
    project_member,
    project,
    app_user,
    department
RESTART IDENTITY;

INSERT INTO department (department_code, name, created_at) VALUES
    ('DEV', '개발팀',       TIMESTAMPTZ '2026-01-02 09:00:00+09'),
    ('QA',  '품질관리팀',   TIMESTAMPTZ '2026-01-02 09:05:00+09'),
    ('OPS', '서비스운영팀', TIMESTAMPTZ '2026-01-02 09:10:00+09'),
    ('PMO', '프로젝트관리팀', TIMESTAMPTZ '2026-01-02 09:15:00+09');

INSERT INTO app_user
    (department_id, name, email, is_active, created_at, deactivated_at)
VALUES
    ((SELECT department_id FROM department WHERE department_code = 'DEV'),
     '김하늘', 'haneul.kim@example.test', TRUE,
     TIMESTAMPTZ '2026-01-03 09:00:00+09', NULL),
    ((SELECT department_id FROM department WHERE department_code = 'DEV'),
     '박가람', 'garam.park@example.test', TRUE,
     TIMESTAMPTZ '2026-01-03 09:10:00+09', NULL),
    ((SELECT department_id FROM department WHERE department_code = 'QA'),
     '이여름', 'yeoreum.lee@example.test', TRUE,
     TIMESTAMPTZ '2026-01-03 09:20:00+09', NULL),
    ((SELECT department_id FROM department WHERE department_code = 'OPS'),
     '최겨울', 'gyeoul.choi@example.test', TRUE,
     TIMESTAMPTZ '2026-01-03 09:30:00+09', NULL),
    ((SELECT department_id FROM department WHERE department_code = 'PMO'),
     '정다온', 'daon.jeong@example.test', TRUE,
     TIMESTAMPTZ '2026-01-03 09:40:00+09', NULL),
    ((SELECT department_id FROM department WHERE department_code = 'QA'),
     '윤새봄', 'saebom.yoon@example.test', FALSE,
     TIMESTAMPTZ '2026-01-03 09:50:00+09',
     TIMESTAMPTZ '2026-06-30 18:00:00+09');

INSERT INTO project
    (project_code, manager_user_id, name, description, status,
     start_date, end_date, created_at, updated_at)
VALUES
    ('PORTAL',
     (SELECT user_id FROM app_user WHERE email = 'daon.jeong@example.test'),
     '사내 포털 개편', '업무 포털의 사용성과 검색 기능을 개선한다.',
     'active', DATE '2026-07-01', DATE '2026-10-31',
     TIMESTAMPTZ '2026-06-15 10:00:00+09',
     TIMESTAMPTZ '2026-07-20 15:00:00+09'),
    ('MOBILE',
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     '모바일 업무 앱', '현장 업무 확인용 모바일 앱을 만든다.',
     'planned', DATE '2026-09-01', NULL,
     TIMESTAMPTZ '2026-07-01 11:00:00+09',
     TIMESTAMPTZ '2026-07-01 11:00:00+09'),
    ('MIGRATION',
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     '문서 서버 이전', '기존 문서 서버를 신규 환경으로 이전한다.',
     'completed', DATE '2026-02-01', DATE '2026-05-31',
     TIMESTAMPTZ '2026-01-20 14:00:00+09',
     TIMESTAMPTZ '2026-05-31 17:30:00+09');

INSERT INTO project_member (project_id, user_id, member_role, joined_at)
SELECT p.project_id, u.user_id, v.member_role, v.joined_at
FROM (VALUES
    ('PORTAL', 'daon.jeong@example.test',   'manager', TIMESTAMPTZ '2026-06-15 10:00:00+09'),
    ('PORTAL', 'haneul.kim@example.test',   'member',  TIMESTAMPTZ '2026-06-16 09:00:00+09'),
    ('PORTAL', 'garam.park@example.test',   'member',  TIMESTAMPTZ '2026-06-16 09:05:00+09'),
    ('PORTAL', 'yeoreum.lee@example.test',  'member',  TIMESTAMPTZ '2026-06-20 09:00:00+09'),
    ('MOBILE', 'haneul.kim@example.test',   'manager', TIMESTAMPTZ '2026-07-01 11:00:00+09'),
    ('MOBILE', 'garam.park@example.test',   'member',  TIMESTAMPTZ '2026-07-02 09:00:00+09'),
    ('MIGRATION', 'gyeoul.choi@example.test','manager',TIMESTAMPTZ '2026-01-20 14:00:00+09'),
    ('MIGRATION', 'yeoreum.lee@example.test','viewer', TIMESTAMPTZ '2026-02-01 09:00:00+09')
) AS v(project_code, email, member_role, joined_at)
JOIN project p ON p.project_code = v.project_code
JOIN app_user u ON u.email = v.email;

INSERT INTO task
    (project_id, parent_task_id, created_by, assignee_id, title, description,
     status, priority, start_date, due_date, created_at, updated_at, completed_at)
VALUES
    ((SELECT project_id FROM project WHERE project_code = 'PORTAL'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'daon.jeong@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     '요구사항 정리', '부서별 개선 요구사항을 정리한다.',
     'done', 'high', DATE '2026-07-01', DATE '2026-07-10',
     TIMESTAMPTZ '2026-07-01 09:00:00+09', TIMESTAMPTZ '2026-07-09 16:00:00+09',
     TIMESTAMPTZ '2026-07-09 16:00:00+09'),
    ((SELECT project_id FROM project WHERE project_code = 'PORTAL'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'daon.jeong@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'garam.park@example.test'),
     '화면 설계', NULL, 'in_progress', 'high', DATE '2026-07-11', DATE '2026-08-08',
     TIMESTAMPTZ '2026-07-10 10:00:00+09', TIMESTAMPTZ '2026-08-01 13:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'PORTAL'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     '검색 API 구현', '제목과 본문 검색 API를 구현한다.',
     'blocked', 'urgent', DATE '2026-07-20', DATE '2026-08-15',
     TIMESTAMPTZ '2026-07-18 11:00:00+09', TIMESTAMPTZ '2026-08-03 10:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'PORTAL'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'yeoreum.lee@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'yeoreum.lee@example.test'),
     '통합 테스트 작성', NULL, 'todo', 'medium', NULL, DATE '2026-08-31',
     TIMESTAMPTZ '2026-07-25 14:00:00+09', TIMESTAMPTZ '2026-07-25 14:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'PORTAL'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'daon.jeong@example.test'), NULL,
     '사용자 교육 자료', NULL, 'todo', 'low', NULL, DATE '2026-10-15',
     TIMESTAMPTZ '2026-08-01 09:00:00+09', TIMESTAMPTZ '2026-08-01 09:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'MOBILE'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'garam.park@example.test'),
     '기술 검토', '지원할 운영체제와 배포 방식을 검토한다.',
     'todo', 'high', DATE '2026-09-01', DATE '2026-09-10',
     TIMESTAMPTZ '2026-07-05 10:00:00+09', TIMESTAMPTZ '2026-07-05 10:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'MOBILE'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'), NULL,
     '로그인 화면 시안', NULL, 'todo', 'medium', DATE '2026-09-11', NULL,
     TIMESTAMPTZ '2026-07-06 10:00:00+09', TIMESTAMPTZ '2026-07-06 10:00:00+09', NULL),
    ((SELECT project_id FROM project WHERE project_code = 'MIGRATION'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     '이전 계획 수립', NULL, 'done', 'high', DATE '2026-02-01', DATE '2026-02-14',
     TIMESTAMPTZ '2026-02-01 09:00:00+09', TIMESTAMPTZ '2026-02-13 17:00:00+09',
     TIMESTAMPTZ '2026-02-13 17:00:00+09'),
    ((SELECT project_id FROM project WHERE project_code = 'MIGRATION'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     '데이터 복사', NULL, 'done', 'urgent', DATE '2026-05-01', DATE '2026-05-20',
     TIMESTAMPTZ '2026-04-20 09:00:00+09', TIMESTAMPTZ '2026-05-18 20:00:00+09',
     TIMESTAMPTZ '2026-05-18 20:00:00+09'),
    ((SELECT project_id FROM project WHERE project_code = 'MIGRATION'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'yeoreum.lee@example.test'),
     '복원 검증', NULL, 'done', 'high', DATE '2026-05-21', DATE '2026-05-28',
     TIMESTAMPTZ '2026-05-01 09:00:00+09', TIMESTAMPTZ '2026-05-27 15:00:00+09',
     TIMESTAMPTZ '2026-05-27 15:00:00+09'),
    ((SELECT project_id FROM project WHERE project_code = 'MIGRATION'), NULL,
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     (SELECT user_id FROM app_user WHERE email = 'gyeoul.choi@example.test'),
     '기존 서버 종료', NULL, 'cancelled', 'low', NULL, NULL,
     TIMESTAMPTZ '2026-05-25 09:00:00+09', TIMESTAMPTZ '2026-05-30 10:00:00+09', NULL);

-- 첫 업무의 하위 업무: 부모 ID는 제목과 프로젝트 코드로 찾는다.
INSERT INTO task
    (project_id, parent_task_id, created_by, assignee_id, title, status, priority,
     start_date, due_date, created_at, updated_at, completed_at)
SELECT p.project_id, parent.task_id, u.user_id, u.user_id,
       '부서 인터뷰', 'done', 'medium', DATE '2026-07-02', DATE '2026-07-07',
       TIMESTAMPTZ '2026-07-01 10:00:00+09', TIMESTAMPTZ '2026-07-07 17:00:00+09',
       TIMESTAMPTZ '2026-07-07 17:00:00+09'
FROM project p
JOIN task parent ON parent.project_id = p.project_id AND parent.title = '요구사항 정리'
JOIN app_user u ON u.email = 'haneul.kim@example.test'
WHERE p.project_code = 'PORTAL';

INSERT INTO task_comment (task_id, author_id, content, created_at) VALUES
    ((SELECT task_id FROM task WHERE title = '화면 설계'),
     (SELECT user_id FROM app_user WHERE email = 'daon.jeong@example.test'),
     '첫 번째 시안을 금요일까지 공유해 주세요.', TIMESTAMPTZ '2026-07-20 10:00:00+09'),
    ((SELECT task_id FROM task WHERE title = '화면 설계'),
     (SELECT user_id FROM app_user WHERE email = 'garam.park@example.test'),
     '검색 화면 시안을 추가했습니다.', TIMESTAMPTZ '2026-07-24 16:30:00+09'),
    ((SELECT task_id FROM task WHERE title = '검색 API 구현'),
     (SELECT user_id FROM app_user WHERE email = 'haneul.kim@example.test'),
     '검색 사전 구성이 필요해 진행을 보류합니다.', TIMESTAMPTZ '2026-08-03 10:00:00+09'),
    ((SELECT task_id FROM task WHERE title = '복원 검증'),
     (SELECT user_id FROM app_user WHERE email = 'yeoreum.lee@example.test'),
     '표본 문서와 권한을 확인했습니다.', TIMESTAMPTZ '2026-05-27 14:00:00+09');

INSERT INTO task_history (task_id, changed_by, event_type, details, changed_at)
SELECT t.task_id, t.created_by, 'created',
       jsonb_build_object('status', t.status), t.created_at
FROM task t;

COMMIT;

SELECT 'department' AS table_name, count(*) AS row_count FROM department
UNION ALL SELECT 'app_user', count(*) FROM app_user
UNION ALL SELECT 'project', count(*) FROM project
UNION ALL SELECT 'project_member', count(*) FROM project_member
UNION ALL SELECT 'task', count(*) FROM task
UNION ALL SELECT 'task_comment', count(*) FROM task_comment
UNION ALL SELECT 'task_history', count(*) FROM task_history
ORDER BY table_name;
