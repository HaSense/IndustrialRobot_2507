import altair as alt
import streamlit as st

from src import queries
from src.db import DB_PATH, initialize_database
from src.ui import setup_page, show_dataframe


setup_page("대시보드")


def inject_dashboard_style() -> None:
    st.markdown(
        """
        <style>
        .main .block-container { padding-top: 1.4rem; padding-bottom: 2.5rem; max-width: 1480px; }
        .dash-hero {
            border: 1px solid #e2e8f0; border-radius: 8px; padding: 26px 30px; color: #13202f;
            background: linear-gradient(115deg, rgba(255,255,255,0.96) 0%, rgba(255,255,255,0.86) 42%, rgba(255,248,237,0.72) 100%),
                repeating-linear-gradient(90deg, rgba(202, 47, 38, 0.10) 0 1px, transparent 1px 70px),
                linear-gradient(135deg, #fff6e8 0%, #f0f8ff 48%, #eef8ef 100%);
            box-shadow: 0 14px 38px rgba(15, 23, 42, 0.08);
        }
        .hero-grid { display: grid; grid-template-columns: minmax(0, 1.35fr) minmax(320px, 0.65fr); gap: 24px; align-items: stretch; }
        .hero-title { font-size: 2.35rem; line-height: 1.08; font-weight: 800; margin: 0 0 10px; letter-spacing: 0; }
        .hero-copy { font-size: 1rem; color: #405166; margin: 0; max-width: 860px; }
        .hero-strip { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; margin-top: 22px; }
        .hero-chip { border-left: 4px solid; background: rgba(255,255,255,0.78); border-radius: 8px; padding: 12px 14px; min-height: 74px; }
        .hero-chip strong { display: block; font-size: 1.15rem; margin-bottom: 4px; }
        .hero-chip span { color: #5b6b7f; font-size: 0.86rem; }
        .ops-panel { background: #13202f; color: #f8fafc; border-radius: 8px; padding: 18px; height: 100%; box-shadow: inset 0 0 0 1px rgba(255,255,255,0.08); }
        .ops-panel .label { color: #9fb1c7; font-size: 0.78rem; text-transform: uppercase; font-weight: 700; margin-bottom: 8px; }
        .ops-panel .value { font-size: 1.35rem; font-weight: 800; margin-bottom: 14px; }
        .ops-line { display: flex; justify-content: space-between; gap: 12px; padding: 9px 0; border-top: 1px solid rgba(255,255,255,0.12); color: #dbe7f5; font-size: 0.92rem; }
        .kpi-card { border-radius: 8px; padding: 16px 16px 14px; border: 1px solid rgba(15, 23, 42, 0.08); box-shadow: 0 10px 26px rgba(15, 23, 42, 0.06); min-height: 122px; background: #ffffff; }
        .kpi-card.red { border-top: 5px solid #c9342b; } .kpi-card.green { border-top: 5px solid #1f8a5b; } .kpi-card.blue { border-top: 5px solid #2563eb; }
        .kpi-card.amber { border-top: 5px solid #d08818; } .kpi-card.slate { border-top: 5px solid #475569; } .kpi-card.teal { border-top: 5px solid #0f9488; }
        .kpi-label { color: #607085; font-size: 0.84rem; font-weight: 700; margin-bottom: 6px; }
        .kpi-value { color: #111827; font-size: 1.75rem; line-height: 1.1; font-weight: 850; letter-spacing: 0; }
        .kpi-note { color: #64748b; font-size: 0.82rem; margin-top: 8px; }
        .section-title { font-size: 1.05rem; font-weight: 800; margin: 14px 0 8px; color: #172033; }
        .alert-row { border-radius: 8px; padding: 12px 14px; margin-bottom: 10px; background: #fff7ed; border: 1px solid #fed7aa; color: #7c2d12; font-weight: 650; }
        .ok-row { border-radius: 8px; padding: 12px 14px; background: #eefbf4; border: 1px solid #b7ebca; color: #14532d; font-weight: 650; }
        @media (max-width: 900px) { .hero-grid, .hero-strip { grid-template-columns: 1fr; } .hero-title { font-size: 1.85rem; } }
        </style>
        """,
        unsafe_allow_html=True,
    )


def kpi_card(label: str, value: str, note: str, color: str) -> None:
    st.markdown(
        f"""
        <div class="kpi-card {color}">
            <div class="kpi-label">{label}</div>
            <div class="kpi-value">{value}</div>
            <div class="kpi-note">{note}</div>
        </div>
        """,
        unsafe_allow_html=True,
    )


def fmt_qty(value: float) -> str:
    return f"{value:,.0f}"


inject_dashboard_style()

if not DB_PATH.exists():
    st.warning("초기 DB가 없어 라면공장 샘플 DB를 생성합니다.")
    initialize_database()
    st.rerun()

try:
    kpi = queries.kpis()
    inventory = queries.inventory_summary()
    productions = queries.productions()
    orders = queries.orders()
    shipments = queries.shipments()
    quality = queries.quality_inspections()
    by_date = queries.production_by_date()
    by_item = queries.production_by_item()

    total_good = float(kpi["total_good_qty"])
    total_defect = float(kpi["total_defect_qty"])
    total_input = total_good + total_defect
    yield_rate = 0 if total_input == 0 else total_good * 100 / total_input
    shortage = inventory[inventory["stock_status"] == "부족"]
    released_orders = orders[orders["status"].isin(["PLANNED", "RELEASED"])]

    st.markdown(
        f"""
        <div class="dash-hero">
            <div class="hero-grid">
                <div>
                    <div class="hero-title">라면공장 Mini MES 운영 대시보드</div>
                    <p class="hero-copy">작업지시부터 원자재 LOT 투입, 완제품 입고, 품질검사, 출하 추적까지 현재 공장 상태를 한 화면에서 확인합니다.</p>
                    <div class="hero-strip">
                        <div class="hero-chip" style="border-color:#c9342b"><strong>{fmt_qty(total_good)}</strong><span>누적 양품 생산</span></div>
                        <div class="hero-chip" style="border-color:#1f8a5b"><strong>{yield_rate:.1f}%</strong><span>누적 수율</span></div>
                        <div class="hero-chip" style="border-color:#2563eb"><strong>{int(kpi['available_lots'])}</strong><span>사용 가능 LOT</span></div>
                    </div>
                </div>
                <div class="ops-panel">
                    <div class="label">Live Factory Snapshot</div>
                    <div class="value">FG 창고와 생산라인 감시 중</div>
                    <div class="ops-line"><span>미완료 작업지시</span><strong>{int(kpi['open_orders'])}건</strong></div>
                    <div class="ops-line"><span>품질 이슈</span><strong>{int(kpi['quality_issues'])}건</strong></div>
                    <div class="ops-line"><span>누적 출하</span><strong>{fmt_qty(float(kpi['shipped_qty']))} EA</strong></div>
                </div>
            </div>
        </div>
        """,
        unsafe_allow_html=True,
    )

    st.write("")
    k1, k2, k3, k4, k5, k6 = st.columns(6)
    with k1:
        kpi_card("미완료 지시", f"{int(kpi['open_orders'])}", "생산 대기/진행", "red")
    with k2:
        kpi_card("누적 양품", fmt_qty(total_good), "완제품 입고 기준", "green")
    with k3:
        kpi_card("누적 불량", fmt_qty(total_defect), "생산실적 기준", "amber")
    with k4:
        kpi_card("가용 LOT", f"{int(kpi['available_lots'])}", "출하/투입 가능", "blue")
    with k5:
        kpi_card("품질 이슈", f"{int(kpi['quality_issues'])}", "FAIL/HOLD 검사", "slate")
    with k6:
        kpi_card("출하 수량", fmt_qty(float(kpi["shipped_qty"])), "고객 출하 누적", "teal")

    st.write("")
    chart_left, chart_right = st.columns([1.15, 0.85])
    with chart_left:
        st.markdown('<div class="section-title">생산 추이</div>', unsafe_allow_html=True)
        if not by_date.empty:
            st.bar_chart(by_date.set_index("production_date")[["good_qty", "defect_qty"]], height=320)
        show_dataframe(by_date, "생산 데이터가 없습니다.")

    with chart_right:
        st.markdown('<div class="section-title">품목별 생산 기여도</div>', unsafe_allow_html=True)
        if not by_item.empty:
            st.bar_chart(by_item.set_index("item_name")["good_qty"], height=320)
        show_dataframe(by_item, "품목별 생산 데이터가 없습니다.")

    st.markdown('<div class="section-title">추가 운영 차트</div>', unsafe_allow_html=True)
    extra1, extra2 = st.columns(2)
    with extra1:
        st.caption("라인 차트: 일자별 양품 생산 흐름")
        if not by_date.empty:
            st.line_chart(by_date.set_index("production_date")["good_qty"], height=260)

    with extra2:
        st.caption("영역 차트: 누적 양품/불량 흐름")
        if not by_date.empty:
            cumulative = by_date.sort_values("production_date").copy()
            cumulative["누적 양품"] = cumulative["good_qty"].cumsum()
            cumulative["누적 불량"] = cumulative["defect_qty"].cumsum()
            st.area_chart(cumulative.set_index("production_date")[["누적 양품", "누적 불량"]], height=260)

    extra3, extra4 = st.columns(2)
    with extra3:
        st.caption("산점도: 생산수량과 수율 관계")
        if not productions.empty:
            scatter = productions[["production_no", "item_name", "good_qty", "yield_rate"]].copy()
            scatter["display_size"] = scatter["good_qty"] / 12
            st.scatter_chart(
                scatter,
                x="good_qty",
                y="yield_rate",
                color="item_name",
                size="display_size",
                height=280,
            )

    with extra4:
        st.caption("재고 바: 현재고 / 안전재고")
        stock_rows = inventory.sort_values("stock_status", ascending=True).head(6)
        for _, row in stock_rows.iterrows():
            current_qty = float(row["current_qty"] or 0)
            safety_stock = float(row["safety_stock"] or 0)
            ratio = 1.0 if safety_stock == 0 else min(current_qty / safety_stock, 1.5) / 1.5
            st.progress(ratio, text=f"{row['item_name']}  {fmt_qty(current_qty)} / 안전 {fmt_qty(safety_stock)}")

    st.markdown('<div class="section-title">Altair 설명용 차트</div>', unsafe_allow_html=True)
    alt1, alt2 = st.columns(2)
    with alt1:
        st.caption("도넛 차트: 제품별 생산 비중")
        if not by_item.empty:
            donut_data = by_item.copy()
            total_item_qty = float(donut_data["good_qty"].sum())
            donut_data["share"] = donut_data["good_qty"] / total_item_qty
            label_names = {
                "진라면 매운맛": "진매운",
                "진라면 순한맛": "진순한",
                "신라면 치즈맛": "신치즈",
            }
            donut_data["short_name"] = donut_data["item_name"].map(label_names).fillna(donut_data["item_name"])
            donut_data["label"] = donut_data.apply(
                lambda row: f"{row['short_name']} {row['share'] * 100:.1f}%",
                axis=1,
            )
            donut_base = alt.Chart(donut_data).encode(
                theta=alt.Theta("good_qty:Q", title="양품수량"),
                color=alt.Color(
                    "item_name:N",
                    title="제품",
                    scale=alt.Scale(range=["#c9342b", "#1f8a5b", "#2563eb", "#d08818"]),
                ),
                tooltip=[
                    alt.Tooltip("item_name:N", title="제품"),
                    alt.Tooltip("good_qty:Q", title="양품수량", format=",.0f"),
                    alt.Tooltip("share:Q", title="비중", format=".1%"),
                    alt.Tooltip("defect_qty:Q", title="불량수량", format=",.0f"),
                ],
            )
            donut = donut_base.mark_arc(innerRadius=65, outerRadius=120, stroke="#ffffff", strokeWidth=2)
            donut_label = donut_base.mark_text(radius=135, size=12, fontWeight="bold", color="#1f2937").encode(
                text="label:N"
            )
            st.altair_chart((donut + donut_label).properties(height=330), use_container_width=True)

    with alt2:
        st.caption("히트맵: 검사 유형별 품질 판정")
        if not quality.empty:
            quality_matrix = quality.groupby(["inspection_type", "result"]).size().reset_index(name="count")
            heatmap = (
                alt.Chart(quality_matrix)
                .mark_rect(cornerRadius=3)
                .encode(
                    x=alt.X("result:N", title="판정", sort=["PASS", "HOLD", "FAIL"]),
                    y=alt.Y("inspection_type:N", title="검사 유형", sort=["INCOMING", "PROCESS", "FINAL"]),
                    color=alt.Color(
                        "count:Q",
                        title="건수",
                        scale=alt.Scale(scheme="yelloworangered"),
                    ),
                    tooltip=[
                        alt.Tooltip("inspection_type:N", title="검사 유형"),
                        alt.Tooltip("result:N", title="판정"),
                        alt.Tooltip("count:Q", title="건수"),
                    ],
                )
                .properties(height=310)
            )
            text_layer = (
                alt.Chart(quality_matrix)
                .mark_text(fontWeight="bold", color="#1f2937")
                .encode(
                    x=alt.X("result:N", sort=["PASS", "HOLD", "FAIL"]),
                    y=alt.Y("inspection_type:N", sort=["INCOMING", "PROCESS", "FINAL"]),
                    text="count:Q",
                )
            )
            st.altair_chart(heatmap + text_layer, use_container_width=True)

    alert_col, order_col = st.columns([0.95, 1.05])
    with alert_col:
        st.markdown('<div class="section-title">운영 알림</div>', unsafe_allow_html=True)
        if shortage.empty and int(kpi["quality_issues"]) == 0:
            st.markdown('<div class="ok-row">재고 부족과 품질 보류가 없습니다.</div>', unsafe_allow_html=True)
        else:
            if not shortage.empty:
                for _, row in shortage.iterrows():
                    st.markdown(
                        f"""
                        <div class="alert-row">
                            {row['item_name']} 현재고 {fmt_qty(float(row['current_qty'] or 0))} /
                            안전재고 {fmt_qty(float(row['safety_stock']))}
                        </div>
                        """,
                        unsafe_allow_html=True,
                    )
            if int(kpi["quality_issues"]) > 0:
                st.markdown(
                    f'<div class="alert-row">품질 보류 또는 불합격 검사 {int(kpi["quality_issues"])}건 확인 필요</div>',
                    unsafe_allow_html=True,
                )

        st.markdown('<div class="section-title">재고 부족 감시</div>', unsafe_allow_html=True)
        show_dataframe(shortage, "안전재고 미만 품목이 없습니다.")

    with order_col:
        st.markdown('<div class="section-title">진행 대상 작업지시</div>', unsafe_allow_html=True)
        show_dataframe(released_orders, "진행할 작업지시가 없습니다.")
        st.markdown('<div class="section-title">최근 출하</div>', unsafe_allow_html=True)
        show_dataframe(shipments.head(5), "출하 이력이 없습니다.")

    st.markdown('<div class="section-title">최근 생산실적</div>', unsafe_allow_html=True)
    show_dataframe(productions.head(8), "생산실적이 없습니다.")

except Exception as exc:
    st.error("대시보드 데이터를 불러오지 못했습니다.")
    st.exception(exc)

with st.expander("초기 DB 재생성"):
    st.warning("현재 DB 파일을 스키마와 샘플 데이터 기준으로 다시 만듭니다.")
    if st.button("mini_mes_final.db 재생성"):
        initialize_database()
        st.success("DB를 재생성했습니다.")
        st.rerun()
