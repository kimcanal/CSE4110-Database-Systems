#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <iostream>

// DB 연결 설정
#define DB_HOST "127.0.0.1"
#define DB_USER "admin"
#define DB_PASS "1234"
#define DB_NAME "car_db"
#define DB_PORT 3306

// DB 연결/해제
MYSQL *db_connect();
void db_disconnect(MYSQL *conn);

// 유틸
void print_separator(int width = 80);
void wait_enter();

// 7개 쿼리 함수
void query1_sales_trends(MYSQL *conn);
void query2_defective_parts(MYSQL *conn);
void query3_top_brands_revenue(MYSQL *conn);
void query4_top_brands_unit(MYSQL *conn);
void query5_seasonal_sales(MYSQL *conn);
void query6_dealer_inventory(MYSQL *conn);
void query7_supplier_coverage(MYSQL *conn);

#endif