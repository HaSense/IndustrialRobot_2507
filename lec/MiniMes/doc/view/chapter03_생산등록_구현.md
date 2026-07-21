\# 03. 생산 등록 화면 구현



\## 연결되는 기존 교재



\- GitHub Mini MES 교재: Chapter 4, Chapter 8, Chapter 9

\- 기존 교재에서는 생산실적과 원자재 투입 이력의 의미를 설명한다.

\- 이 문서에서는 그 데이터를 Streamlit 입력 화면에서 저장하는 과정을 다룬다.



\## 학습 목표



\- `st.form()`으로 입력 화면을 만드는 방법

\- 제품 품목과 원자재 LOT를 선택하는 방법

\- 입력값을 검증하는 방법

\- 여러 테이블 INSERT를 하나의 트랜잭션으로 처리하는 방법



\## 구현 파일



생산 등록은 두 파일이 함께 담당한다.



| 파일 | 역할 |

| --- | --- |

| `pages/04\_생산\_등록.py` | 입력 화면 구성 |

| `src/services.py` | 입력 검증, INSERT 트랜잭션 처리 |



조회 화면과 달리 생산 등록은 DB를 변경한다. 그래서 화면 코드와 저장 로직을 분리하는 것이 중요하다.



\## 입력 화면 흐름



생산 등록 화면은 먼저 제품 품목과 원자재 LOT 목록을 DB에서 읽어 온다.



```python

products = queries.active\_items\_for\_select("PRODUCT")

material\_lots = queries.lots\_for\_select("RECEIPT")

```



제품 품목은 완제품으로 생산할 대상을 고르는 데 사용한다. 원자재 LOT는 생산에 투입할 LOT를 고르는 데 사용한다.



필요한 데이터가 없으면 등록을 진행할 수 없으므로 화면을 멈춘다.



```python

if not products or not material\_lots:

&#x20;   st.warning("생산 등록에 필요한 제품 품목 또는 원자재 LOT가 없습니다.")

&#x20;   st.stop()

```



\## `st.form()` 사용



생산 등록은 여러 입력값을 한 번에 저장해야 한다. 그래서 `st.form()`을 사용한다.



```python

with st.form("production\_form"):

&#x20;   product\_label = st.selectbox("생산할 완제품 품목", list(product\_options.keys()))

&#x20;   production\_date = st.date\_input("생산일자", value=date.today())

&#x20;   production\_no = st.text\_input("생산번호", value=f"PRD-{date.today().strftime('%Y%m%d')}-NEW")

&#x20;   output\_lot\_no = st.text\_input("생성할 완제품 LOT 번호", value=f"FG-NEW-{date.today().strftime('%Y%m%d')}-001")

&#x20;   qty = st.number\_input("생산수량", min\_value=0.0, value=1000.0, step=100.0)

&#x20;   expire\_date = st.date\_input("완제품 유효기한", value=date.today() + timedelta(days=180))



&#x20;   submitted = st.form\_submit\_button("생산실적 저장")

```



`st.form()` 안의 위젯은 저장 버튼을 눌렀을 때 한 번에 처리된다. 입력 중간마다 DB 저장 코드가 실행되지 않는다.



\## 원자재 LOT 다중 선택



원자재는 여러 LOT가 투입될 수 있으므로 `st.multiselect()`를 사용한다.



```python

selected\_material\_labels = st.multiselect(

&#x20;   "원자재 LOT 선택",

&#x20;   list(material\_options.keys()),

&#x20;   default=list(material\_options.keys())\[:3],

)

```



선택한 원자재 LOT마다 투입수량 입력칸을 만든다.



```python

for label in selected\_material\_labels:

&#x20;   lot = material\_options\[label]

&#x20;   used\_qty = st.number\_input(

&#x20;       f"{lot\['lot\_no']} 투입수량",

&#x20;       min\_value=0.0,

&#x20;       value=float(qty),

&#x20;       step=100.0,

&#x20;       key=f"material\_qty\_{lot\['lot\_id']}",

&#x20;   )

```



여기서 `key`를 지정하는 이유는 원자재 LOT마다 별도의 입력 상태를 유지하기 위해서다.



\## 저장 데이터 객체



저장 버튼을 누르면 화면 입력값을 `ProductionRegistration` 객체로 묶는다.



```python

data = ProductionRegistration(

&#x20;   product\_item\_id=product\_options\[product\_label],

&#x20;   output\_lot\_no=output\_lot\_no,

&#x20;   production\_no=production\_no,

&#x20;   production\_date=production\_date,

&#x20;   qty=qty,

&#x20;   expire\_date=expire\_date,

&#x20;   material\_rows=material\_rows,

)

```



화면에서 받은 여러 값을 하나의 객체로 묶으면 검증과 저장 함수의 인자가 단순해진다.



\## 입력 검증



`src/services.py`의 `validate\_registration()`은 저장 전에 입력값을 검사한다.



검사해야 하는 항목은 다음과 같다.



\- 생산번호가 비어 있지 않은가

\- 완제품 LOT 번호가 비어 있지 않은가

\- 생산수량이 0보다 큰가

\- 원자재 LOT를 하나 이상 선택했는가

\- 원자재 투입수량이 0보다 큰가

\- 생산번호가 이미 존재하지 않는가

\- 완제품 LOT 번호가 이미 존재하지 않는가



검증 실패 시 DB에 저장하지 않고 사용자에게 오류 메시지를 보여준다.



```python

except ValueError as exc:

&#x20;   st.error(str(exc))

```



\## 트랜잭션 저장



생산 등록은 세 테이블에 데이터를 저장한다.



```text

1\. lot 테이블에 완제품 LOT 생성

2\. production 테이블에 생산실적 생성

3\. production\_material 테이블에 원자재 투입 이력 여러 건 생성

```



이 세 작업은 하나의 트랜잭션으로 처리되어야 한다. 중간에 하나라도 실패하면 전체 저장이 취소되어야 데이터 관계가 깨지지 않는다.



`services.py`에서는 연결 객체의 트랜잭션을 사용해 INSERT를 처리한다.



```python

with get\_connection() as conn:

&#x20;   cursor = conn.cursor()

&#x20;   ...

&#x20;   cursor.execute(...)

&#x20;   cursor.execute(...)

&#x20;   cursor.execute(...)

```



`with` 블록 안에서 예외가 발생하면 커밋되지 않고 롤백된다.



\## 저장 후 화면 표시



저장이 성공하면 사용자가 무엇이 저장되었는지 확인할 수 있게 결과를 보여준다.



```python

st.success("생산실적이 정상적으로 등록되었습니다.")

st.write(result)

```



그리고 저장된 작업을 요약한다.



```text

1\. 완제품 LOT 1건 생성

2\. 생산실적 1건 생성

3\. 선택한 원자재 LOT별 투입 이력 생성

```



\## 중요한 제한



현재 스키마에는 재고 이동 이력 테이블이 없다. 따라서 생산 등록을 해도 원자재 LOT 수량은 자동 차감하지 않는다.



이 내용은 화면 하단에도 표시되어 있다.



```text

현재 스키마에는 재고 이동 테이블이 없으므로 원자재 LOT 수량 차감은 하지 않는다.

```



\## 실습



\### 실습 1. 중복 LOT 번호 확인



이미 존재하는 LOT 번호를 입력하고 저장한다. 오류 메시지가 표시되는지 확인한다.



\### 실습 2. 원자재 없이 저장 시도



원자재 LOT 선택을 모두 해제한 뒤 저장한다. 검증 오류가 발생하는지 확인한다.



\### 실습 3. 저장 후 조회 확인



새 생산실적을 저장한 뒤 다음 화면에서 데이터가 보이는지 확인한다.



1\. 생산실적 조회

2\. 생산이력

3\. 역방향 추적

4\. 생산현황



\### 실습 4. 상태값 변경



현재 생산 등록은 `COMPLETED` 상태로 저장한다. 화면에 상태 선택박스를 추가해 `PLANNED`, `COMPLETED`, `CANCELED` 중 하나를 저장하도록 수정해 본다.



\## 정리



생산 등록 화면은 단순 조회 화면보다 복잡하다. 여러 입력값을 받고, 검증하고, 세 테이블에 데이터를 저장한다. 이때 화면 코드는 입력과 결과 표시를 담당하고, 저장 로직은 `src/services.py`에 분리한다.



