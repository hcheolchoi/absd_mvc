// Controller/NetworkService.h

#ifndef NetworkServiceH
#define NetworkServiceH

#include <IdTCPClient.hpp>
#include <System.Classes.hpp>
#include <vector>
#include <functional>

class NetworkService; // 전방 선언

// Indy TCPClient로부터 데이터를 수신하는 스레드 클래스
class TTCPClientRawHandleThread : public TThread
{
private:
	TIdTCPClient* TCPClient;
	NetworkService* parentService; // 자신을 생성한 NetworkService를 가리킴
protected:
	void __fastcall Execute();
public:
	__fastcall TTCPClientRawHandleThread(TIdTCPClient *AClient, NetworkService* parent);
};


// 비동기 TCP 통신을 담당하는 서비스 클래스
class NetworkService
{
private:
	std::unique_ptr<TIdTCPClient> tcpClient;
	TTCPClientRawHandleThread *clientThread;
    // 데이터 수신 시 호출될 콜백 함수
	std::function<void(const std::vector<char>&)> onDataCallback;

public:
	NetworkService();
	~NetworkService();

	// AppController가 데이터 처리 함수를 등록하기 위해 호출
	void setDataCallback(std::function<void(const std::vector<char>&)> cb);
	// 스레드가 데이터 수신 시 호출하는 내부 함수
	void onRawDataReceived(const std::vector<char>& data);

	void connect(const String& ip, int port);
	void disconnect();
};

#endif