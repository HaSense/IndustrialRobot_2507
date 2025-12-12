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

