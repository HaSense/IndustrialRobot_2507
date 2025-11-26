#include <iostream>
using namespace std;

class Shallow {
private:
    int* data;

public:
    Shallow(int value) {
        data = new int(value);
    }

    // 얇은 복사: pointer만 복사 → 같은 메모리 공유
    Shallow(const Shallow& other) {
        data = other.data;
    }

    ~Shallow() {
        cout << "delete: " << *data << endl;
        delete data;
    }

    void set(int v) { *data = v; }
    int get() const { return *data; }
};

int main() {
    Shallow a(10);
    Shallow b = a;  // 얇은 복사(Shallow Copy)

    b.set(20);

    cout << "a = " << a.get() << endl;  // 20 (같은 메모리 참조 → 값이 공유됨)
    cout << "b = " << b.get() << endl;  // 20

    return 0;
}
