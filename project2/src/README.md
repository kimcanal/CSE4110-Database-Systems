# CSE4110 Project 2 - Car Database Application
20211518 김윤하

## Tested Environment
- **Machine**: MacBook Air M4
- **OS**: macOS 26.5 (Darwin arm64)
- **Compiler**: Apple clang version 21.0.0 (clang-2100.1.1.101)
- **MySQL**: Ver 8.0.46 for macos26.4 on arm64 (Homebrew)

## Requirements
- MySQL 8.0+
- MySQL C API (libmysqlclient)
- C++11 or later
- GCC / Apple Clang / MSVC

## Database Setup
Run the following scripts in order after connecting to MySQL as admin:

### macOS / Linux
```bash
mysql -u admin -p < schema.sql
# Enter password: 1234
mysql -u admin -p < sample_data.sql
# Enter password: 1234
```

### Windows
```bash
mysql -u admin -p < schema.sql
mysql -u admin -p < sample_data.sql
```

Or open MySQL Workbench, connect with admin/1234, and run schema.sql then sample_data.sql via:
File → Open SQL Script → ⚡

## Build

### macOS / Linux
```bash
g++ main.cpp -o car_app \
    $(mysql_config --cflags) \
    $(mysql_config --libs)
```

### Windows (MSVC)
```bash
# MySQL 설치 경로는 환경에 따라 다를 수 있습니다.
cl main.cpp ^
    /I"C:\Program Files\MySQL\MySQL Server 8.0\include" ^
    /link "C:\Program Files\MySQL\MySQL Server 8.0\lib\libmysql.lib"
```

## Run
```bash
./car_app        # macOS/Linux
car_app.exe      # Windows
```

## DB Connection Settings
Defined in `database.h`:
```cpp
#define DB_HOST  "127.0.0.1"
#define DB_USER  "admin"
#define DB_PASS  "1234"
#define DB_NAME  "car_db"
#define DB_PORT  3306
```

## Query Usage

| No  | Description                            | Input                         |
| --- | -------------------------------------- | ----------------------------- |
| 1   | Sales Trends (Past 3 Years)            | 없음                          |
| 2   | Defective Part Tracking                | Supplier Name, Start/End Year |
| 3   | Top 2 Brands by Revenue (Past Year)    | 없음                          |
| 4   | Top 2 Brands by Unit Sales (Past Year) | 없음                          |
| 5   | Seasonal Sales Patterns                | Body Style                    |
| 6   | Dealer Inventory Efficiency            | 없음                          |
| 7   | Supplier Coverage                      | 없음                          |

## Example
```
./car_app

Choice: 2
Supplier Name (e.g. Getrag): Getrag
Start Year: 2022
End Year  : 2023

Choice: 5
Body Style (e.g. Convertible, Sedan, SUV): Convertible
```

## Note
- MySQL C API only (ODBC not required).
- Tested on macOS 26.5 with Apple clang 21.0.0 and MySQL 8.0.46.
- Queries 3, 4 filter by YEAR(CURDATE()) - 1 as "past year".