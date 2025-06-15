#pragma hdrstop
#include "AppController.h"
#include "../View/MainView.h" // TForm1의 정의가 포함된 헤더
#include "../Model/AircraftModel.h"
#include "DataParser.h"
#include "NetworkService.h"
#include "../Utils/CPA.h"
#include <sstream>
#pragma package(smart_init)

// 생성자 파라미터 타입을 TForm1* 로 변경
AppController::AppController(TForm1* view) : view_(view) {
    model_ = std::make_unique<AircraftModel>();
    parser_ = std::make_unique<DataParser>();

    networkService_ = std::make_unique<NetworkService>(view,
        [this](const std::string& data) {
            this->processReceivedData(data);
        });

    model_->addObserver([this]() { this->onModelUpdate(); });
}

AppController::~AppController() {}

// 연결 시 MainView(TForm1)의 타이머를 제어
void AppController::toggleConnection() {
    if (networkService_->isConnected()) {
        networkService_->disconnect();
        view_->NetworkTimer->Enabled = false; // 타이머 중지
        view_->displayStatus("Disconnected.");
    } else {
        networkService_->connect("127.0.0.1", 30003); // 이 주소는 예시입니다.
        if (networkService_->isConnected()) {
            view_->NetworkTimer->Enabled = true; // 타이머 시작
            view_->displayStatus("Connected. Receiving data...");
        } else {
            view_->displayStatus("Connection failed.");
        }
    }
}

// 타이머에 의해 주기적으로 호출됩니다.
void AppController::checkForData()
{
    networkService_->checkForData();
}

// 이 아래 코드는 이전과 동일합니다.
void AppController::processReceivedData(const std::string& data) {
    std::stringstream ss(data);
    std::string line;

    // 여러 줄의 메시지가 한번에 올 수 있으므로 한 줄씩 처리
    while (std::getline(ss, line, '\n')) {
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }
        
        // 1. DataParser로 데이터 변환
        auto parsedData = parser_->parseSbsMessage(line);

        // 2. 파싱 성공 시, Model 업데이트 요청
        if (parsedData) {
            model_->updateAircraft(
                parsedData->icao24,
                parsedData->latitude,
                parsedData->longitude,
                parsedData->altitude,
                parsedData->speed,
                parsedData->track,
                parsedData->flightId
            );
        }
    }
}

void AppController::selectAircraft(const std::string& icao) {
    const Aircraft* ac = model_->getAircraft(icao);
    if (ac) {
        view_->displaySelectedAircraftDetails(ac);
    }
}

void AppController::onModelUpdate() {
    // 1. Model로부터 최신 항공기 목록을 가져옴
    auto aircrafts = model_->getAllAircraft();

    // 2. View에 데이터를 전달하여 화면 업데이트를 요청
    view_->updateDisplay(aircrafts);

    // 3. 상태 메시지 업데이트
    if (networkService_->isConnected() && !aircrafts.empty()) {
        view_->displayStatus("Receiving data... Aircraft count: " + std::to_string(aircrafts.size()));
    } else if (!networkService_->isConnected()) {
         if (view_->getNetworkTimer()) view_->getNetworkTimer()->Enabled = false;
         view_->displayStatus("Disconnected.");
    }
}
void AppController::checkForCPAAlerts() {}