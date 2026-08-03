PRAGMA foreign_keys = ON;

INSERT INTO item (item_id, item_code, item_name, item_type, unit, shelf_life_days, safety_stock, is_active) VALUES
    (1, 'FG-JIN-HOT', '진라면 매운맛', 'PRODUCT', 'EA', 180, 1000, 'Y'),
    (2, 'FG-JIN-MILD', '진라면 순한맛', 'PRODUCT', 'EA', 180, 800, 'Y'),
    (3, 'RM-NOODLE-BLOCK', '면 블록', 'MATERIAL', 'EA', 90, 3000, 'Y'),
    (4, 'RM-SOUP-HOT', '매운맛 스프', 'MATERIAL', 'EA', 180, 2500, 'Y'),
    (5, 'RM-SOUP-MILD', '순한맛 스프', 'MATERIAL', 'EA', 180, 2000, 'Y'),
    (6, 'RM-PACK-POUCH', '봉지 포장재', 'MATERIAL', 'EA', NULL, 3000, 'Y'),
    (7, 'RM-BOX', '출하 박스', 'MATERIAL', 'BOX', NULL, 100, 'Y');

INSERT INTO partner (partner_id, partner_code, partner_name, partner_type, is_active) VALUES
    (1, 'SUP-NOODLE', '대한제면', 'SUPPLIER', 'Y'),
    (2, 'SUP-SEASON', '한맛식품', 'SUPPLIER', 'Y'),
    (3, 'SUP-PACK', '푸드패키징', 'SUPPLIER', 'Y'),
    (4, 'CUS-MART', '우리마트 물류센터', 'CUSTOMER', 'Y'),
    (5, 'CUS-CVS', '편의점 DC', 'CUSTOMER', 'Y');

INSERT INTO warehouse (warehouse_id, warehouse_code, warehouse_name, warehouse_type) VALUES
    (1, 'RAW-A', '원자재 창고 A', 'RAW'),
    (2, 'WIP-1', '공정 대기 구역', 'WIP'),
    (3, 'FG-A', '완제품 창고 A', 'FG'),
    (4, 'QC-HOLD', '품질 보류 창고', 'QC');

INSERT INTO work_center (work_center_id, work_center_code, work_center_name) VALUES
    (1, 'LINE-1', '라면 포장 1라인'),
    (2, 'LINE-2', '라면 포장 2라인');

INSERT INTO equipment (equipment_id, equipment_code, equipment_name, work_center_id, status) VALUES
    (1, 'MIX-01', '스프 투입기 1호', 1, 'RUN'),
    (2, 'PACK-01', '포장기 1호', 1, 'RUN'),
    (3, 'PACK-02', '포장기 2호', 2, 'IDLE');

INSERT INTO bom_header (bom_id, product_item_id, version, yield_qty, is_active) VALUES
    (1, 1, 'HOT-2026A', 1, 'Y'),
    (2, 2, 'MILD-2026A', 1, 'Y');

INSERT INTO bom_detail (bom_detail_id, bom_id, material_item_id, qty_per, loss_rate) VALUES
    (1, 1, 3, 1, 0.01),
    (2, 1, 4, 1, 0.01),
    (3, 1, 6, 1, 0.005),
    (4, 2, 3, 1, 0.01),
    (5, 2, 5, 1, 0.01),
    (6, 2, 6, 1, 0.005);

INSERT INTO lot (lot_id, lot_no, item_id, warehouse_id, lot_type, initial_qty, current_qty, status, received_date, produced_date, expire_date, source_partner_id) VALUES
    (1, 'RM-NOODLE-20260701-001', 3, 1, 'RECEIPT', 12000, 4410, 'AVAILABLE', '2026-07-01', NULL, '2026-10-01', 1),
    (2, 'RM-SOUP-HOT-20260701-001', 4, 1, 'RECEIPT', 9000, 3440, 'AVAILABLE', '2026-07-01', NULL, '2026-12-31', 2),
    (3, 'RM-SOUP-MILD-20260701-001', 5, 1, 'RECEIPT', 7000, 4980, 'AVAILABLE', '2026-07-01', NULL, '2026-12-31', 2),
    (4, 'RM-PACK-20260701-001', 6, 1, 'RECEIPT', 15000, 7400, 'AVAILABLE', '2026-07-01', NULL, NULL, 3),
    (5, 'FG-HOT-20260710-001', 1, 3, 'PRODUCTION', 3000, 1800, 'AVAILABLE', NULL, '2026-07-10', '2027-01-06', NULL),
    (6, 'FG-MILD-20260711-001', 2, 3, 'PRODUCTION', 2000, 1500, 'AVAILABLE', NULL, '2026-07-11', '2027-01-07', NULL),
    (7, 'FG-HOT-20260712-001', 1, 3, 'PRODUCTION', 2500, 2500, 'AVAILABLE', NULL, '2026-07-12', '2027-01-08', NULL);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (1, '2026-07-01', 'RECEIPT', 1, 3, 1, 12000, 'RECEIPT', 1, '초기 원자재 입고'),
    (2, '2026-07-01', 'RECEIPT', 2, 4, 1, 9000, 'RECEIPT', 2, '초기 원자재 입고'),
    (3, '2026-07-01', 'RECEIPT', 3, 5, 1, 7000, 'RECEIPT', 3, '초기 원자재 입고'),
    (4, '2026-07-01', 'RECEIPT', 4, 6, 1, 15000, 'RECEIPT', 4, '초기 원자재 입고');

INSERT INTO production_order (order_id, order_no, product_item_id, work_center_id, plan_date, due_date, order_qty, status, priority) VALUES
    (1, 'WO-20260710-001', 1, 1, '2026-07-10', '2026-07-10', 3000, 'COMPLETED', 2),
    (2, 'WO-20260711-001', 2, 1, '2026-07-11', '2026-07-11', 2000, 'COMPLETED', 3),
    (3, 'WO-20260712-001', 1, 2, '2026-07-12', '2026-07-12', 2500, 'COMPLETED', 2),
    (4, 'WO-20260720-001', 1, 1, '2026-07-20', '2026-07-20', 1500, 'RELEASED', 1);

INSERT INTO production (production_id, production_no, order_id, product_item_id, output_lot_id, equipment_id, production_date, start_time, end_time, good_qty, defect_qty, status) VALUES
    (1, 'PRD-20260710-001', 1, 1, 5, 2, '2026-07-10', '08:30', '11:10', 3000, 35, 'COMPLETED'),
    (2, 'PRD-20260711-001', 2, 2, 6, 2, '2026-07-11', '09:00', '10:45', 2000, 18, 'COMPLETED'),
    (3, 'PRD-20260712-001', 3, 1, 7, 3, '2026-07-12', '13:00', '15:15', 2500, 22, 'COMPLETED');

INSERT INTO production_material (production_material_id, production_id, material_item_id, material_lot_id, required_qty, issued_qty) VALUES
    (1, 1, 3, 1, 3030, 3030),
    (2, 1, 4, 2, 3030, 3030),
    (3, 1, 6, 4, 3015, 3015),
    (4, 2, 3, 1, 2020, 2020),
    (5, 2, 5, 3, 2020, 2020),
    (6, 2, 6, 4, 2010, 2010),
    (7, 3, 3, 1, 2525, 2525),
    (8, 3, 4, 2, 2525, 2525),
    (9, 3, 6, 4, 2515, 2515);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (5, '2026-07-10', 'ISSUE', 1, 3, 1, -3030, 'PRODUCTION', 1, '생산투입'),
    (6, '2026-07-10', 'ISSUE', 2, 4, 1, -3030, 'PRODUCTION', 1, '생산투입'),
    (7, '2026-07-10', 'ISSUE', 4, 6, 1, -3015, 'PRODUCTION', 1, '생산투입'),
    (8, '2026-07-10', 'PRODUCTION_IN', 5, 1, 3, 3000, 'PRODUCTION', 1, '완제품 입고'),
    (9, '2026-07-11', 'ISSUE', 1, 3, 1, -2020, 'PRODUCTION', 2, '생산투입'),
    (10, '2026-07-11', 'ISSUE', 3, 5, 1, -2020, 'PRODUCTION', 2, '생산투입'),
    (11, '2026-07-11', 'ISSUE', 4, 6, 1, -2010, 'PRODUCTION', 2, '생산투입'),
    (12, '2026-07-11', 'PRODUCTION_IN', 6, 2, 3, 2000, 'PRODUCTION', 2, '완제품 입고'),
    (13, '2026-07-12', 'ISSUE', 1, 3, 1, -2525, 'PRODUCTION', 3, '생산투입'),
    (14, '2026-07-12', 'ISSUE', 2, 4, 1, -2525, 'PRODUCTION', 3, '생산투입'),
    (15, '2026-07-12', 'ISSUE', 4, 6, 1, -2515, 'PRODUCTION', 3, '생산투입'),
    (16, '2026-07-12', 'PRODUCTION_IN', 7, 1, 3, 2500, 'PRODUCTION', 3, '완제품 입고');

INSERT INTO quality_inspection (inspection_id, inspection_no, inspection_type, lot_id, production_id, inspection_date, sample_qty, defect_qty, result, memo) VALUES
    (1, 'IQC-20260701-001', 'INCOMING', 1, NULL, '2026-07-01', 80, 0, 'PASS', '면 블록 입고검사 합격'),
    (2, 'IQC-20260701-002', 'INCOMING', 2, NULL, '2026-07-01', 80, 1, 'PASS', '스프 입고검사 합격'),
    (3, 'FQC-20260710-001', 'FINAL', 5, 1, '2026-07-10', 120, 1, 'PASS', '중량/실링 검사 합격'),
    (4, 'FQC-20260711-001', 'FINAL', 6, 2, '2026-07-11', 100, 0, 'PASS', '최종검사 합격'),
    (5, 'FQC-20260712-001', 'FINAL', 7, 3, '2026-07-12', 120, 2, 'PASS', '최종검사 합격');

INSERT INTO shipment (shipment_id, shipment_no, customer_id, shipment_date, status) VALUES
    (1, 'SHP-20260713-001', 4, '2026-07-13', 'SHIPPED'),
    (2, 'SHP-20260714-001', 5, '2026-07-14', 'SHIPPED');

INSERT INTO shipment_detail (shipment_detail_id, shipment_id, lot_id, item_id, qty) VALUES
    (1, 1, 5, 1, 1200),
    (2, 2, 6, 2, 500);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (17, '2026-07-13', 'SHIP', 5, 1, 3, -1200, 'SHIPMENT', 1, '완제품 출하'),
    (18, '2026-07-14', 'SHIP', 6, 2, 3, -500, 'SHIPMENT', 2, '완제품 출하');

-- 추가 샘플 데이터: 화면별 업무 상태가 고르게 보이도록 최소 범위만 확장한다.
INSERT INTO lot (lot_id, lot_no, item_id, warehouse_id, lot_type, initial_qty, current_qty, status, received_date, produced_date, expire_date, source_partner_id) VALUES
    (8, 'RM-BOX-20260708-001', 7, 1, 'RECEIPT', 80, 40, 'AVAILABLE', '2026-07-08', NULL, NULL, 3),
    (9, 'RM-NOODLE-20260708-001', 3, 1, 'RECEIPT', 6000, 2465, 'AVAILABLE', '2026-07-08', NULL, '2026-10-08', 1),
    (10, 'RM-SOUP-HOT-20260708-001', 4, 1, 'RECEIPT', 5000, 3485, 'AVAILABLE', '2026-07-08', NULL, '2027-01-04', 2),
    (11, 'RM-SOUP-MILD-20260708-001', 5, 1, 'RECEIPT', 4000, 1980, 'AVAILABLE', '2026-07-08', NULL, '2027-01-04', 2),
    (12, 'FG-MILD-20260715-001', 2, 3, 'PRODUCTION', 2000, 1600, 'AVAILABLE', NULL, '2026-07-15', '2027-01-11', NULL),
    (13, 'FG-HOT-20260716-001', 1, 4, 'PRODUCTION', 1500, 1500, 'HOLD', NULL, '2026-07-16', '2027-01-12', NULL);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (19, '2026-07-08', 'RECEIPT', 8, 7, 1, 80, 'RECEIPT', 8, '출하 박스 소량 입고'),
    (20, '2026-07-08', 'RECEIPT', 9, 3, 1, 6000, 'RECEIPT', 9, '면 블록 추가 입고'),
    (21, '2026-07-08', 'RECEIPT', 10, 4, 1, 5000, 'RECEIPT', 10, '매운맛 스프 추가 입고'),
    (22, '2026-07-08', 'RECEIPT', 11, 5, 1, 4000, 'RECEIPT', 11, '순한맛 스프 추가 입고');

INSERT INTO production_order (order_id, order_no, product_item_id, work_center_id, plan_date, due_date, order_qty, status, priority) VALUES
    (5, 'WO-20260715-001', 2, 1, '2026-07-15', '2026-07-15', 2000, 'COMPLETED', 2),
    (6, 'WO-20260716-001', 1, 2, '2026-07-16', '2026-07-16', 1500, 'COMPLETED', 1),
    (7, 'WO-20260720-002', 2, 1, '2026-07-20', '2026-07-21', 1200, 'PLANNED', 3),
    (8, 'WO-20260718-001', 1, 2, '2026-07-18', '2026-07-18', 800, 'CANCELED', 5);

INSERT INTO production (production_id, production_no, order_id, product_item_id, output_lot_id, equipment_id, production_date, start_time, end_time, good_qty, defect_qty, status) VALUES
    (4, 'PRD-20260715-001', 5, 2, 12, 2, '2026-07-15', '08:20', '10:05', 2000, 12, 'COMPLETED'),
    (5, 'PRD-20260716-001', 6, 1, 13, 3, '2026-07-16', '14:00', '15:35', 1500, 48, 'COMPLETED');

INSERT INTO production_material (production_material_id, production_id, material_item_id, material_lot_id, required_qty, issued_qty) VALUES
    (10, 4, 3, 9, 2020, 2020),
    (11, 4, 5, 11, 2020, 2020),
    (12, 4, 6, 4, 2010, 2010),
    (13, 5, 3, 9, 1515, 1515),
    (14, 5, 4, 10, 1515, 1515),
    (15, 5, 6, 4, 1508, 1508);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (23, '2026-07-15', 'ISSUE', 9, 3, 1, -2020, 'PRODUCTION', 4, '생산투입'),
    (24, '2026-07-15', 'ISSUE', 11, 5, 1, -2020, 'PRODUCTION', 4, '생산투입'),
    (25, '2026-07-15', 'ISSUE', 4, 6, 1, -2010, 'PRODUCTION', 4, '생산투입'),
    (26, '2026-07-15', 'PRODUCTION_IN', 12, 2, 3, 2000, 'PRODUCTION', 4, '완제품 입고'),
    (27, '2026-07-16', 'ISSUE', 9, 3, 1, -1515, 'PRODUCTION', 5, '생산투입'),
    (28, '2026-07-16', 'ISSUE', 10, 4, 1, -1515, 'PRODUCTION', 5, '생산투입'),
    (29, '2026-07-16', 'ISSUE', 4, 6, 1, -1508, 'PRODUCTION', 5, '생산투입'),
    (30, '2026-07-16', 'PRODUCTION_IN', 13, 1, 4, 1500, 'PRODUCTION', 5, '완제품 품질보류 입고');

INSERT INTO quality_inspection (inspection_id, inspection_no, inspection_type, lot_id, production_id, inspection_date, sample_qty, defect_qty, result, memo) VALUES
    (6, 'IQC-20260708-001', 'INCOMING', 8, NULL, '2026-07-08', 20, 0, 'PASS', '박스 입고검사 합격'),
    (7, 'FQC-20260715-001', 'FINAL', 12, 4, '2026-07-15', 100, 0, 'PASS', '최종검사 합격'),
    (8, 'FQC-20260716-001', 'FINAL', 13, 5, '2026-07-16', 120, 5, 'HOLD', '실링 강도 재확인 필요'),
    (9, 'PQC-20260716-001', 'PROCESS', 13, 5, '2026-07-16', 60, 4, 'FAIL', '라인 2 포장 조건 점검 필요');

INSERT INTO shipment (shipment_id, shipment_no, customer_id, shipment_date, status) VALUES
    (3, 'SHP-20260716-001', 4, '2026-07-16', 'SHIPPED'),
    (4, 'SHP-20260717-001', 5, '2026-07-17', 'SHIPPED'),
    (5, 'SHP-20260719-001', 4, '2026-07-19', 'PLANNED');

INSERT INTO shipment_detail (shipment_detail_id, shipment_id, lot_id, item_id, qty) VALUES
    (3, 3, 7, 1, 800),
    (4, 4, 12, 2, 400);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (31, '2026-07-16', 'SHIP', 7, 1, 3, -800, 'SHIPMENT', 3, '완제품 출하'),
    (32, '2026-07-17', 'SHIP', 12, 2, 3, -400, 'SHIPMENT', 4, '완제품 출하');

-- 신라면 치즈맛 최소 세트: 제품 1개, 재료 1개, BOM과 생산 흐름 1건만 추가한다.
INSERT INTO item (item_id, item_code, item_name, item_type, unit, shelf_life_days, safety_stock, is_active) VALUES
    (8, 'FG-SHIN-CHEESE', '신라면 치즈맛', 'PRODUCT', 'EA', 180, 600, 'Y'),
    (9, 'RM-SOUP-CHEESE', '치즈맛 스프', 'MATERIAL', 'EA', 180, 1200, 'Y');

INSERT INTO bom_header (bom_id, product_item_id, version, yield_qty, is_active) VALUES
    (3, 8, 'CHEESE-2026A', 1, 'Y');

INSERT INTO bom_detail (bom_detail_id, bom_id, material_item_id, qty_per, loss_rate) VALUES
    (7, 3, 3, 1, 0.01),
    (8, 3, 9, 1, 0.01),
    (9, 3, 6, 1, 0.005);

INSERT INTO lot (lot_id, lot_no, item_id, warehouse_id, lot_type, initial_qty, current_qty, status, received_date, produced_date, expire_date, source_partner_id) VALUES
    (14, 'RM-SOUP-CHEESE-20260709-001', 9, 1, 'RECEIPT', 4000, 2990, 'AVAILABLE', '2026-07-09', NULL, '2027-01-05', 2),
    (15, 'FG-SHIN-CHEESE-20260717-001', 8, 3, 'PRODUCTION', 1000, 700, 'AVAILABLE', NULL, '2026-07-17', '2027-01-13', NULL);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (33, '2026-07-09', 'RECEIPT', 14, 9, 1, 4000, 'RECEIPT', 14, '치즈맛 스프 입고');

INSERT INTO production_order (order_id, order_no, product_item_id, work_center_id, plan_date, due_date, order_qty, status, priority) VALUES
    (9, 'WO-20260717-001', 8, 1, '2026-07-17', '2026-07-17', 1000, 'COMPLETED', 2);

INSERT INTO production (production_id, production_no, order_id, product_item_id, output_lot_id, equipment_id, production_date, start_time, end_time, good_qty, defect_qty, status) VALUES
    (6, 'PRD-20260717-001', 9, 8, 15, 2, '2026-07-17', '10:30', '11:40', 1000, 9, 'COMPLETED');

INSERT INTO production_material (production_material_id, production_id, material_item_id, material_lot_id, required_qty, issued_qty) VALUES
    (16, 6, 3, 9, 1010, 1010),
    (17, 6, 9, 14, 1010, 1010),
    (18, 6, 6, 4, 1005, 1005);

INSERT INTO inventory_txn (txn_id, txn_date, txn_type, lot_id, item_id, warehouse_id, qty, ref_type, ref_id, memo) VALUES
    (34, '2026-07-17', 'ISSUE', 9, 3, 1, -1010, 'PRODUCTION', 6, '신라면 치즈맛 생산투입'),
    (35, '2026-07-17', 'ISSUE', 14, 9, 1, -1010, 'PRODUCTION', 6, '신라면 치즈맛 생산투입'),
    (36, '2026-07-17', 'ISSUE', 4, 6, 1, -1005, 'PRODUCTION', 6, '신라면 치즈맛 생산투입'),
    (37, '2026-07-17', 'PRODUCTION_IN', 15, 8, 3, 1000, 'PRODUCTION', 6, '신라면 치즈맛 완제품 입고');

INSERT INTO quality_inspection (inspection_id, inspection_no, inspection_type, lot_id, production_id, inspection_date, sample_qty, defect_qty, result, memo) VALUES
    (10, 'FQC-20260717-001', 'FINAL', 15, 6, '2026-07-17', 80, 0, 'PASS', '신라면 치즈맛 최종검사 합격');

INSERT INTO shipment_detail (shipment_detail_id, shipment_id, lot_id, item_id, qty) VALUES
    (5, 5, 15, 8, 300);

-- 추가 생산/출하분이 기존 LOT 현재고에도 반영되도록 보정한다.
UPDATE lot SET current_qty = 2887 WHERE lot_id = 4;
UPDATE lot SET current_qty = 1455 WHERE lot_id = 9;
UPDATE lot SET current_qty = 1500 WHERE lot_id = 10;
UPDATE lot SET current_qty = 1980 WHERE lot_id = 11;
UPDATE lot SET current_qty = 1200 WHERE lot_id = 12;
UPDATE lot SET current_qty = 700 WHERE lot_id = 15;
