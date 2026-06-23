import sqlite3
from dataclasses import dataclass


@dataclass
class Product:
    id: int
    name: str
    quantity: int
    price: int


def connect_db():
    return sqlite3.connect("products.db")


def create_table(conn):
    sql = """
    CREATE TABLE IF NOT EXISTS products (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        quantity INTEGER NOT NULL,
        price INTEGER NOT NULL
    );
    """

    conn.execute(sql)
    conn.commit()


def insert_initial_data(conn):
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

    conn.executemany(sql, products)
    conn.commit()


def print_menu():
    print("\n==== 자동차 부품 재고 관리 프로그램(SQLite) ====")
    print("1. 전체 재고 조회")
    print("2. 부품 검색")
    print("3. 입고 처리")
    print("4. 출고 처리")
    print("5. 신규 부품 등록")
    print("6. 부품 삭제")
    print("0. 종료")
    print("메뉴 선택: ", end="")


def print_product(product):
    print(f"{product.id}\t{product.name:<15}\t{product.quantity}개\t{product.price}원")


def print_all(conn):
    sql = """
    SELECT id, name, quantity, price
    FROM products
    ORDER BY id;
    """

    cursor = conn.execute(sql)

    print("\nID\t부품명\t\t수량\t가격")
    print("----------------------------------------")

    for row in cursor:
        product = Product(
            id=row[0],
            name=row[1],
            quantity=row[2],
            price=row[3]
        )

        print_product(product)


def search_product(conn):
    product_id = int(input("검색할 부품 ID 입력: "))

    sql = """
    SELECT id, name, quantity, price
    FROM products
    WHERE id = ?;
    """

    cursor = conn.execute(sql, (product_id,))
    row = cursor.fetchone()

    if row is None:
        print("해당 부품을 찾을 수 없습니다.")
    else:
        product = Product(
            id=row[0],
            name=row[1],
            quantity=row[2],
            price=row[3]
        )

        print("\nID\t부품명\t\t수량\t가격")
        print("----------------------------------------")
        print_product(product)


def stock_in(conn):
    product_id = int(input("입고할 부품 ID 입력: "))
    amount = int(input("입고 수량 입력: "))

    if amount <= 0:
        print("입고 수량은 1개 이상이어야 합니다.")
        return

    sql = """
    UPDATE products
    SET quantity = quantity + ?
    WHERE id = ?;
    """

    cursor = conn.execute(sql, (amount, product_id))
    conn.commit()

    if cursor.rowcount == 0:
        print("해당 부품을 찾을 수 없습니다.")
    else:
        print("입고 처리가 완료되었습니다.")


def stock_out(conn):
    product_id = int(input("출고할 부품 ID 입력: "))
    amount = int(input("출고 수량 입력: "))

    if amount <= 0:
        print("출고 수량은 1개 이상이어야 합니다.")
        return

    sql = """
    UPDATE products
    SET quantity = quantity - ?
    WHERE id = ? AND quantity >= ?;
    """

    cursor = conn.execute(sql, (amount, product_id, amount))
    conn.commit()

    if cursor.rowcount == 0:
        print("해당 부품이 없거나 재고가 부족합니다.")
    else:
        print("출고 처리가 완료되었습니다.")


def add_product(conn):
    product = Product(
        id=int(input("신규 부품 ID 입력: ")),
        name=input("부품명 입력: "),
        quantity=int(input("재고 수량 입력: ")),
        price=int(input("가격 입력: "))
    )

    sql = """
    INSERT INTO products(id, name, quantity, price)
    VALUES (?, ?, ?, ?);
    """

    try:
        conn.execute(
            sql,
            (product.id, product.name, product.quantity, product.price)
        )
        conn.commit()
        print("신규 부품이 등록되었습니다.")

    except sqlite3.IntegrityError:
        print("등록 실패: 이미 존재하는 ID일 수 있습니다.")


def delete_product(conn):
    product_id = int(input("삭제할 부품 ID 입력: "))

    sql = """
    DELETE FROM products
    WHERE id = ?;
    """

    cursor = conn.execute(sql, (product_id,))
    conn.commit()

    if cursor.rowcount == 0:
        print("해당 부품을 찾을 수 없습니다.")
    else:
        print("부품이 삭제되었습니다.")


def main():
    conn = connect_db()

    create_table(conn)
    insert_initial_data(conn)

    while True:
        print_menu()
        choice = input()

        if choice == "1":
            print_all(conn)

        elif choice == "2":
            search_product(conn)

        elif choice == "3":
            stock_in(conn)

        elif choice == "4":
            stock_out(conn)

        elif choice == "5":
            add_product(conn)

        elif choice == "6":
            delete_product(conn)

        elif choice == "0":
            print("프로그램을 종료합니다.")
            break

        else:
            print("잘못된 메뉴입니다.")

    conn.close()


if __name__ == "__main__":
    main()
