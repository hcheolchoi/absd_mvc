#pragma hdrstop
#include "NetworkService.h"
#include <System.SysUtils.hpp>
#pragma package(smart_init)

NetworkService::NetworkService(TComponent* owner, DataReceivedCallback callback)
    : onDataReceived_(callback) {
    idTcpClient_ = new TIdTCPClient(owner);
    // Indy�� ���ŷ ������� �����ϹǷ� OnRead ���� �̺�Ʈ �ڵ鷯��
    // Ŭ���̾�Ʈ �������� ���� �ٸ��� ���˴ϴ�. ���⼭�� �ܼ�ȭ�մϴ�.
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
            // ���� ���� ó�� (��: ShowMessage)
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

// �� �Լ��� ������ �����忡�� �ֱ������� ȣ��Ǿ�� �մϴ�.
void NetworkService::checkForData() {
    if (idTcpClient_ && idTcpClient_->Connected() && idTcpClient_->IOHandler) {
        // ���� �����Ͱ� �ִ��� Ȯ��
        idTcpClient_->IOHandler->CheckForDataOnSource(10); // 10ms Ÿ�Ӿƿ�
        if (idTcpClient_->IOHandler->InputBufferIsEmpty() == false) {
            // �����Ͱ� ������ �� �� �о �ݹ����� ����
            std::string s = AnsiString(idTcpClient_->IOHandler->ReadLn()).c_str();
            if (onDataReceived_) {
                onDataReceived_(s);
            }
        }
    }
}
