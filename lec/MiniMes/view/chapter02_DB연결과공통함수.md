# 01. DB 연결과 공통 함수 구현

## 연결되는 기존 교재

- GitHub Mini MES 교재: Chapter 4, Chapter 5
- 기존 교재에서는 SQLite 테이블 생성과 기본 SELECT를 다룬다.
- 이 문서에서는 Streamlit 화면에서 SQLite DB를 안정적으로 사용하는 코드를 확인한다.

## 학습 목표

- `sql/mini_mes.db`를 Python 파일 기준 경로로 찾는 방법
- SQLite 연결 시 `PRAGMA foreign_keys = ON`을 사용하는 이유
- 조회 결과를 Pandas DataFrame으로 바꾸는 흐름
- 화면 공통 UI 함수를 분리하는 이유

## DB 파일 경로

Streamlit 앱은 실행 위치에 따라 상대 경로가 달라질 수 있다. 그래서 DB 경로는 현재 Python 파일 위치를 기준으로 계산한다.

```python
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent.parent
DB_PATH = BASE_DIR / "sql" / "mini_mes.db"
```

이렇게 하면 터미널이 어느 위치에서 실행되더라도 프로젝트 안의 DB 파일을 찾을 수 있다.

## DB 연결 함수

`src/db.py`의 핵심 역할은 다음과 같다.

```python
def get_connection() -> sqlite3.Connection:
    if not database_exists():
        raise FileNotFoundError(f"데이터베이스 파일을 찾을 수 없습니다: {DB_PATH}")

    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA foreign_keys = ON")
    return conn
```

학생이 확인할 점은 세 가지다.

- DB 파일이 없으면 이해하기 쉬운 오류를 발생시킨다.
- `row_factory`를 지정해서 컬럼명으로 값을 읽을 수 있게 한다.
- 외래키 검사를 켜서 잘못된 관계의 데이터가 저장되지 않게 한다.

## DataFrame 조회 함수

조회 화면은 대부분 표를 보여준다. 그래서 SELECT 결과를 DataFrame으로 변환하는 함수가 필요하다.

```python
def fetch_dataframe(sql: str, params: tuple = ()) -> pd.DataFrame:
    with get_connection() as conn:
        return pd.read_sql_query(sql, conn, params=params)
```

화면 파일은 이 함수를 직접 호출하지 않고, 보통 `src/queries.py`의 함수들을 호출한다.

```text
화면 파일
-> queries.py 함수
-> db.py fetch_dataframe()
-> SQLite
```

## 공통 UI 함수

`src/ui.py`는 여러 화면에서 반복되는 코드를 모아 둔다.

| 함수 | 역할 |
| --- | --- |
| `setup_page()` | 페이지 설정과 기본 제목 |
| `page_title()` | 학습 목표, 관련 테이블, 작업 안내 표시 |
| `show_database_status()` | DB 파일 존재 여부 표시 |
| `show_dataframe()` | DataFrame이 비었을 때 경고 표시 |
| `metric_row()` | KPI 지표를 가로로 표시 |
| `row_to_dict()` | 단일 조회 결과를 화면 표시용 dict로 변환 |

## 빈 데이터 처리

조회 결과가 없을 때 빈 표만 나오면 학생은 오류인지 정상인지 알기 어렵다. 그래서 공통 출력 함수에서 안내 메시지를 보여준다.

```python
def show_dataframe(df: pd.DataFrame, empty_message: str = "조건에 해당하는 데이터가 없습니다."):
    if df.empty:
        st.warning(empty_message)
    else:
        st.dataframe(df, use_container_width=True)
```

이 패턴은 모든 조회 화면에서 반복해서 사용한다.

## 화면에서 공통 함수 사용하기

품목 조회 화면은 다음과 같이 공통 함수를 사용한다.

```python
setup_page("품목 조회")

page_title(
    "품목 조회",
    "품목 기준정보가 제품과 원자재를 함께 관리하는 방식을 학습합니다.",
    "item -> lot, item -> production_material",
    "품목 유형과 검색어로 품목을 조회하고 LOT 연결 건수를 확인합니다.",
)
```

각 화면마다 제목, 학습 내용, 관련 테이블, 수행할 작업을 같은 형식으로 보여줄 수 있다.

## 실습

### 실습 1. DB 경로 확인

`src/db.py`에서 `DB_PATH` 값을 화면에 출력해 본다.

```python
st.write(DB_PATH)
```

앱을 실행하고 실제 경로가 `sql/mini_mes.db`를 가리키는지 확인한다.

### 실습 2. 빈 결과 메시지 확인

품목 조회 화면에서 존재하지 않는 검색어를 입력한다.

```text
없는품목
```

`조건에 해당하는 데이터가 없습니다.` 메시지가 나오는지 확인한다.

### 실습 3. KPI 함수 재사용

다른 화면에서 `metric_row()`를 사용해 새로운 지표를 하나 추가해 본다.

예를 들어 LOT 조회 화면에 `LOT 유형 수`를 추가할 수 있다.

## 정리

DB 연결, 표 출력, KPI 표시 같은 코드는 여러 화면에서 반복된다. 이런 코드를 `src/db.py`와 `src/ui.py`로 분리하면 화면 파일은 학습 흐름에 집중할 수 있다.
