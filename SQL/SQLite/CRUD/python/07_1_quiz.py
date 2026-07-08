import sqlite3

conn = None

# 메뉴를 출력한다.
def menu_print():
    print("""
==============================
친구 관리 프로그램
==============================
1. 친구 리스트 조회
2. 친구추가
3. 친구삭제
4. 이름변경
9. 종료
==============================
""")
def init_db():
    try:
        # buddy_track.db 열기 (없으면 자동 생성)
        conn = sqlite3.connect("buddy_track.db")

        # SQL 실행을 위한 커서 생성
        cursor = conn.cursor()

        # student 테이블 생성
        # 외래키 활성화
        conn.execute('PRAGMA foreign_keys = ON')

        # 테이블 생성
        conn.executescript("""
        DROP TABLE IF EXISTS buddys;
                                            
        CREATE TABLE buddys
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        );
        
        INSERT INTO buddys(name) VALUES
        ('홍길동'),
        ('이순신'),
        ('강감찬');
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

def show_buddys():
    print('친구 리스트 조회')
    conn = sqlite3.connect('buddy_track.db')
    cur = conn.cursor()

    cur.execute("""
    SELECT id, name FROM buddys ORDER BY id
    """)

    rows = cur.fetchall()

    print("\n[친구 목록]")
    for row in rows:
        print(row)

    conn.close()

def add_buddys():
    print('### 친구추가 ###')
    print('이름을 입력하시오 : ', end='')
    name = input()
    try:
        conn = sqlite3.connect("buddy_track.db")
        cur = conn.cursor()
        cur.execute('''
            INSERT INTO buddys(name) VALUES (?)
        ''', (name,))

        conn.commit()
    except sqlite3.Error as e:
        print('sql 동작 실패:', e)
    finally:
        conn.close()

def delete_buddys():
    print('### 친구삭제 ###')
    print('삭제할 id를 입력하시오 : ', end='')
    id = input()
    conn = sqlite3.connect("buddy_track.db")
    cur = conn.cursor()

    try:
        cur.execute('''
            DELETE FROM buddys WHERE ID = (?)
        ''', (id,))
        conn.commit()
    except sqlite3.Error as e:
        print('sql 동작 실패:', e)
    finally:
        conn.close()

def change_buddys():
    print('### 이름변경 ###')
    print('수정할 id를 입력하시오 : ', end='')
    id = input()
    print('수정할 이름을 입력하시오 : ', end='')
    name = input()
    conn = sqlite3.connect("buddy_track.db")
    cur = conn.cursor()

    try:
        cur.execute('''
            UPDATE buddys
            SET NAME = ?
            WHERE id = ?
        ''', (name, id))
        conn.commit()
    except sqlite3.Error as e:
        print('sql 동작 실패:', e)
    finally:
        conn.close()


if __name__ == "__main__":
    
    init_db()

    while True:
        menu_print()
        choice = input('메뉴 선택:')

        if choice == "1":
            show_buddys()
        elif choice == "2":
            add_buddys()
        elif choice == "3":
            delete_buddys()
        elif choice == "4":
            change_buddys()
        elif choice == "9":
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 메뉴입니다.")




