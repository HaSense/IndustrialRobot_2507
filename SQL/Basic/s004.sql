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


DROP TABLE IF EXISTS student;
DROP TABLE IF EXISTS course;

CREATE TABLE student (
    id    INTEGER PRIMARY KEY,
    name  TEXT NOT NULL,
    major TEXT NOT NULL,
    year  INTEGER NOT NULL   -- 학년
);

CREATE TABLE course (
    id       INTEGER PRIMARY KEY,
    title    TEXT NOT NULL,
    category TEXT NOT NULL,
    credit   INTEGER NOT NULL
);

INSERT INTO student (id, name, major, year) VALUES
(1, '김민수', '컴퓨터공학', 1),
(2, '이서연', '컴퓨터공학', 2),
(3, '박지훈', '컴퓨터공학', 3),
(4, '최유진', '컴퓨터공학', 4),

(5, '정우성', '전자공학', 1),
(6, '한지민', '전자공학', 2),
(7, '오세훈', '전자공학', 3),
(8, '신하늘', '전자공학', 4),

(9,  '장민지', '기계공학', 1),
(10, '임도윤', '기계공학', 2),
(11, '서준호', '기계공학', 3),
(12, '윤채원', '기계공학', 4),

(13, '강민혁', '산업공학', 1),
(14, '문지아', '산업공학', 2),
(15, '백승우', '산업공학', 3),
(16, '조수빈', '산업공학', 4),

(17, '홍준표', '컴퓨터공학', 2),
(18, '김하린', '전자공학', 3),
(19, '이도현', '기계공학', 4),
(20, '박소연', '산업공학', 1);



INSERT INTO course (id, title, category, credit) VALUES
(1,  'C 프로그래밍',        '프로그래밍', 3),
(2,  'C++ 객체지향',        '프로그래밍', 3),
(3,  'Python 기초',         '프로그래밍', 3),
(4,  '자료구조',            '프로그래밍', 3),

(5,  '데이터베이스 기초',   'DB',         2),
(6,  'SQL 활용',            'DB',         3),

(7,  '임베디드 시스템',     '임베디드',   3),
(8,  'ROS 입문',            '로봇',       3),

(9,  '머신러닝 기초',       'AI',         3),
(10, '산업공학 개론',       '이론',       2);


ALTER TABLE student ADD COLUMN major_code TEXT;
ALTER TABLE course  ADD COLUMN major_code TEXT;



UPDATE student
SET major_code =
    CASE major
        WHEN '컴퓨터공학' THEN 'CSE'
        WHEN '전자공학'   THEN 'EEE'
        WHEN '기계공학'   THEN 'ME'
        WHEN '산업공학'   THEN 'IE'
        ELSE 'GEN'
END;


UPDATE course
SET major_code =
    CASE
        WHEN title IN ('C 프로그래밍','C++ 객체지향','Python 기초','자료구조') THEN 'CSE'
        WHEN title IN ('임베디드 시스템') THEN 'EEE'
        WHEN title IN ('ROS 입문') THEN 'ME'
        WHEN title IN ('산업공학 개론') THEN 'IE'
        WHEN title IN ('데이터베이스 기초','SQL 활용','머신러닝 기초') THEN 'CSE'
        ELSE 'GEN'
END;

SELECT s.name, s.major, c.title, c.category
FROM student s
JOIN course c
ON s.major_code = c.major_code
ORDER BY s.id, c.id;




