#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// SRP(단일 책임 원칙)를 지키도록 책임을 분리한 예제
// - UserInfo       : 사용자 정보만 관리
// - ServerInfo     : 서버 정보 및 출력 책임
// - AdminInfo      : 관리자 정보만 관리
// - AdminInfoRepository : 관리자 정보 저장 책임

// ① 사용자 정보만 담당하는 UserInfo
class UserInfo {
private:
    string userName;
    string userEmail;

public:
    UserInfo(const string& userName,
             const string& userEmail)
        : userName(userName),
          userEmail(userEmail)
    {}

    const string& getName()  const { return userName; }
    const string& getEmail() const { return userEmail; }

    void printUserInfo() const {
        cout << "[User] 이름: " << userName
             << ", 이메일: " << userEmail << endl;
    }
};

// ② 서버와 관련된 책임을 가진 ServerInfo 클래스
class ServerInfo {
private:
    string serverIp;
    int    serverPort;

public:
    ServerInfo(const string& ip, int port)
        : serverIp(ip), serverPort(port)
    {}

    const string& getIp()   const { return serverIp; }
    int           getPort() const { return serverPort; }

    // 서버 정보 출력은 서버 관련 클래스가 담당
    void printServerInfo() const {
        cout << "[Server] IP: " << serverIp
             << ", Port: " << serverPort << endl;
    }
};

// ③ 관리자와 관련된 정보를 담는 AdminInfo
class AdminInfo {
private:
    string adminName;
    string adminEmail;

public:
    AdminInfo(const string& name, const string& email)
        : adminName(name), adminEmail(email)
    {}

    const string& getName()  const { return adminName; }
    const string& getEmail() const { return adminEmail; }
};

// ④ 관리자 정보를 저장하는 책임만 가지는 클래스
class AdminInfoRepository {
public:
    void save(const AdminInfo& admin, const string& filename) const {
        ofstream ofs(filename);
        if (!ofs) {
            cerr << "파일 열기 실패: " << filename << endl;
            return;
        }
        ofs << "AdminName="  << admin.getName()  << "\n"
            << "AdminEmail=" << admin.getEmail() << "\n";
        cout << "관리자 정보를 파일에 저장했습니다: " << filename << endl;
    }
};

int main() {
    cout << "===== Good Example (SRP 준수) =====" << endl;

    // 각 클래스가 자기 책임만 가진다
    UserInfo user("홍길동", "hong@example.com");
    ServerInfo server("192.168.0.10", 8080);
    AdminInfo admin("관리자", "admin@example.com");
    AdminInfoRepository adminRepo;

    user.printUserInfo();                 // 사용자 출력
    server.printServerInfo();             // 서버 출력
    adminRepo.save(admin, "admin_good.txt"); // 관리자 정보 저장

    return 0;
}
