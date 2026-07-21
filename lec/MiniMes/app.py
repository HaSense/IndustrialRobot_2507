import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_database_status, show_dataframe


setup_page("소개")

page_title(
    "Mini MES 교육 앱",
    "MES의 기본 개념과 품목, LOT, 생산실적, 원자재 투입 이력의 관계를 확인합니다.",
    "item, lot, production, production_material",
    "DB 연결 상태와 테이블 구성을 확인한 뒤 왼쪽 메뉴에서 실습 화면으로 이동합니다.",
)

show_database_status()

st.subheader("Mini MES에서 다루는 현장 질문")
st.markdown(
    """
    - 오늘 어떤 제품을 얼마나 생산했는가?
    - 특정 완제품 LOT는 언제 만들어졌는가?
    - 생산에 어떤 원자재 LOT가 사용되었는가?
    - 문제가 생긴 원자재 LOT를 사용한 완제품 LOT는 무엇인가?
    - 완제품 품질 문제가 발생했을 때 어떤 원자재 LOT를 확인해야 하는가?
    """
)

try:
    counts = queries.table_counts()
    count_map = dict(zip(counts["table_name"], counts["row_count"]))
    metric_row(
        [
            ("품목 수", count_map.get("item", 0)),
            ("LOT 수", count_map.get("lot", 0)),
            ("생산실적 수", count_map.get("production", 0)),
            ("원자재 투입 행 수", count_map.get("production_material", 0)),
        ]
    )

    st.subheader("주요 테이블")
    show_dataframe(queries.table_list())

    st.subheader("수업 진행 권장 순서")
    st.markdown(
        """
        1. 소개에서 전체 데이터 흐름을 확인한다.
        2. 품목 조회와 LOT 조회로 기준정보와 LOT 개념을 구분한다.
        3. 생산실적 조회와 생산이력에서 생산 이벤트와 원자재 투입의 1:N 관계를 확인한다.
        4. 생산 등록으로 트랜잭션 저장 흐름을 실습한다.
        5. 정방향 추적과 역방향 추적으로 LOT 추적의 목적을 비교한다.
        6. 생산현황에서 집계 SQL이 화면에 어떻게 표현되는지 확인한다.
        """
    )
except Exception as exc:
    st.error("데이터베이스 구조를 확인하는 중 오류가 발생했습니다.")
    st.exception(exc)
