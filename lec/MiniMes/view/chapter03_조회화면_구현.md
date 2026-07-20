# 02. 조회 화면 구현

## 연결되는 기존 교재

- GitHub Mini MES 교재: Chapter 5, Chapter 6, Chapter 7, Chapter 8, Chapter 10
- 기존 교재의 SQL 예제를 Streamlit 입력 위젯과 조회 결과 화면으로 연결한다.

## 학습 목표

- 검색어, 선택박스, 날짜 필터를 화면에 배치하는 방법
- 화면 입력값을 `queries.py` 함수로 전달하는 흐름
- 조회 결과와 KPI를 함께 표시하는 방법
- 선택한 행의 상세 정보를 보여주는 방법

## 구현 대상 화면

이 문서에서 다루는 화면은 세 개다.

| 화면 | 파일 | 핵심 기능 |
| --- | --- | --- |
| 품목 조회 | `pages/01_품목_조회.py` | 품목명 검색, 품목 유형 필터 |
| LOT 조회 | `pages/02_LOT_조회.py` | LOT 번호 검색, LOT 유형 필터, 품목 필터 |
| 생산실적 조회 | `pages/03_생산실적_조회.py` | 생산번호/품목/LOT 검색, 날짜 필터 |

## 품목 조회 화면

품목 조회 화면은 가장 단순한 조회 화면이다.

```python
keyword = st.text_input("품목 코드 또는 품목명 검색")
item_type = st.selectbox("품목 유형", ["전체", "PRODUCT", "MATERIAL"])

df = queries.items(keyword=keyword, item_type=item_type)
```

화면에서 입력받은 `keyword`와 `item_type`은 `queries.items()`로 전달된다.

조회 결과가 있으면 품목 수를 KPI로 보여준다.

```python
metric_row(
    [
        ("전체 품목", int(type_counts["item_count"].sum())),
        ("제품", count_map.get("PRODUCT", 0)),
        ("원자재", count_map.get("MATERIAL", 0)),
    ]
)
```

조회 결과 아래에는 선택한 품목의 상세 정보를 표시한다.

```python
selected_item = st.selectbox("상세 확인 품목", df["item_name"].tolist())
selected_row = df[df["item_name"] == selected_item].iloc[0]
```

이 코드는 DataFrame에서 선택한 품목명과 같은 행을 찾아 상세 표시용으로 사용한다.

## LOT 조회 화면

LOT 조회 화면은 필터가 세 개다.

```python
col1, col2, col3 = st.columns(3)
keyword = col1.text_input("LOT 번호 검색")
lot_type = col2.selectbox("LOT 유형", ["전체", "RECEIPT", "PRODUCTION"])
item_label = col3.selectbox("품목", list(item_options.keys()))
```

`st.columns(3)`을 사용하면 필터를 한 줄에 배치할 수 있다. 조회 조건이 많아질 때 화면을 더 읽기 쉽게 만든다.

품목 선택박스는 DB에서 읽어 온 품목 목록으로 만든다.

```python
items = queries.active_items_for_select()
item_options = {"전체": None}
for item in items:
    item_options[f"{item['item_code']} | {item['item_name']}"] = item["item_id"]
```

화면에 보이는 값은 사람이 읽기 쉬운 문자열이고, 실제 쿼리에 전달하는 값은 `item_id`다.

## 생산실적 조회 화면

생산실적 조회 화면은 날짜 필터를 선택적으로 사용한다.

```python
use_date_filter = col2.checkbox("생산일자 필터 사용")
date_from = None
date_to = None
if use_date_filter:
    date_from = col2.date_input("시작일")
    date_to = col3.date_input("종료일")
```

체크박스를 선택하지 않으면 날짜 조건을 사용하지 않는다. 이 패턴은 실무 화면에서도 자주 사용된다.

조회 결과에서 생산번호를 선택하면 해당 생산의 원자재 투입 이력을 추가로 조회한다.

```python
selected_no = st.selectbox("원자재 투입 확인 생산번호", df["production_no"].tolist())
production_id = int(df[df["production_no"] == selected_no].iloc[0]["production_id"])
show_dataframe(queries.production_materials(production_id))
```

한 화면 안에서 다음 흐름을 보여주는 구조다.

```text
생산실적 목록 조회
-> 생산번호 선택
-> 선택한 생산의 원자재 투입 이력 조회
```

## 조회 화면 구현 패턴

세 조회 화면은 같은 패턴을 따른다.

```text
1. 페이지 제목과 학습 목표 표시
2. 검색 조건 입력
3. queries.py 함수 호출
4. KPI 표시
5. DataFrame 표시
6. 선택한 행 상세 표시
7. 관계 설명 표시
```

이 패턴을 기억하면 새로운 조회 화면을 쉽게 만들 수 있다.

## 실습

### 실습 1. 품목 조회에 비활성 품목 필터 추가

`item` 테이블에는 `is_active` 컬럼이 있다. 품목 조회 화면에 사용 여부 필터를 추가해 본다.

```text
전체 / Y / N
```

필터를 추가하려면 `pages/01_품목_조회.py`와 `src/queries.py`의 `items()` 함수를 함께 수정해야 한다.

### 실습 2. LOT 조회에 수량 조건 추가

LOT 조회 화면에 최소 수량 입력칸을 추가한다.

```python
min_qty = st.number_input("최소 수량", min_value=0.0, value=0.0)
```

그 다음 `queries.lots()`에 조건을 전달하도록 수정한다.

### 실습 3. 생산실적 조회 결과 정렬 바꾸기

현재 생산실적 조회는 생산일자와 생산번호 기준으로 정렬된다. 최신 생산일자가 위로 오도록 `ORDER BY`를 바꿔 본다.

## 정리

조회 화면은 Streamlit 입력 위젯과 SQL 조회 함수를 연결하는 구조다. 화면 파일은 사용자의 입력과 결과 표시를 담당하고, 실제 SQL은 `src/queries.py`에 둔다.
