# Chapter 13. Transaction

## 학습 목표

- 트랜잭션의 원자성을 설명한다.
- `SQLite::Transaction`으로 commit과 자동 rollback을 구현한다.
- 여러 INSERT를 안전하고 빠르게 처리한다.

## 1. 트랜잭션의 필요성

여러 SQL이 하나의 업무라면 전부 성공하거나 전부 실패해야 한다. 예를 들어 학생 100명을 가져오다 51번째에서 중복 이메일 오류가 나면 앞의 50명만 남는 결과는 원치 않을 수 있다.

## 2. SQLite::Transaction

```cpp
void insertMany(SQLite::Database& db,
                const std::vector<Student>& students) {
    SQLite::Transaction transaction(db);
    SQLite::Statement insert(db,
        "INSERT INTO student(name, age, email) VALUES (?, ?, ?)");

    for (const auto& student : students) {
        insert.reset();
        insert.clearBindings();
        insert.bind(1, student.name());
        insert.bind(2, student.age());
        if (student.email()) insert.bind(3, *student.email());
        else insert.bind(3);
        insert.exec();
    }

    transaction.commit();
}
```

`commit()` 전에 예외가 발생하면 stack unwinding 중 `Transaction` 소멸자가 rollback한다. 소멸자에 의한 rollback 자체에 의존해 오류를 숨기지 말고 바깥 계층에서 원래 예외를 처리한다.

## 3. 성능

SQLite에서 각 쓰기는 트랜잭션을 필요로 한다. INSERT마다 자동 트랜잭션을 열고 닫는 것보다 여러 건을 한 트랜잭션에 넣으면 디스크 동기화 횟수가 줄어 보통 훨씬 빠르다.

트랜잭션은 너무 크게 잡지 않는다. 오래 열린 쓰기 트랜잭션은 다른 쓰기를 지연시키고 실패 시 재처리 범위를 키운다.

## 4. 명시적 실패 처리

```cpp
try {
    SQLite::Transaction tx(db);
    repository.insert(first);
    repository.insert(second);
    tx.commit();
} catch (const SQLite::Exception& e) {
    std::cerr << "일괄 등록 취소: " << e.what() << '\n';
}
```

catch 블록에 도달하기 전에 `tx`가 파괴되어 rollback된 상태다.

## 5. 중첩 주의

같은 연결에서 단순하게 트랜잭션을 중첩할 수 있다고 가정하지 않는다. 상위 서비스가 트랜잭션 경계를 소유하도록 설계하거나 고급 단계에서 SAVEPOINT를 사용한다.

## 실습

학생 3명을 하나의 트랜잭션에 등록하되 마지막 학생의 이메일을 중복시킨다. 실행 전후 행 수를 비교해 모두 rollback됐는지 확인한다. 이후 중복을 고쳐 commit 결과를 확인한다.

## 확인 문제

1. RAII transaction이 commit 없이 파괴되면 어떻게 되는가?
2. 일괄 INSERT에서 트랜잭션이 성능을 높이는 이유는?
3. 사용자 입력을 기다리는 동안 트랜잭션을 유지하면 안 되는 이유는?

