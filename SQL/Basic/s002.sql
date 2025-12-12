-- 검색
SELECT * FROM Person;

INSERT INTO Person
VALUES (2, '강호동', '2000-08-08' );

-- 삽입
INSERT INTO Person (ID, Name, Birthday)
VALUES (3, '유재석', '1999-01-01')

-- 오르차순 생일컬럼 기준 조회
SELECT Name, Birthday FROM Person
ORDER BY Birthday ASC;

-- 삭제
DELETE FROM Person
WHERE ID = 1;

-- 업데이트
UPDATE Person SET Name = '김혜리'
WHERE Name = '이혜리'


-- 검색
SELECT * FROM Person;

INSERT INTO Person
VALUES (2, '강호동', '2000-08-08' );

-- 삽입
INSERT INTO Person (ID, Name, Birthday)
VALUES (3, '유재석', '1999-01-01')

-- 오르차순 생일컬럼 기준 조회
SELECT Name, Birthday FROM Person
ORDER BY Birthday ASC;

-- 삭제
DELETE FROM Person
WHERE ID = 1;

-- 업데이트
UPDATE Person SET Name = '김혜리'
WHERE Name = '이혜리'

--Quiz 연습
  
INSERT INTO Addressbook
VALUES (10, '홍길동', '010-1111-1111');

INSERT INTO Addressbook
VALUES (20, '이순신', '010-2222-2222');

INSERT INTO Addressbook
VALUES (30, '강감찬', '010-3333-3333');

UPDATE Addressbook SET ID=50
WHERE ID=10;

DELETE FROM Addressbook 
WHERE ID = 30

DELETE FROM Addressbook 
WHERE Name = '강감찬'

