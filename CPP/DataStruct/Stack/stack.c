// stack_cpp.cpp
#include <iostream>
#include <vector>

using namespace std;

class Stack {
private:
    vector<int> data;
    const size_t MAX_SIZE = 5;

public:
    void push(int value) {
        if (data.size() >= MAX_SIZE) {
            cout << "스택이 가득 찼습니다." << endl;
            return;
        }
        data.push_back(value);
    }

    int pop() {
        if (isEmpty()) {
            cout << "스택이 비었습니다." << endl;
            return -1;
        }
        int value = data.back();
        data.pop_back();
        return value;
    }

    int top() const {
        if (isEmpty()) {
            cout << "스택이 비었습니다." << endl;
            return -1;
        }
        return data.back();
    }

    bool isEmpty() const {
        return data.empty();
    }
};

int main() {
    Stack s;
    s.push(10);
    s.push(20);
    cout << "Top: " << s.top() << endl;
    cout << "Pop: " << s.pop() << endl;
    cout << "Pop: " << s.pop() << endl;
    cout << "IsEmpty: " << s.isEmpty() << endl;
    return 0;
}
