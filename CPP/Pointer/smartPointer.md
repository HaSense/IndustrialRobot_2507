핵심부터 정리하면 이렇습니다.

“누가 이 객체의 수명을 책임지는가?” → 소유권(ownership)

C++ 스마트 포인터는 소유권을 타입으로 표현하는 도구입니다.

unique_ptr : 딱 1명만 소유

shared_ptr : 여러 명이 나눠 소유(참조 카운트)

make_unique / make_shared : 위 둘을 안전하게 생성하는 팩토리 함수

아래에서 각각 개념 → 예제 코드 → 장단점 / 사용 이유 순으로 정리하겠습니다.
(코드는 C++17 이상 기준, 주석은 한국어로 달겠습니다.)

1. 소유권(Ownership) 기본 개념

소유자(owner): 객체를 삭제할 책임이 있는 주체

비소유(non-owner): 객체를 잠깐 빌려 쓰지만 삭제 책임은 없는 주체

C 스타일

int* p = new int(10);
// ...
delete p;


p가 사실상 “소유자”이지만, 타입에 전혀 드러나지 않습니다.

복사, 대입, 예외 발생 시 delete를 빼먹기 쉽습니다.

C++ 스마트 포인터는 소유권이 타입에 묶여 있어서,

복사 가능 / 불가능

공유 가능 / 불가능

자동 파괴 시점
이 모두 명시적으로 드러납니다.

2. std::unique_ptr : 단일 소유(Exclusive Ownership)
2-1. 개념

“이 객체의 주인은 단 한 명이다”를 표현.

복사(copy) 불가능, 이동(move)만 가능.

포인터가 수명(스코프)에서 벗어날 때 자동으로 delete 호출.

2-2. 간단 예제
#include <iostream>
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource 생성\n"; }
    ~Resource() { std::cout << "Resource 소멸\n"; }

    void do_something() {
        std::cout << "Resource 사용 중...\n";
    }
};

int main() {
    // unique_ptr로 Resource 소유
    std::unique_ptr<Resource> p1 = std::make_unique<Resource>();
    p1->do_something();

    // 복사는 불가능
    // std::unique_ptr<Resource> p2 = p1; // 컴파일 에러

    // 이동은 가능 (소유권 이전)
    std::unique_ptr<Resource> p2 = std::move(p1);

    if (!p1) {
        std::cout << "p1은 더 이상 Resource를 소유하지 않음\n";
    }

    p2->do_something();

    // main 종료 시 p2가 소멸되며 Resource도 자동으로 delete
}


실행 흐름(개념적으로)

make_unique<Resource>()에서 new Resource() 수행

p1이 단일 소유자

std::move(p1) → p2에게 소유권 이전, p1은 null

main() 끝나면 p2 소멸 → Resource 소멸 출력

2-3. 장단점 및 사용 이유

장점

오버헤드(참조 카운트) 거의 없음 → 가장 가볍고 빠름.

“단일 소유”라는 의도가 타입에 박혀 있어서 코드 읽기가 좋음.

new/delete 직접 사용을 대체하여 메모리 누수 방지.

단점

복사가 안 되므로, 설계 상 공유 소유가 필요한 상황에는 부적합.

std::move에 대한 이해가 필요(초보에게는 약간 진입장벽).

언제 쓰는가?

객체의 소유자가 명확히 1개인 경우 (대부분의 상황)

예:

std::vector<std::unique_ptr<Base>>

“이 컨테이너가 이 객체들을 완전히 책임진다”는 구조

“기본값”으로 생각해도 될 정도로 추천되는 선택입니다.

3. std::make_unique : unique_ptr를 안전하게 생성
3-1. 개념

std::unique_ptr<T>를 생성하기 위한 팩토리 함수.

C++14부터 표준에 들어옴.

new를 직접 쓰지 않고도 unique_ptr를 만들 수 있음.

auto p = std::make_unique<Resource>();

3-2. 예외 안전성 측면

이렇게 쓰는 것보다:

std::unique_ptr<Resource> p(new Resource());  // 가능은 함


이렇게 쓰는 것이 더 안전합니다:

auto p = std::make_unique<Resource>();


왜냐하면,

new Resource()와 다른 인자의 평가 순서, 예외 발생 시점이 섞이면
드물게 예외 안전성 문제가 생길 수 있습니다.

make_unique는 내부에서 이 과정을 원자적으로 처리해 줍니다.

3-3. 간단 예제
#include <iostream>
#include <memory>

class Foo {
public:
    Foo(int x, const std::string& name)
        : x_(x), name_(name) {
        std::cout << "Foo(" << x_ << ", " << name_ << ") 생성\n";
    }
    ~Foo() {
        std::cout << "Foo 소멸\n";
    }

    void print() const {
        std::cout << "x = " << x_ << ", name = " << name_ << "\n";
    }

private:
    int x_;
    std::string name_;
};

int main() {
    auto p = std::make_unique<Foo>(42, "hello");
    p->print();
}


정리

unique_ptr를 쓴다면 항상 make_unique를 함께 쓴다고 생각하시면 됩니다.

예외 안전 + 간결함 + 타입 추론(auto)까지 한 번에 해결.

4. std::shared_ptr : 공유 소유(Shared Ownership)
4-1. 개념

여러 개의 스마트 포인터가 동일 객체를 함께 소유.

내부적으로 참조 카운트(reference count) 를 유지.

use_count() 로 현재 카운트 확인 가능.

카운트가 0이 되는 순간 객체 자동 삭제.

4-2. 간단 예제
#include <iostream>
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource 생성\n"; }
    ~Resource() { std::cout << "Resource 소멸\n"; }
};

void func(std::shared_ptr<Resource> p) {
    std::cout << "func 내부 use_count = " << p.use_count() << "\n";
}

int main() {
    std::shared_ptr<Resource> p1 = std::make_shared<Resource>();
    std::cout << "p1 use_count = " << p1.use_count() << "\n"; // 1

    {
        std::shared_ptr<Resource> p2 = p1;     // 카운트 증가
        std::cout << "p2 생성 후 use_count = " << p1.use_count() << "\n"; // 2

        func(p1);  // 값으로 전달 → 내부에서 use_count = 3
    } // p2 스코프 종료 → use_count = 1로 감소

    std::cout << "블록 종료 후 use_count = " << p1.use_count() << "\n"; // 1

    // main 끝나면 p1 소멸 → use_count 0 → Resource 소멸
}


핵심 포인트

p1, p2, func 인자를 포함해 모든 shared_ptr이 소유자로 계산됩니다.

마지막 소유자가 사라지는 시점에 객체가 삭제됩니다.

4-3. 장단점 및 사용 이유

장점

객체의 소유권을 여러 곳에서 합의적으로 공유할 수 있음.

“누가 delete할지 애매한 구조”에서 해결책을 줄 수 있음.

단점

오버헤드 존재:

참조 카운트 증가/감소는 원자적 연산(멀티스레드 환경에서는 더 비쌈).

순환 참조(circular reference) 문제:

A가 shared_ptr<B>를 가지고,
B가 shared_ptr<A>를 가지면,
둘 다 use_count가 0이 되지 않아 메모리 누수 발생.

소유 구조가 복잡해지기 쉬움 → “아무나 다 소유”가 되어 설계가 흐려짐.

언제 쓰는가?

진짜로 “여러 컴포넌트가 동일 객체의 생존을 공동으로 책임져야 할 때”

예:

그래프 구조에서 노드가 여러 곳에서 참조되지만, 명확한 부모 하나를 못 정할 때

GUI 위젯 트리나, 게임 오브젝트 일부 구조 등

현대 C++ 가이드라인에서는

“일단 unique_ptr를 기본으로 하고, 정말 공유가 필요할 때만 shared_ptr로 승급하라”
는 방향을 권장합니다.

5. std::make_shared : shared_ptr를 효율적으로 생성
5-1. 개념
auto p = std::make_shared<Resource>();


shared_ptr + 내부 컨트롤 블록(참조 카운트 등)을 한 번에 할당.

shared_ptr<Resource>(new Resource()) 보다 일반적으로 더 효율적이고 캐시 친화적.

5-2. 예제
#include <iostream>
#include <memory>

class Foo {
public:
    Foo(int x) : x_(x) {
        std::cout << "Foo(" << x_ << ") 생성\n";
    }
    ~Foo() {
        std::cout << "Foo 소멸\n";
    }
private:
    int x_;
};

int main() {
    auto p1 = std::make_shared<Foo>(100);
    auto p2 = p1;  // 공유

    std::cout << "use_count = " << p1.use_count() << "\n"; // 2
}

5-3. 장단점 및 주의점

장점

shared_ptr + 컨트롤 블록을 한 번에 할당 → 할당 비용 감소.

예외 안전성, 코드 간결성, 타입 추론(auto) 모두 장점.

shared_ptr를 쓴다면 거의 항상 make_shared를 함께 씀.

단점 / 주의

아주 특수한 경우(커스텀 deleter, 메모리 풀 관리 등)에는 make_shared를 쓰기 곤란할 수 있음.

weak_ptr과 함께 순환 참조를 끊는 설계까지 같이 고려해야 함(아래 참고).

6. 순환 참조와 std::weak_ptr (간단 언급)

shared_ptr 사용 시 반드시 옆에 따라붙는 개념입니다.

#include <memory>

struct B; // 전방 선언

struct A {
    std::shared_ptr<B> b;  // B를 shared로 소유
};

struct B {
    std::shared_ptr<A> a;  // A를 shared로 소유 (문제!)
};


A ↔ B 서로 shared_ptr를 들고 있으면, use_count가 0이 절대 되지 않음 → 메모리 누수.

해결책: 한쪽을 std::weak_ptr로 바꿉니다.

#include <memory>

struct B;

struct A {
    std::shared_ptr<B> b;      // A는 B를 실제로 소유
};

struct B {
    std::weak_ptr<A> a;        // B는 A를 '참조'만 하고, 소유하지 않음
};


이제 A가 사라지면 B의 weak_ptr<A>는 자동으로 무효가 되고,
필요 시 lock()으로 shared_ptr로 잠깐 승격해서 사용합니다.

즉, shared_ptr = 소유, weak_ptr = 소유하지 않는 관찰자(observer) 개념입니다.

7. 네 가지를 한 번에 비교 정리
7-1. 요약 표
타입	소유자 수	복사 가능	오버헤드	대표 사용처
unique_ptr	1명	X (move만)	거의 없음	기본 선택, 단일 소유
make_unique	- (유틸 함수)	-	-	unique_ptr 생성용
shared_ptr	여러 명	O	참조 카운트 비용	진짜 공유 소유 필요할 때
make_shared	- (유틸 함수)	-	-	shared_ptr 생성용, 더 효율적
7-2. 실무적 가이드라인

기본값: unique_ptr + make_unique

“누가 이 객체를 책임지는가?”가 명확하다면 거의 항상 이것.

정말 필요한 경우에만: shared_ptr + make_shared

객체 수명이 여러 모듈에 걸쳐 얽혀 있고,
특정 한 곳에 “부모”를 두기 어려울 때.

순환 구조가 필요하면

한쪽/일부는 weak_ptr로 바꾸어 비소유 참조로 만든다.

**생 날것의 new / delete**는

라이브러리 구현 수준이 아닌 이상, 일반 코드에서는 거의 쓰지 않는 것을 목표로.

8. 간단 종합 예제: unique_ptr + shared_ptr + weak_ptr

간단한 “장면(Scene)과 객체(GameObject)” 구조를 예로 듭니다.

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
            updateObject(obj);
        }
    }

private:
    void updateObject(const std::shared_ptr<GameObject>& obj);

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
        scene_ = scene; // Scene을 소유하지는 않음
    }

    void doSomething() {
        std::cout << name_ << " 동작 중\n";
        // scene_이 아직 살아있는지 확인 후 사용
        if (auto s = scene_.lock()) {
            std::cout << "Scene이 아직 살아있음\n";
        } else {
            std::cout << "Scene이 이미 파괴됨\n";
        }
    }

private:
    std::string name_;
    std::weak_ptr<Scene> scene_; // Scene에 대한 비소유 참조
};

void Scene::updateObject(const std::shared_ptr<GameObject>& obj) {
    obj->doSomething();
}

int main() {
    // Scene은 unique_ptr로 단일 소유
    auto sceneOwner = std::make_unique<Scene>();

    // Scene을 shared_ptr로도 관리하고 싶다면 별도 설계 필요하지만,
    // 여기서는 단일 소유라고 가정.
    // GameObject는 shared_ptr로 여러 곳에서 공유
    auto player = std::make_shared<GameObject>("Player");
    auto enemy  = std::make_shared<GameObject>("Enemy");

    // GameObject는 Scene을 weak_ptr로만 본다 (순환참조 방지)
    // (여기서는 Scene을 shared_ptr로 만들지 않았으므로 단순 예시)
    // 현실적 구조에서는 Scene도 shared_ptr 관리 구조로 올릴 수 있음.

    sceneOwner->addObject(player);
    sceneOwner->addObject(enemy);

    sceneOwner->updateAll();

    // main 종료 시:
    // 1) sceneOwner unique_ptr 소멸 → Scene 소멸
    // 2) player/enemy shared_ptr 소멸 → GameObject 소멸
}


여기서 포인트만 뽑으면:

전체 Scene 소유: unique_ptr<Scene>

Scene 내부의 GameObject들: std::vector<std::shared_ptr<GameObject>>

GameObject에서 Scene으로의 역참조: std::weak_ptr<Scene> (소유 X)

즉,

상위 구조(Scene): 보통 unique_ptr 혹은 단일 소유 컨테이너

하위 객체(GameObject): 필요시 shared_ptr로 여러 곳에서 공유

역참조/관찰자: weak_ptr

이 패턴을 이해하면 스마트 포인터 + 소유권 모델을 실전에서 바로 적용할 수 있습니다.
