# C++ 스마트 포인터와 소유권(Ownership) 정리

## 0. 핵심 요약

- “**누가 이 객체의 수명을 책임지는가?**” → **소유권(ownership)**
- C++ 스마트 포인터는 소유권을 **타입으로 표현**하는 도구이다.
  - `std::unique_ptr` : 단일 소유(exclusive ownership)
  - `std::shared_ptr` : 공유 소유(shared ownership)
  - `std::make_unique` / `std::make_shared` : 안전한 생성기(factory function)
  - `std::weak_ptr` : 비소유(non-owning) 관찰자(pointer)

---

## 1. 소유권(Ownership) 기본 개념

### 1.1 용어 정의

- **소유자(owner)**  
  객체의 생명주기를 관리하고 삭제 책임을 갖는 주체.

- **비소유자(non-owner)**  
  객체를 참조하지만 삭제 책임이 없는 주체.

### 1.2 C 스타일 예시

```cpp
int* p = new int(10);
// ...
delete p;
```

- `p`는 사실상 소유자이지만 **타입에 소유권 정보가 없다.**
- 복사/대입/예외 흐름에서 `delete` 누락될 수 있음 → **메모리 누수 위험**

### 1.3 C++ 스마트 포인터가 해결하는 점

- 소유권 정책을 **타입 시스템으로 명확히 표현**한다.
- 단일 소유/공유 소유/비소유 참조를 명시적으로 분리.
- 스코프 기반 자동 파괴(RAII) 적용.

---

## 2. `std::unique_ptr` — 단일 소유(Exclusive Ownership)

### 2.1 특징

- 객체의 소유자는 **오직 1명**.
- **복사 불가능**, **이동(move) 가능**.
- 소유 포인터가 스코프에서 벗어나면 자동 삭제.

### 2.2 예제

```cpp
#include <iostream>
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource 생성\n"; }
    ~Resource() { std::cout << "Resource 소멸\n"; }
    void do_something() { std::cout << "Resource 사용 중...\n"; }
};

int main() {
    std::unique_ptr<Resource> p1 = std::make_unique<Resource>();
    p1->do_something();

    // 복사는 불가능
    // std::unique_ptr<Resource> p2 = p1; // 오류

    // 이동은 가능
    std::unique_ptr<Resource> p2 = std::move(p1);

    if (!p1) {
        std::cout << "p1 소유권 없음\n";
    }

    p2->do_something();
}
```

### 2.3 장점

- 참조 카운트가 없어 **가볍고 빠름**.
- 단일 소유 모델이 명확히 드러남.
- RAII 기반 안전한 메모리 관리.

### 2.4 단점

- 소유권 이동 필요(`std::move`).
- 공유 소유가 필요한 경우 사용 불가.

### 2.5 주 사용처

- “이 객체는 여기(owner)에서만 관리한다”가 명확한 경우.
- 컨테이너에서 객체 소유를 맡길 때.

---

## 3. `std::make_unique` — 안전한 unique_ptr 생성기

### 3.1 특징

- C++14부터 지원.
- 예외 안전성 우수.
- new 직접 사용보다 안전.

### 3.2 예제

```cpp
auto p = std::make_unique<Foo>(10, "hello");
```

### 3.3 장점

- 예외 안전성
- 코드 간결성
- 타입 추론(auto) 활용 가능

---

## 4. `std::shared_ptr` — 공유 소유(Shared Ownership)

### 4.1 특징

- 여러 포인터가 동일 객체를 **공동 소유**.
- 참조 카운트(reference count) 기반.
- 마지막 소유자가 사라질 때 객체 소멸.

### 4.2 예제

```cpp
#include <iostream>
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource 생성\n"; }
    ~Resource() { std::cout << "Resource 소멸\n"; }
};

void func(std::shared_ptr<Resource> p) {
    std::cout << "func 내부 count = " << p.use_count() << "\n";
}

int main() {
    std::shared_ptr<Resource> p1 = std::make_shared<Resource>();
    std::cout << "p1 count = " << p1.use_count() << "\n";

    {
        std::shared_ptr<Resource> p2 = p1;
        std::cout << "p2 생성 후 count = " << p1.use_count() << "\n";
        func(p1);
    }

    std::cout << "블록 종료 후 count = " << p1.use_count() << "\n";
}
```

### 4.3 장점

- 공유 구조 표현에 적합.
- 수명 관리가 여러 컴포넌트에 걸칠 때 사용.

### 4.4 단점

- 참조 카운트 연산 비용 발생.
- 멀티스레드 환경에서 더 무거워짐.
- **순환 참조 문제** 존재.

---

## 5. `std::make_shared` — 효율적 shared_ptr 생성

### 5.1 특징

- 객체 + 컨트롤 블록을 한 번에 할당.
- new 보다 빠르고 캐시 친화적.

### 5.2 예제

```cpp
auto p = std::make_shared<Foo>(100);
```

### 5.3 장점

- 예외 안전.
- 메모리 할당 1회 → 효율적.

---

## 6. `std::weak_ptr` — 비소유 참조(순환참조 방지)

### 6.1 순환참조 문제

```cpp
struct B;

struct A {
    std::shared_ptr<B> b;
};
struct B {
    std::shared_ptr<A> a;  // 순환참조 발생
};
```

- 서로가 서로를 소유 → use_count가 0이 되지 않음 → **메모리 누수**

### 6.2 weak_ptr로 해결

```cpp
struct B;

struct A {
    std::shared_ptr<B> b;
};
struct B {
    std::weak_ptr<A> a; // 비소유 참조
};
```

### 6.3 lock() 사용

```cpp
if (auto sp = a.lock()) {
    sp->doSomething();
}
```

---

## 7. 스마트 포인터 비교표

| 포인터 종류        | 소유 형태 | 복사 | 오버헤드 | 용도 |
|-------------------|-----------|------|----------|------|
| unique_ptr        | 단일 소유 | X    | 매우 낮음 | 기본 선택 |
| make_unique       | unique 생성 | - | - | 안전 생성 |
| shared_ptr        | 공유 소유 | O | 높음 | 공유 모델 |
| make_shared       | shared 생성 | - | - | 효율적 생성 |
| weak_ptr          | 비소유 참조 | - | 낮음 | 순환참조 방지 |

---

## 8. 종합 예제: Scene + GameObject 구조

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>

class GameObject;

class Scene {
public:
    void addObject(std::shared_ptr<GameObject> obj) {
        objects_.push_back(obj);
    }

    void updateAll() {
        std::cout << "=== Scene Update ===\n";
        for (auto& obj : objects_) {
            obj->doSomething();
        }
    }

private:
    std::vector<std::shared_ptr<GameObject>> objects_;
};

class GameObject {
public:
    GameObject(std::string name) : name_(std::move(name)) {
        std::cout << "GameObject(" << name_ << ") 생성\n";
    }

    ~GameObject() {
        std::cout << "GameObject(" << name_ << ") 소멸\n";
    }

    void setScene(std::weak_ptr<Scene> scene) {
        scene_ = scene;
    }

    void doSomething() {
        std::cout << name_ << " 동작 중\n";
        if (auto s = scene_.lock()) {
            std::cout << "Scene 살아 있음\n";
        } else {
            std::cout << "Scene 사라짐\n";
        }
    }

private:
    std::string name_;
    std::weak_ptr<Scene> scene_;
};

int main() {
    auto sceneOwner = std::make_unique<Scene>();

    auto player = std::make_shared<GameObject>("Player");
    auto enemy  = std::make_shared<GameObject>("Enemy");

    sceneOwner->addObject(player);
    sceneOwner->addObject(enemy);

    sceneOwner->updateAll();
}
```

---

## 9. 결론

- **unique_ptr = 기본값**
- **shared_ptr = 공유 필요할 때만**
- **weak_ptr = 순환참조 방지**
- **생성은 make_unique / make_shared 사용**
- **new/delete 직접 사용 금지**

스마트 포인터는 C++에서 안전하고 효율적인 메모리 관리의 핵심이다.
