//---------------------------------------------------------------------------
#ifndef NetworkServiceH
#define NetworkServiceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <IdTCPClient.hpp>
#include <string>

// Forward declaration
class DataParser;

// VCL의 TThread를 상속받아 실제 네트워크 작업을 수행하는 쓰레드 클래스
class TNetworkThread : public TThread
{
private:
    std::string FHost;
    int FPort;
    TIdTCPClient* FTcpClient;
    DataParser* FDataParser;

protected:
    void __fastcall Execute();

public:
    __fastcall TNetworkThread(const std::string& host, int port, DataParser* parser);
};


// 네트워크 서비스를 관리하는 메인 클래스
class NetworkService
{
private:
    TNetworkThread* FNetworkThread;
    DataParser* FDataParser;

public:
    NetworkService(DataParser* parser);
    ~NetworkService();

    void Start(const std::string& host, int port);
    void Stop();
};

#endif