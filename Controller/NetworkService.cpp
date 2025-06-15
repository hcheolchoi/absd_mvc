// Controller/NetworkService.cpp

#pragma hdrstop

#include "Controller/NetworkService.h"

// --- TTCPClientRawHandleThread ---
__fastcall TTCPClientRawHandleThread::TTCPClientRawHandleThread(TIdTCPClient *AClient, NetworkService* parent)
    : TThread(false), TCPClient(AClient), parentService(parent) // 부모 포인터 저장
{
}

void __fastcall TTCPClientRawHandleThread::Execute()
{
    std::vector<char> buffer(2048);
    while (!Terminated)
    {
        if (TCPClient && TCPClient->IOHandler && TCPClient->Connected())
        {
            try {
                int bytesRead = TCPClient->IOHandler->ReadBytes(buffer, buffer.size(), false);
                if (bytesRead > 0)
                {
                    // 부모의 메소드를 직접 호출
                    if (parentService) {
                         std::vector<char> receivedData(buffer.begin(), buffer.begin() + bytesRead);
                         // Synchronize를 사용해 메인 스레드에서 처리하도록 요청할 수도 있지만,
                         // 일단 직접 호출로 구현. 데이터 경합이 발생하면 Synchronize 필요.
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

// 스레드로부터 호출되는 함수
void NetworkService::onRawDataReceived(const std::vector<char>& data)
{
    // AppController가 등록한 콜백 함수를 실행
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
            // 스레드 생성 시 this (NetworkService 자신)를 넘겨줌
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