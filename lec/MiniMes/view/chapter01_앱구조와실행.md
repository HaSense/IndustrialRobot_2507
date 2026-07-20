# 00. Streamlit 앱 구조와 실행

## 연결되는 기존 교재

- GitHub Mini MES 교재: Chapter 1, Chapter 3, Chapter 4
- 이 문서는 기존 교재의 MES 개념과 테이블 설명을 반복하지 않는다.
- 목표는 교재에서 만든 Mini MES 데이터를 Streamlit 화면으로 보여주는 전체 구조를 이해하는 것이다.

## 학습 목표

이 강의에서는 다음 내용을 확인한다.

- Streamlit 멀티페이지 앱의 기본 구조
- `app.py`, `pages/`, `src/`, `sql/` 폴더의 역할
- 화면 코드와 조회 함수가 분리되는 이유
- 앱 실행 후 학생이 확인해야 할 흐름

## 프로젝트 구조

현재 앱은 다음과 같은 구조로 구성되어 있다.

```text
mini_mes/
    app.py
    pages/
        01_품목_조회.py
        02_LOT_조회.py
        03_생산실적_조회.py
        04_생산_등록.py
        05_생산이력.py
        06_정방향_추적.py
        07_역방향_추적.py
        08_생산현황.py
    src/
        db.py
        queries.py
        services.py
        ui.py
    sql/
        mini_mes.db
        sample_data.sql
        schema.sql
```

`app.py`는 첫 화면이다. Streamlit은 `pages/` 폴더의 Python 파일을 자동으로 왼쪽 메뉴에 표시한다. 따라서 화면을 추가하려면 `pages/` 아래에 새 `.py` 파일을 만들면 된다.

`src/` 폴더는 화면에서 반복되는 코드를 모아 둔 곳이다.

| 파일 | 역할 |
| --- | --- |
| `src/db.py` | SQLite 연결, 조회 결과를 DataFrame으로 변환 |
| `src/queries.py` | 화면에서 사용하는 SELECT SQL |
| `src/services.py` | 생산 등록처럼 여러 INSERT가 필요한 업무 로직 |
| `src/ui.py` | 페이지 제목, KPI, 표 출력 같은 공통 UI |

## 실행 방법

터미널에서 프로젝트 폴더로 이동한 뒤 실행한다.

```bash
pip install -r requirements.txt
streamlit run app.py
```

브라우저가 열리면 첫 화면에서 DB 연결 상태와 주요 테이블 목록을 확인한다.

## 첫 화면 코드 읽기

`app.py`의 핵심 흐름은 다음과 같다.

```python
setup_page("소개")

page_title(
    "Mini MES 교육 앱",
    "MES의 기본 개념과 품목, LOT, 생산실적, 원자재 투입 이력의 관계를 확인합니다.",
    "item, lot, production, production_material",
    "DB 연결 상태와 테이블 구성을 확인한 뒤 왼쪽 메뉴에서 실습 화면으로 이동합니다.",
)

show_database_status()
```

여기서 학생이 봐야 할 점은 다음이다.

- 화면 제목과 설명은 `page_title()`로 통일한다.
- DB 상태 표시는 `show_database_status()`로 분리되어 있다.
- 화면 파일은 SQL을 직접 많이 쓰지 않고 `queries` 함수를 호출한다.

## 화면 이동 순서

수업에서는 다음 순서로 화면을 확인하는 것이 좋다.

1. 소개
2. 품목 조회
3. LOT 조회
4. 생산실적 조회
5. 생산이력
6. 생산 등록
7. 정방향 추적
8. 역방향 추적
9. 생산현황

이 순서는 데이터 모델을 읽는 흐름과 같다.

```text
품목
-> LOT
-> 생산실적
-> 원자재 투입 이력
-> LOT 추적
-> 집계와 대시보드
```

## 실습

### 실습 1. 앱 실행 확인

1. `streamlit run app.py`를 실행한다.
2. 소개 화면에서 DB 연결 상태가 정상인지 확인한다.
3. 왼쪽 메뉴에 8개 기능 화면이 표시되는지 확인한다.

### 실습 2. 화면 파일 찾기

왼쪽 메뉴의 화면 이름과 `pages/` 폴더의 파일명을 연결해 본다.

예를 들어 `품목 조회` 화면은 다음 파일이다.

```text
pages/01_품목_조회.py
```

### 실습 3. 화면 하나 추가해 보기

`pages/09_테스트.py` 파일을 만들고 다음 코드를 작성한다.

```python
import streamlit as st

st.title("테스트 화면")
st.write("새로운 Streamlit 페이지입니다.")
```

앱을 새로고침한 뒤 왼쪽 메뉴에 화면이 추가되는지 확인한다.

## 정리

이 앱은 교재의 테이블 구조를 다시 설명하는 앱이 아니라, 실제 DB 데이터를 화면으로 조회하고 조작하는 실습 앱이다. 화면 코드는 `pages/`에 있고, SQL과 업무 로직은 `src/`에 분리되어 있다.
