#ifndef NetworkService_H
#define NetworkService_H

#include <IdTCPClient.hpp> // Vcl.Sockets.hpp ��� ���
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

    // �����͸� �д� ������ ��Ʈ�ѷ��� ���� �����忡�� ó��
    void checkForData();

private:
    TIdTCPClient* idTcpClient_;
    DataReceivedCallback onDataReceived_;
};

#endif // NetworkService_H
