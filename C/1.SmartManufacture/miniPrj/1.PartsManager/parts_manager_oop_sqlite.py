import sqlite3
from dataclasses import dataclass


@dataclass
class Product:
    id: int
    name: str
    quantity: int
    price: int


class ProductRepository:
    def __init__(self, db_name="products.db"):
        self.conn = sqlite3.connect(db_name)

    def create_table(self):
        sql = """
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            quantity INTEGER NOT NULL,
            price INTEGER NOT NULL
        );
        """
        self.conn.execute(sql)
        self.conn.commit()

    def insert_initial_data(self):
        products = [
            (1001, "타이어", 20, 120000),
            (1002, "와이퍼", 35, 15000),
            (1003, "엔진오일", 50, 35000),
            (1004, "배터리", 12, 90000),
            (1005, "브레이크패드", 25, 60000),
            (1006, "에어컨필터", 40, 18000),
            (1007, "전조등", 18, 45000),
        ]

        sql = """
        INSERT OR IGNORE INTO products(id, name, quantity, price)
        VALUES (?, ?, ?, ?);
        """

        self.conn.executemany(sql, products)
        self.conn.commit()

    def find_all(self):
        sql = """
        SELECT id, name, quantity, price
        FROM products
        ORDER BY id;
        """

        cursor = self.conn.execute(sql)

        products = []

        for row in cursor:
            product = Product(
                id=row[0],
                name=row[1],
                quantity=row[2],
                price=row[3]
            )
            products.append(product)

        return products

    def find_by_id(self, product_id):
        sql = """
        SELECT id, name, quantity, price
        FROM products
        WHERE id = ?;
        """

        cursor = self.conn.execute(sql, (product_id,))
        row = cursor.fetchone()

        if row is None:
            return None

        return Product(
            id=row[0],
            name=row[1],
            quantity=row[2],
            price=row[3]
        )

    def save(self, product):
        sql = """
        INSERT INTO products(id, name, quantity, price)
        VALUES (?, ?, ?, ?);
        """

        try:
            self.conn.execute(
                sql,
                (product.id, product.name, product.quantity, product.price)
            )
            self.conn.commit()
            return True

        except sqlite3.IntegrityError:
            return False

    def stock_in(self, product_id, amount):
        sql = """
        UPDATE products
        SET quantity = quantity + ?
        WHERE id = ?;
        """

        cursor = self.conn.execute(sql, (amount, product_id))
        self.conn.commit()

        return cursor.rowcount > 0

    def stock_out(self, product_id, amount):
        sql = """
        UPDATE products
        SET quantity = quantity - ?
        WHERE id = ? AND quantity >= ?;
        """

        cursor = self.conn.execute(sql, (amount, product_id, amount))
        self.conn.commit()

        return cursor.rowcount > 0

    def delete_by_id(self, product_id):
        sql = """
        DELETE FROM products
        WHERE id = ?;
        """

        cursor = self.conn.execute(sql, (product_id,))
        self.conn.commit()

        return cursor.rowcount > 0

    def close(self):
        self.conn.close()


class InventoryApp:
    def __init__(self):
        self.repository = ProductRepository()

    def print_menu(self):
        print("\n==== 자동차 부품 재고 관리 프로그램(SQLite + OOP) ====")
        print("1. 전체 재고 조회")
        print("2. 부품 검색")
        print("3. 입고 처리")
        print("4. 출고 처리")
        print("5. 신규 부품 등록")
        print("6. 부품 삭제")
        print("0. 종료")
        print("메뉴 선택: ", end="")

    def print_product(self, product):
        print(f"{product.id}\t{product.name:<15}\t{product.quantity}개\t{product.price}원")

    def print_header(self):
        print("\nID\t부품명\t\t수량\t가격")
        print("----------------------------------------")

    def print_all_products(self):
        products = self.repository.find_all()

        self.print_header()

        for product in products:
            self.print_product(product)

    def search_product(self):
        product_id = int(input("검색할 부품 ID 입력: "))

        product = self.repository.find_by_id(product_id)

        if product is None:
            print("해당 부품을 찾을 수 없습니다.")
        else:
            self.print_header()
            self.print_product(product)

    def stock_in(self):
        product_id = int(input("입고할 부품 ID 입력: "))
        amount = int(input("입고 수량 입력: "))

        if amount <= 0:
            print("입고 수량은 1개 이상이어야 합니다.")
            return

        result = self.repository.stock_in(product_id, amount)

        if result:
            print("입고 처리가 완료되었습니다.")
        else:
            print("해당 부품을 찾을 수 없습니다.")

    def stock_out(self):
        product_id = int(input("출고할 부품 ID 입력: "))
        amount = int(input("출고 수량 입력: "))

        if amount <= 0:
            print("출고 수량은 1개 이상이어야 합니다.")
            return

        result = self.repository.stock_out(product_id, amount)

        if result:
            print("출고 처리가 완료되었습니다.")
        else:
            print("해당 부품이 없거나 재고가 부족합니다.")

    def add_product(self):
        product = Product(
            id=int(input("신규 부품 ID 입력: ")),
            name=input("부품명 입력: "),
            quantity=int(input("재고 수량 입력: ")),
            price=int(input("가격 입력: "))
        )

        result = self.repository.save(product)

        if result:
            print("신규 부품이 등록되었습니다.")
        else:
            print("등록 실패: 이미 존재하는 ID일 수 있습니다.")

    def delete_product(self):
        product_id = int(input("삭제할 부품 ID 입력: "))

        result = self.repository.delete_by_id(product_id)

        if result:
            print("부품이 삭제되었습니다.")
        else:
            print("해당 부품을 찾을 수 없습니다.")

    def run(self):
        self.repository.create_table()
        self.repository.insert_initial_data()

        while True:
            self.print_menu()
            choice = input()

            if choice == "1":
                self.print_all_products()

            elif choice == "2":
                self.search_product()

            elif choice == "3":
                self.stock_in()

            elif choice == "4":
                self.stock_out()

            elif choice == "5":
                self.add_product()

            elif choice == "6":
                self.delete_product()

            elif choice == "0":
                print("프로그램을 종료합니다.")
                self.repository.close()
                break

            else:
                print("잘못된 메뉴입니다.")


app = InventoryApp()
app.run()
