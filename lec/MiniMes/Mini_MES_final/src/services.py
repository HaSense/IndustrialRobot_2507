from __future__ import annotations

from dataclasses import dataclass
from datetime import date, timedelta
import sqlite3

from src.db import get_connection


@dataclass
class MaterialReceipt:
    lot_no: str
    item_id: int
    warehouse_id: int
    supplier_id: int
    qty: float
    received_date: date
    expire_date: date | None


@dataclass
class ProductionOrderInput:
    order_no: str
    product_item_id: int
    work_center_id: int
    plan_date: date
    due_date: date
    order_qty: float
    priority: int


@dataclass
class ProductionCompletion:
    order_id: int | None
    production_no: str
    product_item_id: int
    output_lot_no: str
    equipment_id: int
    production_date: date
    good_qty: float
    defect_qty: float
    material_rows: list[dict]


@dataclass
class InspectionInput:
    inspection_no: str
    inspection_type: str
    lot_id: int
    production_id: int | None
    inspection_date: date
    sample_qty: float
    defect_qty: float
    result: str
    memo: str


@dataclass
class ShipmentInput:
    shipment_no: str
    customer_id: int
    shipment_date: date
    detail_rows: list[dict]


def _next_id(cursor: sqlite3.Cursor, table_name: str, id_column: str) -> int:
    return int(cursor.execute(f"SELECT COALESCE(MAX({id_column}), 0) + 1 FROM {table_name}").fetchone()[0])


def _ensure_unique(cursor: sqlite3.Cursor, table_name: str, column: str, value: str, label: str) -> None:
    row = cursor.execute(f"SELECT 1 FROM {table_name} WHERE {column} = ?", (value.strip(),)).fetchone()
    if row:
        raise ValueError(f"이미 존재하는 {label}입니다: {value}")


def receive_material(data: MaterialReceipt) -> dict:
    if data.qty <= 0:
        raise ValueError("입고수량은 0보다 커야 합니다.")
    if not data.lot_no.strip():
        raise ValueError("LOT 번호를 입력하세요.")

    with get_connection() as connection:
        cursor = connection.cursor()
        _ensure_unique(cursor, "lot", "lot_no", data.lot_no, "LOT 번호")
        lot_id = _next_id(cursor, "lot", "lot_id")
        txn_id = _next_id(cursor, "inventory_txn", "txn_id")
        cursor.execute(
            """
            INSERT INTO lot (
                lot_id, lot_no, item_id, warehouse_id, lot_type, initial_qty, current_qty,
                status, received_date, produced_date, expire_date, source_partner_id
            )
            VALUES (?, ?, ?, ?, 'RECEIPT', ?, ?, 'AVAILABLE', ?, NULL, ?, ?)
            """,
            (
                lot_id,
                data.lot_no.strip(),
                data.item_id,
                data.warehouse_id,
                data.qty,
                data.qty,
                str(data.received_date),
                str(data.expire_date) if data.expire_date else None,
                data.supplier_id,
            ),
        )
        cursor.execute(
            """
            INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo)
            VALUES (?, ?, 'RECEIPT', ?, ?, ?, ?, 'RECEIPT', ?, '원자재 입고')
            """,
            (txn_id, str(data.received_date), lot_id, data.item_id, data.warehouse_id, data.qty, lot_id),
        )
        connection.commit()
        return {"lot_id": lot_id, "lot_no": data.lot_no.strip(), "qty": data.qty}


def create_production_order(data: ProductionOrderInput) -> dict:
    if data.order_qty <= 0:
        raise ValueError("지시수량은 0보다 커야 합니다.")
    if not data.order_no.strip():
        raise ValueError("작업지시 번호를 입력하세요.")

    with get_connection() as connection:
        cursor = connection.cursor()
        _ensure_unique(cursor, "production_order", "order_no", data.order_no, "작업지시 번호")
        order_id = _next_id(cursor, "production_order", "order_id")
        cursor.execute(
            """
            INSERT INTO production_order (
                order_id, order_no, product_item_id, work_center_id, plan_date, due_date,
                order_qty, status, priority
            )
            VALUES (?, ?, ?, ?, ?, ?, ?, 'RELEASED', ?)
            """,
            (
                order_id,
                data.order_no.strip(),
                data.product_item_id,
                data.work_center_id,
                str(data.plan_date),
                str(data.due_date),
                data.order_qty,
                data.priority,
            ),
        )
        connection.commit()
        return {"order_id": order_id, "order_no": data.order_no.strip(), "status": "RELEASED"}


def bom_requirements(product_item_id: int, good_qty: float) -> list[dict]:
    with get_connection() as connection:
        rows = connection.execute(
            """
            SELECT bd.material_item_id, i.item_code, i.item_name, bd.qty_per, bd.loss_rate
            FROM bom_header AS bh
            JOIN bom_detail AS bd ON bh.bom_id = bd.bom_id
            JOIN item AS i ON bd.material_item_id = i.item_id
            WHERE bh.product_item_id = ? AND bh.is_active = 'Y'
            ORDER BY i.item_code
            """,
            (product_item_id,),
        ).fetchall()

    return [
        {
            "material_item_id": int(row["material_item_id"]),
            "item_code": row["item_code"],
            "item_name": row["item_name"],
            "required_qty": round(good_qty * float(row["qty_per"]) * (1 + float(row["loss_rate"])), 3),
        }
        for row in rows
    ]


def complete_production(data: ProductionCompletion) -> dict:
    if data.good_qty <= 0:
        raise ValueError("양품수량은 0보다 커야 합니다.")
    if data.defect_qty < 0:
        raise ValueError("불량수량은 0 이상이어야 합니다.")
    if not data.material_rows:
        raise ValueError("투입 원자재 LOT를 선택하세요.")

    with get_connection() as connection:
        cursor = connection.cursor()
        _ensure_unique(cursor, "production", "production_no", data.production_no, "생산실적 번호")
        _ensure_unique(cursor, "lot", "lot_no", data.output_lot_no, "완제품 LOT 번호")

        for row in data.material_rows:
            lot = cursor.execute(
                "SELECT current_qty FROM lot WHERE lot_id = ? AND status = 'AVAILABLE'",
                (row["material_lot_id"],),
            ).fetchone()
            if lot is None:
                raise ValueError("사용 가능한 원자재 LOT가 아닙니다.")
            if float(lot["current_qty"]) < float(row["issued_qty"]):
                raise ValueError(f"원자재 LOT 재고가 부족합니다. LOT ID: {row['material_lot_id']}")

        product = cursor.execute(
            "SELECT shelf_life_days FROM item WHERE item_id = ?",
            (data.product_item_id,),
        ).fetchone()
        expire_date = None
        if product and product["shelf_life_days"]:
            expire_date = data.production_date + timedelta(days=int(product["shelf_life_days"]))

        fg_warehouse = cursor.execute(
            "SELECT warehouse_id FROM warehouse WHERE warehouse_type = 'FG' ORDER BY warehouse_id LIMIT 1"
        ).fetchone()
        if fg_warehouse is None:
            raise ValueError("완제품 창고가 없습니다.")

        lot_id = _next_id(cursor, "lot", "lot_id")
        production_id = _next_id(cursor, "production", "production_id")
        material_id = _next_id(cursor, "production_material", "production_material_id")
        txn_id = _next_id(cursor, "inventory_txn", "txn_id")

        cursor.execute(
            """
            INSERT INTO lot (
                lot_id, lot_no, item_id, warehouse_id, lot_type, initial_qty, current_qty,
                status, received_date, produced_date, expire_date, source_partner_id
            )
            VALUES (?, ?, ?, ?, 'PRODUCTION', ?, ?, 'AVAILABLE', NULL, ?, ?, NULL)
            """,
            (
                lot_id,
                data.output_lot_no.strip(),
                data.product_item_id,
                int(fg_warehouse["warehouse_id"]),
                data.good_qty,
                data.good_qty,
                str(data.production_date),
                str(expire_date) if expire_date else None,
            ),
        )
        cursor.execute(
            """
            INSERT INTO production (
                production_id, production_no, order_id, product_item_id, output_lot_id,
                equipment_id, production_date, start_time, end_time, good_qty, defect_qty, status
            )
            VALUES (?, ?, ?, ?, ?, ?, ?, NULL, NULL, ?, ?, 'COMPLETED')
            """,
            (
                production_id,
                data.production_no.strip(),
                data.order_id,
                data.product_item_id,
                lot_id,
                data.equipment_id,
                str(data.production_date),
                data.good_qty,
                data.defect_qty,
            ),
        )

        for offset, row in enumerate(data.material_rows):
            cursor.execute(
                """
                INSERT INTO production_material (
                    production_material_id, production_id, material_item_id, material_lot_id,
                    required_qty, issued_qty
                )
                VALUES (?, ?, ?, ?, ?, ?)
                """,
                (
                    material_id + offset,
                    production_id,
                    row["material_item_id"],
                    row["material_lot_id"],
                    row["required_qty"],
                    row["issued_qty"],
                ),
            )
            cursor.execute(
                "UPDATE lot SET current_qty = current_qty - ? WHERE lot_id = ?",
                (row["issued_qty"], row["material_lot_id"]),
            )
            cursor.execute(
                """
                INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo)
                SELECT ?, ?, 'ISSUE', lot_id, item_id, warehouse_id, ?, 'PRODUCTION', ?, '생산투입'
                FROM lot WHERE lot_id = ?
                """,
                (txn_id + offset, str(data.production_date), -float(row["issued_qty"]), production_id, row["material_lot_id"]),
            )

        cursor.execute(
            """
            INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo)
            VALUES (?, ?, 'PRODUCTION_IN', ?, ?, ?, ?, 'PRODUCTION', ?, '완제품 입고')
            """,
            (
                txn_id + len(data.material_rows),
                str(data.production_date),
                lot_id,
                data.product_item_id,
                int(fg_warehouse["warehouse_id"]),
                data.good_qty,
                production_id,
            ),
        )
        if data.order_id:
            cursor.execute(
                "UPDATE production_order SET status = 'COMPLETED' WHERE order_id = ?",
                (data.order_id,),
            )
        connection.commit()
        return {"production_id": production_id, "production_no": data.production_no.strip(), "output_lot_id": lot_id, "output_lot_no": data.output_lot_no.strip()}


def register_inspection(data: InspectionInput) -> dict:
    if data.sample_qty < 0 or data.defect_qty < 0:
        raise ValueError("검사수량과 불량수량은 0 이상이어야 합니다.")
    with get_connection() as connection:
        cursor = connection.cursor()
        _ensure_unique(cursor, "quality_inspection", "inspection_no", data.inspection_no, "검사번호")
        inspection_id = _next_id(cursor, "quality_inspection", "inspection_id")
        cursor.execute(
            """
            INSERT INTO quality_inspection (
                inspection_id, inspection_no, inspection_type, lot_id, production_id,
                inspection_date, sample_qty, defect_qty, result, memo
            )
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            """,
            (
                inspection_id,
                data.inspection_no.strip(),
                data.inspection_type,
                data.lot_id,
                data.production_id,
                str(data.inspection_date),
                data.sample_qty,
                data.defect_qty,
                data.result,
                data.memo,
            ),
        )
        if data.result in {"FAIL", "HOLD"}:
            cursor.execute("UPDATE lot SET status = 'HOLD' WHERE lot_id = ?", (data.lot_id,))
        connection.commit()
        return {"inspection_id": inspection_id, "inspection_no": data.inspection_no.strip()}


def create_shipment(data: ShipmentInput) -> dict:
    if not data.detail_rows:
        raise ValueError("출하 LOT를 1개 이상 선택하세요.")
    with get_connection() as connection:
        cursor = connection.cursor()
        _ensure_unique(cursor, "shipment", "shipment_no", data.shipment_no, "출하번호")
        for row in data.detail_rows:
            lot = cursor.execute(
                "SELECT current_qty FROM lot WHERE lot_id = ? AND status = 'AVAILABLE'",
                (row["lot_id"],),
            ).fetchone()
            if lot is None:
                raise ValueError("사용 가능한 완제품 LOT가 아닙니다.")
            if float(lot["current_qty"]) < float(row["qty"]):
                raise ValueError(f"출하 가능 수량이 부족합니다. LOT ID: {row['lot_id']}")

        shipment_id = _next_id(cursor, "shipment", "shipment_id")
        detail_id = _next_id(cursor, "shipment_detail", "shipment_detail_id")
        txn_id = _next_id(cursor, "inventory_txn", "txn_id")
        cursor.execute(
            """
            INSERT INTO shipment (shipment_id, shipment_no, customer_id, shipment_date, status)
            VALUES (?, ?, ?, ?, 'SHIPPED')
            """,
            (shipment_id, data.shipment_no.strip(), data.customer_id, str(data.shipment_date)),
        )
        for offset, row in enumerate(data.detail_rows):
            cursor.execute(
                """
                INSERT INTO shipment_detail (shipment_detail_id, shipment_id, lot_id, item_id, qty)
                VALUES (?, ?, ?, ?, ?)
                """,
                (detail_id + offset, shipment_id, row["lot_id"], row["item_id"], row["qty"]),
            )
            cursor.execute("UPDATE lot SET current_qty = current_qty - ? WHERE lot_id = ?", (row["qty"], row["lot_id"]))
            cursor.execute(
                """
                INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo)
                SELECT ?, ?, 'SHIP', lot_id, item_id, warehouse_id, ?, 'SHIPMENT', ?, '완제품 출하'
                FROM lot WHERE lot_id = ?
                """,
                (txn_id + offset, str(data.shipment_date), -float(row["qty"]), shipment_id, row["lot_id"]),
            )
        connection.commit()
        return {"shipment_id": shipment_id, "shipment_no": data.shipment_no.strip(), "line_count": len(data.detail_rows)}
