import sqlite3
from abc import ABC, abstractmethod

DB_PATH = "/home/smart/work/dbfiles/test1.db"


# =========================
# 1. Infrastructure
# =========================

class Database:
    def __init__(self, db_path):
        self.db_path = db_path

    def connect(self):
        conn = sqlite3.connect(self.db_path)
        conn.execute("PRAGMA foreign_keys = ON")
        return conn

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


# =========================
# 2. Repository Layer
# =========================

class OrderRepository:
    def __init__(self, db):
        self.db = db

    def find_customers(self):
        with self.db.connect() as conn:
            return conn.execute("""
            SELECT customer_id, customer_name, email, is_active, deleted_at
            FROM customers
            ORDER BY customer_id
            """).fetchall()

    def find_orders(self):
        with self.db.connect() as conn:
            return conn.execute("""
            SELECT 
                o.order_id,
                c.customer_name,
                o.order_date,
                o.order_status
            FROM orders o
            JOIN customers c ON o.customer_id = c.customer_id
            ORDER BY o.order_id
            """).fetchall()

    def find_order_items(self):
        with self.db.connect() as conn:
            return conn.execute("""
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
            JOIN orders o ON oi.order_id = o.order_id
            JOIN customers c ON o.customer_id = c.customer_id
            ORDER BY oi.order_item_id
            """).fetchall()

    def insert_customer(self, customer_id, name, email):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO customers(customer_id, customer_name, email)
            VALUES (?, ?, ?)
            """, (customer_id, name, email))

    def insert_order(self, order_id, customer_id, order_date):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO orders(order_id, customer_id, order_date)
            VALUES (?, ?, ?)
            """, (order_id, customer_id, order_date))

    def insert_order_item(self, item_id, order_id, product_name, quantity, price):
        with self.db.connect() as conn:
            conn.execute("""
            INSERT INTO order_items(
                order_item_id, order_id, product_name, quantity, price
            )
            VALUES (?, ?, ?, ?, ?)
            """, (item_id, order_id, product_name, quantity, price))

    def update_customer(self, customer_id, name, email):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE customers
            SET customer_name = ?, email = ?
            WHERE customer_id = ?
            """, (name, email, customer_id))
            return cur.rowcount

    def deactivate_customer(self, customer_id):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE customers
            SET is_active = 0,
                deleted_at = CURRENT_TIMESTAMP
            WHERE customer_id = ?
            """, (customer_id,))
            return cur.rowcount

    def cancel_order(self, order_id):
        with self.db.connect() as conn:
            cur = conn.execute("""
            UPDATE orders
            SET order_status = 'cancelled'
            WHERE order_id = ?
            """, (order_id,))
            return cur.rowcount


# =========================
# 3. Service Layer
# =========================

class OrderService:
    def __init__(self, repository):
        self.repository = repository

    def get_customers(self):
        return self.repository.find_customers()

    def get_orders(self):
        return self.repository.find_orders()

    def get_order_items(self):
        return self.repository.find_order_items()

    def create_customer(self, customer_id, name, email):
        if not name.strip():
            raise ValueError("고객명은 비어 있을 수 없습니다.")

        self.repository.insert_customer(customer_id, name, email)

    def create_order(self, order_id, customer_id, order_date):
        self.repository.insert_order(order_id, customer_id, order_date)

    def create_order_item(self, item_id, order_id, product_name, quantity, price):
        if quantity <= 0:
            raise ValueError("수량은 1 이상이어야 합니다.")

        if price < 0:
            raise ValueError("가격은 0 이상이어야 합니다.")

        self.repository.insert_order_item(
            item_id,
            order_id,
            product_name,
            quantity,
            price
        )

    def change_customer(self, customer_id, name, email):
        return self.repository.update_customer(customer_id, name, email)

    def remove_customer_safely(self, customer_id):
        return self.repository.deactivate_customer(customer_id)

    def cancel_order_safely(self, order_id):
        return self.repository.cancel_order(order_id)


# =========================
# 4. Console IO
# =========================

class ConsoleIO:
    def input_int(self, message):
        return int(input(message))

    def input_text(self, message):
        return input(message)

    def print_rows(self, title, rows):
        print(f"\n[{title}]")
        for row in rows:
            print(row)

    def print_message(self, message):
        print(message)


# =========================
# 5. Command Pattern
# =========================

class Command(ABC):
    @abstractmethod
    def execute(self):
        pass


class ShowCustomersCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        rows = self.service.get_customers()
        self.io.print_rows("고객 목록", rows)


class ShowOrdersCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        rows = self.service.get_orders()
        self.io.print_rows("주문 목록", rows)


class ShowOrderItemsCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        rows = self.service.get_order_items()
        self.io.print_rows("주문 상세 목록", rows)


class CreateCustomerCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        customer_id = self.io.input_int("고객 ID: ")
        name = self.io.input_text("고객명: ")
        email = self.io.input_text("이메일: ")

        self.service.create_customer(customer_id, name, email)
        self.io.print_message("고객 등록 완료")


class CreateOrderCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        order_id = self.io.input_int("주문 ID: ")
        customer_id = self.io.input_int("고객 ID: ")
        order_date = self.io.input_text("주문일자 예: 2026-07-07: ")

        self.service.create_order(order_id, customer_id, order_date)
        self.io.print_message("주문 등록 완료")


class CreateOrderItemCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        item_id = self.io.input_int("주문상세 ID: ")
        order_id = self.io.input_int("주문 ID: ")
        product_name = self.io.input_text("상품명: ")
        quantity = self.io.input_int("수량: ")
        price = self.io.input_int("가격: ")

        self.service.create_order_item(
            item_id,
            order_id,
            product_name,
            quantity,
            price
        )

        self.io.print_message("주문 상세 등록 완료")


class UpdateCustomerCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        customer_id = self.io.input_int("수정할 고객 ID: ")
        name = self.io.input_text("새 고객명: ")
        email = self.io.input_text("새 이메일: ")

        count = self.service.change_customer(customer_id, name, email)

        if count == 0:
            self.io.print_message("수정할 고객이 없습니다.")
        else:
            self.io.print_message("고객 수정 완료")


class DeactivateCustomerCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        customer_id = self.io.input_int("비활성화할 고객 ID: ")

        count = self.service.remove_customer_safely(customer_id)

        if count == 0:
            self.io.print_message("비활성화할 고객이 없습니다.")
        else:
            self.io.print_message("고객 비활성화 완료")
            self.io.print_message("이력 보존을 위해 실제 삭제하지 않았습니다.")


class CancelOrderCommand(Command):
    def __init__(self, service, io):
        self.service = service
        self.io = io

    def execute(self):
        order_id = self.io.input_int("취소할 주문 ID: ")

        count = self.service.cancel_order_safely(order_id)

        if count == 0:
            self.io.print_message("취소할 주문이 없습니다.")
        else:
            self.io.print_message("주문 취소 완료")
            self.io.print_message("주문 상세 데이터는 이력 보존을 위해 삭제하지 않습니다.")


# =========================
# 6. Application Layer
# =========================

class Menu:
    def __init__(self):
        self.commands = {}

    def add_command(self, menu_number, title, command):
        self.commands[menu_number] = {
            "title": title,
            "command": command
        }

    def print_menu(self):
        print("""
==============================
스마트팩토리 주문 관리 프로그램
==============================""")

        for number, item in self.commands.items():
            print(f"{number}. {item['title']}")

        print("0. 종료")
        print("==============================")

    def execute(self, menu_number):
        item = self.commands.get(menu_number)

        if item is None:
            print("잘못된 메뉴입니다.")
            return

        item["command"].execute()


class OrderApplication:
    def __init__(self, menu):
        self.menu = menu

    def run(self):
        while True:
            self.menu.print_menu()
            choice = input("메뉴 선택: ")

            if choice == "0":
                print("프로그램을 종료합니다.")
                break

            try:
                self.menu.execute(choice)

            except ValueError as e:
                print("입력 오류:", e)

            except sqlite3.IntegrityError as e:
                print("무결성 오류:", e)

            except sqlite3.Error as e:
                print("데이터베이스 오류:", e)


# =========================
# 7. Composition Root
# =========================

def create_app():
    db = Database(DB_PATH)
    db.init_db()

    repository = OrderRepository(db)
    service = OrderService(repository)
    io = ConsoleIO()

    menu = Menu()
    menu.add_command("1", "고객 조회", ShowCustomersCommand(service, io))
    menu.add_command("2", "주문 조회", ShowOrdersCommand(service, io))
    menu.add_command("3", "주문 상세 조회", ShowOrderItemsCommand(service, io))
    menu.add_command("4", "고객 등록", CreateCustomerCommand(service, io))
    menu.add_command("5", "주문 등록", CreateOrderCommand(service, io))
    menu.add_command("6", "주문 상세 등록", CreateOrderItemCommand(service, io))
    menu.add_command("7", "고객 수정", UpdateCustomerCommand(service, io))
    menu.add_command("8", "고객 비활성화", DeactivateCustomerCommand(service, io))
    menu.add_command("9", "주문 취소", CancelOrderCommand(service, io))

    return OrderApplication(menu)


def main():
    app = create_app()
    app.run()


if __name__ == "__main__":
    main()
