
-- 테이블 만들기
CREATE TABLE "Person" (
	"ID"	INTEGER NOT NULL,
	"Name"	TEXT NOT NULL,
	"Birthday"	TEXT,
	PRIMARY KEY("ID" AUTOINCREMENT)
);

-- Student 테이블 만들기
CREATE TABLE "Student" (
	"SN"	INTEGER NOT NULL,
	"NAME"	TEXT NOT NULL,
	"ADDRESS"	TEXT NOT NULL,
	PRIMARY KEY("SN" AUTOINCREMENT)
);
-- 데이터 삽입
INSERT INTO Person (Name, Birthday) 
VALUES ('유재석', '1970-01-01')

INSERT INTO Person (Name, Birthday) 
VALUES ('강호동', '1965-12-01');

INSERT INTO Person (Name, Birthday) 
VALUES ('신동엽', '1960-09-30');

-- 조회/select
SELECT * FROM Person;

SELECT Name FROM Person ORDER BY Name DESC; -- 역순출력


-- 업데이트
UPDATE Person SET Name='강감찬' WHERE ID=2;

-- 삭제
DELETE FROM Person WHERE Name='신동엽';




--12/16 update.


pets 테이블에

----------------------------
1 뽀삐 강아지
2 나나 고양이
3 푸우 곰

INSERT INTO pets (Name, Animal)
VALUES 
('뽀삐', '강아지'),
('나나', '고양이'),
('푸우','곰');
-----------------------------------
Q1) id가 3, 2, 1 순으로 데이터를
출력해 보아라?(내림차순)
order by
Q2) 이름 순으로 오름차순 출력!
Q) DB 테이블의 구조를 보는 SQL.
desc [테이블]
?????

PRAGMA table_info(pets)



-- 집게함수
SELECT count(Height), 
	max(Height), 
	min(Height), 
	avg(Height),
	sum(Height) 
FROM Person;

-- 급여가 5천이상인 사람들의 부서별 평균급여
SELECT dept, ROUND(AVG(salary),2)
FROM employees
GROUP BY dept
HAVING AVG(salary) >=5000;

-- 직원이 4명 이상인 부서는?
SELECT dept, count(*) 
FROM employees
GROUP BY dept
HAVING count(*) > 3
;

-- 직무별 평균 급여가 6000 이상인 직무(job)는 어떻게 구하나??
SELECT job, AVG(salary) 
FROM employees
GROUP BY job
HAVING AVG(salary) >= 6000
;














