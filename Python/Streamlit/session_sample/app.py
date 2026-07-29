from __future__ import annotations

import hashlib
import hmac
import os
import sqlite3
from datetime import datetime
from pathlib import Path

import pandas as pd
import streamlit as st


BASE_DIR = Path(__file__).resolve().parent
DB_PATH = BASE_DIR / "sql" / "sample.db"
ITERATIONS = 120_000
PRODUCTS = ["노트북", "마우스", "키보드", "모니터"]


def get_connection() -> sqlite3.Connection:
    """SQLite DB 연결을 만들고 결과 row를 컬럼명으로 읽을 수 있게 설정한다."""
    # 앱이 처음 실행되는 환경에서도 DB 폴더가 자동으로 준비되도록 한다.
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def hash_password(password: str, salt: bytes | None = None) -> tuple[str, str]:
    """입력받은 비밀번호를 salt와 함께 해시로 변환한다."""
    # 비밀번호는 평문 저장하지 않고, 사용자별 salt를 섞은 PBKDF2 해시로 저장한다.
    salt = salt or os.urandom(16)
    password_hash = hashlib.pbkdf2_hmac(
        "sha256",
        password.encode("utf-8"),
        salt,
        ITERATIONS,
    )
    return salt.hex(), password_hash.hex()


def verify_password(password: str, salt_hex: str, password_hash_hex: str) -> bool:
    """로그인 입력 비밀번호가 DB에 저장된 해시와 일치하는지 검사한다."""
    salt = bytes.fromhex(salt_hex)
    _, checked_hash = hash_password(password, salt)
    # compare_digest는 문자열 비교 시간 차이로 인한 추측 공격을 줄여준다.
    return hmac.compare_digest(checked_hash, password_hash_hex)


def init_db() -> None:
    """앱 실행에 필요한 테이블과 교육용 기본 계정을 준비한다."""
    with get_connection() as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL UNIQUE,
                password_salt TEXT NOT NULL,
                password_hash TEXT NOT NULL,
                display_name TEXT NOT NULL,
                created_at TEXT NOT NULL
            )
            """
        )
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS saved_cart_items (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER NOT NULL,
                product TEXT NOT NULL,
                saved_at TEXT NOT NULL,
                FOREIGN KEY (user_id) REFERENCES users (id)
            )
            """
        )
        # 교육용 샘플이므로 실행 직후 바로 로그인해 볼 수 있는 기본 계정을 만든다.
        create_user(
            conn,
            username="admin",
            password="admin1234",
            display_name="관리자",
            ignore_duplicate=True,
        )


def create_user(
    conn: sqlite3.Connection,
    username: str,
    password: str,
    display_name: str,
    ignore_duplicate: bool = False,
) -> bool:
    """회원가입 또는 기본 계정 생성을 위해 users 테이블에 사용자를 추가한다."""
    salt, password_hash = hash_password(password)
    sql = "INSERT OR IGNORE INTO users" if ignore_duplicate else "INSERT INTO users"
    cursor = conn.execute(
        f"""
        {sql} (username, password_salt, password_hash, display_name, created_at)
        VALUES (?, ?, ?, ?, ?)
        """,
        (
            username.strip(),
            salt,
            password_hash,
            display_name.strip(),
            datetime.now().isoformat(timespec="seconds"),
        ),
    )
    return cursor.rowcount == 1


def save_cart_to_db(user_id: int, products: list[str]) -> None:
    """세션 장바구니에 있던 상품 목록을 saved_cart_items 테이블에 저장한다."""
    saved_at = datetime.now().isoformat(timespec="seconds")
    rows = [(user_id, product, saved_at) for product in products]

    with get_connection() as conn:
        conn.executemany(
            """
            INSERT INTO saved_cart_items (user_id, product, saved_at)
            VALUES (?, ?, ?)
            """,
            rows,
        )


def saved_cart_dataframe(user_id: int) -> pd.DataFrame:
    """DB에 저장된 장바구니 항목을 pandas DataFrame으로 조회한다."""
    with get_connection() as conn:
        return pd.read_sql_query(
            """
            SELECT
                id AS ID,
                product AS 상품명,
                saved_at AS 저장시각
            FROM saved_cart_items
            WHERE user_id = ?
            ORDER BY id DESC
            """,
            conn,
            params=(user_id,),
        )


def delete_saved_cart_item(user_id: int, item_id: int) -> None:
    """현재 사용자에게 속한 DB 장바구니 항목 하나를 삭제한다."""
    with get_connection() as conn:
        conn.execute(
            "DELETE FROM saved_cart_items WHERE id = ? AND user_id = ?",
            (item_id, user_id),
        )


def find_user(username: str) -> sqlite3.Row | None:
    """아이디로 사용자 계정 정보를 DB에서 조회한다."""
    with get_connection() as conn:
        return conn.execute(
            """
            SELECT id, username, password_salt, password_hash, display_name, created_at
            FROM users
            WHERE username = ?
            """,
            (username.strip(),),
        ).fetchone()


def login(username: str, password: str) -> bool:
    """아이디와 비밀번호를 확인하고 로그인 성공 시 세션 상태를 초기화한다."""
    user = find_user(username)
    if user is None:
        return False

    if not verify_password(password, user["password_salt"], user["password_hash"]):
        return False

    # Streamlit은 요청마다 스크립트를 다시 실행하므로 로그인 상태는 session_state에 둔다.
    # 이 값들은 현재 브라우저 세션에만 유지되고, DB에는 저장되지 않는다.
    st.session_state.user = {
        "id": user["id"],
        "username": user["username"],
        "display_name": user["display_name"],
    }
    st.session_state.login_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    st.session_state.page_views = 0
    st.session_state.button_clicks = 0
    st.session_state.page_reset_requested = False
    st.session_state.cart = []
    st.session_state.cart_message = ""
    st.session_state.display_mode = "기본"
    st.session_state.memo = ""
    return True


def logout() -> None:
    """로그인 사용자와 세션 실습용 임시값을 모두 제거한다."""
    st.session_state.pop("user", None)
    st.session_state.pop("login_time", None)
    st.session_state.pop("page_views", None)
    st.session_state.pop("button_clicks", None)
    st.session_state.pop("page_reset_requested", None)
    st.session_state.pop("cart", None)
    st.session_state.pop("cart_message", None)
    st.session_state.pop("display_mode", None)
    st.session_state.pop("memo", None)


def ensure_session_defaults() -> None:
    """기존 세션에 새 키가 없을 때 기본값을 채워 KeyError를 방지한다."""
    st.session_state.setdefault("page_views", 0)
    st.session_state.setdefault("button_clicks", 0)
    st.session_state.setdefault("page_reset_requested", False)
    st.session_state.setdefault("cart", [])
    st.session_state.setdefault("cart_message", "")
    st.session_state.setdefault("display_mode", "기본")
    st.session_state.setdefault("memo", "")


def update_page_views() -> None:
    """Streamlit 스크립트가 다시 실행된 횟수를 세션에 기록한다."""
    # 버튼 콜백에서 바로 0으로 바꾸면 다음 rerun에서 다시 1 증가하므로 플래그로 처리한다.
    if st.session_state.pop("page_reset_requested", False):
        st.session_state.page_views = 0
        return

    st.session_state.page_views = st.session_state.get("page_views", 0) + 1


def increment_button_clicks() -> None:
    """카운터 증가 버튼의 콜백으로 세션 카운터를 1 증가시킨다."""
    st.session_state.button_clicks += 1


def reset_button_clicks() -> None:
    """카운터 초기화 버튼의 콜백으로 세션 카운터를 0으로 만든다."""
    st.session_state.button_clicks = 0


def request_page_view_reset() -> None:
    """화면 실행 횟수를 다음 rerun에서 0으로 만들도록 요청한다."""
    st.session_state.page_reset_requested = True


def add_to_cart(product: str) -> None:
    """상품 담기 버튼의 콜백으로 상품을 세션 장바구니에 추가한다."""
    st.session_state.cart.append(product)
    st.session_state.cart_message = f"{product}을 세션 장바구니에 담았습니다."


def clear_cart() -> None:
    """DB에 저장하지 않고 현재 세션 장바구니만 비운다."""
    st.session_state.cart = []
    st.session_state.cart_message = "세션 장바구니를 비웠습니다. DB에는 저장되지 않았습니다."


def delete_saved_cart_item_from_ui(item_id: int) -> None:
    """화면 버튼에서 호출되어 DB 항목을 삭제하고 사용자 메시지를 남긴다."""
    delete_saved_cart_item(st.session_state.user["id"], item_id)
    st.session_state.cart_message = f"DB 저장 항목 ID {item_id}를 삭제했습니다."


def save_session_cart() -> None:
    """현재 세션 장바구니를 DB에 저장한 뒤 세션 장바구니를 비운다."""
    if not st.session_state.cart:
        return

    item_count = len(st.session_state.cart)
    save_cart_to_db(st.session_state.user["id"], st.session_state.cart)
    st.session_state.cart = []
    st.session_state.cart_message = f"{item_count}개 항목을 DB에 저장했습니다."


def render_login_tab() -> None:
    """로그인 폼을 그리고 제출 시 인증을 수행한다."""
    with st.form("login_form"):
        username = st.text_input("아이디", placeholder="admin")
        password = st.text_input("비밀번호", type="password", placeholder="admin1234")
        submitted = st.form_submit_button("로그인", width="stretch")

    if submitted:
        if login(username, password):
            st.success("로그인되었습니다.")
            st.rerun()
        else:
            st.error("아이디 또는 비밀번호가 올바르지 않습니다.")


def render_signup_tab() -> None:
    """회원가입 폼을 그리고 입력값 검증 후 사용자를 DB에 추가한다."""
    with st.form("signup_form"):
        username = st.text_input("새 아이디")
        display_name = st.text_input("이름")
        password = st.text_input("새 비밀번호", type="password")
        password_confirm = st.text_input("비밀번호 확인", type="password")
        submitted = st.form_submit_button("회원가입", width="stretch")

    if not submitted:
        return

    if not username.strip() or not display_name.strip() or not password:
        st.error("아이디, 이름, 비밀번호를 모두 입력하세요.")
        return

    if len(password) < 8:
        st.error("비밀번호는 8자 이상으로 입력하세요.")
        return

    if password != password_confirm:
        st.error("비밀번호 확인이 일치하지 않습니다.")
        return

    try:
        with get_connection() as conn:
            create_user(conn, username, password, display_name)
    except sqlite3.IntegrityError:
        st.error("이미 사용 중인 아이디입니다.")
        return

    st.success("회원가입이 완료되었습니다. 로그인 탭에서 로그인하세요.")


def render_cart_persistence_demo() -> None:
    """세션 장바구니가 DB 저장 데이터로 바뀌는 과정을 보여주는 실습 화면이다."""
    user_id = st.session_state.user["id"]

    st.divider()
    st.subheader("세션 장바구니와 DB 저장 실습")
    st.write("상품을 담으면 먼저 세션 장바구니에만 들어갑니다. `DB에 저장`을 눌러야 SQLite에 저장됩니다.")

    product_cols = st.columns(len(PRODUCTS))
    for col, product in zip(product_cols, PRODUCTS):
        col.button(
            f"{product} 담기",
            key=f"add_{product}",
            width="stretch",
            on_click=add_to_cart,
            args=(product,),
        )

    if st.session_state.cart:
        st.write("현재 세션 장바구니:", ", ".join(st.session_state.cart))
    else:
        st.info("세션 장바구니가 비어 있습니다. 상품을 담아도 DB 저장 전까지는 임시 상태입니다.")

    if st.session_state.cart_message:
        st.caption(st.session_state.cart_message)

    save_col, cancel_col = st.columns(2)
    save_col.button(
        "DB에 저장",
        width="stretch",
        disabled=not st.session_state.cart,
        on_click=save_session_cart,
    )
    cancel_col.button(
        "세션 장바구니 취소",
        width="stretch",
        disabled=not st.session_state.cart,
        on_click=clear_cart,
    )

    saved_cart_df = saved_cart_dataframe(user_id)
    st.markdown("**DB에 저장된 장바구니 내역**")
    st.dataframe(saved_cart_df, width="stretch", hide_index=True)

    if saved_cart_df.empty:
        st.info("DB에 저장된 장바구니 내역이 없습니다.")
        return

    delete_col, button_col = st.columns([2, 1])
    item_ids = saved_cart_df["ID"].tolist()
    selected_item_id = delete_col.selectbox("삭제할 저장 항목 ID", item_ids)

    button_col.button(
        "선택 항목 삭제",
        width="stretch",
        on_click=delete_saved_cart_item_from_ui,
        args=(int(selected_item_id),),
    )


def render_storage_comparison() -> None:
    """DB에 남는 값과 현재 세션에만 있는 값을 나란히 보여준다."""
    user = st.session_state.user
    db_user = find_user(user["username"])
    saved_cart_df = saved_cart_dataframe(user["id"])

    st.divider()
    st.subheader("DB 저장값과 세션 저장값 비교")

    db_col, session_col = st.columns(2)
    with db_col:
        st.markdown("**DB에 저장되는 값**")
        st.caption("앱을 다시 실행해도 남는 계정 정보입니다.")
        if db_user is not None:
            st.json(
                {
                    "id": db_user["id"],
                    "username": db_user["username"],
                    "display_name": db_user["display_name"],
                    "created_at": db_user["created_at"],
                    "saved_cart_item_count": len(saved_cart_df),
                },
                expanded=False,
            )

    with session_col:
        st.markdown("**세션에만 저장되는 값**")
        st.caption("현재 브라우저 세션 동안만 유지되는 임시 상태입니다.")
        st.json(
            {
                "login_time": st.session_state.login_time,
                "page_views": st.session_state.page_views,
                "manual_counter": st.session_state.button_clicks,
                "display_mode": st.session_state.display_mode,
                "cart": st.session_state.cart,
                "memo": st.session_state.memo,
            },
            expanded=False,
        )

    st.selectbox("화면 표시 모드", ["기본", "집중", "넓게"], key="display_mode")


def render_practice_page() -> None:
    """세션 값 조작과 장바구니 저장 실습 화면을 그린다."""
    user = st.session_state.user
    ensure_session_defaults()
    # Streamlit은 위젯 조작 때마다 전체 스크립트를 다시 실행하므로 이 값이 증가한다.
    update_page_views()

    st.subheader(f"{user['display_name']}님, 환영합니다.")
    st.write("로그인 후에만 볼 수 있는 예시 화면입니다.")

    col1, col2, col3, col4 = st.columns(4)
    col1.metric("사용자 ID", user["id"])
    col2.metric("아이디", user["username"])
    col3.metric("화면 실행 횟수", st.session_state.page_views)
    col4.metric("수동 카운터", st.session_state.button_clicks)

    st.divider()
    st.subheader("Session State 예시")

    st.write("아래 값들은 DB에 저장하지 않고 현재 브라우저 세션에서만 유지됩니다.")
    st.text_input("로그인 시각", value=st.session_state.login_time, disabled=True)

    st.text_area(
        "임시 메모",
        key="memo",
        height=120,
        placeholder="여기에 입력한 내용은 새로고침이나 버튼 클릭 후에도 세션 동안 유지됩니다.",
    )

    count_col, reset_col, page_reset_col = st.columns(3)

    # on_click 콜백을 쓰면 버튼 클릭으로 발생하는 기본 rerun만 사용한다.
    # 버튼 안에서 st.rerun()을 추가 호출하면 화면 실행 횟수가 두 번 증가할 수 있다.
    count_col.button(
        "카운터 증가",
        width="stretch",
        on_click=increment_button_clicks,
    )
    reset_col.button(
        "카운터 초기화",
        width="stretch",
        on_click=reset_button_clicks,
    )
    page_reset_col.button(
        "화면 실행 횟수 초기화",
        width="stretch",
        on_click=request_page_view_reset,
    )

    render_cart_persistence_demo()



def render_session_guide_page() -> None:
    """session_state와 DB 저장의 차이를 설명하는 읽기용 페이지를 그린다."""
    st.subheader("세션과 DB의 차이")
    st.write(
        "Streamlit은 위젯을 조작할 때마다 스크립트를 위에서 아래로 다시 실행합니다. "
        "`st.session_state`는 이 rerun 사이에서 현재 사용자의 임시 상태를 유지하기 위한 공간입니다."
    )

    st.markdown("**세션에 넣기 좋은 값**")
    st.write("로그인 사용자 ID, 화면 설정, 임시 입력값, 아직 저장하지 않은 장바구니처럼 현재 브라우저 세션에만 필요한 값입니다.")

    st.markdown("**DB에 저장해야 하는 값**")
    st.write("계정 정보, 사용자가 저장 버튼을 누른 데이터, 로그아웃 후에도 남아야 하는 업무 데이터입니다.")

    st.markdown("**주의할 값**")
    st.warning("비밀번호, 비밀번호 해시, salt, DB 비밀값은 세션 화면이나 JSON 출력에 넣지 않습니다.")

    st.markdown("**이 앱에서 확인할 수 있는 흐름**")
    st.code(
        "상품 담기 -> session_state.cart에만 저장\n"
        "세션 장바구니 취소 -> DB에 저장되지 않고 사라짐\n"
        "DB에 저장 -> saved_cart_items 테이블에 저장되고 DataFrame에 표시",
        language="text",
    )


def render_session_examples_page() -> None:
    """세션 기능을 기초부터 중급까지 단계별 코드 예제로 설명한다."""
    st.subheader("세션 단계별 예제")
    st.write("아래 예제는 이 앱에 들어간 세션 기능을 작은 단위부터 순서대로 나눈 것입니다.")

    st.markdown("**1단계. 세션 기본값 만들기**")
    st.write("Streamlit은 스크립트를 반복 실행하므로, 처음 접근하는 세션 키에는 기본값을 넣어둡니다.")
    st.code(
        "if 'count' not in st.session_state:\n"
        "    st.session_state.count = 0",
        language="python",
    )
    st.caption("이 앱에서는 `ensure_session_defaults()`가 이 역할을 합니다.")

    st.markdown("**2단계. 버튼으로 세션값 바꾸기**")
    st.write("버튼을 누르면 세션값을 바꾸고, 다음 화면 실행에서 변경된 값이 유지됩니다.")
    st.code(
        "def increment():\n"
        "    st.session_state.count += 1\n\n"
        "st.button('증가', on_click=increment)",
        language="python",
    )
    st.caption("이 앱에서는 `increment_button_clicks()`와 `reset_button_clicks()`가 같은 패턴입니다.")

    st.markdown("**3단계. 입력값을 세션에 보관하기**")
    st.write("위젯에 `key`를 주면 Streamlit이 해당 값을 session_state에 자동으로 연결합니다.")
    st.code(
        "st.text_area('임시 메모', key='memo')\n"
        "st.write(st.session_state.memo)",
        language="python",
    )
    st.caption("이 앱의 임시 메모는 DB 저장 없이 현재 세션에만 남습니다.")

    st.markdown("**4단계. 리스트 형태의 세션 장바구니**")
    st.write("여러 값을 누적해야 할 때는 리스트를 세션에 두고 버튼 콜백에서 append 합니다.")
    st.code(
        "def add_to_cart(product):\n"
        "    st.session_state.cart.append(product)\n\n"
        "st.button('노트북 담기', on_click=add_to_cart, args=('노트북',))",
        language="python",
    )
    st.caption("이 앱에서는 상품 담기 버튼들이 `st.session_state.cart`에만 먼저 저장합니다.")

    st.markdown("**5단계. 세션 취소와 DB 저장 구분하기**")
    st.write("세션 데이터는 저장 전 임시 상태입니다. 취소하면 세션만 비우고, 저장하면 DB에 넣은 뒤 세션을 비웁니다.")
    st.code(
        "def clear_cart():\n"
        "    st.session_state.cart = []\n\n"
        "def save_session_cart():\n"
        "    save_cart_to_db(user_id, st.session_state.cart)\n"
        "    st.session_state.cart = []",
        language="python",
    )
    st.caption("이 앱의 핵심 학습 지점입니다. 세션 데이터와 DB 데이터의 생명주기가 달라집니다.")

    st.markdown("**6단계. DB 데이터를 DataFrame으로 확인하기**")
    st.write("여기서는 DB 대신 세션 리스트를 pandas DataFrame으로 보여줍니다. 실제 DB 저장은 `실습` 메뉴의 장바구니 영역에서 확인합니다.")
    st.code(
        "df = pd.read_sql_query(sql, conn, params=(user_id,))\n"
        "st.dataframe(df, width='stretch')",
        language="python",
    )
    st.caption("이 앱에서는 `saved_cart_dataframe()`이 이 역할을 합니다.")

    st.markdown("**7단계. 로그인 상태도 세션으로 유지하기**")
    st.write("로그인 성공 후 최소한의 사용자 정보만 세션에 저장하고, 비밀번호나 해시는 넣지 않습니다.")
    st.code(
        "st.session_state.user = {\n"
        "    'id': user['id'],\n"
        "    'username': user['username'],\n"
        "    'display_name': user['display_name'],\n"
        "}",
        language="python",
    )
    st.warning("세션에 민감정보를 넣지 않는 습관이 중요합니다.")


def reset_step_demo_state() -> None:
    """단계별 실습 페이지에서 쓰는 demo_ 세션값만 초기화한다."""
    for key in [
        "demo_count",
        "demo_text",
        "demo_cart",
        "demo_saved_items",
        "demo_message",
    ]:
        st.session_state.pop(key, None)


def ensure_step_demo_defaults() -> None:
    """단계별 실습 페이지 전용 세션 키 기본값을 준비한다."""
    st.session_state.setdefault("demo_count", 0)
    st.session_state.setdefault("demo_text", "")
    st.session_state.setdefault("demo_cart", [])
    st.session_state.setdefault("demo_saved_items", [])
    st.session_state.setdefault("demo_message", "")


def increment_demo_count() -> None:
    """단계별 실습 카운터를 1 증가시킨다."""
    st.session_state.demo_count += 1


def reset_demo_count() -> None:
    """단계별 실습 카운터를 0으로 초기화한다."""
    st.session_state.demo_count = 0


def add_demo_cart(product: str) -> None:
    """단계별 실습 장바구니에 상품을 추가한다."""
    st.session_state.demo_cart.append(product)
    st.session_state.demo_message = f"{product}을 demo_cart에 담았습니다. 아직 DB 저장 예시는 아닙니다."


def clear_demo_cart() -> None:
    """단계별 실습 장바구니만 비운다."""
    st.session_state.demo_cart = []
    st.session_state.demo_message = "demo_cart를 비웠습니다."


def save_demo_cart() -> None:
    """단계별 실습 장바구니를 세션 저장 목록으로 옮긴다."""
    if not st.session_state.demo_cart:
        return

    saved_at = datetime.now().strftime("%H:%M:%S")
    st.session_state.demo_saved_items.extend(
        {"상품명": product, "저장시각": saved_at}
        for product in st.session_state.demo_cart
    )
    st.session_state.demo_message = f"{len(st.session_state.demo_cart)}개 항목을 세션 저장 목록으로 옮겼습니다."
    st.session_state.demo_cart = []


def clear_demo_saved_items() -> None:
    """단계별 실습의 세션 저장 목록을 비운다."""
    st.session_state.demo_saved_items = []
    st.session_state.demo_message = "세션 저장 목록을 비웠습니다."


def render_session_examples_lab_page() -> None:
    """단계별 예제를 직접 조작해 보는 실습 페이지를 그린다."""
    ensure_step_demo_defaults()

    st.subheader("단계별 예제 실습")
    st.write("이 페이지는 실제 DB를 건드리지 않고 `demo_`로 시작하는 세션 키만 사용합니다.")

    st.markdown("**1단계. 세션 기본값 확인**")
    st.json(
        {
            "demo_count": st.session_state.demo_count,
            "demo_text": st.session_state.demo_text,
            "demo_cart": st.session_state.demo_cart,
            "demo_saved_items": st.session_state.demo_saved_items,
        },
        expanded=False,
    )

    st.markdown("**2단계. 버튼으로 세션 카운터 변경**")
    st.metric("demo_count", st.session_state.demo_count)
    count_col, reset_col = st.columns(2)
    count_col.button("demo_count 증가", width="stretch", on_click=increment_demo_count)
    reset_col.button("demo_count 초기화", width="stretch", on_click=reset_demo_count)

    st.markdown("**3단계. 입력값을 세션에 보관**")
    st.text_input("demo_text", key="demo_text", placeholder="입력 후 메뉴를 이동해도 현재 세션 동안 유지됩니다.")
    st.caption(f"현재 demo_text: {st.session_state.demo_text or '(비어 있음)'}")

    st.markdown("**4단계. 리스트 형태의 세션 장바구니**")
    product_cols = st.columns(len(PRODUCTS))
    for col, product in zip(product_cols, PRODUCTS):
        col.button(
            f"{product} 담기",
            key=f"demo_add_{product}",
            width="stretch",
            on_click=add_demo_cart,
            args=(product,),
        )
    st.write("현재 demo_cart:", st.session_state.demo_cart)

    st.markdown("**5단계. 취소와 임시 저장 흐름 비교**")
    cancel_col, save_col = st.columns(2)
    cancel_col.button(
        "demo_cart 취소",
        width="stretch",
        disabled=not st.session_state.demo_cart,
        on_click=clear_demo_cart,
    )
    save_col.button(
        "세션 저장 목록으로 이동",
        width="stretch",
        disabled=not st.session_state.demo_cart,
        on_click=save_demo_cart,
    )
    if st.session_state.demo_message:
        st.caption(st.session_state.demo_message)

    st.markdown("**6단계. DataFrame으로 세션 저장 목록 확인**")
    st.write("여기서는 DB 대신 세션 리스트를 pandas DataFrame으로 보여줍니다. 실제 DB 저장은 `실습` 메뉴의 장바구니 영역에서 확인합니다.")
    demo_df = pd.DataFrame(st.session_state.demo_saved_items)
    st.dataframe(demo_df, width="stretch", hide_index=True)
    st.button(
        "세션 저장 목록 비우기",
        width="stretch",
        disabled=demo_df.empty,
        on_click=clear_demo_saved_items,
    )

    st.markdown("**7단계. 실습 세션값만 초기화**")
    st.button("단계별 실습 초기화", width="stretch", on_click=reset_step_demo_state)


def render_build_process_page() -> None:
    """현재 샘플 앱을 어떤 순서로 만들면 되는지 설명하는 페이지를 그린다."""
    st.subheader("프로그램 제작 과정")
    st.write(
        "이 샘플은 처음부터 모든 기능을 한 번에 만든 것이 아니라, "
        "로그인 -> 세션 상태 -> DB 저장 -> 화면 분리 순서로 확장하면 이해하기 쉽습니다."
    )

    st.markdown("**1. DB 기반 사용자 계정 준비**")
    st.write("`init_db()`, `create_user()`, `find_user()`로 사용자 테이블을 만들고 계정을 조회합니다.")
    st.code(
        "users 테이블 생성\n"
        "기본 계정 admin / admin1234 생성\n"
        "회원가입 시 users 테이블에 새 사용자 저장",
        language="text",
    )

    st.markdown("**2. 보이지 않는 DB 테이블 스키마 설계**")
    st.write("화면을 만들기 전에 어떤 데이터가 DB에 남아야 하는지 먼저 정하면 구현 순서가 명확해집니다.")
    st.code(
        """CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password_salt TEXT NOT NULL,
    password_hash TEXT NOT NULL,
    display_name TEXT NOT NULL,
    created_at TEXT NOT NULL
);

CREATE TABLE saved_cart_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    product TEXT NOT NULL,
    saved_at TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users (id)
);""",
        language="sql",
    )

    st.markdown("**3. 비밀번호는 해시로 저장**")
    st.write("`hash_password()`와 `verify_password()`가 평문 비밀번호 저장을 피하는 역할을 합니다.")
    st.warning("교육용이어도 비밀번호를 DB나 session_state에 평문으로 넣는 습관은 피해야 합니다.")

    st.markdown("**4. 로그인 성공 후 세션 초기화**")
    st.write("`login()`은 인증이 성공했을 때 `st.session_state.user`와 실습용 세션값을 준비합니다.")
    st.code(
        "st.session_state.user          # 로그인 사용자 정보\n"
        "st.session_state.cart          # 아직 DB에 저장하지 않은 장바구니\n"
        "st.session_state.memo          # 세션 동안만 유지되는 임시 메모\n"
        "st.session_state.page_views    # Streamlit rerun 이해용 카운터",
        language="python",
    )

    st.markdown("**5. 세션 키 설계**")
    st.write("세션 키는 DB 컬럼처럼 이름과 역할을 정해두면 화면이 커져도 헷갈리지 않습니다.")
    st.dataframe(
        pd.DataFrame(
            [
                {"키": "user", "역할": "로그인한 사용자 ID, 아이디, 표시 이름"},
                {"키": "login_time", "역할": "현재 세션에서 로그인한 시각"},
                {"키": "page_views", "역할": "Streamlit rerun 횟수 확인"},
                {"키": "button_clicks", "역할": "수동 카운터 실습값"},
                {"키": "cart", "역할": "아직 DB에 저장하지 않은 세션 장바구니"},
                {"키": "memo", "역할": "세션에만 남는 임시 메모"},
                {"키": "display_mode", "역할": "현재 세션의 화면 표시 설정"},
            ]
        ),
        width="stretch",
        hide_index=True,
    )

    st.markdown("**6. 버튼은 on_click 콜백으로 세션값 변경**")
    st.write("`add_to_cart()`, `clear_cart()`, `save_session_cart()` 같은 함수는 버튼 클릭 시 호출되는 콜백입니다.")
    st.info("버튼 내부에서 `st.rerun()`을 추가 호출하면 화면 실행 횟수가 예상보다 많이 증가할 수 있습니다.")

    st.markdown("**7. 세션 데이터를 DB 데이터로 전환**")
    st.write("상품 담기는 먼저 세션 장바구니에만 들어가고, `DB에 저장`을 눌렀을 때 `saved_cart_items` 테이블에 저장됩니다.")
    st.code(
        "상품 담기 -> session_state.cart\n"
        "세션 장바구니 취소 -> session_state.cart = []\n"
        "DB에 저장 -> saved_cart_items INSERT -> session_state.cart = []",
        language="text",
    )

    st.markdown("**8. pandas DataFrame으로 DB 확인**")
    st.write("`saved_cart_dataframe()`은 SQLite 데이터를 `pd.read_sql_query()`로 읽어 `st.dataframe()`에 표시합니다.")

    st.markdown("**9. 페이지가 길어지면 메뉴로 나누기**")
    st.write("현재 앱은 로그인 후 사이드바 메뉴로 실습, 설명, 단계별 예제, 단계별 실습, 제작 과정, 개발 지침, DB/세션 비교를 나누어 보여줍니다.")
    st.code(
        "render_practice_page()            # 직접 조작하는 실습\n"
        "render_session_guide_page()       # 세션 개념 설명\n"
        "render_build_process_page()       # 제작 순서 설명\n"
        "render_development_guide_page()   # 확장 지침\n"
        "render_storage_comparison()       # DB와 세션 상태 비교",
        language="python",
    )

def render_development_guide_page() -> None:
    """샘플 앱을 확장할 때의 구조와 주의점을 설명하는 페이지를 그린다."""
    st.subheader("개발 지침")

    st.markdown("**현재는 왜 한 파일인가**")
    st.write(
        "초보자 실습에서는 `streamlit run app.py` 하나로 실행되고, 위에서 아래로 읽을 수 있는 구조가 이해하기 쉽습니다. "
        "기능이 작을 때는 한 파일이 오히려 학습 비용을 줄입니다."
    )

    st.markdown("**기능이 커지면 나눌 파일 구조**")
    st.code(
        "app.py       # Streamlit 화면 흐름\n"
        "db.py        # SQLite 연결, 테이블 생성\n"
        "auth.py      # 비밀번호 해시, 로그인, 회원가입\n"
        "cart.py      # 장바구니 저장/조회/삭제\n"
        "session.py   # session_state 기본값과 콜백",
        language="text",
    )

    st.markdown("**조심할 부분**")
    st.write("- 버튼 안에서 `st.rerun()`을 남용하지 않습니다.")
    st.write("- DB 함수는 DB 작업만 하고, 화면 메시지는 UI 함수나 콜백에서 처리합니다.")
    st.write("- SQL은 문자열 조립 대신 `?` 파라미터를 사용합니다.")
    st.write("- 교육용 기본 계정 `admin / admin1234`는 실제 배포 전에 제거합니다.")
    st.write("- 중요한 권한 판단은 세션값만 믿지 말고 DB의 사용자/권한 정보를 다시 확인합니다.")


def render_authenticated_page() -> None:
    """로그인한 사용자의 사이드바 메뉴와 선택된 페이지를 그린다."""
    ensure_session_defaults()

    user = st.session_state.user
    st.sidebar.success(f"{user['display_name']}님 로그인 중")
    page = st.sidebar.radio(
        "메뉴",
        ["실습", "세션 설명", "단계별 예제", "단계별 실습", "제작 과정", "개발 지침", "DB/세션 비교"],
    )

    if st.sidebar.button("로그아웃", width="stretch"):
        logout()
        st.rerun()

    if page == "실습":
        render_practice_page()
    elif page == "세션 설명":
        render_session_guide_page()
    elif page == "단계별 예제":
        render_session_examples_page()
    elif page == "단계별 실습":
        render_session_examples_lab_page()
    elif page == "제작 과정":
        render_build_process_page()
    elif page == "개발 지침":
        render_development_guide_page()
    else:
        render_storage_comparison()


def main() -> None:
    """Streamlit 앱의 시작점으로 DB 초기화 후 로그인 여부에 따라 화면을 나눈다."""
    st.set_page_config(page_title="Streamlit 로그인 샘플", page_icon="🔐")
    init_db()

    st.title("Streamlit 로그인 샘플")
    st.caption(f"SQLite DB: {DB_PATH.relative_to(BASE_DIR)}")

    if "user" in st.session_state:
        render_authenticated_page()
        return

    login_tab, signup_tab = st.tabs(["로그인", "회원가입"])
    with login_tab:
        render_login_tab()
    with signup_tab:
        render_signup_tab()

    st.info("기본 계정: admin / admin1234")


if __name__ == "__main__":
    main()
