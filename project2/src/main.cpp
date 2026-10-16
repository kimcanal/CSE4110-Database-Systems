// ============================================================
// CSE4110 Project 2 - Database Application
// 20211518 김윤하
// MySQL C API 사용
// ============================================================

#include "database.h"
#include <cstring>
#include <climits>

// ============================================================
// DB 연결/해제
// ============================================================
MYSQL *db_connect()
{
    MYSQL *conn = mysql_init(nullptr);
    if (!conn)
    {
        std::cerr << "[ERROR] mysql_init failed\n";
        return nullptr;
    }
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, nullptr, 0))
    {
        std::cerr << "[ERROR] Connection failed: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return nullptr;
    }
    mysql_set_character_set(conn, "utf8mb4");
    std::cout << "[INFO] Connected to MySQL (" << DB_NAME << ")\n";
    return conn;
}

void db_disconnect(MYSQL *conn)
{
    if (conn)
    {
        mysql_close(conn);
        std::cout << "[INFO] Disconnected.\n";
    }
}

// ============================================================
// 유틸
// ============================================================
void print_separator(int width)
{
    for (int i = 0; i < width; i++)
        std::cout << '-';
    std::cout << '\n';
}

void wait_enter()
{
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore(INT_MAX, '\n');
    std::cin.get();
}

// ============================================================
// Query 1: Sales Trends
// ============================================================
void query1_sales_trends(MYSQL *conn)
{
    std::cout << "\n=== Query 1: Sales Trends (Past 3 Years) ===\n";

    // 1-1. Brand / Year / Month
    const char *sql1 =
        "SELECT b.Brand_Name, st.Sold_Year, st.Sold_Month, "
        "       COUNT(*) AS Unit_Sales, SUM(st.Sale_Price) AS Total_Revenue "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v ON st.VIN = v.VIN "
        "JOIN Model m   ON v.Model_ID = m.Model_ID "
        "JOIN Brand b   ON m.Brand_ID = b.Brand_ID "
        "WHERE st.Sold_Year >= YEAR(CURDATE()) - 3 "
        "GROUP BY b.Brand_Name, st.Sold_Year, st.Sold_Month "
        "ORDER BY b.Brand_Name, st.Sold_Year, st.Sold_Month;";

    std::cout << "\n[1-1] Brand / Year / Month\n";
    print_separator();
    printf("%-15s %-6s %-6s %-12s %-15s\n", "Brand", "Year", "Month", "Units", "Revenue");
    print_separator();

    if (mysql_query(conn, sql1))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
        printf("%-15s %-6s %-6s %-12s %-15s\n", row[0], row[1], row[2], row[3], row[4]);
    mysql_free_result(res);

    // 1-2. Brand / Year / Week
    const char *sql2 =
        "SELECT b.Brand_Name, st.Sold_Year, "
        "       WEEK(STR_TO_DATE(CONCAT(st.Sold_Year,'-',st.Sold_Month,'-',st.Sold_Day),'%Y-%m-%d')) AS Week_Num, "
        "       COUNT(*) AS Unit_Sales "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v ON st.VIN = v.VIN "
        "JOIN Model m   ON v.Model_ID = m.Model_ID "
        "JOIN Brand b   ON m.Brand_ID = b.Brand_ID "
        "WHERE st.Sold_Year >= YEAR(CURDATE()) - 3 "
        "GROUP BY b.Brand_Name, st.Sold_Year, Week_Num "
        "ORDER BY b.Brand_Name, st.Sold_Year, Week_Num;";

    std::cout << "\n[1-2] Brand / Year / Week\n";
    print_separator();
    printf("%-15s %-6s %-6s %-12s\n", "Brand", "Year", "Week", "Units");
    print_separator();

    if (mysql_query(conn, sql2))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
        printf("%-15s %-6s %-6s %-12s\n", row[0], row[1], row[2], row[3]);
    mysql_free_result(res);

    // 1-3. Brand / Year / Gender
    const char *sql3 =
        "SELECT b.Brand_Name, st.Sold_Year, c.Gender, "
        "       COUNT(*) AS Unit_Sales, SUM(st.Sale_Price) AS Total_Revenue "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v  ON st.VIN = v.VIN "
        "JOIN Model m    ON v.Model_ID = m.Model_ID "
        "JOIN Brand b    ON m.Brand_ID = b.Brand_ID "
        "JOIN Customer c ON st.Customer_ID = c.Customer_ID "
        "WHERE st.Sold_Year >= YEAR(CURDATE()) - 3 "
        "GROUP BY b.Brand_Name, st.Sold_Year, c.Gender "
        "ORDER BY b.Brand_Name, st.Sold_Year, c.Gender;";

    std::cout << "\n[1-3] Brand / Year / Gender\n";
    print_separator();
    printf("%-15s %-6s %-8s %-12s %-15s\n", "Brand", "Year", "Gender", "Units", "Revenue");
    print_separator();

    if (mysql_query(conn, sql3))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
        printf("%-15s %-6s %-8s %-12s %-15s\n", row[0], row[1], row[2], row[3], row[4]);
    mysql_free_result(res);

    // 1-4. Brand / Year / Income Range
    const char *sql4 =
        "SELECT b.Brand_Name, st.Sold_Year, "
        "       CASE "
        "           WHEN c.Annual_Income < 50000  THEN 'Under 50K' "
        "           WHEN c.Annual_Income < 80000  THEN '50K-80K' "
        "           WHEN c.Annual_Income < 120000 THEN '80K-120K' "
        "           ELSE 'Over 120K' "
        "       END AS Income_Range, "
        "       COUNT(*) AS Unit_Sales "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v  ON st.VIN = v.VIN "
        "JOIN Model m    ON v.Model_ID = m.Model_ID "
        "JOIN Brand b    ON m.Brand_ID = b.Brand_ID "
        "JOIN Customer c ON st.Customer_ID = c.Customer_ID "
        "WHERE st.Sold_Year >= YEAR(CURDATE()) - 3 "
        "GROUP BY b.Brand_Name, st.Sold_Year, Income_Range "
        "ORDER BY b.Brand_Name, st.Sold_Year, Income_Range;";

    std::cout << "\n[1-4] Brand / Year / Income Range\n";
    print_separator();
    printf("%-15s %-6s %-12s %-12s\n", "Brand", "Year", "Income Range", "Units");
    print_separator();

    if (mysql_query(conn, sql4))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
        printf("%-15s %-6s %-12s %-12s\n", row[0], row[1], row[2], row[3]);
    mysql_free_result(res);

    wait_enter();
}

// ============================================================
// Query 2: Defective Part Tracking
// ============================================================
void query2_defective_parts(MYSQL *conn)
{
    std::cout << "\n=== Query 2: Defective Part Tracking ===\n";

    char supplier_name[100];
    int y1, y2;

    std::cout << "Supplier Name (e.g. Getrag): ";
    std::cin >> supplier_name;
    std::cout << "Start Year: ";
    std::cin >> y1;
    std::cout << "End Year  : ";
    std::cin >> y2;

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    const char *sql =
        "SELECT v.VIN, c.Customer_Name, c.Address, "
        "       p.Part_Name, sp.Supplier_Plant_Name "
        "FROM Part p "
        "JOIN Supplier_Plant sp ON p.Supplier_Plant_ID = sp.Supplier_Plant_ID "
        "JOIN Supplier s        ON sp.Supplier_ID = s.Supplier_ID "
        "JOIN Vehicle_Part vp   ON p.Part_ID = vp.Part_ID "
        "JOIN Vehicle v         ON vp.VIN = v.VIN "
        "LEFT JOIN Sales_Transaction st ON v.VIN = st.VIN "
        "LEFT JOIN Customer c           ON st.Customer_ID = c.Customer_ID "
        "WHERE s.Supplier_Name = ? "
        "  AND p.Part_Type = 'Transmission' "
        "  AND p.Part_Year BETWEEN ? AND ?;";

    mysql_stmt_prepare(stmt, sql, strlen(sql));

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));
    unsigned long supplier_len = strlen(supplier_name);

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = supplier_name;
    bind[0].buffer_length = sizeof(supplier_name);
    bind[0].length = &supplier_len;
    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &y1;
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &y2;

    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);

    char vin[18], cust_name[101], cust_addr[201], part_name[101], plant_name[101];
    unsigned long lens[5];
    bool nulls[5];
    MYSQL_BIND res_bind[5];
    memset(res_bind, 0, sizeof(res_bind));

    auto bind_str = [&](int i, char *buf, int len)
    {
        res_bind[i].buffer_type = MYSQL_TYPE_STRING;
        res_bind[i].buffer = buf;
        res_bind[i].buffer_length = len;
        res_bind[i].length = &lens[i];
        res_bind[i].is_null = &nulls[i];
    };
    bind_str(0, vin, sizeof(vin));
    bind_str(1, cust_name, sizeof(cust_name));
    bind_str(2, cust_addr, sizeof(cust_addr));
    bind_str(3, part_name, sizeof(part_name));
    bind_str(4, plant_name, sizeof(plant_name));

    mysql_stmt_bind_result(stmt, res_bind);
    mysql_stmt_store_result(stmt);

    print_separator();
    printf("%-20s %-20s %-25s %-20s %-25s\n",
           "VIN", "Customer", "Address", "Part", "Plant");
    print_separator();

    while (mysql_stmt_fetch(stmt) == 0)
    {
        printf("%-20s %-20s %-25s %-20s %-25s\n",
               vin,
               nulls[1] ? "(not sold)" : cust_name,
               nulls[2] ? "-" : cust_addr,
               part_name, plant_name);
    }

    mysql_stmt_close(stmt);
    wait_enter();
}

// ============================================================
// Query 3: Top 2 Brands by Revenue
// ============================================================
void query3_top_brands_revenue(MYSQL *conn)
{
    std::cout << "\n=== Query 3: Top 2 Brands by Revenue (Past Year) ===\n";

    const char *sql =
        "SELECT b.Brand_Name, SUM(st.Sale_Price) AS Total_Revenue "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v ON st.VIN = v.VIN "
        "JOIN Model m   ON v.Model_ID = m.Model_ID "
        "JOIN Brand b   ON m.Brand_ID = b.Brand_ID "
        "WHERE st.Sold_Year = YEAR(CURDATE()) - 1 "
        "GROUP BY b.Brand_Name "
        "ORDER BY Total_Revenue DESC "
        "LIMIT 2;";

    if (mysql_query(conn, sql))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    print_separator();
    printf("%-5s %-20s %-20s\n", "Rank", "Brand", "Total Revenue");
    print_separator();

    int rank = 1;
    while ((row = mysql_fetch_row(res)))
        printf("%-5d %-20s $%-20s\n", rank++, row[0], row[1]);
    mysql_free_result(res);
    wait_enter();
}

// ============================================================
// Query 4: Top 2 Brands by Unit Sales
// ============================================================
void query4_top_brands_unit(MYSQL *conn)
{
    std::cout << "\n=== Query 4: Top 2 Brands by Unit Sales (Past Year) ===\n";

    const char *sql =
        "SELECT b.Brand_Name, COUNT(*) AS Unit_Sales "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v ON st.VIN = v.VIN "
        "JOIN Model m   ON v.Model_ID = m.Model_ID "
        "JOIN Brand b   ON m.Brand_ID = b.Brand_ID "
        "WHERE st.Sold_Year = YEAR(CURDATE()) - 1 "
        "GROUP BY b.Brand_Name "
        "ORDER BY Unit_Sales DESC "
        "LIMIT 2;";

    if (mysql_query(conn, sql))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    print_separator();
    printf("%-5s %-20s %-15s\n", "Rank", "Brand", "Unit Sales");
    print_separator();

    int rank = 1;
    while ((row = mysql_fetch_row(res)))
        printf("%-5d %-20s %-15s\n", rank++, row[0], row[1]);
    mysql_free_result(res);
    wait_enter();
}

// ============================================================
// Query 5: Seasonal Sales Patterns
// ============================================================
void query5_seasonal_sales(MYSQL *conn)
{
    std::cout << "\n=== Query 5: Seasonal Sales Patterns ===\n";

    char body_style[50];
    std::cout << "Body Style (e.g. Convertible, Sedan, SUV): ";
    std::cin >> body_style;

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    const char *sql =
        "SELECT st.Sold_Month, COUNT(*) AS Unit_Sales "
        "FROM Sales_Transaction st "
        "JOIN Vehicle v ON st.VIN = v.VIN "
        "JOIN Model m   ON v.Model_ID = m.Model_ID "
        "WHERE m.Body_Style = ? "
        "GROUP BY st.Sold_Month "
        "ORDER BY Unit_Sales DESC, st.Sold_Month;";

    mysql_stmt_prepare(stmt, sql, strlen(sql));

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));
    unsigned long body_len = strlen(body_style);
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = body_style;
    bind[0].buffer_length = sizeof(body_style);
    bind[0].length = &body_len;

    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);

    int month, units;
    MYSQL_BIND res_bind[2];
    memset(res_bind, 0, sizeof(res_bind));
    res_bind[0].buffer_type = MYSQL_TYPE_LONG;
    res_bind[0].buffer = &month;
    res_bind[1].buffer_type = MYSQL_TYPE_LONG;
    res_bind[1].buffer = &units;

    mysql_stmt_bind_result(stmt, res_bind);
    mysql_stmt_store_result(stmt);

    print_separator();
    printf("%-10s %-15s\n", "Month", "Unit Sales");
    print_separator();

    while (mysql_stmt_fetch(stmt) == 0)
        printf("%-10d %-15d\n", month, units);

    mysql_stmt_close(stmt);
    wait_enter();
}

// ============================================================
// Query 6: Dealer Inventory Efficiency
// ============================================================
void query6_dealer_inventory(MYSQL *conn)
{
    std::cout << "\n=== Query 6: Dealer Inventory Efficiency ===\n";

    const char *sql =
        "SELECT d.Dealer_Name, "
        "       AVG(DATEDIFF( "
        "           COALESCE( "
        "               STR_TO_DATE(CONCAT(st.Sold_Year,'-',st.Sold_Month,'-',st.Sold_Day), '%Y-%m-%d'), "
        "               CURDATE() "
        "           ), "
        "           STR_TO_DATE(CONCAT(i.Arrived_Year,'-',i.Arrived_Month,'-',i.Arrived_Day), '%Y-%m-%d') "
        "       )) AS Avg_Days "
        "FROM Inventory i "
        "JOIN Dealer d              ON i.Dealer_ID = d.Dealer_ID "
        "LEFT JOIN Sales_Transaction st ON i.VIN = st.VIN "
        "GROUP BY d.Dealer_Name "
        "ORDER BY Avg_Days DESC;";

    if (mysql_query(conn, sql))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    print_separator();
    printf("%-25s %-20s\n", "Dealer", "Avg Inventory Days");
    print_separator();

    while ((row = mysql_fetch_row(res)))
        printf("%-25s %-20.1f\n", row[0], row[1] ? atof(row[1]) : 0.0);
    mysql_free_result(res);
    wait_enter();
}

// ============================================================
// Query 7: Supplier Coverage
// ============================================================
void query7_supplier_coverage(MYSQL *conn)
{
    std::cout << "\n=== Query 7: Supplier Coverage ===\n";

    const char *sql =
        "SELECT s.Supplier_Name, COUNT(DISTINCT m.Model_ID) AS Model_Count "
        "FROM Supplier s "
        "JOIN Supplier_Plant sp ON s.Supplier_ID = sp.Supplier_ID "
        "JOIN Part p            ON sp.Supplier_Plant_ID = p.Supplier_Plant_ID "
        "JOIN Vehicle_Part vp   ON p.Part_ID = vp.Part_ID "
        "JOIN Vehicle v         ON vp.VIN = v.VIN "
        "JOIN Model m           ON v.Model_ID = m.Model_ID "
        "GROUP BY s.Supplier_Name "
        "ORDER BY Model_Count DESC;";

    if (mysql_query(conn, sql))
    {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    print_separator();
    printf("%-25s %-20s\n", "Supplier", "Distinct Models");
    print_separator();

    while ((row = mysql_fetch_row(res)))
        printf("%-25s %-20s\n", row[0], row[1]);
    mysql_free_result(res);
    wait_enter();
}

// ============================================================
// Main Menu
// ============================================================
void display_menu()
{
    std::cout << "\n";
    print_separator(50);
    std::cout << "  CSE4110 Project 2 - Car Database Application\n";
    std::cout << "  20211518 김윤하\n";
    print_separator(50);
    std::cout << "  1. Sales Trends (Past 3 Years)\n";
    std::cout << "  2. Defective Part Tracking\n";
    std::cout << "  3. Top 2 Brands by Revenue (Past Year)\n";
    std::cout << "  4. Top 2 Brands by Unit Sales (Past Year)\n";
    std::cout << "  5. Seasonal Sales Patterns\n";
    std::cout << "  6. Dealer Inventory Efficiency\n";
    std::cout << "  7. Supplier Coverage\n";
    std::cout << "  0. Exit\n";
    print_separator(50);
    std::cout << "  Choice: ";
}

int main()
{
    MYSQL *conn = db_connect();
    if (!conn)
        return 1;

    int choice;
    while (true)
    {
        display_menu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            query1_sales_trends(conn);
            break;
        case 2:
            query2_defective_parts(conn);
            break;
        case 3:
            query3_top_brands_revenue(conn);
            break;
        case 4:
            query4_top_brands_unit(conn);
            break;
        case 5:
            query5_seasonal_sales(conn);
            break;
        case 6:
            query6_dealer_inventory(conn);
            break;
        case 7:
            query7_supplier_coverage(conn);
            break;
        case 0:
            db_disconnect(conn);
            std::cout << "Bye!\n";
            return 0;
        default:
            std::cout << "[ERROR] Invalid choice.\n";
        }
    }
    return 0;
}