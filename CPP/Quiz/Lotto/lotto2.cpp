#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <vector>
#include <random>
using namespace std;

class Lotto {
public:
    void generate() {
    // 1. 난수 엔진
    random_device rd;
    mt19937 gen(rd());

    // 2. 1~45 자동 생성
    vector<int> pool(45);
    iota(pool.begin(), pool.end(), 1);   // 1~45 채움

    // 3. 셔플하여 순서를 무작위로 섞기
    shuffle(pool.begin(), pool.end(), gen);

    // 4. 로또 6개와 보너스 번호 가져오기
    vector<int> lotto(pool.begin(), pool.begin() + 6);
    int bonus = pool[6];

    // 5. 로또 번호 정렬
    sort(lotto.begin(), lotto.end());

    // 6. 출력
    cout << "로또 번호: ";
    for (int x : lotto) {
        cout << x << ' ';
    }
    cout << "\n보너스 번호: " << bonus << endl;    

    }
private:
    
};

int main(int argc, char* argv[]) {
    
    Lotto lotto;
    lotto.generate();
    return 0;
}
