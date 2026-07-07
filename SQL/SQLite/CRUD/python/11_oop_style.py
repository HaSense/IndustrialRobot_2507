import sqlite3

DB_PATH = "/home/smart/work/dbfiles/test1.db"


class Database:
    # DB 연결 정보와 초기화 작업을 관리한다.
    def __init__(self, db_path):
        self.db_path = db_path

    # DB 연결을 생성하고 외래키 제약조건을 활성화한다.
    def connect(self):
        conn = sqlite3.connect(self.db_path)
        conn.execute("PRAGMA foreign_keys = ON")
        return conn

    # 기존 테이블을 삭제하고 새 테이블과 샘플 데이터를 생성한다.
    def init_db(self):
        with self.connect() as conn:
            conn.execute("PRAGMA foreign_keys = OFF")

            conn.executescript("""
            DROP TABLE IF EXISTS order_items;
            DROP TABLE IF EXISTS orders;
            DROP TABLE IF EXISTS customers;
            """)

            conn.execute("PRAGMA foreign_keys = ON")

            conn.executescript("""
            CREATE TABLE customers
            (
                customer_id INTEGER PRIMARY KEY,
                customer_name TEXT NOT NULL,
                email TEXT UNIQUE,
                is_active INTEGER NOT NULL DEFAULT 1,
                deleted_at TEXT
            );

            CREATE TABLE orders
            (
                order_id INTEGER PRIMARY KEY,
                customer_id INTEGER NOT NULL,
                order_date TEXT DEFAULT CURRENT_DATE,
                order_status TEXT NOT NULL DEFAULT 'planned',

                FOREIGN KEY(customer_id)
                    REFERENCES customers(customer_id)
                    ON DELETE RESTRICT
            );

            CREATE TABLE order_items
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

            INSERT INTO customers VALUES
            (1, 'Kim', 'kim@test.com', 1, NULL),
            (2, 'Lee', 'lee@test.com', 1, NULL),
            (3, 'Park', 'park@test.com', 1, NULL);

            INSERT INTO orders VALUES
            (1, 1, '2026-07-01', 'planned'),
            (2, 1, '2026-07-02', 'planned'),
            (3, 2, '2026-07-03', 'planned');

            INSERT INTO order_items VALUES
            (1, 1, 'Keyboard', 2, 50000, 'active'),
            (2, 1, 'Mouse', 1, 30000, 'active'),
            (3, 2, 'Monitor', 1, 250000, 'active'),
            (4, 3, 'USB', 5, 10000, 'active');
            """)


class OrderRepository:
    # 고객, 주문, 주문상세 관련 SQL을 처리한다.
    def __init__(self, db):
        self.db = db

    # 등록된 고객 목록을 조회한다.
    def find_customers(self):
        with self.db.connect() as conn:
            cur = conn.execute("""
            SELECT customer_id, customer_name, email, is_active, deleted_at
            FROM customers
            ORDER BY customer_id
            """)
            return cur.fetchall()

    # 고객과 연결된 주문 목록을 조회한다.
    def find_orders(self):
        with self.db.connect() as conn:
            cur = conn.execute("""
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
            return cur.fetchall()

    # 주문 상세 목록과 금액 합계를 조회한다.
    def find_order_items(self):
        with self.db.connect() as conn:
            cur = conn.execute("""
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
            return cur.fetchall()

    # 신규 고객을 등록한다.
    def add_customer(self, customer_id, customer_name, email):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO customers(customer_id, customer_name, email)
            VALUES (?, ?, ?)
            """, (customer_id, customer_name, email))

    # 신규 주문을 등록한다.
    def add_order(self, order_id, customer_id, order_date):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO orders(order_id, customer_id, order_date)
            VALUES (?, ?, ?)
            """, (order_id, customer_id, order_date))

    # 주문 상세 품목을 등록한다.
    def add_order_item(self, order_item_id, order_id, product_name, quantity, price):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO order_items(
                order_item_id, order_id, product_name, quantity, price
            )
            VALUES (?, ?, ?, ?, ?)
            """, (order_item_id, order_id, product_name, quantity, price))

    # 고객 정보를 수정한다.
    def update_customer(self, customer_id, customer_name, email):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE customers
            SET customer_name = ?, email = ?
            WHERE customer_id = ?
            """, (customer_name, email, customer_id))
            return cur.rowcount

    # 고객을 실제 삭제하지 않고 비활성 상태로 변경한다.
    def deactivate_customer(self, customer_id):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE customers
            SET is_active = 0,
                deleted_at = CURRENT_TIMESTAMP
            WHERE customer_id = ?
            """, (customer_id,))
            return cur.rowcount

    # 주문을 실제 삭제하지 않고 취소 상태로 변경한다.
    def cancel_order(self, order_id):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE orders
            SET order_status = 'cancelled'
            WHERE order_id = ?
            """, (order_id,))
            return cur.rowcount


class OrderApp:
    # 메뉴 기반 콘솔 프로그램의 실행 흐름을 관리한다.
    def __init__(self, repository):
        self.repository = repository

    # 메뉴를 출력한다.
    def print_menu(self):
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
    def run(self):
        while True:
            self.print_menu()
            choice = input("메뉴 선택: ")

            try:
                if choice == "1":
                    self.show_customers()
                elif choice == "2":
                    self.show_orders()
                elif choice == "3":
                    self.show_order_items()
                elif choice == "4":
                    self.create_customer()
                elif choice == "5":
                    self.create_order()
                elif choice == "6":
                    self.create_order_item()
                elif choice == "7":
                    self.modify_customer()
                elif choice == "8":
                    self.remove_customer()
                elif choice == "9":
                    self.remove_order()
                elif choice == "0":
                    print("프로그램을 종료합니다.")
                    break
                else:
                    print("잘못된 메뉴입니다.")

            except ValueError:
                print("입력 오류: 숫자를 입력해야 합니다.")

            except sqlite3.IntegrityError as e:
                print("무결성 오류:", e)

            except sqlite3.Error as e:
                print("데이터베이스 오류:", e)

    # 고객 목록을 출력한다.
    def show_customers(self):
        rows = self.repository.find_customers()

        print("\n[고객 목록]")
        for row in rows:
            print(row)

    # 주문 목록을 출력한다.
    def show_orders(self):
        rows = self.repository.find_orders()

        print("\n[주문 목록]")
        for row in rows:
            print(row)

    # 주문 상세 목록을 출력한다.
    def show_order_items(self):
        rows = self.repository.find_order_items()

        print("\n[주문 상세 목록]")
        for row in rows:
            print(row)

    # 고객 등록 입력을 받아 처리한다.
    def create_customer(self):
        customer_id = int(input("고객 ID: "))
        customer_name = input("고객명: ")
        email = input("이메일: ")

        self.repository.add_customer(customer_id, customer_name, email)
        print("고객 등록 완료")

    # 주문 등록 입력을 받아 처리한다.
    def create_order(self):
        order_id = int(input("주문 ID: "))
        customer_id = int(input("고객 ID: "))
        order_date = input("주문일자 예: 2026-07-07: ")

        self.repository.add_order(order_id, customer_id, order_date)
        print("주문 등록 완료")

    # 주문 상세 등록 입력을 받아 처리한다.
    def create_order_item(self):
        order_item_id = int(input("주문상세 ID: "))
        order_id = int(input("주문 ID: "))
        product_name = input("상품명: ")
        quantity = int(input("수량: "))
        price = int(input("가격: "))

        self.repository.add_order_item(
            order_item_id,
            order_id,
            product_name,
            quantity,
            price
        )

        print("주문 상세 등록 완료")

    # 고객 수정 입력을 받아 처리한다.
    def modify_customer(self):
        customer_id = int(input("수정할 고객 ID: "))
        customer_name = input("새 고객명: ")
        email = input("새 이메일: ")

        count = self.repository.update_customer(
            customer_id,
            customer_name,
            email
        )

        if count == 0:
            print("수정할 고객이 없습니다.")
        else:
            print("고객 수정 완료")

    # 고객 비활성화 입력을 받아 처리한다.
    def remove_customer(self):
        customer_id = int(input("비활성화할 고객 ID: "))

        count = self.repository.deactivate_customer(customer_id)

        if count == 0:
            print("비활성화할 고객이 없습니다.")
        else:
            print("고객 비활성화 완료")
            print("이력 보존을 위해 실제 삭제하지 않았습니다.")

    # 주문 취소 입력을 받아 처리한다.
    def remove_order(self):
        order_id = int(input("취소할 주문 ID: "))

        count = self.repository.cancel_order(order_id)

        if count == 0:
            print("취소할 주문이 없습니다.")
        else:
            print("주문 취소 완료")
            print("주문 상세 데이터는 이력 보존을 위해 삭제하지 않습니다.")


def main():
    db = Database(DB_PATH)
    db.init_db()

    repository = OrderRepository(db)
    app = OrderApp(repository)
    app.run()


if __name__ == "__main__":
    main()
