\# 05. 대시보드 구현과 종합 실습



\## 연결되는 기존 교재



\- GitHub Mini MES 교재: Chapter 11, Chapter 13, Chapter 14

\- 기존 교재에서는 GROUP BY 집계와 분석 과제를 다룬다.

\- 이 문서에서는 집계 SQL 결과를 Streamlit 대시보드로 표시하는 방법을 다룬다.



\## 학습 목표



\- KPI 지표를 `st.metric()` 형태로 표시하는 방법

\- 집계 결과를 표와 차트로 함께 보여주는 방법

\- 스키마에 없는 지표를 임의로 만들지 않는 원칙

\- 기존 화면을 바탕으로 종합 실습 과제를 수행하는 방법



\## 구현 대상 화면



대시보드 화면은 다음 파일이다.



```text

pages/08\_생산현황.py

```



이 화면에서 호출하는 주요 함수는 다음과 같다.



| 함수 | 표시 내용 |

| --- | --- |

| `table\_counts()` | 품목 수, LOT 수, 생산실적 수, 투입 이력 행 수 |

| `production\_by\_date()` | 일자별 생산량 |

| `production\_by\_item()` | 품목별 생산량 |

| `lot\_use\_counts()` | LOT별 원자재 사용 횟수 |

| `productions()` | 최근 생산실적 |



\## KPI 표시



대시보드 첫 부분은 테이블별 행 수를 KPI로 보여준다.



```python

counts = queries.table\_counts()

count\_map = dict(zip(counts\["table\_name"], counts\["row\_count"]))

metric\_row(

&#x20;   \[

&#x20;       ("품목 수", count\_map.get("item", 0)),

&#x20;       ("LOT 수", count\_map.get("lot", 0)),

&#x20;       ("생산실적 수", count\_map.get("production", 0)),

&#x20;       ("투입 이력 행 수", count\_map.get("production\_material", 0)),

&#x20;   ]

)

```



`table\_counts()`는 여러 테이블의 행 수를 한 번에 조회한다. 화면에서는 이 결과를 딕셔너리로 바꿔 KPI 표시 함수에 전달한다.



\## 일자별 생산량



일자별 생산량은 `production` 테이블을 생산일자로 묶어 계산한다.



```python

by\_date = queries.production\_by\_date()

```



화면에서는 표와 막대그래프를 함께 보여준다.



```python

st.subheader("일자별 생산량")

show\_dataframe(by\_date)

if not by\_date.empty:

&#x20;   st.bar\_chart(by\_date.set\_index("production\_date")\["production\_qty"])

```



학생은 같은 집계 결과가 표와 차트로 어떻게 다르게 보이는지 비교할 수 있다.



\## 품목별 생산량



품목별 생산량은 제품 품목 기준으로 생산수량을 합산한다.



```python

by\_item = queries.production\_by\_item()

```



화면 코드는 일자별 생산량과 같은 패턴을 사용한다.



```python

st.subheader("품목별 생산량")

show\_dataframe(by\_item)

if not by\_item.empty:

&#x20;   st.bar\_chart(by\_item.set\_index("item\_name")\["production\_qty"])

```



이처럼 같은 출력 패턴을 반복하면 학생들이 코드를 따라가기 쉽다.



\## 최근 생산실적



최근 생산실적은 전체 생산실적 조회 결과를 생산일자 기준으로 정렬한 뒤 일부만 보여준다.



```python

recent = queries.productions().sort\_values("production\_date", ascending=False).head(5)

```



여기서는 SQL이 아니라 DataFrame 정렬을 사용한다. 수업에서는 SQL 정렬과 DataFrame 정렬의 차이를 간단히 비교할 수 있다.



\## LOT별 원자재 사용 횟수



LOT별 사용 횟수는 원자재 LOT가 생산에 몇 번 사용되었는지 보여준다.



```python

lot\_use = queries.lot\_use\_counts()

```



이 지표는 정방향 추적과 연결된다. 사용 횟수가 높은 원자재 LOT는 문제가 발생했을 때 영향을 주는 생산실적이 많을 수 있다.



\## 스키마 기준 지표만 만들기



대시보드 화면 하단에는 다음 안내가 있다.



```text

현재 스키마에 없는 설비, 작업자, 불량 수량 통계는 임의로 만들지 않는다.

```



이 문장은 중요하다. 현재 DB에는 설비, 작업자, 불량수량, 작업시간 테이블이 없다. 따라서 다음 지표는 만들지 않는다.



\- 설비별 가동률

\- 작업자별 생산량

\- 불량률

\- 작업시간 대비 생산성



대시보드는 실제 스키마에서 계산 가능한 지표만 보여줘야 한다.



\## 종합 실습 과제



\### 과제 1. 재고 수량 KPI 추가



LOT 전체 수량 합계를 KPI로 추가한다.



힌트:



```sql

SELECT SUM(qty) AS total\_qty

FROM lot;

```



구현 위치:



```text

src/queries.py

pages/08\_생산현황.py

```



\### 과제 2. 완제품 LOT 수 표시



`lot\_type = 'PRODUCTION'`인 LOT 개수를 대시보드에 표시한다.



힌트:



```sql

SELECT COUNT(\*) AS output\_lot\_count

FROM lot

WHERE lot\_type = 'PRODUCTION';

```



\### 과제 3. 원자재 사용량 차트 추가



원자재 품목별 투입수량 합계를 조회하고 막대그래프로 표시한다.



필요한 테이블:



```text

production\_material

item

```



\### 과제 4. 추적 화면과 대시보드 연결 질문 만들기



다음 질문에 답할 수 있도록 화면을 사용한다.



1\. 가장 많이 사용된 원자재 LOT는 무엇인가?

2\. 그 LOT를 정방향 추적하면 어떤 완제품 LOT가 나오는가?

3\. 해당 완제품 LOT를 역방향 추적하면 같은 원자재 LOT가 다시 보이는가?



\### 과제 5. 생산 등록 후 대시보드 변화 확인



생산 등록 화면에서 새 생산실적을 저장한 뒤 대시보드로 이동한다.



확인할 항목:



\- 생산실적 수 증가

\- LOT 수 증가

\- 투입 이력 행 수 증가

\- 일자별 생산량 변화

\- 품목별 생산량 변화



\## 수업 마무리 체크리스트



학생이 다음 내용을 설명할 수 있으면 수업 목표를 달성한 것이다.



\- Streamlit 화면 파일이 `pages/`에 들어가는 이유

\- 조회 SQL을 `queries.py`에 분리하는 이유

\- 생산 등록에서 트랜잭션이 필요한 이유

\- 정방향 추적과 역방향 추적의 시작점 차이

\- 대시보드 지표를 실제 스키마 기준으로만 만들어야 하는 이유



\## 정리



대시보드는 SQL 집계 결과를 사용자가 빠르게 이해할 수 있게 만든 화면이다. 단순히 차트를 추가하는 것이 목표가 아니라, 실제 테이블 관계에서 계산 가능한 현장 질문을 화면으로 표현하는 것이 핵심이다.



