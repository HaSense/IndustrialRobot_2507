CREATE TABLE contents (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL,
    title TEXT NOT NULL,
    creator TEXT,
    rating INTEGER,
    memo TEXT,
    created_at TEXT DEFAULT (datetime('now','localtime'))
);

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('영화','인터스텔라','크리스토퍼 놀란',10,'시간과 우주를 다룬 최고의 SF 영화','2026-06-01 20:30:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('영화','탑건: 매버릭','조셉 코신스키',9,'비행 장면이 매우 인상적이었다.','2026-06-05 21:10:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('드라마','오징어 게임','황동혁',9,'긴장감이 뛰어나고 몰입감이 좋았다.','2026-06-08 22:15:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('드라마','미생','김원석',10,'직장인의 현실을 잘 표현한 작품이다.','2026-06-10 19:40:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('책','클린 코드','로버트 C. 마틴',10,'좋은 코드를 작성하기 위한 필독서이다.','2026-06-12 18:20:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('책','아토믹 해빗','제임스 클리어',9,'작은 습관의 힘을 이해하게 되었다.','2026-06-15 09:10:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('음악','Dynamite','BTS',8,'기분이 좋아지는 밝은 노래이다.','2026-06-18 08:50:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('음악','Bohemian Rhapsody','Queen',10,'들을 때마다 감탄하게 되는 명곡이다.','2026-06-20 23:05:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('영화','기생충','봉준호',10,'사회적 메시지가 강렬한 작품이다.','2026-06-22 20:00:00');

INSERT INTO contents(type, title, creator, rating, memo, created_at)
VALUES
('책','이펙티브 C++','스콧 마이어스',9,'C++ 개발자라면 꼭 읽어야 할 책이다.','2026-06-25 17:30:00');
