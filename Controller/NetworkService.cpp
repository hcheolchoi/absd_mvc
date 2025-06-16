//---------------------------------------------------------------------------
#pragma hdrstop

#include "NetworkService.h"
#include "DataParser.h"
#include <System.SysUtils.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//--- TNetworkThread --------------------------------------------------------

__fastcall TNetworkThread::TNetworkThread(const std::string& host, int port, DataParser* parser)
    : TThread(true), FHost(host), FPort(port), FDataParser(parser), FTcpClient(nullptr)
{
    FreeOnTerminate = true; // 쓰레드 종료 시 자동으로 메모리 해제
}

void __fastcall TNetworkThread::Execute()
{
    // 쓰레드가 시작되면 이 함수가 호출됩니다.
    FTcpClient = new TIdTCPClient(NULL);
    FTcpClient->Host = FHost.c_str();
    FTcpClient->Port = FPort;

    while (!Terminated)
    {
        try
        {
            // 서버에 연결 시도
            if (!FTcpClient->Connected())
            {
                // Synchronize를 사용하여 UI 쓰레드에서 상태 표시 (선택 사항)
                // Synchronize([this]() { StatusBar_Bottom->SimpleText = "Connecting..."; });
                FTcpClient->Connect();
            }

            // 연결된 상태에서 데이터 수신
            while (!Terminated && FTcpClient->Connected())
            {
                std::string sbs_message = AnsiString(FTcpClient->IOHandler->ReadLn()).c_str();
                if (!sbs_message.empty() && FDataParser)
                {
                    // 수신된 데이터를 파서로 전달
                    FDataParser->ParseMessage(sbs_message);
                }
            }
        }
        catch (const Exception& e)
        {
            // 연결 오류 발생 시
            if (FTcpClient->Connected())
            {
                FTcpClient->Disconnect();
            }
            // 오류 로그 출력 (선택 사항)
            // Synchronize([&]() { ShowMessage("Network Error: " + e.Message); });
        }

        // 재연결 시도 전 잠시 대기
        if (!Terminated)
        {
            Sleep(5000); // 5초 후 재시도
        }
    }

    // 쓰레드 종료 전 리소스 정리
    if (FTcpClient->Connected())
    {
        FTcpClient->Disconnect();
    }
    delete FTcpClient;
    FTcpClient = nullptr;
}


//--- NetworkService --------------------------------------------------------

NetworkService::NetworkService(DataParser* parser)
    : FDataParser(parser), FNetworkThread(nullptr)
{
}

NetworkService::~NetworkService()
{
    Stop();
}

void NetworkService::Start(const std::string& host, int port)
{
    // 기존 쓰레드가 있으면 종료 후 새로 시작
    if (FNetworkThread)
    {
        Stop();
    }
    // 새로운 네트워크 쓰레드를 생성하고 시작
    FNetworkThread = new TNetworkThread(host, port, FDataParser);
    FNetworkThread->Start();
}

void NetworkService::Stop()
{
    if (FNetworkThread)
    {
        // 쓰레드에 종료 신호를 보내고 끝날 때까지 대기
        FNetworkThread->Terminate();
        // FNetworkThread->WaitFor(); // FreeOnTerminate=true일 경우 WaitFor는 교착 상태를 유발할 수 있음
        FNetworkThread = nullptr;
    }
}
//---------------------------------------------------------------------------