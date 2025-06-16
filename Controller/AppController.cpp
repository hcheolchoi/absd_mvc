//---------------------------------------------------------------------------
#pragma hdrstop

#include "AppController.h"
#include <vcl.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

AppController::AppController() {
    // 1. Model 생성
    aircraftModel = new AircraftModel();

    // 2. View 생성
    Application->CreateForm(__classid(TMainViewForm), &mainView);

    // 3. 다른 Controller 컴포넌트 생성
    dataParser = new DataParser(aircraftModel);
    networkService = new NetworkService(dataParser); // NetworkService는 파싱을 DataParser에 위임

    // 4. View와 Model 연결
    // Model의 데이터가 변경되면 View가 알림을 받을 수 있도록 Observer로 등록
    aircraftModel->AddObserver(mainView);
    // View가 Model의 데이터를 직접 읽을 수 있도록 포인터 전달
    mainView->SetModel(aircraftModel);

    // 5. View와 Controller 연결
    // View에서 발생하는 UI 이벤트를 Controller가 처리하도록 설정
    mainView->SetController(this);
}

AppController::~AppController() {
    // AppController가 모든 주요 객체의 소유자이므로 여기서 해제
    delete networkService;
    delete dataParser;
    delete aircraftModel;
    // VCL Form은 Application 객체가 관리하므로 직접 delete하지 않음
}

void AppController::Run() {
    // 네트워크 서비스 시작 (백그라운드 데이터 수신)
    networkService->Start("127.0.0.1", 30005); // 예시 주소/포트
    // VCL 애플리케이션 메인 루프 시작
    Application->Run();
}

void AppController::HandleMapPan(int deltaX, int deltaY) {
    if (mainView) {
        mainView->PanMap(deltaX, deltaY);
    }
}

void AppController::HandleMapZoom(int delta) {
    if (mainView) {
        mainView->ZoomMap(delta);
    }
}

void AppController::HandleAircraftSelection(int screenX, int screenY) {
    if (aircraftModel && mainView) {
        // View에 스크린 좌표를 지리적 좌표로 변환 요청
        TPointF geoCoords = mainView->ScreenToGeo(screenX, screenY);
        uint32_t selectedICAO = aircraftModel->FindAircraftAtPos(geoCoords.Y, geoCoords.X);
        aircraftModel->SetSelectedAircraft(selectedICAO);
    }
}