import sqlite3

conn = None

try:
    # student.db 열기 (없으면 자동 생성)
    conn = sqlite3.connect("/home/smart/work/dbfiles/test1.db")

    # SQL 실행을 위한 커서 생성
    cursor = conn.cursor()

    # student 테이블 생성
    # 외래키 활성화
    conn.execute("PRAGMA foreign_keys = ON")

    # 테이블 생성
    conn.executescript("""
        CREATE TABLE IF NOT EXISTS Customers
        (
            customer_id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            email TEXT UNIQUE
        );

    CREATE TABLE IF NOT EXISTS Orders
    (
        order_id INTEGER PRIMARY KEY,
        customer_id INTEGER NOT NULL,
        order_date TEXT DEFAULT CURRENT_DATE,

        FOREIGN KEY(customer_id)
            REFERENCES Customers(customer_id)
            ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS Order_Items
    (
        item_id INTEGER PRIMARY KEY,
        order_id INTEGER NOT NULL,
        product_name TEXT,
        quantity INTEGER,
        price INTEGER,

        FOREIGN KEY(order_id)
            REFERENCES Orders(order_id)
            ON DELETE CASCADE
    );
    
    INSERT INTO Customers VALUES
    (1,'Kim','kim@test.com'),
    (2,'Lee','lee@test.com'),
    (3,'Park','park@test.com');
    
    INSERT INTO Orders VALUES
    (1,1,'2026-07-01'),
    (2,1,'2026-07-02'),
    (3,2,'2026-07-03');
    
    INSERT INTO Order_Items VALUES
    (1,1,'Keyboard',2,50000),
    (2,1,'Mouse',1,30000),
    (3,2,'Monitor',1,250000),
    (4,3,'USB',5,10000);
    """)

    print("테이블 생성 완료!")
    
    # 변경사항 저장
    conn.commit()

    print("테이블 생성 완료")

except sqlite3.Error as e:
    print("테이블 생성 실패 :", e)

finally:
    # 데이터베이스 닫기
    if conn:
        conn.close()
