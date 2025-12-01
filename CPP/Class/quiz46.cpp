#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <vector>
using namespace std;

class Account 
{
private:
    int id;             // 계좌번호
    string name;        // 예금주 이름
    int balance;        // 잔액 (원)

public:
    Account(int id, const string& name, int balance){
        this->id = id;
        this->name = name;
        this->balance = balance;
    }

    int getId() const { return id; }
    const string& getName() const { return name; }
    int getBalance() const { return balance; }

    // 입금
    bool deposit(int amount) {
        if (amount <= 0) {
            cout << "입금 금액은 0원보다 커야 합니다.\n";
            return false;
        }
        balance += amount;
        return true;
    }

    // 출금
    bool withdraw(int amount) {
        if (amount <= 0) {
            cout << "출금 금액은 0원보다 커야 합니다.\n";
            return false;
        }
        if (balance < amount) {
            cout << "잔액이 부족합니다. 현재 잔액: " << balance << "원\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    void printInfo() const {
        cout << "[계좌번호] " << id
             << " / [예금주] " << name
             << " / [잔액] " << balance << "원\n";
    }
};

int main(int argc, char* argv[]) {
     // 1. 계좌 생성
    Account acc1(1001, "홍길동", 50000);
    Account acc2(1002, "이순신", 30000);

    // 2. 입금/출금 테스트
    acc1.deposit(20000);    // 정상 입금
    acc2.withdraw(5000);    // 정상 출금
    acc2.withdraw(50000);   // 실패 테스트 (잔액 부족)

    // 3. 계좌 정보 출력
    acc1.printInfo();
    acc2.printInfo();

    // 4. vector로 여러 계좌 관리
    vector<Account> v;
    v.push_back(acc1);
    v.push_back(acc2);

    cout << "\n=== 전체 계좌 목록 ===\n";
    for (const auto& acc : v) {
        acc.printInfo();
    }
    return 0;
}
