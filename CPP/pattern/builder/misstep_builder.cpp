#include <iostream>
#include <string>

using namespace std;

class Car {
private:
    string engine;
    string tires;
    string color;
    string interior;
    bool hasSunroof;

public:
    // 매개변수가 많은 생성자
    Car(const string& engine, const string& tires, const string& color, const string& interior, bool hasSunroof)
        : engine(engine), tires(tires), color(color), interior(interior), hasSunroof(hasSunroof) {}

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

int main() {
    // 매개변수가 많아 순서를 헷갈리거나 실수를 할 가능성이 높습니다.
    Car myCar("V8 엔진", "사계절 타이어", "검정색", "가죽 시트", true);
    myCar.printDetails();

    return 0;
}
