// Controller/NetworkService.cpp

#pragma hdrstop

#include "Controller/NetworkService.h"
#include <IdGlobal.hpp>

// --- TTCPClientRawHandleThread ---
__fastcall TTCPClientRawHandleThread::TTCPClientRawHandleThread(TIdTCPClient *AClient, NetworkService* parent)
	: TThread(false), TCPClient(AClient), parentService(parent)
{
}

void __fastcall TTCPClientRawHandleThread::Execute()
{
	Idglobal::TIdBytes buffer;

	while (!Terminated)
	{
		if (TCPClient && TCPClient->IOHandler && TCPClient->Connected())
		{
			try {
                // [수정] ReadBytes를 호출하여 버퍼를 직접 채우도록 함 (반환값 없음)
                // -1 인자는 현재 읽을 수 있는 모든 데이터를 읽으라는 의미입니다.
				TCPClient->IOHandler->ReadBytes(buffer, -1, false);

                // [수정] 읽은 바이트 수는 버퍼의 길이로 확인
				int bytesRead = buffer.Length;

				if (bytesRead > 0)
				{
					if (parentService) {
						std::vector<char> receivedData;
						receivedData.resize(bytesRead);
						if (bytesRead > 0) {
                            // TIdBytes는 내부적으로 Byte(unsigned char) 배열이므로 포인터를 직접 사용 가능
							memcpy(&receivedData[0], &buffer[0], bytesRead);
						}
						parentService->onRawDataReceived(receivedData);
					}
				}
			} catch(...) { /* 예외 처리 */ }
		} else {
			Sleep(100);
		}
	}
}

// --- NetworkService ---
NetworkService::NetworkService() : clientThread(nullptr)
{
	tcpClient = std::make_unique<TIdTCPClient>(nullptr);
}

NetworkService::~NetworkService()
{
	disconnect();
}

void NetworkService::setDataCallback(std::function<void(const std::vector<char>&)> cb)
{
	this->onDataCallback = cb;
}

void NetworkService::onRawDataReceived(const std::vector<char>& data)
{
	if (onDataCallback) {
		onDataCallback(data);
	}
}

void NetworkService::connect(const String& ip, int port)
{
	if (tcpClient->Connected()) disconnect();
	try {
		tcpClient->Host = ip;
		tcpClient->Port = port;
		tcpClient->Connect();
		if (tcpClient->Connected())
		{
			clientThread = new TTCPClientRawHandleThread(tcpClient.get(), this);
		}
	} catch (...) { /* 연결 실패 처리 */ }
}

void NetworkService::disconnect()
{
	if (clientThread) {
		clientThread->Terminate();
		clientThread->WaitFor();
		delete clientThread;
		clientThread = nullptr;
	}
	if (tcpClient && tcpClient->Connected()) {
		tcpClient->Disconnect();
	}
}