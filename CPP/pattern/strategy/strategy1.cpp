#include <iostream>
#include <string>

using namespace std;

// 전방선언
class Document;

// 1. Strategy 인터페이스
class IPrintStrategy {
public:
    virtual void print(const Document& doc) = 0;
    virtual ~IPrintStrategy() {}
};

// 2. Document 클래스 (Context)
class Document {
private:
    string content;
    IPrintStrategy* strategy;

public:
    Document(const string& text) : content(text), strategy(nullptr) {}

    void setStrategy(IPrintStrategy* s) {
        strategy = s;
    }

    string getContent() const {
        return content;
    }

    void print() const {
        if (strategy)
            strategy->print(*this);
        else
            cout << "[에러] 출력 전략이 설정되지 않았습니다." << endl;
    }
};

// 3. PDF 출력 전략
class PDFPrinter : public IPrintStrategy {
public:
    void print(const Document& doc) override {
        cout << "[PDF 출력] 문서 내용을 PDF로 저장합니다." << endl;
        cout << "내용: " << doc.getContent() << endl;
    }
};

// 4. 실제 프린터 출력 전략
class PhysicalPrinter : public IPrintStrategy {
public:
    void print(const Document& doc) override {
        cout << "[프린터 출력] 문서를 실제 프린터로 출력합니다." << endl;
        cout << "내용: " << doc.getContent() << endl;
    }
};

// 5. 클라이언트 코드
int main() {
    Document doc("C++ 전략 패턴 예제입니다.");

    PDFPrinter pdf;
    PhysicalPrinter printer;

    doc.setStrategy(&pdf);
    doc.print();

    cout << endl;

    doc.setStrategy(&printer);
    doc.print();

    return 0;
}
