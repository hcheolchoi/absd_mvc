#ifndef NetworkService_H
#define NetworkService_H

#include <IdTCPClient.hpp> // Vcl.Sockets.hpp 대신 사용
#include <string>
#include <functional>

class NetworkService {
public:
    using DataReceivedCallback = std::function<void(const std::string&)>;

    NetworkService(TComponent* owner, DataReceivedCallback callback);
    ~NetworkService();

    void connect(const std::string& host, int port);
    void disconnect();
    bool isConnected();

    // 데이터를 읽는 로직은 컨트롤러의 별도 스레드에서 처리
    void checkForData();

private:
    TIdTCPClient* idTcpClient_;
    DataReceivedCallback onDataReceived_;
};

#endif // NetworkService_H
