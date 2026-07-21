\# 04. 생산이력 조회와 LOT 추적 화면 구현



\## 연결되는 기존 교재



\- GitHub Mini MES 교재: Chapter 9, Chapter 10, Chapter 12

\- 기존 교재에서는 생산실적, 원자재 투입 이력, 정방향/역방향 추적 SQL을 설명한다.

\- 이 문서에서는 그 SQL을 Streamlit 선택 화면으로 연결하는 방법을 다룬다.



\## 학습 목표



\- 생산실적 1건의 상세 정보를 조회하는 화면 흐름

\- 원자재 LOT에서 완제품 LOT를 찾는 정방향 추적 화면 구현

\- 완제품 LOT에서 원자재 LOT를 찾는 역방향 추적 화면 구현

\- 선택박스 값과 추적 SQL 함수를 연결하는 방법



\## 구현 대상 화면



| 화면 | 파일 | 핵심 함수 |

| --- | --- | --- |

| 생산이력 | `pages/05\_생산이력.py` | `production\_detail()`, `production\_materials()` |

| 정방향 추적 | `pages/06\_정방향\_추적.py` | `forward\_trace()` |

| 역방향 추적 | `pages/07\_역방향\_추적.py` | `reverse\_trace()` |



\## 생산이력 상세조회



생산이력 화면은 먼저 생산실적 목록을 조회한다.



```python

productions = queries.productions()

```



조회할 생산실적이 없으면 화면을 멈춘다.



```python

if productions.empty:

&#x20;   st.warning("조회할 생산실적이 없습니다.")

&#x20;   st.stop()

```



사용자가 생산번호를 선택하면 해당 행에서 `production\_id`를 찾는다.



```python

selected\_no = st.selectbox("생산번호", productions\["production\_no"].tolist())

production\_id = int(productions\[productions\["production\_no"] == selected\_no].iloc\[0]\["production\_id"])

```



그 다음 두 가지 정보를 조회한다.



```python

detail = row\_to\_dict(queries.production\_detail(production\_id))

materials = queries.production\_materials(production\_id)

```



화면에는 생산실적 기본정보와 투입 원자재 목록을 나누어 보여준다.



```text

생산실적 기본정보

\-> 생산번호, 생산일자, 제품, 완제품 LOT, 생산수량



투입 원자재

\-> 원자재 품목, 원자재 LOT, 투입수량

```



\## 정방향 LOT 추적



정방향 추적은 원자재 LOT에서 시작한다.



```text

원자재 LOT

\-> production\_material

\-> production

\-> 완제품 LOT

\-> 완제품 품목

```



화면에서는 원자재 입고 LOT만 선택할 수 있게 한다.



```python

material\_lots = queries.lots\_for\_select("RECEIPT")

```



선택박스에 표시할 문자열과 실제 쿼리에 전달할 `lot\_id`를 딕셔너리로 연결한다.



```python

options = {

&#x20;   f"{lot\['lot\_no']} | {lot\['item\_name']} | 보유 {lot\['qty']:,.0f}": lot\["lot\_id"]

&#x20;   for lot in material\_lots

}

selected\_label = st.selectbox("원자재 LOT", list(options.keys()))

```



선택된 LOT ID를 `forward\_trace()`에 전달한다.



```python

df = queries.forward\_trace(options\[selected\_label])

```



결과가 있으면 추적 경로를 텍스트로 보여준다.



```python

st.markdown(

&#x20;   f"""

&#x20;   `{first\['material\_lot\_no']}`

&#x20;   -> `production\_material`

&#x20;   -> 생산실적 {', '.join(df\['production\_no'].tolist())}

&#x20;   -> 완제품 LOT {', '.join(df\['output\_lot\_no'].tolist())}

&#x20;   """

)

```



이 화면은 원자재 LOT에 문제가 있을 때 영향을 받는 완제품 LOT를 찾는 용도다.



\## 역방향 LOT 추적



역방향 추적은 완제품 LOT에서 시작한다.



```text

완제품 LOT

\-> production

\-> production\_material

\-> 원자재 LOT

\-> 원자재 품목

```



화면에서는 생산 LOT만 선택할 수 있게 한다.



```python

output\_lots = queries.lots\_for\_select("PRODUCTION")

```



선택된 완제품 LOT ID를 `reverse\_trace()`에 전달한다.



```python

df = queries.reverse\_trace(options\[selected\_label])

```



결과가 있으면 어떤 생산실적을 거쳐 어떤 원자재 LOT가 사용되었는지 보여준다.



```python

st.markdown(

&#x20;   f"""

&#x20;   `{first\['output\_lot\_no']}`

&#x20;   -> 생산실적 `{first\['production\_no']}`

&#x20;   -> `production\_material`

&#x20;   -> 원자재 LOT {', '.join(df\['material\_lot\_no'].tolist())}

&#x20;   """

)

```



이 화면은 완제품 품질 문제가 생겼을 때 어떤 원자재 LOT를 확인해야 하는지 찾는 용도다.



\## 세 화면의 공통 구현 패턴



생산이력과 LOT 추적 화면은 같은 방식으로 구현된다.



```text

1\. 선택 가능한 목록 조회

2\. 선택박스 표시

3\. 선택한 값의 ID 찾기

4\. 상세 조회 또는 추적 SQL 실행

5\. 결과 표 표시

6\. 추적 경로 또는 관계 설명 표시

```



이 패턴을 이해하면 다른 추적 화면도 만들 수 있다.



\## 실습



\### 실습 1. 생산이력 화면에 총 투입수량 추가



`materials` DataFrame의 `qty` 합계를 계산해 KPI로 표시한다.



```python

materials\["qty"].sum()

```



\### 실습 2. 정방향 추적 결과에 생산수량 합계 표시



원자재 LOT 하나가 여러 생산에 사용될 수 있다. 추적 결과의 `production\_qty` 합계를 화면에 표시한다.



\### 실습 3. 역방향 추적에서 원자재 품목 수 표시



완제품 LOT 하나에 사용된 원자재 품목 수를 계산한다.



```python

df\["material\_item\_name"].nunique()

```



\### 실습 4. 추적 결과 다운로드 버튼 추가



`st.download\_button()`을 사용해 추적 결과를 CSV로 내려받는 기능을 추가한다.



\## 정리



생산이력과 LOT 추적 화면은 MES의 핵심 관계를 화면으로 확인하는 부분이다. 사용자는 LOT나 생산번호를 선택하고, 앱은 선택한 ID를 기준으로 JOIN SQL을 실행해 관계를 보여준다.



