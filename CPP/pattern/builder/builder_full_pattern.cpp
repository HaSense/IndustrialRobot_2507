#include <iostream>
#include <string>

using namespace std;

// 1. Product: 빌더가 생성할 복잡한 객체 (여기서는 Car)
// 최종적으로 완성될 객체의 모든 구성 요소를 정의합니다.
class Car {
private:
    string engine;
    string tires;
    string color;
    string interior;
    bool hasSunroof = false;

public:
    // 소멸자: 메모리 정리할 것이 없지만, 다른 리소스가 있다면 여기서 정리합니다.
    ~Car() {
        cout << "Car 객체 소멸" << endl;
    }

    void setEngine(const string& e) { this->engine = e; }
    void setTires(const string& t) { this->tires = t; }
    void setColor(const string& c) { this->color = c; }
    void setInterior(const string& i) { this->interior = i; }
    void setHasSunroof(bool s) { this->hasSunroof = s; }

    void printDetails() const {
        cout << "--- 자동차 세부 정보 ---" << endl;
        cout << "엔진: " << this->engine << endl;
        cout << "타이어: " << this->tires << endl;
        cout << "색상: " << this->color << endl;
        cout << "내부: " << this->interior << endl;
        cout << "선루프: " << (this->hasSunroof ? "있음" : "없음") << endl;
        cout << "----------------------" << endl;
    }
};

// 2. Builder: 객체의 각 부분을 만드는 추상 인터페이스
// 모든 빌더는 이 인터페이스를 구현해야 합니다.
class CarBuilder {
public:
    virtual ~CarBuilder() {}
    virtual void buildEngine(const string& engine) = 0;
    virtual void buildTires(const string& tires) = 0;
    virtual void buildColor(const string& color) = 0;
    virtual void buildInterior(const string& interior) = 0;
    virtual void withSunroof() = 0;
    virtual Car* getResult() = 0; // Car* 포인터를 반환
};

// 3. ConcreteBuilder: Builder 인터페이스를 구현하는 구체적인 빌더 클래스
// Car 객체의 실제 구성 로직을 포함합니다.
class ConcreteCarBuilder : public CarBuilder {
private:
    Car* car; // 원시 포인터

public:
    ConcreteCarBuilder() {
        this->car = new Car();
    }
    
    ~ConcreteCarBuilder() {
        // ConcreteCarBuilder 소멸 시 Car 객체가 남아있다면 정리
        // getResult() 호출 후에는 car가 nullptr이 되어 이중 삭제를 방지합니다.
        delete this->car;
    }

    void buildEngine(const string& engine) override {
        this->car->setEngine(engine);
    }
    void buildTires(const string& tires) override {
        this->car->setTires(tires);
    }
    void buildColor(const string& color) override {
        this->car->setColor(color);
    }
    void buildInterior(const string& interior) override {
        this->car->setInterior(interior);
    }
    void withSunroof() override {
        this->car->setHasSunroof(true);
    }
    
    // 최종적으로 완성된 Car 객체(Product)를 반환하고,
    // 소유권을 호출자에게 넘겨줍니다.
    Car* getResult() override {
        Car* result = this->car;
        this->car = nullptr; // 소유권을 넘겼으므로 포인터를 비웁니다.
        return result;
    }
};

// 4. Director: 빌더를 사용하여 복잡한 객체 생성 과정을 관리
// 여러 단계를 거쳐 객체를 만드는 표준화된 방법을 제공합니다.
class CarManufacturingDirector {
private:
    CarBuilder* builder;

public:
    // Director는 Builder의 포인터를 가집니다.
    // 이 포인터의 메모리 관리는 Director 외부에서 이루어져야 합니다.
    void setBuilder(CarBuilder* b) {
        this->builder = b;
    }

    // 표준형 자동차를 만드는 메서드
    void constructStandardCar() {
        if (!builder) return;
        this->builder->buildEngine("4기통 엔진");
        this->builder->buildTires("사계절 타이어");
        this->builder->buildColor("은색");
        this->builder->buildInterior("패브릭 시트");
    }

    // 고급형 자동차를 만드는 메서드
    void constructLuxuryCar() {
        if (!builder) return;
        this->builder->buildEngine("V8 엔진");
        this->builder->buildTires("스포츠 타이어");
        this->builder->buildColor("검정색");
        this->builder->buildInterior("고급 가죽 시트");
        this->builder->withSunroof();
    }
};

// 5. Client: 빌더와 디렉터를 사용하여 객체를 생성하고 사용하는 주체
int main() {
    // 힙에 빌더 객체 생성
    ConcreteCarBuilder* builder = new ConcreteCarBuilder();
    CarManufacturingDirector director;

    // Director에게 Builder 객체를 전달
    director.setBuilder(builder);

    cout << "--- Director를 통해 표준형 자동차 생성 ---" << endl;
    director.constructStandardCar();
    Car* standardCar = builder->getResult();
    standardCar->printDetails();

    // 힙에 빌더 객체 생성
    ConcreteCarBuilder* builder2 = new ConcreteCarBuilder();
    director.setBuilder(builder2);

    cout << "\n--- Director를 통해 고급형 자동차 생성 ---" << endl;
    director.constructLuxuryCar();
    Car* luxuryCar = builder2->getResult();
    luxuryCar->printDetails();

    cout << "\n--- Director 없이 Client가 직접 빌더 사용 ---" << endl;
    // Client가 직접 빌더 메서드들을 호출하여 커스텀 객체 생성
    ConcreteCarBuilder* customBuilder = new ConcreteCarBuilder();
    customBuilder->buildEngine("전기 모터");
    customBuilder->buildColor("파란색");
    customBuilder->buildInterior("친환경 소재 시트");
    customBuilder->withSunroof();
    Car* customCar = customBuilder->getResult();
    customCar->printDetails();

    // !!! 중요: new로 할당한 메모리를 delete로 해제
    delete standardCar;
    delete builder; // 빌더 소멸자에서 Car*가 nullptr이므로 문제없음
    
    delete luxuryCar;
    delete builder2; // 빌더2 소멸자에서 Car*가 nullptr이므로 문제없음

    delete customCar;
    delete customBuilder; // 커스텀 빌더 소멸자에서 Car*가 nullptr이므로 문제없음

    return 0;
}
