PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS production_material;
DROP TABLE IF EXISTS production;
DROP TABLE IF EXISTS lot;
DROP TABLE IF EXISTS item;

CREATE TABLE item (
    item_id INTEGER PRIMARY KEY,
    item_code TEXT NOT NULL UNIQUE,
    item_name TEXT NOT NULL,
    item_type TEXT NOT NULL CHECK (item_type IN ('PRODUCT', 'MATERIAL')),
    unit TEXT NOT NULL,
    is_active TEXT NOT NULL DEFAULT 'Y' CHECK (is_active IN ('Y', 'N'))
);

CREATE TABLE lot (
    lot_id INTEGER PRIMARY KEY,
    lot_no TEXT NOT NULL UNIQUE,
    item_id INTEGER NOT NULL,
    lot_type TEXT NOT NULL CHECK (lot_type IN ('RECEIPT', 'PRODUCTION')),
    qty REAL NOT NULL CHECK (qty >= 0),
    received_date TEXT,
    produced_date TEXT,
    expire_date TEXT,
    FOREIGN KEY (item_id) REFERENCES item (item_id)
);

CREATE TABLE production (
    production_id INTEGER PRIMARY KEY,
    production_no TEXT NOT NULL UNIQUE,
    item_id INTEGER NOT NULL,
    output_lot_id INTEGER NOT NULL UNIQUE,
    production_date TEXT NOT NULL,
    qty REAL NOT NULL CHECK (qty > 0),
    status TEXT NOT NULL CHECK (status IN ('PLANNED', 'COMPLETED', 'CANCELED')),
    FOREIGN KEY (item_id) REFERENCES item (item_id),
    FOREIGN KEY (output_lot_id) REFERENCES lot (lot_id)
);

CREATE TABLE production_material (
    production_material_id INTEGER PRIMARY KEY,
    production_id INTEGER NOT NULL,
    material_item_id INTEGER NOT NULL,
    material_lot_id INTEGER NOT NULL,
    qty REAL NOT NULL CHECK (qty > 0),
    FOREIGN KEY (production_id) REFERENCES production (production_id),
    FOREIGN KEY (material_item_id) REFERENCES item (item_id),
    FOREIGN KEY (material_lot_id) REFERENCES lot (lot_id)
);
