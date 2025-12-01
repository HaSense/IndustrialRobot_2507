#include <iostream>
#include <string>
using namespace std;

class ServerConfig {
private:
    string host;
    int    port;
    bool   useSSL;
    int    timeoutMs;
    int    maxConnections;
    int    retryCount;
    string logLevel;

public:
    // 매개변수 7개짜리 생성자
    ServerConfig(const string& host,
                 int port,
                 bool useSSL,
                 int timeoutMs,
                 int maxConnections,
                 int retryCount,
                 const string& logLevel)
        : host(host),
          port(port),
          useSSL(useSSL),
          timeoutMs(timeoutMs),
          maxConnections(maxConnections),
          retryCount(retryCount),
          logLevel(logLevel)
    {
    }

    void print() const {
        cout << "호스트: " << host << "\n"
             << "포트: " << port << "\n"
             << "SSL 사용: " << (useSSL ? "예" : "아니오") << "\n"
             << "타임아웃(ms): " << timeoutMs << "\n"
             << "최대 접속 수: " << maxConnections << "\n"
             << "재시도 횟수: " << retryCount << "\n"
             << "로그 레벨: " << logLevel << "\n";
    }
};

int main() {
    // 이런 식으로 호출하면 가독성이 매우 떨어짐
    ServerConfig config(
        "example.com", // host
        443,           // port
        true,          // useSSL
        5000,          // timeoutMs
        100,           // maxConnections
        3,             // retryCount
        "INFO"         // logLevel
    );

    config.print();
    return 0;
}
