from __future__ import annotations

from dataclasses import dataclass
from datetime import date
import sqlite3

from src.db import get_connection
from src.queries import lot_no_exists, production_no_exists


@dataclass
class ProductionRegistration:
    product_item_id: int
    output_lot_no: str
    production_no: str
    production_date: date
    qty: float
    expire_date: date | None
    material_rows: list[dict]


def validate_registration(data: ProductionRegistration) -> list[str]:
    errors: list[str] = []

    if not data.output_lot_no.strip():
        errors.append("완제품 LOT 번호를 입력하세요.")
    if not data.production_no.strip():
        errors.append("생산번호를 입력하세요.")
    if data.qty <= 0:
        errors.append("생산수량은 0보다 커야 합니다.")
    if not data.material_rows:
        errors.append("투입할 원자재 LOT를 1개 이상 선택하세요.")

    material_lot_ids = [row["material_lot_id"] for row in data.material_rows]
    if len(material_lot_ids) != len(set(material_lot_ids)):
        errors.append("동일한 원자재 LOT를 중복 선택할 수 없습니다.")

    for row in data.material_rows:
        if row["qty"] <= 0:
            errors.append("원자재 투입수량은 모두 0보다 커야 합니다.")
            break

    if lot_no_exists(data.output_lot_no.strip()):
        errors.append("이미 존재하는 완제품 LOT 번호입니다.")
    if production_no_exists(data.production_no.strip()):
        errors.append("이미 존재하는 생산번호입니다.")

    return errors


def register_production(data: ProductionRegistration) -> dict:
    errors = validate_registration(data)
    if errors:
        raise ValueError("\n".join(errors))

    try:
        with get_connection() as connection:
            cursor = connection.cursor()

            next_lot_id = cursor.execute("SELECT COALESCE(MAX(lot_id), 0) + 1 FROM lot").fetchone()[0]
            next_production_id = cursor.execute(
                "SELECT COALESCE(MAX(production_id), 0) + 1 FROM production"
            ).fetchone()[0]
            next_material_id = cursor.execute(
                "SELECT COALESCE(MAX(production_material_id), 0) + 1 FROM production_material"
            ).fetchone()[0]

            # 완제품 LOT와 생산실적은 하나의 업무로 묶이므로 같은 트랜잭션에서 저장한다.
            cursor.execute(
                """
                INSERT INTO lot (
                    lot_id,
                    lot_no,
                    item_id,
                    lot_type,
                    qty,
                    received_date,
                    produced_date,
                    expire_date
                )
                VALUES (?, ?, ?, 'PRODUCTION', ?, NULL, ?, ?)
                """,
                (
                    next_lot_id,
                    data.output_lot_no.strip(),
                    data.product_item_id,
                    data.qty,
                    str(data.production_date),
                    str(data.expire_date) if data.expire_date else None,
                ),
            )

            cursor.execute(
                """
                INSERT INTO production (
                    production_id,
                    production_no,
                    item_id,
                    output_lot_id,
                    production_date,
                    qty,
                    status
                )
                VALUES (?, ?, ?, ?, ?, ?, 'COMPLETED')
                """,
                (
                    next_production_id,
                    data.production_no.strip(),
                    data.product_item_id,
                    next_lot_id,
                    str(data.production_date),
                    data.qty,
                ),
            )

            # 생산 1건에 원자재가 여러 개 들어가므로 production_material에는 여러 행을 저장한다.
            for offset, row in enumerate(data.material_rows):
                cursor.execute(
                    """
                    INSERT INTO production_material (
                        production_material_id,
                        production_id,
                        material_item_id,
                        material_lot_id,
                        qty
                    )
                    VALUES (?, ?, ?, ?, ?)
                    """,
                    (
                        next_material_id + offset,
                        next_production_id,
                        row["material_item_id"],
                        row["material_lot_id"],
                        row["qty"],
                    ),
                )

            connection.commit()

            return {
                "production_id": next_production_id,
                "production_no": data.production_no.strip(),
                "output_lot_id": next_lot_id,
                "output_lot_no": data.output_lot_no.strip(),
                "material_count": len(data.material_rows),
            }
    except sqlite3.IntegrityError as exc:
        raise ValueError("데이터베이스 제약조건을 만족하지 못해 저장하지 못했습니다.") from exc
