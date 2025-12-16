CREATE TABLE employees (
    dept TEXT,
    job TEXT,
    salary INTEGER
);

INSERT INTO employees VALUES
('개발','백엔드',6000),
('개발','백엔드',6500),
('개발','프론트',5500),
('개발','프론트',5200),
('영업','영업사원',4000),
('영업','영업사원',4200),
('영업','팀장',7000),
('인사','인사담당',4500),
('인사','팀장',6000),
('인사','팀장',6200);



-- 3개 테이블 Join 설명을 위한 테이블 생성 및 데이터

DROP TABLE IF EXISTS enrollment;
DROP TABLE IF EXISTS student;
DROP TABLE IF EXISTS course;

CREATE TABLE student (
    id    INTEGER PRIMARY KEY,
    name  TEXT NOT NULL,
    major TEXT NOT NULL,
    year  INTEGER NOT NULL
);

CREATE TABLE course (
    id       INTEGER PRIMARY KEY,
    title    TEXT NOT NULL,
    category TEXT NOT NULL,
    credit   INTEGER NOT NULL
);

CREATE TABLE enrollment (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id INTEGER NOT NULL,
    course_id  INTEGER NOT NULL,
    semester   TEXT NOT NULL,
    grade      TEXT,

    FOREIGN KEY (student_id) REFERENCES student(id),
    FOREIGN KEY (course_id)  REFERENCES course(id),

    UNIQUE(student_id, course_id, semester)
);

-- 데이터 삽입
INSERT INTO student VALUES
(1, '김민수', '컴퓨터공학', 1),
(2, '이서연', '컴퓨터공학', 2),
(3, '박지훈', '컴퓨터공학', 3),
(4, '최유진', '컴퓨터공학', 4),

(5, '정우성', '전자공학', 1),
(6, '한지민', '전자공학', 2),
(7, '오세훈', '전자공학', 3),
(8, '신하늘', '전자공학', 4),

(9, '장민지', '기계공학', 1),
(10,'임도윤', '기계공학', 2),
(11,'서준호', '기계공학', 3),
(12,'윤채원', '기계공학', 4),

(13,'강민혁', '산업공학', 1),
(14,'문지아', '산업공학', 2),
(15,'백승우', '산업공학', 3),
(16,'조수빈', '산업공학', 4),

(17,'홍준표', '컴퓨터공학', 2),
(18,'김하린', '전자공학', 3),
(19,'이도현', '기계공학', 4),
(20,'박소연', '산업공학', 1);

INSERT INTO course VALUES
(1, 'C 프로그래밍',        '프로그래밍', 3),
(2, 'C++ 객체지향',        '프로그래밍', 3),
(3, 'Python 기초',         '프로그래밍', 3),
(4, '자료구조',            '프로그래밍', 3),

(5, '데이터베이스 기초',   'DB',         2),
(6, 'SQL 활용',            'DB',         3),

(7, '임베디드 시스템',     '임베디드',   3),
(8, 'ROS 입문',            '로봇',       3),

(9, '머신러닝 기초',       'AI',         3),
(10,'산업공학 개론',       '이론',       2);

INSERT INTO enrollment (student_id, course_id, semester, grade) VALUES
(1, 1, '2025-1', 'A0'),
(1, 5, '2025-1', 'B+'),

(2, 2, '2025-1', 'A+'),
(2, 6, '2025-1', 'A0'),

(3, 4, '2025-1', 'B0'),
(3, 9, '2025-1', 'A0'),

(4, 8, '2025-1', 'A0'),

(5, 7, '2025-1', 'A0'),
(6, 3, '2025-1', 'B+'),

(7, 6, '2025-1', 'A0'),
(8, 2, '2025-1', 'B+'),

(9, 10,'2025-1', 'A0'),
(10,5, '2025-1', 'B+'),

(11,9, '2025-1', 'A+'),
(12,4, '2025-1', 'B0'),

(13,6, '2025-1', 'A0'),
(14,1, '2025-1', 'B+'),

(15,2, '2025-1', 'A0'),
(16,8, '2025-1', 'A+'),

(17,9, '2025-1', 'B0'),
(18,7, '2025-1', 'A0'),

(19,4, '2025-1', 'B+'),
(20,10,'2025-1', 'A0');


-- 기본 join
SELECT s.name, c.title, e.grade
FROM enrollment e
JOIN student s ON e.student_id = s.id
JOIN course  c ON e.course_id  = c.id;

-- 특정 학생의 수강 과목
SELECT s.name, c.title, e.grade
FROM enrollment e
JOIN student s ON e.student_id = s.id
JOIN course  c ON e.course_id  = c.id
WHERE s.id = 1;

-- 특정 과목의 수강 학생
SELECT c.title, s.name, s.major
FROM enrollment e
JOIN course  c ON e.course_id  = c.id
JOIN student s ON e.student_id = s.id
WHERE c.id = 6;

-- 학과별 수강 건수
SELECT s.major, COUNT(*) AS enroll_count
FROM enrollment e
JOIN student s ON e.student_id = s.id
GROUP BY s.major;

-- 3학년 이상 학생 수강 목록
SELECT s.name, s.year, c.title
FROM enrollment e
JOIN student s ON e.student_id = s.id
JOIN course  c ON e.course_id  = c.id
WHERE s.year >= 3;

-- DB 계열 과목을 듣는 학생
SELECT DISTINCT s.name
FROM enrollment e
JOIN student s ON e.student_id = s.id
JOIN course  c ON e.course_id  = c.id
WHERE c.category = 'DB';


-- LEFT JOIN – 수강하지 않은 학생 찾기
SELECT s.id, s.name
FROM student s
LEFT JOIN enrollment e ON s.id = e.student_id
WHERE e.id IS NULL;

-- LEFT JOIN – 수강자가 없는 과목 찾기
SELECT c.id, c.title
FROM course c
LEFT JOIN enrollment e ON c.id = e.course_id
WHERE e.id IS NULL;


