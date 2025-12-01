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

    // 생성자는 Builder만 호출할 수 있게 private
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

public:
    // === Builder 선언 ===
    class Builder {
    private:
        // 빌더가 관리하는 설정 값들 (기본값 포함)
        string host;
        int    port;
        bool   useSSL        = false;
        int    timeoutMs     = 3000;
        int    maxConnections= 50;
        int    retryCount    = 0;
        string logLevel      = "INFO";

    public:
        // 필수 값은 생성자에서 받도록 설계 (예: host, port)
        Builder(const string& host, int port)
            : host(host), port(port) {}

        // 이하 체이닝 메서드들
        Builder& setUseSSL(bool value) {
            useSSL = value;
            return *this;
        }

        Builder& setTimeoutMs(int value) {
            timeoutMs = value;
            return *this;
        }

        Builder& setMaxConnections(int value) {
            maxConnections = value;
            return *this;
        }

        Builder& setRetryCount(int value) {
            retryCount = value;
            return *this;
        }

        Builder& setLogLevel(const string& value) {
            logLevel = value;
            return *this;
        }

        // 최종적으로 ServerConfig 객체를 생성
        ServerConfig build() const {
            return ServerConfig(
                host,
                port,
                useSSL,
                timeoutMs,
                maxConnections,
                retryCount,
                logLevel
            );
        }
    };

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
    // 빌더 패턴 사용 예
    ServerConfig config = ServerConfig::Builder("example.com", 443)
        .setUseSSL(true)
        .setTimeoutMs(5000)
        .setMaxConnections(100)
        .setRetryCount(3)
        .setLogLevel("DEBUG")
        .build();

    cout << "[서버 설정 출력]\n";
    config.print();

    // 선택 옵션을 줄여서 호출하는 것도 자연스러움
    ServerConfig simpleConfig = ServerConfig::Builder("localhost", 8080)
        .setLogLevel("WARN")   // 필요한 것만 설정
        .build();              // 나머지는 기본값 사용

    cout << "\n[단순 서버 설정 출력]\n";
    simpleConfig.print();

    return 0;
}
