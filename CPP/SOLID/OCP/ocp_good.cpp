#include <iostream>
#include <string>
using namespace std;

// OCP 준수 예제
// 같은 시나리오(고객 유형별 할인)를
// "할인 정책 인터페이스 + 구체 정책 클래스"로 분리.
//
// - 핵심: 결제 계산 코드는 DiscountPolicy 인터페이스에만 의존.
// - 새로운 할인 정책은 기존 코드를 수정하지 않고
//   새로운 클래스를 "추가"만 하면 된다.

// 1) 할인 정책 인터페이스
class DiscountPolicy {
public:
    virtual ~DiscountPolicy() = default;
    virtual double apply(double basePrice) const = 0;
};

// 2) 구체 정책들

// 일반 고객: 할인 없음
class NormalDiscount : public DiscountPolicy {
public:
    double apply(double basePrice) const override {
        return basePrice;
    }
};

// VIP 고객: 10% 할인
class VipDiscount : public DiscountPolicy {
public:
    double apply(double basePrice) const override {
        return basePrice * 0.9;
    }
};

// 임직원: 20% 할인
class EmployeeDiscount : public DiscountPolicy {
public:
    double apply(double basePrice) const override {
        return basePrice * 0.8;
    }
};

// 새로 추가된 정책: 학생 할인 15%
// -> 기존 결제 코드 수정 없이 이 클래스만 추가하면 됨.
class StudentDiscount : public DiscountPolicy {
public:
    double apply(double basePrice) const override {
        return basePrice * 0.85;
    }
};

// 3) 결제 계산기: 구체 클래스가 아니라 추상 인터페이스에만 의존
class OrderCalculator {
public:
    double calculate(double basePrice, const DiscountPolicy& policy) const {
        return policy.apply(basePrice);
    }
};

int main()
{
    cout << "===== Good Example (OCP 준수) =====\n";

    double price = 10000.0;
    OrderCalculator calc;

    NormalDiscount   normalPolicy;
    VipDiscount      vipPolicy;
    EmployeeDiscount empPolicy;
    StudentDiscount  studentPolicy; // 새로 추가

    cout << "일반 고객 결제 금액: "
         << calc.calculate(price, normalPolicy) << "원\n";

    cout << "VIP 고객 결제 금액: "
         << calc.calculate(price, vipPolicy) << "원\n";

    cout << "임직원 결제 금액: "
         << calc.calculate(price, empPolicy) << "원\n";

    cout << "학생 고객 결제 금액: "
         << calc.calculate(price, studentPolicy) << "원\n";

    // 주목할 점:
    // - 학생 할인(StudentDiscount)을 추가하기 위해
    //   OrderCalculator를 전혀 수정하지 않았다.
    // - 새로운 할인 정책은 DiscountPolicy를 상속하는
    //   클래스를 추가하는 것만으로 확장 가능.
    // -> "확장에는 열려 있고, 변경에는 닫혀 있다"는 OCP를 만족.

    return 0;
}
