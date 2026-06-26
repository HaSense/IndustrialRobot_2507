# SQLiteStudio 설치 및 사용하기 (Ubuntu 24.04 + WSL2 + Windows 11)

## 학습 목표

이번 실습에서는 다음 내용을 학습합니다.

- WSL2에서 SQLiteStudio 실행하기
- WSLg의 동작 원리 이해하기
- SQLite 데이터베이스 연결하기
- SQL Editor 사용하기
- 테이블 생성 및 데이터 수정하기
- CSV/JSON Import, Export 사용하기

---

# 실습 환경

|항목|내용|
|----|----|
|운영체제|Windows 11|
|리눅스|Ubuntu 24.04 (WSL2)|
|GUI|WSLg|
|DBMS|SQLite|
|GUI 프로그램|SQLiteStudio(AppImage)|

---

# 1. WSLg 동작 확인

SQLiteStudio는 GUI 프로그램이므로 WSLg가 정상 동작해야 합니다.

## DISPLAY 확인

```bash
echo $DISPLAY
```

예상 결과

```
:0
```

또는

```
:0.0
```

---

## WAYLAND 확인

```bash
echo $WAYLAND_DISPLAY
```

예상 결과

```
wayland-0
```

---

## WSL 버전 확인

Windows PowerShell에서 실행합니다.

```powershell
wsl --version
```

예시

```
WSL version: 2.6.x
WSLg version: 1.0.x
```

WSLg가 표시되면 GUI 프로그램 실행이 가능합니다.

---

# 2. Ubuntu 업데이트

먼저 패키지를 최신 상태로 업데이트합니다.

```bash
sudo apt update
sudo apt upgrade
```

---

# 3. SQLiteStudio 다운로드

AppImage 버전을 사용하는 것이 가장 간단합니다.

다운로드 도구 설치

```bash
sudo apt install wget
```

SQLiteStudio 다운로드

(버전은 변경될 수 있습니다.)

```bash
wget https://github.com/pawelsalawa/letos/releases/download/3.4.21/sqlitestudio-3.4.21-linux-x64.tar.xz

tar -xJf sqlitestudio-3.4.21-linux-x64.tar.xz

cd sqlitestudio
./sqlitestudio
```

---

# 4. 실행 권한 부여

```bash
chmod +x sqlitestudio-3.4.17-linux-x64.AppImage
```

---

# 5. SQLiteStudio 실행

```bash
./sqlitestudio-3.4.17-linux-x64.AppImage
```

잠시 후 Windows 화면에 SQLiteStudio가 실행됩니다.

---

# 6. 프로그램 관리

AppImage 파일을 별도의 폴더에 보관하면 관리하기 편합니다.

```bash
mkdir -p ~/Apps
```

파일 이동

```bash
mv sqlitestudio-3.4.17-linux-x64.AppImage ~/Apps/
```

이후 실행

```bash
~/Apps/sqlitestudio-3.4.17-linux-x64.AppImage
```

---

# 7. SQLite 데이터베이스 연결

예를 들어 데이터베이스가

```
/home/smart/factory.db
```

에 존재한다고 가정합니다.

메뉴

```
Database
    ↓
Add Database
    ↓
Browse
```

에서

```
/home/smart/factory.db
```

를 선택합니다.

완료되면 왼쪽에 데이터베이스가 추가됩니다.

---

# 8. 테이블 보기

왼쪽 트리에서

```
factory.db
    └── Tables
            └── products
```

를 더블 클릭합니다.

Data 탭에서 데이터를 확인할 수 있습니다.

---

# 9. SQL Editor 사용하기

상단 메뉴

```
SQL Editor
```

를 클릭합니다.

예제

```sql
SELECT *
FROM products;
```

실행

```
Ctrl + Enter
```

또는

```
Execute
```

버튼을 클릭합니다.

---

# 10. 데이터 수정

Data 탭에서

셀을 클릭하여 수정합니다.

예)

|id|name|price|
|--|----|------|
|1|Motor|5000|

↓

```
Motor
```

↓

```
Motor-X
```

수정 후

```
Ctrl + S
```

또는

```
Commit
```

을 눌러 저장합니다.

---

# 11. 새 테이블 생성

메뉴

```
Tables
```

우클릭

```
Create Table
```

컬럼 이름

자료형

Primary Key

Auto Increment

등을 GUI에서 설정할 수 있습니다.

---

# 12. SQL 자동완성

SQL Editor에서

```sql
SEL
```

입력 후

```
Ctrl + Space
```

를 누르면 자동완성이 나타납니다.

---

# 13. CSV Import

메뉴

```
Tools
```

↓

```
Import
```

↓

```
CSV
```

↓

CSV 파일 선택

↓

컬럼 지정

↓

Import

---

# 14. CSV Export

메뉴

```
Export
```

↓

```
CSV
```

↓

파일명 입력

↓

저장

---

# 15. JSON Export

SQLiteStudio는 JSON 저장도 지원합니다.

메뉴

```
Export
```

↓

```
JSON
```

↓

저장

예시

```json
[
  {
    "id":1,
    "name":"Motor",
    "price":5000
  },
  {
    "id":2,
    "name":"Sensor",
    "price":12000
  }
]
```

---

# 16. ERD(테이블 관계도)

메뉴

```
Database
```

↓

```
Diagram
```

선택하면 테이블 관계를 그림으로 확인할 수 있습니다.

---

# 17. SQL 자동 정렬

작성 전

```sql
select*from products where price>5000 order by price;
```

단축키

```
Ctrl + Shift + F
```

정렬 후

```sql
SELECT *
FROM products
WHERE price > 5000
ORDER BY price;
```

---

# 18. 현재 연결된 데이터베이스 확인

```sql
PRAGMA database_list;
```

예상 결과

```
main
/home/smart/factory.db
```

---

# WSL에서 SQLiteStudio가 동작하는 구조

```
                 Windows 11
                      │
                  WSL2 + WSLg
                      │
      ┌───────────────┴───────────────┐
      │                               │
 Ubuntu 24.04                   Windows GUI
      │                               │
SQLiteStudio(AppImage) ───────────────┘
      │
      │
factory.db
(/home/smart/)
```

SQLiteStudio는 Linux에서 실행되지만 화면은 Windows에 출력됩니다.

---

# 장점

- Linux의 SQLite 파일을 직접 사용할 수 있다.
- Windows와 Ubuntu를 함께 사용할 수 있다.
- SQL Editor가 편리하다.
- SQL 자동완성을 지원한다.
- GUI로 테이블을 쉽게 생성할 수 있다.
- CSV Import / Export 기능을 제공한다.
- JSON Export 기능을 제공한다.
- ERD(관계도)를 확인할 수 있다.
- 교육용으로 사용하기 매우 적합하다.

---

# 실습

## 실습 1

WSLg가 정상 동작하는지 확인하시오.

```
echo $DISPLAY
echo $WAYLAND_DISPLAY
```

---

## 실습 2

SQLiteStudio를 실행하시오.

```
~/Apps/sqlitestudio-3.4.17-linux-x64.AppImage
```

---

## 실습 3

다음 데이터베이스를 연결하시오.

```
/home/smart/factory.db
```

---

## 실습 4

SQL Editor에서 모든 데이터를 조회하시오.

```sql
SELECT *
FROM products;
```

---

## 실습 5

가격이 10000원 이상인 제품만 조회하시오.

```sql
SELECT *
FROM products
WHERE price >= 10000;
```

---

## 실습 6

제품 가격을 수정한 후 저장하시오.

예)

```
Motor
```

↓

```
Motor-X
```

Commit 버튼을 눌러 저장합니다.

---

## 실습 7

CSV 파일을 Import하여 새로운 테이블을 생성하시오.

---

## 실습 8

현재 테이블을 JSON 형식으로 Export하시오.

---

# 핵심 정리

- SQLiteStudio는 WSLg를 이용하여 Windows 화면에서 실행된다.
- AppImage 버전이 설치와 관리가 가장 쉽다.
- SQL Editor에서 SQL을 작성하고 실행할 수 있다.
- GUI를 이용하여 테이블을 생성하고 데이터를 수정할 수 있다.
- CSV와 JSON Import/Export 기능을 제공한다.
- SQLite 학습 및 교육용 GUI 프로그램으로 매우 적합하다.
