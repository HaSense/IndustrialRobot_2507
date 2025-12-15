
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









