PRAGMA foreign_keys = ON;

INSERT INTO item (item_id, item_code, item_name, item_type, unit, is_active) VALUES
    (1, 'FG-RAMEN-001', '봉지라면 매운맛', 'PRODUCT', 'EA', 'Y'),
    (2, 'FG-RAMEN-002', '봉지라면 순한맛', 'PRODUCT', 'EA', 'Y'),
    (3, 'RM-NOODLE-001', '면 블록', 'MATERIAL', 'EA', 'Y'),
    (4, 'RM-SOUP-001', '매운맛 스프', 'MATERIAL', 'EA', 'Y'),
    (5, 'RM-SOUP-002', '순한맛 스프', 'MATERIAL', 'EA', 'Y'),
    (6, 'RM-PACK-001', '봉지 포장재', 'MATERIAL', 'EA', 'Y');

INSERT INTO lot (lot_id, lot_no, item_id, lot_type, qty, received_date, produced_date, expire_date) VALUES
    (1, 'RM-NOODLE-20260701-001', 3, 'RECEIPT', 10000, '2026-07-01', NULL, '2026-10-01'),
    (2, 'RM-SOUP-HOT-20260701-001', 4, 'RECEIPT', 8000, '2026-07-01', NULL, '2026-12-31'),
    (3, 'RM-SOUP-MILD-20260701-001', 5, 'RECEIPT', 6000, '2026-07-01', NULL, '2026-12-31'),
    (4, 'RM-PACK-20260701-001', 6, 'RECEIPT', 12000, '2026-07-01', NULL, NULL),
    (5, 'FG-RAMEN-HOT-20260710-001', 1, 'PRODUCTION', 3000, NULL, '2026-07-10', '2027-01-10'),
    (6, 'FG-RAMEN-MILD-20260711-001', 2, 'PRODUCTION', 2000, NULL, '2026-07-11', '2027-01-11'),
    (7, 'FG-RAMEN-HOT-20260712-001', 1, 'PRODUCTION', 2500, NULL, '2026-07-12', '2027-01-12');

INSERT INTO production (production_id, production_no, item_id, output_lot_id, production_date, qty, status) VALUES
    (1, 'PRD-20260710-001', 1, 5, '2026-07-10', 3000, 'COMPLETED'),
    (2, 'PRD-20260711-001', 2, 6, '2026-07-11', 2000, 'COMPLETED'),
    (3, 'PRD-20260712-001', 1, 7, '2026-07-12', 2500, 'COMPLETED');

INSERT INTO production_material (production_material_id, production_id, material_item_id, material_lot_id, qty) VALUES
    (1, 1, 3, 1, 3000),
    (2, 1, 4, 2, 3000),
    (3, 1, 6, 4, 3000),
    (4, 2, 3, 1, 2000),
    (5, 2, 5, 3, 2000),
    (6, 2, 6, 4, 2000),
    (7, 3, 3, 1, 2500),
    (8, 3, 4, 2, 2500),
    (9, 3, 6, 4, 2500);
