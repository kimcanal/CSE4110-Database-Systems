# CSE4110 Database Systems

서강대학교 CSE4110 (데이터베이스 시스템) 프로젝트 모음입니다. 자동차 제조·판매 도메인을 대상으로 DB를 설계하고, 이를 활용하는 응용 프로그램을 구현했습니다.

## 프로젝트 구성

| 프로젝트 | 내용 | 산출물 |
| -------- | ---- | ------ |
| [Project 1](project1/) | 자동차 도메인 ER 모델링 (ERD 설계) | ERD 이미지, 보고서(PDF) |
| [Project 2](project2/) | 관계형 스키마 설계 및 MySQL 연동 C++ 콘솔 애플리케이션 | SQL 스크립트, 소스코드, 스키마 다이어그램, 보고서(PDF) |

## 기술 스택

| 구분 | 기술 |
| ---- | ---- |
| Database | MySQL 8.0 (utf8mb4) |
| Language | C++ (C++11 이상) |
| DB 연동 | MySQL C API (`libmysqlclient`, `mysql.h`) |
| Query | SQL (DDL / DML, JOIN, GROUP BY, 집계 함수, 서브쿼리 등) |
| Build | `g++` / Apple Clang / MSVC + `mysql_config` |
| 설계/문서 | ERD, Logical/Physical Schema 다이어그램, PDF 보고서 |
| 테스트 환경 | MacBook Air (Apple Silicon), macOS, Homebrew MySQL 8.0 |

## 디렉터리 구조

```
.
├── project1/
│   ├── [ERD]project1_*.png          # ER 다이어그램
│   └── [REPORT]project1_*.pdf       # 프로젝트 보고서
└── project2/
    ├── database/
    │   ├── schema.sql               # 테이블 생성 (car_db)
    │   └── sample_data.sql          # 샘플 데이터
    ├── docs/
    │   ├── logical_schema.png       # 논리 스키마
    │   ├── physical_schema.png      # 물리 스키마
    │   └── project_report.pdf       # 프로젝트 보고서
    └── src/
        ├── main.cpp                 # 애플리케이션 본체 (메뉴 + 7개 쿼리)
        ├── database.h               # DB 연결 설정 및 함수 선언
        └── README.md                # 빌드/실행 상세 가이드
```

## Project 1 - ER 모델링

자동차 도메인(브랜드, 모델, 차량, 딜러, 고객, 공급업체, 부품 등)의 개체와 관계를 정의한 ERD와 설계 보고서입니다.

## Project 2 - Car Database Application

### 데이터베이스 (`car_db`)

총 16개 테이블로 구성됩니다.

- **차량**: `Brand`, `Model`, `Vehicle`, `Manufacturing_Plant`
- **판매/유통**: `Dealer`, `Dealer_Contacts`, `Inventory`, `Customer`, `Customer_Contacts`, `Sales_Transaction`
- **공급망**: `Supplier`, `Supplier_Contacts`, `Supplier_Plant`, `Part`, `Vehicle_Part`

### 애플리케이션 기능

C++ 콘솔 메뉴에서 아래 7개 쿼리를 실행할 수 있습니다.

| No | 기능 | 입력 |
| -- | ---- | ---- |
| 1 | 최근 3년 판매 추이 (브랜드/연/월) | - |
| 2 | 결함 부품 추적 | 공급업체명, 시작/종료 연도 |
| 3 | 작년 매출 상위 2개 브랜드 | - |
| 4 | 작년 판매량 상위 2개 브랜드 | - |
| 5 | 차종(Body Style)별 계절 판매 패턴 | Body Style |
| 6 | 딜러 재고 효율 | - |
| 7 | 공급업체 커버리지 | - |

### 빠른 시작

```bash
cd project2

# 1) DB 생성 및 샘플 데이터 적재 (admin / 1234)
mysql -u admin -p < database/schema.sql
mysql -u admin -p < database/sample_data.sql

# 2) 빌드
cd src
g++ main.cpp -o car_app $(mysql_config --cflags) $(mysql_config --libs)

# 3) 실행
./car_app
```

DB 접속 정보는 `project2/src/database.h`에서 변경할 수 있습니다. 자세한 환경 설정과 Windows 빌드 방법은 [`project2/src/README.md`](project2/src/README.md)를 참고하세요.

## 작성자

20211518 김윤하
