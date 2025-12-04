#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>   // iota
#include <memory>    // unique_ptr
using namespace std;

// =========================
// 데이터 구조: 로또 결과
// =========================
struct LottoResult {
    vector<int> numbers;  // 메인 번호 6개
    int bonus;            // 보너스 번호
};

// =====================================
// 인터페이스: 로또 번호 생성 책임 (SRP)
// =====================================
class ILottoNumberGenerator {
public:
    virtual ~ILottoNumberGenerator() = default;
    virtual LottoResult generate() = 0;  // 로또 번호 생성
};

// ==========================================
// 구현체: 난수 기반 로또 번호 생성기 (OCP)
// ==========================================
class RandomLottoNumberGenerator : public ILottoNumberGenerator {
private:
    mt19937 gen;

public:
    RandomLottoNumberGenerator()
        : gen(random_device{}()) // 난수 엔진 초기화
    {}

    LottoResult generate() override {
        // 1. 1~45 자동 생성
        vector<int> pool(45);
        iota(pool.begin(), pool.end(), 1);

        // 2. 셔플하여 순서를 무작위로 섞기
        shuffle(pool.begin(), pool.end(), gen);

        // 3. 로또 6개와 보너스 번호 추출
        LottoResult result;
        result.numbers.assign(pool.begin(), pool.begin() + 6);
        result.bonus = pool[6];

        // 4. 번호 정렬
        sort(result.numbers.begin(), result.numbers.end());

        return result;
    }
};

// ====================================
// 인터페이스: 로또 결과 출력 책임 (SRP)
// ====================================
class ILottoPrinter {
public:
    virtual ~ILottoPrinter() = default;
    virtual void print(const LottoResult& result) = 0;
};

// =========================================
// 구현체: 콘솔에 로또 결과 출력 (OCP)
// =========================================
class ConsoleLottoPrinter : public ILottoPrinter {
public:
    void print(const LottoResult& result) override {
        cout << "로또 번호: ";
        for (int x : result.numbers) {
            cout << x << ' ';
        }
        cout << "\n보너스 번호: " << result.bonus << endl;
    }
};

// =========================================
// 상위 수준 모듈: 로또 게임 실행기 (DIP)
// =========================================
class LottoGame {
private:
    unique_ptr<ILottoNumberGenerator> generator; // 추상화에 의존
    unique_ptr<ILottoPrinter> printer;           // 추상화에 의존

public:
    LottoGame(unique_ptr<ILottoNumberGenerator> gen,
              unique_ptr<ILottoPrinter> prt)
        : generator(move(gen)), printer(move(prt)) {}

    void run() {
        LottoResult result = generator->generate();
        printer->print(result);
    }
};

int main(int argc, char* argv[]) {
    // 구체 구현체는 여기서만 선택 (조립부)
    auto generator = make_unique<RandomLottoNumberGenerator>();
    auto printer   = make_unique<ConsoleLottoPrinter>();

    LottoGame game(move(generator), move(printer));
    game.run();

    return 0;
}
