PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS shipment_detail;
DROP TABLE IF EXISTS shipment;
DROP TABLE IF EXISTS quality_inspection;
DROP TABLE IF EXISTS production_material;
DROP TABLE IF EXISTS production;
DROP TABLE IF EXISTS production_order;
DROP TABLE IF EXISTS inventory_txn;
DROP TABLE IF EXISTS lot;
DROP TABLE IF EXISTS bom_detail;
DROP TABLE IF EXISTS bom_header;
DROP TABLE IF EXISTS equipment;
DROP TABLE IF EXISTS work_center;
DROP TABLE IF EXISTS warehouse;
DROP TABLE IF EXISTS partner;
DROP TABLE IF EXISTS item;

CREATE TABLE item (
    item_id INTEGER PRIMARY KEY,
    item_code TEXT NOT NULL UNIQUE,
    item_name TEXT NOT NULL,
    item_type TEXT NOT NULL CHECK (item_type IN ('PRODUCT', 'MATERIAL')),
    unit TEXT NOT NULL,
    shelf_life_days INTEGER,
    safety_stock REAL NOT NULL DEFAULT 0 CHECK (safety_stock >= 0),
    is_active TEXT NOT NULL DEFAULT 'Y' CHECK (is_active IN ('Y', 'N'))
);

CREATE TABLE partner (
    partner_id INTEGER PRIMARY KEY,
    partner_code TEXT NOT NULL UNIQUE,
    partner_name TEXT NOT NULL,
    partner_type TEXT NOT NULL CHECK (partner_type IN ('SUPPLIER', 'CUSTOMER')),
    is_active TEXT NOT NULL DEFAULT 'Y' CHECK (is_active IN ('Y', 'N'))
);

CREATE TABLE warehouse (
    warehouse_id INTEGER PRIMARY KEY,
    warehouse_code TEXT NOT NULL UNIQUE,
    warehouse_name TEXT NOT NULL,
    warehouse_type TEXT NOT NULL CHECK (warehouse_type IN ('RAW', 'WIP', 'FG', 'QC'))
);

CREATE TABLE work_center (
    work_center_id INTEGER PRIMARY KEY,
    work_center_code TEXT NOT NULL UNIQUE,
    work_center_name TEXT NOT NULL
);

CREATE TABLE equipment (
    equipment_id INTEGER PRIMARY KEY,
    equipment_code TEXT NOT NULL UNIQUE,
    equipment_name TEXT NOT NULL,
    work_center_id INTEGER NOT NULL,
    status TEXT NOT NULL CHECK (status IN ('RUN', 'IDLE', 'DOWN', 'PM')),
    FOREIGN KEY (work_center_id) REFERENCES work_center (work_center_id)
);

CREATE TABLE bom_header (
    bom_id INTEGER PRIMARY KEY,
    product_item_id INTEGER NOT NULL UNIQUE,
    version TEXT NOT NULL,
    yield_qty REAL NOT NULL CHECK (yield_qty > 0),
    is_active TEXT NOT NULL DEFAULT 'Y' CHECK (is_active IN ('Y', 'N')),
    FOREIGN KEY (product_item_id) REFERENCES item (item_id)
);

CREATE TABLE bom_detail (
    bom_detail_id INTEGER PRIMARY KEY,
    bom_id INTEGER NOT NULL,
    material_item_id INTEGER NOT NULL,
    qty_per REAL NOT NULL CHECK (qty_per > 0),
    loss_rate REAL NOT NULL DEFAULT 0 CHECK (loss_rate >= 0),
    FOREIGN KEY (bom_id) REFERENCES bom_header (bom_id),
    FOREIGN KEY (material_item_id) REFERENCES item (item_id),
    UNIQUE (bom_id, material_item_id)
);

CREATE TABLE lot (
    lot_id INTEGER PRIMARY KEY,
    lot_no TEXT NOT NULL UNIQUE,
    item_id INTEGER NOT NULL,
    warehouse_id INTEGER NOT NULL,
    lot_type TEXT NOT NULL CHECK (lot_type IN ('RECEIPT', 'PRODUCTION')),
    initial_qty REAL NOT NULL CHECK (initial_qty >= 0),
    current_qty REAL NOT NULL CHECK (current_qty >= 0),
    status TEXT NOT NULL CHECK (status IN ('QUARANTINE', 'AVAILABLE', 'HOLD', 'CONSUMED', 'SHIPPED')),
    received_date TEXT,
    produced_date TEXT,
    expire_date TEXT,
    source_partner_id INTEGER,
    FOREIGN KEY (item_id) REFERENCES item (item_id),
    FOREIGN KEY (warehouse_id) REFERENCES warehouse (warehouse_id),
    FOREIGN KEY (source_partner_id) REFERENCES partner (partner_id)
);

CREATE TABLE inventory_txn (
    txn_id INTEGER PRIMARY KEY,
    txn_date TEXT NOT NULL,
    txn_type TEXT NOT NULL CHECK (txn_type IN ('RECEIPT', 'ISSUE', 'PRODUCTION_IN', 'SHIP', 'ADJUST')),
    lot_id INTEGER NOT NULL,
    item_id INTEGER NOT NULL,
    warehouse_id INTEGER NOT NULL,
    qty REAL NOT NULL,
    ref_type TEXT,
    ref_id INTEGER,
    memo TEXT,
    FOREIGN KEY (lot_id) REFERENCES lot (lot_id),
    FOREIGN KEY (item_id) REFERENCES item (item_id),
    FOREIGN KEY (warehouse_id) REFERENCES warehouse (warehouse_id)
);

CREATE TABLE production_order (
    order_id INTEGER PRIMARY KEY,
    order_no TEXT NOT NULL UNIQUE,
    product_item_id INTEGER NOT NULL,
    work_center_id INTEGER NOT NULL,
    plan_date TEXT NOT NULL,
    due_date TEXT NOT NULL,
    order_qty REAL NOT NULL CHECK (order_qty > 0),
    status TEXT NOT NULL CHECK (status IN ('PLANNED', 'RELEASED', 'COMPLETED', 'CANCELED')),
    priority INTEGER NOT NULL DEFAULT 3,
    FOREIGN KEY (product_item_id) REFERENCES item (item_id),
    FOREIGN KEY (work_center_id) REFERENCES work_center (work_center_id)
);

CREATE TABLE production (
    production_id INTEGER PRIMARY KEY,
    production_no TEXT NOT NULL UNIQUE,
    order_id INTEGER,
    product_item_id INTEGER NOT NULL,
    output_lot_id INTEGER NOT NULL UNIQUE,
    equipment_id INTEGER NOT NULL,
    production_date TEXT NOT NULL,
    start_time TEXT,
    end_time TEXT,
    good_qty REAL NOT NULL CHECK (good_qty >= 0),
    defect_qty REAL NOT NULL DEFAULT 0 CHECK (defect_qty >= 0),
    status TEXT NOT NULL CHECK (status IN ('COMPLETED', 'CANCELED')),
    FOREIGN KEY (order_id) REFERENCES production_order (order_id),
    FOREIGN KEY (product_item_id) REFERENCES item (item_id),
    FOREIGN KEY (output_lot_id) REFERENCES lot (lot_id),
    FOREIGN KEY (equipment_id) REFERENCES equipment (equipment_id)
);

CREATE TABLE production_material (
    production_material_id INTEGER PRIMARY KEY,
    production_id INTEGER NOT NULL,
    material_item_id INTEGER NOT NULL,
    material_lot_id INTEGER NOT NULL,
    required_qty REAL NOT NULL CHECK (required_qty > 0),
    issued_qty REAL NOT NULL CHECK (issued_qty > 0),
    FOREIGN KEY (production_id) REFERENCES production (production_id),
    FOREIGN KEY (material_item_id) REFERENCES item (item_id),
    FOREIGN KEY (material_lot_id) REFERENCES lot (lot_id)
);

CREATE TABLE quality_inspection (
    inspection_id INTEGER PRIMARY KEY,
    inspection_no TEXT NOT NULL UNIQUE,
    inspection_type TEXT NOT NULL CHECK (inspection_type IN ('INCOMING', 'PROCESS', 'FINAL')),
    lot_id INTEGER NOT NULL,
    production_id INTEGER,
    inspection_date TEXT NOT NULL,
    sample_qty REAL NOT NULL CHECK (sample_qty >= 0),
    defect_qty REAL NOT NULL DEFAULT 0 CHECK (defect_qty >= 0),
    result TEXT NOT NULL CHECK (result IN ('PASS', 'FAIL', 'HOLD')),
    memo TEXT,
    FOREIGN KEY (lot_id) REFERENCES lot (lot_id),
    FOREIGN KEY (production_id) REFERENCES production (production_id)
);

CREATE TABLE shipment (
    shipment_id INTEGER PRIMARY KEY,
    shipment_no TEXT NOT NULL UNIQUE,
    customer_id INTEGER NOT NULL,
    shipment_date TEXT NOT NULL,
    status TEXT NOT NULL CHECK (status IN ('PLANNED', 'SHIPPED', 'CANCELED')),
    FOREIGN KEY (customer_id) REFERENCES partner (partner_id)
);

CREATE TABLE shipment_detail (
    shipment_detail_id INTEGER PRIMARY KEY,
    shipment_id INTEGER NOT NULL,
    lot_id INTEGER NOT NULL,
    item_id INTEGER NOT NULL,
    qty REAL NOT NULL CHECK (qty > 0),
    FOREIGN KEY (shipment_id) REFERENCES shipment (shipment_id),
    FOREIGN KEY (lot_id) REFERENCES lot (lot_id),
    FOREIGN KEY (item_id) REFERENCES item (item_id)
);

CREATE INDEX idx_lot_item ON lot (item_id);
CREATE INDEX idx_lot_no ON lot (lot_no);
CREATE INDEX idx_inventory_lot ON inventory_txn (lot_id);
CREATE INDEX idx_production_order ON production (order_id);
CREATE INDEX idx_production_material_lot ON production_material (material_lot_id);
CREATE INDEX idx_shipment_detail_lot ON shipment_detail (lot_id);
