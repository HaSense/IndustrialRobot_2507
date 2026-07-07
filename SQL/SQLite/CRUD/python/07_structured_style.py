import sqlite3

DB_PATH = "/home/smart/work/dbfiles/test1.db"


# DB 연결을 생성하고 외래키 제약조건을 활성화한다.
def get_conn():
    conn = sqlite3.connect(DB_PATH)
    conn.execute("PRAGMA foreign_keys = ON")
    return conn


# 테이블을 생성하고 초기 샘플 데이터를 입력한다.
def init_db():
    conn = get_conn()

    conn.executescript("""
    -- 기존 테이블 삭제 (자식 테이블부터 삭제)
    DROP TABLE IF EXISTS order_items;
    DROP TABLE IF EXISTS orders;
    DROP TABLE IF EXISTS customers;
    
                       CREATE TABLE IF NOT EXISTS customers
    (
        customer_id INTEGER PRIMARY KEY,
        customer_name TEXT NOT NULL,
        email TEXT UNIQUE,
        is_active INTEGER NOT NULL DEFAULT 1,
        deleted_at TEXT
    );

    CREATE TABLE IF NOT EXISTS orders
    (
        order_id INTEGER PRIMARY KEY,
        customer_id INTEGER NOT NULL,
        order_date TEXT DEFAULT CURRENT_DATE,
        order_status TEXT NOT NULL DEFAULT 'planned',

        FOREIGN KEY(customer_id)
            REFERENCES customers(customer_id)
            ON DELETE RESTRICT
    );

    CREATE TABLE IF NOT EXISTS order_items
    (
        order_item_id INTEGER PRIMARY KEY,
        order_id INTEGER NOT NULL,
        product_name TEXT NOT NULL,
        quantity INTEGER NOT NULL CHECK(quantity > 0),
        price INTEGER NOT NULL CHECK(price >= 0),
        item_status TEXT NOT NULL DEFAULT 'active',

        FOREIGN KEY(order_id)
            REFERENCES orders(order_id)
            ON DELETE RESTRICT
    );

    INSERT OR IGNORE INTO customers VALUES
    (1, 'Kim', 'kim@test.com', 1, NULL),
    (2, 'Lee', 'lee@test.com', 1, NULL),
    (3, 'Park', 'park@test.com', 1, NULL);

    INSERT OR IGNORE INTO orders VALUES
    (1, 1, '2026-07-01', 'planned'),
    (2, 1, '2026-07-02', 'planned'),
    (3, 2, '2026-07-03', 'planned');

    INSERT OR IGNORE INTO order_items VALUES
    (1, 1, 'Keyboard', 2, 50000, 'active'),
    (2, 1, 'Mouse', 1, 30000, 'active'),
    (3, 2, 'Monitor', 1, 250000, 'active'),
    (4, 3, 'USB', 5, 10000, 'active');
    """)

    conn.commit()
    conn.close()


# 등록된 고객 목록을 조회한다.
def show_customers():
    conn = get_conn()
    cur = conn.cursor()

    cur.execute("""
    SELECT customer_id, customer_name, email, is_active, deleted_at
    FROM customers
    ORDER BY customer_id
    """)

    rows = cur.fetchall()

    print("\n[고객 목록]")
    for row in rows:
        print(row)

    conn.close()


# 고객과 연결된 주문 목록을 조회한다.
def show_orders():
    conn = get_conn()
    cur = conn.cursor()

    cur.execute("""
    SELECT 
        o.order_id,
        c.customer_name,
        o.order_date,
        o.order_status
    FROM orders o
    JOIN customers c
        ON o.customer_id = c.customer_id
    ORDER BY o.order_id
    """)

    rows = cur.fetchall()

    print("\n[주문 목록]")
    for row in rows:
        print(row)

    conn.close()


# 주문 상세 목록과 금액 합계를 조회한다.
def show_order_items():
    conn = get_conn()
    cur = conn.cursor()

    cur.execute("""
    SELECT
        oi.order_item_id,
        oi.order_id,
        c.customer_name,
        oi.product_name,
        oi.quantity,
        oi.price,
        oi.quantity * oi.price AS total_price,
        oi.item_status
    FROM order_items oi
    JOIN orders o
        ON oi.order_id = o.order_id
    JOIN customers c
        ON o.customer_id = c.customer_id
    ORDER BY oi.order_item_id
    """)

    rows = cur.fetchall()

    print("\n[주문 상세 목록]")
    for row in rows:
        print(row)

    conn.close()


# 신규 고객을 등록한다.
def add_customer():
    customer_id = int(input("고객 ID: "))
    customer_name = input("고객명: ")
    email = input("이메일: ")

    conn = get_conn()
    cur = conn.cursor()

    try:
        cur.execute("""
        INSERT INTO customers(customer_id, customer_name, email)
        VALUES (?, ?, ?)
        """, (customer_id, customer_name, email))

        conn.commit()
        print("고객 등록 완료")

    except sqlite3.Error as e:
        print("고객 등록 실패:", e)

    finally:
        conn.close()


# 신규 주문을 등록한다.
def add_order():
    order_id = int(input("주문 ID: "))
    customer_id = int(input("고객 ID: "))
    order_date = input("주문일자 예: 2026-07-07: ")

    conn = get_conn()
    cur = conn.cursor()

    try:
        cur.execute("""
        INSERT INTO orders(order_id, customer_id, order_date)
        VALUES (?, ?, ?)
        """, (order_id, customer_id, order_date))

        conn.commit()
        print("주문 등록 완료")

    except sqlite3.Error as e:
        print("주문 등록 실패:", e)

    finally:
        conn.close()


# 주문 상세 품목을 등록한다.
def add_order_item():
    order_item_id = int(input("주문상세 ID: "))
    order_id = int(input("주문 ID: "))
    product_name = input("상품명: ")
    quantity = int(input("수량: "))
    price = int(input("가격: "))

    conn = get_conn()
    cur = conn.cursor()

    try:
        cur.execute("""
        INSERT INTO order_items(order_item_id, order_id, product_name, quantity, price)
        VALUES (?, ?, ?, ?, ?)
        """, (order_item_id, order_id, product_name, quantity, price))

        conn.commit()
        print("주문 상세 등록 완료")

    except sqlite3.Error as e:
        print("주문 상세 등록 실패:", e)

    finally:
        conn.close()


# 고객 정보를 수정한다.
def update_customer():
    customer_id = int(input("수정할 고객 ID: "))
    customer_name = input("새 고객명: ")
    email = input("새 이메일: ")

    conn = get_conn()
    cur = conn.cursor()

    cur.execute("""
    UPDATE customers
    SET customer_name = ?, email = ?
    WHERE customer_id = ?
    """, (customer_name, email, customer_id))

    conn.commit()

    if cur.rowcount == 0:
        print("수정할 고객이 없습니다.")
    else:
        print("고객 수정 완료")

    conn.close()


# 고객을 실제 삭제하지 않고 비활성 상태로 변경한다.
def delete_customer():
    customer_id = int(input("비활성화할 고객 ID: "))

    conn = get_conn()
    cur = conn.cursor()

    try:
        cur.execute("""
        UPDATE customers
        SET is_active = 0,
            deleted_at = CURRENT_TIMESTAMP
        WHERE customer_id = ?
        """, (customer_id,))

        conn.commit()

        if cur.rowcount == 0:
            print("비활성화할 고객이 없습니다.")
        else:
            print("고객 비활성화 완료")
            print("스마트팩토리/MES에서는 이력 보존을 위해 실제 삭제보다 상태 변경을 사용합니다.")

    except sqlite3.Error as e:
        print("고객 비활성화 실패:", e)

    finally:
        conn.close()


# 주문을 실제 삭제하지 않고 취소 상태로 변경한다.
def delete_order():
    order_id = int(input("취소할 주문 ID: "))

    conn = get_conn()
    cur = conn.cursor()

    try:
        cur.execute("""
        UPDATE orders
        SET order_status = 'cancelled'
        WHERE order_id = ?
        """, (order_id,))

        conn.commit()

        if cur.rowcount == 0:
            print("취소할 주문이 없습니다.")
        else:
            print("주문 취소 완료")
            print("주문 상세 데이터는 이력 보존을 위해 삭제하지 않습니다.")

    except sqlite3.Error as e:
        print("주문 취소 실패:", e)

    finally:
        conn.close()


# 메뉴를 출력한다.
def menu():
    print("""
==============================
스마트팩토리 주문 관리 프로그램
==============================
1. 고객 조회
2. 주문 조회
3. 주문 상세 조회
4. 고객 등록
5. 주문 등록
6. 주문 상세 등록
7. 고객 수정
8. 고객 비활성화
9. 주문 취소
0. 종료
==============================
""")


# 프로그램의 전체 실행 흐름을 제어한다.
def main():
    init_db()

    while True:
        menu()
        choice = input("메뉴 선택: ")

        if choice == "1":
            show_customers()
        elif choice == "2":
            show_orders()
        elif choice == "3":
            show_order_items()
        elif choice == "4":
            add_customer()
        elif choice == "5":
            add_order()
        elif choice == "6":
            add_order_item()
        elif choice == "7":
            update_customer()
        elif choice == "8":
            delete_customer()
        elif choice == "9":
            delete_order()
        elif choice == "0":
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 메뉴입니다.")


if __name__ == "__main__":
    main()
