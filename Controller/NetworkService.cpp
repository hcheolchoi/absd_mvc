#pragma hdrstop
#include "NetworkService.h"
#include <System.SysUtils.hpp>
#pragma package(smart_init)

NetworkService::NetworkService(TComponent* owner, DataReceivedCallback callback)
    : onDataReceived_(callback) {
    idTcpClient_ = new TIdTCPClient(owner);
    // Indy는 블로킹 방식으로 동작하므로 OnRead 같은 이벤트 핸들러는
    // 클라이언트 측에서는 조금 다르게 사용됩니다. 여기서는 단순화합니다.
}

NetworkService::~NetworkService() {
    delete idTcpClient_;
}

void NetworkService::connect(const std::string& host, int port) {
    if (!idTcpClient_->Connected()) {
        idTcpClient_->Host = host.c_str();
        idTcpClient_->Port = port;
        try {
            idTcpClient_->Connect();
        } catch (const EIdException &e) {
            // 연결 오류 처리 (예: ShowMessage)
        }
    }
}

void NetworkService::disconnect() {
    if (idTcpClient_->Connected()) {
        idTcpClient_->Disconnect();
    }
}

bool NetworkService::isConnected() {
    return idTcpClient_->Connected();
}

// 이 함수는 별도의 스레드에서 주기적으로 호출되어야 합니다.
void NetworkService::checkForData() {
    if (idTcpClient_ && idTcpClient_->Connected() && idTcpClient_->IOHandler) {
        // 읽을 데이터가 있는지 확인
        idTcpClient_->IOHandler->CheckForDataOnSource(10); // 10ms 타임아웃
        if (idTcpClient_->IOHandler->InputBufferIsEmpty() == false) {
            // 데이터가 있으면 한 줄 읽어서 콜백으로 전달
            std::string s = AnsiString(idTcpClient_->IOHandler->ReadLn()).c_str();
            if (onDataReceived_) {
                onDataReceived_(s);
            }
        }
    }
}
