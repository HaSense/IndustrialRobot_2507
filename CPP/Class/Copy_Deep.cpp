#include <iostream>
using namespace std;

class Deep {
private:
    int* data;

public:
    Deep(int value) {
        data = new int(value);
    }

    // 깊은 복사: 새로운 메모리를 할당하여 값을 복사
    Deep(const Deep& other) {
        data = new int(*other.data);
    }

    ~Deep() {
        cout << "delete: " << *data << endl;
        delete data;
    }

    void set(int v) { *data = v; }
    int get() const { return *data; }
};

int main() {
    Deep a(10);
    Deep b = a;  // 깊은 복사(Deep Copy)

    b.set(20);

    cout << "a = " << a.get() << endl;  // 10 (독립된 메모리이므로 영향 없음)
    cout << "b = " << b.get() << endl;  // 20

    return 0;
}
