# Chapter 16. 확장하기

## 학습 목표

- C API, SQLiteCpp, ORM의 선택 기준을 정리한다.
- 다른 C++ DB 라이브러리와 서버형 DB로 확장할 때 달라지는 점을 이해한다.

## 1. sqlite_orm

`sqlite_orm`은 C++ 타입과 SQLite 테이블을 매핑하고 템플릿 기반 질의를 제공하는 헤더 중심 라이브러리다. SQL 문자열을 줄이고 컴파일 시점 타입 검사를 활용하고 싶을 때 검토한다. 복잡한 SQL이나 라이브러리 고유 문법의 학습 비용도 평가해야 한다.

## 2. SOCI

SOCI는 C++용 DB 접근 계층으로 여러 backend를 지원한다. SQLite 외 데이터베이스로 옮길 가능성과 통일된 인터페이스가 중요할 때 후보가 된다. DB별 SQL 문법과 기능 차이까지 완전히 감추는 것은 아니다.

## 3. PostgreSQL과 libpqxx

PostgreSQL은 별도 서버 프로세스, 계정/권한, 네트워크 연결, 동시 접속 운영이 필요하다. `libpqxx`는 PostgreSQL의 공식 C 클라이언트 라이브러리 `libpq` 위의 C++ 인터페이스다.

SQLite에서 서버형 DB로 갈 때 추가되는 관심사:

- 연결 문자열과 비밀정보 관리
- connection pool
- 네트워크 실패와 재시도
- 사용자/권한과 TLS
- 배포 및 백업 운영
- SQL dialect와 타입 차이

## 4. MySQL Connector/C++

MySQL Connector/C++는 MySQL 서버 접근을 위한 C++ 드라이버다. JDBC 유사 API 또는 X DevAPI 등 사용하는 인터페이스에 따라 코드 구조가 달라질 수 있으므로 프로젝트 요구 버전의 공식 문서를 기준으로 선택한다.

## 5. 선택 기준 비교

| 선택지 | 적합한 상황 | 고려할 점 |
|---|---|---|
| SQLite C API | 의존성 최소화, 세밀한 제어 | 수동 자원/오류 관리 |
| SQLiteCpp | 직접 SQL + C++ RAII | SQLite 전용, SQL 직접 관리 |
| sqlite_orm | 객체 매핑과 타입 중심 CRUD | 추상화 학습, 복잡 쿼리 |
| SOCI | 여러 DB backend 가능성 | 공통 계층과 DB별 차이 |
| libpqxx | PostgreSQL 기능 활용 | 서버 운영과 PG 종속성 |
| MySQL Connector/C++ | MySQL 생태계 연동 | API 계열과 버전 호환 |

## 6. 다음 리팩터링

1. migration 도구 또는 스키마 버전 관리
2. 메모리 DB 기반 Repository 통합 테스트
3. prepared statement 재사용과 성능 측정
4. 설정 파일/환경 변수로 DB 경로 분리
5. 로깅, 백업, 오류 코드 정책
6. 관계형 모델, JOIN, 인덱스, 실행 계획 학습

## 실습

현재 Repository 인터페이스를 유지한 채 메모리 구현이나 다른 저장소 구현으로 교체 가능한지 검토한다. SQLiteCpp와 후보 라이브러리 하나로 같은 `findById` 코드를 작성해 가독성, 타입 안전성, SQL 제어 범위를 비교한다.

## 확인 문제

1. SQLiteCpp와 ORM의 핵심 차이는 무엇인가?
2. SQLite에서 서버형 DB로 전환할 때 코드 외 운영상 변화는?
3. 데이터베이스 추상화가 모든 DB 차이를 제거하지 못하는 이유는?

## 참고 자료

- [SQLiteCpp](https://github.com/SRombauts/SQLiteCpp)
- [sqlite_orm](https://github.com/fnc12/sqlite_orm)
- [SOCI](https://soci.sourceforge.net/)
- [libpqxx](https://pqxx.org/libpqxx/)
- [MySQL Connector/C++](https://dev.mysql.com/doc/connector-cpp/en/)

