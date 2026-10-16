-- ============================================================
-- CSE4110 Project 2 - Physical Schema
-- 20211518 김윤하
-- ============================================================

CREATE DATABASE IF NOT EXISTS car_db;
USE car_db;

-- 1. Brand
CREATE TABLE Brand (
    Brand_ID    INT             NOT NULL,
    Brand_Name  VARCHAR(100)    NOT NULL UNIQUE,
    Type        VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Brand_ID)
);

-- 2. Model
CREATE TABLE Model (
    Model_ID    INT             NOT NULL,
    Brand_ID    INT             NOT NULL,
    Model_Name  VARCHAR(100)    NOT NULL,
    Year        YEAR            NOT NULL,
    Price       DECIMAL(12, 2)  NOT NULL,
    Body_Style  VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Model_ID),
    UNIQUE (Model_Name, Year),
    FOREIGN KEY (Brand_ID) REFERENCES Brand(Brand_ID)
);

-- 3. Manufacturing_Plant
CREATE TABLE Manufacturing_Plant (
    Plant_ID    INT             NOT NULL,
    Plant_Name  VARCHAR(100)    NOT NULL UNIQUE,
    Location    VARCHAR(200)    NOT NULL,
    PRIMARY KEY (Plant_ID)
);

-- 4. Vehicle
CREATE TABLE Vehicle (
    VIN                 VARCHAR(17)     NOT NULL,
    Model_ID            INT             NOT NULL,
    Plant_ID            INT             NOT NULL,
    Current_Status      VARCHAR(20)     NOT NULL CHECK (Current_Status IN ('manufacturing', 'completed', 'in_stock', 'sold')),
    Mfg_Year            YEAR            NOT NULL,
    Mfg_Month           INT             NOT NULL CHECK (Mfg_Month BETWEEN 1 AND 12),
    Mfg_Day             INT             NOT NULL CHECK (Mfg_Day BETWEEN 1 AND 31),
    Color               VARCHAR(50)     NOT NULL,
    Engine_Type         VARCHAR(50)     NOT NULL,
    Transmission_Type   VARCHAR(50)     NOT NULL,
    PRIMARY KEY (VIN),
    FOREIGN KEY (Model_ID) REFERENCES Model(Model_ID),
    FOREIGN KEY (Plant_ID) REFERENCES Manufacturing_Plant(Plant_ID)
);

-- 5. Dealer
CREATE TABLE Dealer (
    Dealer_ID   INT             NOT NULL,
    Dealer_Name VARCHAR(100)    NOT NULL UNIQUE,
    Address     VARCHAR(200)    NOT NULL,
    PRIMARY KEY (Dealer_ID)
);

-- 6. Dealer_Contacts
CREATE TABLE Dealer_Contacts (
    Dealer_ID   INT             NOT NULL,
    Contact     VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Dealer_ID, Contact),
    FOREIGN KEY (Dealer_ID) REFERENCES Dealer(Dealer_ID)
);

-- 7. Inventory
CREATE TABLE Inventory (
    Inventory_ID    INT         NOT NULL,
    VIN             VARCHAR(17) NOT NULL UNIQUE,
    Dealer_ID       INT         NOT NULL,
    Arrived_Year    YEAR        NOT NULL,
    Arrived_Month   INT         NOT NULL CHECK (Arrived_Month BETWEEN 1 AND 12),
    Arrived_Day     INT         NOT NULL CHECK (Arrived_Day BETWEEN 1 AND 31),
    PRIMARY KEY (Inventory_ID),
    FOREIGN KEY (VIN) REFERENCES Vehicle(VIN),
    FOREIGN KEY (Dealer_ID) REFERENCES Dealer(Dealer_ID)
);

-- 8. Customer
CREATE TABLE Customer (
    Customer_ID     INT             NOT NULL,
    Customer_Name   VARCHAR(100)    NOT NULL,
    Address         VARCHAR(200)    NOT NULL,
    Gender          CHAR(1)         NOT NULL CHECK (Gender IN ('M', 'F')),
    Annual_Income   DECIMAL(15, 2)  NOT NULL,
    PRIMARY KEY (Customer_ID)
);

-- 9. Customer_Contacts
CREATE TABLE Customer_Contacts (
    Customer_ID INT             NOT NULL,
    Contact     VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Customer_ID, Contact),
    FOREIGN KEY (Customer_ID) REFERENCES Customer(Customer_ID)
);

-- 10. Sales_Transaction
CREATE TABLE Sales_Transaction (
    Transaction_ID  INT             NOT NULL,
    VIN             VARCHAR(17)     NOT NULL UNIQUE,
    Dealer_ID       INT             NOT NULL,
    Customer_ID     INT             NOT NULL,
    Sold_Year       YEAR            NOT NULL,
    Sold_Month      INT             NOT NULL CHECK (Sold_Month BETWEEN 1 AND 12),
    Sold_Day        INT             NOT NULL CHECK (Sold_Day BETWEEN 1 AND 31),
    Payment_Method  VARCHAR(50)     NOT NULL,
    Sale_Price      DECIMAL(12, 2)  NOT NULL CHECK (Sale_Price > 0),
    PRIMARY KEY (Transaction_ID),
    FOREIGN KEY (VIN) REFERENCES Vehicle(VIN),
    FOREIGN KEY (Dealer_ID) REFERENCES Dealer(Dealer_ID),
    FOREIGN KEY (Customer_ID) REFERENCES Customer(Customer_ID)
);

-- 11. Supplier
CREATE TABLE Supplier (
    Supplier_ID     INT             NOT NULL,
    Supplier_Name   VARCHAR(100)    NOT NULL UNIQUE,
    PRIMARY KEY (Supplier_ID)
);

-- 12. Supplier_Contacts
CREATE TABLE Supplier_Contacts (
    Supplier_ID INT             NOT NULL,
    Contact     VARCHAR(50)     NOT NULL,
    PRIMARY KEY (Supplier_ID, Contact),
    FOREIGN KEY (Supplier_ID) REFERENCES Supplier(Supplier_ID)
);

-- 13. Supplier_Plant
CREATE TABLE Supplier_Plant (
    Supplier_Plant_ID   INT             NOT NULL,
    Supplier_ID         INT             NOT NULL,
    Supplier_Plant_Name VARCHAR(100)    NOT NULL UNIQUE,
    Location            VARCHAR(200)    NOT NULL,
    PRIMARY KEY (Supplier_Plant_ID),
    FOREIGN KEY (Supplier_ID) REFERENCES Supplier(Supplier_ID)
);

-- 14. Part
CREATE TABLE Part (
    Part_ID             INT             NOT NULL,
    Supplier_Plant_ID   INT             NOT NULL,
    Part_Type           VARCHAR(50)     NOT NULL,
    Part_Name           VARCHAR(100)    NOT NULL UNIQUE,
    Part_Year           YEAR            NOT NULL,
    Part_Month          INT             NOT NULL CHECK (Part_Month BETWEEN 1 AND 12),
    Part_Day            INT             NOT NULL CHECK (Part_Day BETWEEN 1 AND 31),
    PRIMARY KEY (Part_ID),
    FOREIGN KEY (Supplier_Plant_ID) REFERENCES Supplier_Plant(Supplier_Plant_ID)
);

-- 15. Vehicle_Part
CREATE TABLE Vehicle_Part (
    VIN     VARCHAR(17) NOT NULL,
    Part_ID INT         NOT NULL,
    PRIMARY KEY (VIN, Part_ID),
    FOREIGN KEY (VIN) REFERENCES Vehicle(VIN),
    FOREIGN KEY (Part_ID) REFERENCES Part(Part_ID)
);
