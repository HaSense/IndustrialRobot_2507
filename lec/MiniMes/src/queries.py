from __future__ import annotations

from src.db import fetch_all, fetch_dataframe, fetch_one


def table_counts():
    return fetch_dataframe(
        """
        SELECT 'item' AS table_name, COUNT(*) AS row_count FROM item
        UNION ALL
        SELECT 'lot' AS table_name, COUNT(*) AS row_count FROM lot
        UNION ALL
        SELECT 'production' AS table_name, COUNT(*) AS row_count FROM production
        UNION ALL
        SELECT 'production_material' AS table_name, COUNT(*) AS row_count FROM production_material
        """
    )


def table_list():
    return fetch_dataframe(
        """
        SELECT name AS table_name
        FROM sqlite_master
        WHERE type = 'table'
        ORDER BY name
        """
    )


def items(keyword: str = "", item_type: str = "전체"):
    params: list[str] = []
    where = ["1 = 1"]

    if keyword:
        where.append("(i.item_code LIKE ? OR i.item_name LIKE ?)")
        params.extend([f"%{keyword}%", f"%{keyword}%"])

    if item_type != "전체":
        where.append("i.item_type = ?")
        params.append(item_type)

    return fetch_dataframe(
        f"""
        SELECT
            i.item_id,
            i.item_code,
            i.item_name,
            i.item_type,
            i.unit,
            i.is_active,
            COUNT(DISTINCT l.lot_id) AS lot_count,
            COUNT(DISTINCT pm.production_material_id) AS material_use_count
        FROM item AS i
        LEFT JOIN lot AS l
            ON i.item_id = l.item_id
        LEFT JOIN production_material AS pm
            ON i.item_id = pm.material_item_id
        WHERE {' AND '.join(where)}
        GROUP BY
            i.item_id,
            i.item_code,
            i.item_name,
            i.item_type,
            i.unit,
            i.is_active
        ORDER BY i.item_type, i.item_code
        """,
        tuple(params),
    )


def item_type_counts():
    return fetch_dataframe(
        """
        SELECT item_type, COUNT(*) AS item_count
        FROM item
        GROUP BY item_type
        ORDER BY item_type
        """
    )


def lots(keyword: str = "", lot_type: str = "전체", item_id: int | None = None):
    params: list[object] = []
    where = ["1 = 1"]

    if keyword:
        where.append("l.lot_no LIKE ?")
        params.append(f"%{keyword}%")

    if lot_type != "전체":
        where.append("l.lot_type = ?")
        params.append(lot_type)

    if item_id:
        where.append("l.item_id = ?")
        params.append(item_id)

    return fetch_dataframe(
        f"""
        SELECT
            l.lot_id,
            l.lot_no,
            i.item_code,
            i.item_name,
            i.item_type,
            l.lot_type,
            l.qty,
            l.received_date,
            l.produced_date,
            l.expire_date
        FROM lot AS l
        JOIN item AS i
            ON l.item_id = i.item_id
        WHERE {' AND '.join(where)}
        ORDER BY
            COALESCE(l.received_date, l.produced_date),
            l.lot_no
        """,
        tuple(params),
    )


def lots_for_select(lot_type: str | None = None):
    params: tuple = ()
    where = ""
    if lot_type:
        where = "WHERE l.lot_type = ?"
        params = (lot_type,)

    return fetch_all(
        f"""
        SELECT
            l.lot_id,
            l.lot_no,
            l.item_id,
            i.item_name,
            l.lot_type,
            l.qty
        FROM lot AS l
        JOIN item AS i
            ON l.item_id = i.item_id
        {where}
        ORDER BY l.lot_no
        """,
        params,
    )


def active_items_for_select(item_type: str | None = None):
    params: tuple = ()
    where = "WHERE is_active = 'Y'"
    if item_type:
        where += " AND item_type = ?"
        params = (item_type,)

    return fetch_all(
        f"""
        SELECT item_id, item_code, item_name, item_type, unit
        FROM item
        {where}
        ORDER BY item_code
        """,
        params,
    )


def productions(keyword: str = "", date_from=None, date_to=None):
    params: list[object] = []
    where = ["1 = 1"]

    if keyword:
        where.append("(p.production_no LIKE ? OR output_lot.lot_no LIKE ? OR product.item_name LIKE ?)")
        params.extend([f"%{keyword}%", f"%{keyword}%", f"%{keyword}%"])

    if date_from:
        where.append("p.production_date >= ?")
        params.append(str(date_from))

    if date_to:
        where.append("p.production_date <= ?")
        params.append(str(date_to))

    return fetch_dataframe(
        f"""
        SELECT
            p.production_id,
            p.production_no,
            p.production_date,
            product.item_code AS product_code,
            product.item_name AS product_name,
            output_lot.lot_no AS output_lot_no,
            p.qty AS production_qty,
            p.status,
            COUNT(pm.production_material_id) AS material_row_count
        FROM production AS p
        JOIN item AS product
            ON p.item_id = product.item_id
        JOIN lot AS output_lot
            ON p.output_lot_id = output_lot.lot_id
        LEFT JOIN production_material AS pm
            ON p.production_id = pm.production_id
        WHERE {' AND '.join(where)}
        GROUP BY
            p.production_id,
            p.production_no,
            p.production_date,
            product.item_code,
            product.item_name,
            output_lot.lot_no,
            p.qty,
            p.status
        ORDER BY p.production_date, p.production_no
        """,
        tuple(params),
    )


def production_materials(production_id: int):
    return fetch_dataframe(
        """
        SELECT
            p.production_no,
            material.item_code AS material_code,
            material.item_name AS material_name,
            material_lot.lot_no AS material_lot_no,
            material_lot.qty AS material_lot_qty,
            pm.qty AS used_qty
        FROM production_material AS pm
        JOIN production AS p
            ON pm.production_id = p.production_id
        JOIN item AS material
            ON pm.material_item_id = material.item_id
        JOIN lot AS material_lot
            ON pm.material_lot_id = material_lot.lot_id
        WHERE pm.production_id = ?
        ORDER BY material.item_code, material_lot.lot_no
        """,
        (production_id,),
    )


def production_detail(production_id: int):
    return fetch_one(
        """
        SELECT
            p.production_id,
            p.production_no,
            p.production_date,
            product.item_code AS product_code,
            product.item_name AS product_name,
            output_lot.lot_no AS output_lot_no,
            output_lot.qty AS output_lot_qty,
            output_lot.expire_date AS output_expire_date,
            p.qty AS production_qty,
            p.status
        FROM production AS p
        JOIN item AS product
            ON p.item_id = product.item_id
        JOIN lot AS output_lot
            ON p.output_lot_id = output_lot.lot_id
        WHERE p.production_id = ?
        """,
        (production_id,),
    )


def forward_trace(material_lot_id: int):
    return fetch_dataframe(
        """
        SELECT
            material_lot.lot_no AS material_lot_no,
            material_item.item_name AS material_name,
            pm.qty AS used_qty,
            p.production_no,
            p.production_date,
            p.qty AS production_qty,
            output_lot.lot_no AS output_lot_no,
            output_item.item_name AS output_item_name,
            output_lot.qty AS output_lot_qty
        FROM production_material AS pm
        JOIN lot AS material_lot
            ON pm.material_lot_id = material_lot.lot_id
        JOIN item AS material_item
            ON pm.material_item_id = material_item.item_id
        JOIN production AS p
            ON pm.production_id = p.production_id
        JOIN lot AS output_lot
            ON p.output_lot_id = output_lot.lot_id
        JOIN item AS output_item
            ON p.item_id = output_item.item_id
        WHERE pm.material_lot_id = ?
        ORDER BY p.production_date, p.production_no
        """,
        (material_lot_id,),
    )


def reverse_trace(output_lot_id: int):
    return fetch_dataframe(
        """
        SELECT
            output_lot.lot_no AS output_lot_no,
            output_item.item_name AS output_item_name,
            p.production_no,
            p.production_date,
            p.qty AS production_qty,
            material_lot.lot_no AS material_lot_no,
            material_item.item_name AS material_name,
            pm.qty AS used_qty,
            material_lot.qty AS material_lot_qty
        FROM production AS p
        JOIN lot AS output_lot
            ON p.output_lot_id = output_lot.lot_id
        JOIN item AS output_item
            ON p.item_id = output_item.item_id
        JOIN production_material AS pm
            ON p.production_id = pm.production_id
        JOIN lot AS material_lot
            ON pm.material_lot_id = material_lot.lot_id
        JOIN item AS material_item
            ON pm.material_item_id = material_item.item_id
        WHERE p.output_lot_id = ?
        ORDER BY material_item.item_code, material_lot.lot_no
        """,
        (output_lot_id,),
    )


def production_by_date():
    return fetch_dataframe(
        """
        SELECT production_date, SUM(qty) AS production_qty, COUNT(*) AS production_count
        FROM production
        GROUP BY production_date
        ORDER BY production_date
        """
    )


def production_by_item():
    return fetch_dataframe(
        """
        SELECT
            i.item_code,
            i.item_name,
            SUM(p.qty) AS production_qty,
            COUNT(*) AS production_count
        FROM production AS p
        JOIN item AS i
            ON p.item_id = i.item_id
        GROUP BY i.item_id, i.item_code, i.item_name
        ORDER BY production_qty DESC, i.item_code
        """
    )


def lot_use_counts():
    return fetch_dataframe(
        """
        SELECT
            l.lot_no,
            i.item_name,
            l.lot_type,
            COUNT(pm.production_material_id) AS material_use_count
        FROM lot AS l
        JOIN item AS i
            ON l.item_id = i.item_id
        LEFT JOIN production_material AS pm
            ON l.lot_id = pm.material_lot_id
        GROUP BY l.lot_id, l.lot_no, i.item_name, l.lot_type
        ORDER BY material_use_count DESC, l.lot_no
        """
    )


def next_id(table_name: str, id_column: str) -> int:
    row = fetch_one(f"SELECT COALESCE(MAX({id_column}), 0) + 1 AS next_id FROM {table_name}")
    return int(row["next_id"])


def lot_no_exists(lot_no: str) -> bool:
    row = fetch_one("SELECT lot_id FROM lot WHERE lot_no = ?", (lot_no,))
    return row is not None


def production_no_exists(production_no: str) -> bool:
    row = fetch_one("SELECT production_id FROM production WHERE production_no = ?", (production_no,))
    return row is not None
