#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Product: 빌더가 생성할 객체
class Car {
private:
    string engine;
    string tires;
    string color;
    string interior;
    bool hasSunroof = false;

public:
    // 각 속성을 설정하는 메서드
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

// ConcreteBuilder: Car 객체를 생성하는 빌더 클래스
class CarBuilder {
private:
    // Car 객체를 스마트 포인터로 관리
    unique_ptr<Car> car = make_unique<Car>();

public:
    // 메서드 체이닝을 위해 빌더 객체의 참조를 반환
    CarBuilder& buildEngine(const string& engine) {
        this->car->setEngine(engine);
        return *this;
    }

    CarBuilder& buildTires(const string& tires) {
        this->car->setTires(tires);
        return *this;
    }

    CarBuilder& buildColor(const string& color) {
        this->car->setColor(color);
        return *this;
    }

    CarBuilder& buildInterior(const string& interior) {
        this->car->setInterior(interior);
        return *this;
    }

    CarBuilder& withSunroof() {
        this->car->setHasSunroof(true);
        return *this;
    }

    // 최종적으로 완성된 Car 객체를 반환
    unique_ptr<Car> getResult() {
        return move(car);
    }
};

int main() {
    CarBuilder builder;

    // 빌더 패턴을 사용한 객체 생성 (메서드 체이닝)
    auto myCustomCar = builder.buildEngine("V6 엔진")
                             .buildTires("겨울용 타이어")
                             .buildColor("흰색")
                             .buildInterior("패브릭 시트")
                             .getResult(); // 선루프는 선택적으로 제외

    myCustomCar->printDetails();

    cout << "\n--- 선루프가 있는 다른 자동차 ---" << endl;
    // 모든 속성을 설정하지 않아도 됩니다.
    auto myLuxuryCar = CarBuilder().buildEngine("V8 엔진")
                                   .buildColor("검정색")
                                   .withSunroof() // 선루프 추가
                                   .getResult();

    myLuxuryCar->printDetails();

    return 0;
}
