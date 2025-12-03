#include <iostream>
#include <string>
using namespace std;

// OCP 위반 예제
// 시나리오: 고객 유형에 따라 다른 할인율 적용
// - 일반 고객: 할인 없음
// - VIP 고객: 10% 할인
// - 임직원: 20% 할인
//
// 문제점: 새로운 고객 유형이 추가될 때마다
//        calculateFinalPrice() 함수를 계속 수정해야 한다.

enum class CustomerType {
    Normal,
    Vip,
    Employee
};

double calculateFinalPrice(double basePrice, CustomerType type)
{
    // 새로운 고객 유형이 추가될 때마다 여기 if / else if 를 수정해야 한다.
    if (type == CustomerType::Normal) {
        return basePrice;            // 할인 없음
    }
    else if (type == CustomerType::Vip) {
        return basePrice * 0.9;      // 10% 할인
    }
    else if (type == CustomerType::Employee) {
        return basePrice * 0.8;      // 20% 할인
    }
    else {
        return basePrice;
    }
}

int main()
{
    double price = 10000.0;

    cout << "===== Bad Example (OCP 위반) =====\n";

    cout << "일반 고객 결제 금액: "
         << calculateFinalPrice(price, CustomerType::Normal) << "원\n";

    cout << "VIP 고객 결제 금액: "
         << calculateFinalPrice(price, CustomerType::Vip) << "원\n";

    cout << "임직원 결제 금액: "
         << calculateFinalPrice(price, CustomerType::Employee) << "원\n";

    // 만약 '학생(15% 할인)' 유형이 생기면?
    // 1) enum CustomerType 에 Student 추가
    // 2) calculateFinalPrice() 내부에 else if (Student) 블록 추가
    // -> 기존 함수를 수정해야 하므로 OCP(변경에는 닫혀 있어야 한다) 위반.

    return 0;
}
