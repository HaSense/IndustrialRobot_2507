from __future__ import annotations

from src.db import fetch_all, fetch_dataframe, fetch_one


def table_counts():
    return fetch_dataframe(
        """
        SELECT 'item' AS table_name, COUNT(*) AS row_count FROM item
        UNION ALL SELECT 'lot', COUNT(*) FROM lot
        UNION ALL SELECT 'production_order', COUNT(*) FROM production_order
        UNION ALL SELECT 'production', COUNT(*) FROM production
        UNION ALL SELECT 'quality_inspection', COUNT(*) FROM quality_inspection
        UNION ALL SELECT 'shipment', COUNT(*) FROM shipment
        """
    )


def kpis():
    return fetch_one(
        """
        SELECT
            (SELECT COUNT(*) FROM production_order WHERE status IN ('PLANNED', 'RELEASED')) AS open_orders,
            (SELECT COALESCE(SUM(good_qty), 0) FROM production) AS total_good_qty,
            (SELECT COALESCE(SUM(defect_qty), 0) FROM production) AS total_defect_qty,
            (SELECT COUNT(*) FROM lot WHERE current_qty > 0 AND status = 'AVAILABLE') AS available_lots,
            (SELECT COUNT(*) FROM quality_inspection WHERE result != 'PASS') AS quality_issues,
            (SELECT COALESCE(SUM(qty), 0) FROM shipment_detail) AS shipped_qty
        """
    )


def items(keyword: str = "", item_type: str = "전체"):
    where = ["1 = 1"]
    params: list[object] = []
    if keyword:
        where.append("(item_code LIKE ? OR item_name LIKE ?)")
        params.extend([f"%{keyword}%", f"%{keyword}%"])
    if item_type != "전체":
        where.append("item_type = ?")
        params.append(item_type)
    return fetch_dataframe(
        f"""
        SELECT item_id, item_code, item_name, item_type, unit, shelf_life_days, safety_stock, is_active
        FROM item
        WHERE {' AND '.join(where)}
        ORDER BY item_type, item_code
        """,
        tuple(params),
    )


def active_items(item_type: str | None = None):
    where = "WHERE is_active = 'Y'"
    params: tuple = ()
    if item_type:
        where += " AND item_type = ?"
        params = (item_type,)
    return fetch_all(
        f"""
        SELECT item_id, item_code, item_name, item_type, unit, shelf_life_days
        FROM item
        {where}
        ORDER BY item_code
        """,
        params,
    )


def partners(partner_type: str | None = None):
    where = "WHERE is_active = 'Y'"
    params: tuple = ()
    if partner_type:
        where += " AND partner_type = ?"
        params = (partner_type,)
    return fetch_all(
        f"""
        SELECT partner_id, partner_code, partner_name, partner_type
        FROM partner
        {where}
        ORDER BY partner_code
        """,
        params,
    )


def warehouses(warehouse_type: str | None = None):
    where = ""
    params: tuple = ()
    if warehouse_type:
        where = "WHERE warehouse_type = ?"
        params = (warehouse_type,)
    return fetch_all(
        f"""
        SELECT warehouse_id, warehouse_code, warehouse_name, warehouse_type
        FROM warehouse
        {where}
        ORDER BY warehouse_code
        """,
        params,
    )


def work_centers():
    return fetch_all("SELECT work_center_id, work_center_code, work_center_name FROM work_center ORDER BY work_center_code")


def equipment():
    return fetch_all(
        """
        SELECT e.equipment_id, e.equipment_code, e.equipment_name, e.status, wc.work_center_name
        FROM equipment AS e
        JOIN work_center AS wc ON e.work_center_id = wc.work_center_id
        ORDER BY e.equipment_code
        """
    )


def bom(product_item_id: int | None = None):
    params: list[object] = []
    where = ["bh.is_active = 'Y'"]
    if product_item_id:
        where.append("bh.product_item_id = ?")
        params.append(product_item_id)
    return fetch_dataframe(
        f"""
        SELECT
            p.item_code AS product_code,
            p.item_name AS product_name,
            bh.version,
            m.item_code AS material_code,
            m.item_name AS material_name,
            bd.qty_per,
            bd.loss_rate,
            m.unit
        FROM bom_header AS bh
        JOIN item AS p ON bh.product_item_id = p.item_id
        JOIN bom_detail AS bd ON bh.bom_id = bd.bom_id
        JOIN item AS m ON bd.material_item_id = m.item_id
        WHERE {' AND '.join(where)}
        ORDER BY p.item_code, m.item_code
        """,
        tuple(params),
    )


def lots(keyword: str = "", item_type: str = "전체", status: str = "전체"):
    where = ["1 = 1"]
    params: list[object] = []
    if keyword:
        where.append("(l.lot_no LIKE ? OR i.item_code LIKE ? OR i.item_name LIKE ?)")
        params.extend([f"%{keyword}%", f"%{keyword}%", f"%{keyword}%"])
    if item_type != "전체":
        where.append("i.item_type = ?")
        params.append(item_type)
    if status != "전체":
        where.append("l.status = ?")
        params.append(status)
    return fetch_dataframe(
        f"""
        SELECT
            l.lot_id, l.lot_no, l.item_id, i.item_code, i.item_name, i.item_type,
            w.warehouse_name, l.lot_type, l.initial_qty, l.current_qty,
            l.status, l.received_date, l.produced_date, l.expire_date
        FROM lot AS l
        JOIN item AS i ON l.item_id = i.item_id
        JOIN warehouse AS w ON l.warehouse_id = w.warehouse_id
        WHERE {' AND '.join(where)}
        ORDER BY COALESCE(l.received_date, l.produced_date) DESC, l.lot_no
        """,
        tuple(params),
    )


def available_lots_for_item(item_id: int):
    return fetch_all(
        """
        SELECT lot_id, lot_no, current_qty, expire_date
        FROM lot
        WHERE item_id = ? AND current_qty > 0 AND status = 'AVAILABLE'
        ORDER BY COALESCE(expire_date, '9999-12-31'), lot_no
        """,
        (item_id,),
    )


def inventory_summary():
    return fetch_dataframe(
        """
        SELECT
            i.item_code, i.item_name, i.item_type, i.unit,
            SUM(l.current_qty) AS current_qty,
            i.safety_stock,
            CASE WHEN SUM(l.current_qty) < i.safety_stock THEN '부족' ELSE '정상' END AS stock_status
        FROM item AS i
        LEFT JOIN lot AS l
            ON i.item_id = l.item_id AND l.current_qty > 0
        GROUP BY i.item_id, i.item_code, i.item_name, i.item_type, i.unit, i.safety_stock
        ORDER BY i.item_type, i.item_code
        """
    )


def inventory_txns(lot_id: int | None = None):
    where = ""
    params: tuple = ()
    if lot_id:
        where = "WHERE t.lot_id = ?"
        params = (lot_id,)
    return fetch_dataframe(
        f"""
        SELECT
            t.txn_id, t.txn_date, t.txn_type, l.lot_no, i.item_code, i.item_name,
            w.warehouse_name, t.qty, t.ref_type, t.ref_id, t.memo
        FROM inventory_txn AS t
        JOIN lot AS l ON t.lot_id = l.lot_id
        JOIN item AS i ON t.item_id = i.item_id
        JOIN warehouse AS w ON t.warehouse_id = w.warehouse_id
        {where}
        ORDER BY t.txn_date DESC, t.txn_id DESC
        """,
        params,
    )


def orders(status: str = "전체"):
    where = ["1 = 1"]
    params: list[object] = []
    if status != "전체":
        where.append("po.status = ?")
        params.append(status)
    return fetch_dataframe(
        f"""
        SELECT
            po.order_id, po.order_no, i.item_code, i.item_name, wc.work_center_name,
            po.plan_date, po.due_date, po.order_qty, po.status, po.priority
        FROM production_order AS po
        JOIN item AS i ON po.product_item_id = i.item_id
        JOIN work_center AS wc ON po.work_center_id = wc.work_center_id
        WHERE {' AND '.join(where)}
        ORDER BY po.plan_date DESC, po.priority, po.order_no
        """,
        tuple(params),
    )


def releasable_orders():
    return fetch_all(
        """
        SELECT order_id, order_no, product_item_id, order_qty, item_name
        FROM production_order
        JOIN item ON production_order.product_item_id = item.item_id
        WHERE status IN ('PLANNED', 'RELEASED')
        ORDER BY plan_date, order_no
        """
    )


def productions():
    return fetch_dataframe(
        """
        SELECT
            p.production_id, p.production_no, po.order_no, i.item_code, i.item_name,
            l.lot_no AS output_lot_no, e.equipment_name, p.production_date,
            p.good_qty, p.defect_qty,
            ROUND(CASE WHEN p.good_qty + p.defect_qty = 0 THEN 0 ELSE p.good_qty * 100.0 / (p.good_qty + p.defect_qty) END, 2) AS yield_rate,
            p.status
        FROM production AS p
        LEFT JOIN production_order AS po ON p.order_id = po.order_id
        JOIN item AS i ON p.product_item_id = i.item_id
        JOIN lot AS l ON p.output_lot_id = l.lot_id
        JOIN equipment AS e ON p.equipment_id = e.equipment_id
        ORDER BY p.production_date DESC, p.production_no DESC
        """
    )


def production_materials(production_id: int):
    return fetch_dataframe(
        """
        SELECT
            m.item_code AS material_code,
            m.item_name AS material_name,
            l.lot_no AS material_lot_no,
            pm.required_qty,
            pm.issued_qty
        FROM production_material AS pm
        JOIN item AS m ON pm.material_item_id = m.item_id
        JOIN lot AS l ON pm.material_lot_id = l.lot_id
        WHERE pm.production_id = ?
        ORDER BY m.item_code, l.lot_no
        """,
        (production_id,),
    )


def quality_inspections():
    return fetch_dataframe(
        """
        SELECT
            qi.inspection_id, qi.inspection_no, qi.inspection_type, qi.inspection_date,
            l.lot_no, i.item_code, i.item_name, qi.sample_qty, qi.defect_qty,
            qi.result, qi.memo
        FROM quality_inspection AS qi
        JOIN lot AS l ON qi.lot_id = l.lot_id
        JOIN item AS i ON l.item_id = i.item_id
        ORDER BY qi.inspection_date DESC, qi.inspection_no DESC
        """
    )


def shipments():
    return fetch_dataframe(
        """
        SELECT
            s.shipment_id, s.shipment_no, p.partner_name AS customer_name,
            s.shipment_date, s.status,
            COUNT(sd.shipment_detail_id) AS lot_count,
            COALESCE(SUM(sd.qty), 0) AS shipment_qty
        FROM shipment AS s
        JOIN partner AS p ON s.customer_id = p.partner_id
        LEFT JOIN shipment_detail AS sd ON s.shipment_id = sd.shipment_id
        GROUP BY s.shipment_id, s.shipment_no, p.partner_name, s.shipment_date, s.status
        ORDER BY s.shipment_date DESC, s.shipment_no DESC
        """
    )


def shipment_details(shipment_id: int):
    return fetch_dataframe(
        """
        SELECT sd.shipment_detail_id, l.lot_no, i.item_code, i.item_name, sd.qty
        FROM shipment_detail AS sd
        JOIN lot AS l ON sd.lot_id = l.lot_id
        JOIN item AS i ON sd.item_id = i.item_id
        WHERE sd.shipment_id = ?
        ORDER BY l.lot_no
        """,
        (shipment_id,),
    )


def forward_trace(material_lot_id: int):
    return fetch_dataframe(
        """
        SELECT
            ml.lot_no AS material_lot_no,
            mi.item_name AS material_name,
            pm.issued_qty,
            p.production_no,
            p.production_date,
            ol.lot_no AS output_lot_no,
            oi.item_name AS output_item_name,
            p.good_qty,
            s.shipment_no,
            c.partner_name AS customer_name,
            sd.qty AS shipped_qty
        FROM production_material AS pm
        JOIN lot AS ml ON pm.material_lot_id = ml.lot_id
        JOIN item AS mi ON pm.material_item_id = mi.item_id
        JOIN production AS p ON pm.production_id = p.production_id
        JOIN lot AS ol ON p.output_lot_id = ol.lot_id
        JOIN item AS oi ON p.product_item_id = oi.item_id
        LEFT JOIN shipment_detail AS sd ON ol.lot_id = sd.lot_id
        LEFT JOIN shipment AS s ON sd.shipment_id = s.shipment_id
        LEFT JOIN partner AS c ON s.customer_id = c.partner_id
        WHERE pm.material_lot_id = ?
        ORDER BY p.production_date, p.production_no, s.shipment_no
        """,
        (material_lot_id,),
    )


def reverse_trace(output_lot_id: int):
    return fetch_dataframe(
        """
        SELECT
            ol.lot_no AS output_lot_no,
            oi.item_name AS output_item_name,
            p.production_no,
            p.production_date,
            p.good_qty,
            ml.lot_no AS material_lot_no,
            mi.item_code AS material_code,
            mi.item_name AS material_name,
            pm.issued_qty,
            ml.expire_date AS material_expire_date,
            ml.source_partner_id
        FROM production AS p
        JOIN lot AS ol ON p.output_lot_id = ol.lot_id
        JOIN item AS oi ON p.product_item_id = oi.item_id
        JOIN production_material AS pm ON p.production_id = pm.production_id
        JOIN lot AS ml ON pm.material_lot_id = ml.lot_id
        JOIN item AS mi ON pm.material_item_id = mi.item_id
        WHERE p.output_lot_id = ?
        ORDER BY mi.item_code, ml.lot_no
        """,
        (output_lot_id,),
    )


def production_by_date():
    return fetch_dataframe(
        """
        SELECT production_date, SUM(good_qty) AS good_qty, SUM(defect_qty) AS defect_qty
        FROM production
        GROUP BY production_date
        ORDER BY production_date
        """
    )


def production_by_item():
    return fetch_dataframe(
        """
        SELECT i.item_name, SUM(p.good_qty) AS good_qty, SUM(p.defect_qty) AS defect_qty
        FROM production AS p
        JOIN item AS i ON p.product_item_id = i.item_id
        GROUP BY i.item_id, i.item_name
        ORDER BY good_qty DESC
        """
    )


def next_key(table_name: str, id_column: str):
    return fetch_one(f"SELECT COALESCE(MAX({id_column}), 0) + 1 AS next_value FROM {table_name}")["next_value"]
