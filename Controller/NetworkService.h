// Controller/NetworkService.h

#ifndef NetworkServiceH
#define NetworkServiceH

#include <functional>
#include <vector>
#include <memory>
#include <IdTCPClient.hpp>
#include <System.Classes.hpp> // TThread

class NetworkService; // 전방 선언

// 스레드 클래스: 이제 부모 NetworkService의 포인터를 받음
class TTCPClientRawHandleThread : public TThread
{
private:
    TIdTCPClient *TCPClient;
    NetworkService* parentService; // 부모 포인터

protected:
    void __fastcall Execute();

public:
    __fastcall TTCPClientRawHandleThread(TIdTCPClient *AClient, NetworkService* parent);
};


class NetworkService {
private:
    std::unique_ptr<TIdTCPClient> tcpClient;
    TTCPClientRawHandleThread* clientThread;
    std::function<void(const std::vector<char>&)> onDataCallback;

public:
    NetworkService();
    ~NetworkService();

    void setDataCallback(std::function<void(const std::vector<char>&)> cb);
    void connect(const String& ip, int port);
    void disconnect();

    // 스레드가 데이터를 수신했을 때 호출할 public 메소드
    void onRawDataReceived(const std::vector<char>& data);
};

#endif