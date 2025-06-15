// Controller/AppController.h

#ifndef AppControllerH
#define AppControllerH

// 1. 가장 중요한 TADS_B_Aircraft의 정의를 최상단에 포함시킵니다.
#include "Model/Aircraft.h"

// 2. 그 다음 C++ 표준 라이브러리를 포함합니다.
#include <memory>
#include <vector>

// 3. 마지막으로 VCL 및 기타 라이브러리 헤더를 포함합니다.
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>

// 전방 선언: 포인터로만 사용하는 타입들
class TOpenGLPanel;
class AircraftModel;
class TMainView;
class NetworkService;
class DataParser;
class EarthView;
class TileManager;

class AppController {
private:
    AircraftModel* model;
    TMainView* view;

    std::unique_ptr<NetworkService> networkService;
    std::unique_ptr<DataParser> dataParser;

    EarthView* g_EarthView;
    TileManager* g_GETileManager;
    double mapCenterLat;
    double mapCenterLon;

    int g_MouseLeftDownX;
    int g_MouseLeftDownY;
    int g_MouseDownMask;

    void initializeMap();
    void onDataReceived(const std::vector<char>& data);

public:
    AppController(AircraftModel* model, TMainView* view);
    ~AppController();

    void onFormCreate();
    void onFormDestroy();
    void connect(const String& ip, int port);
    void disconnect();
    void onTimerTick();

    // 이 함수가 TADS_B_Aircraft의 전체 정의를 필요로 합니다.
    void getAllAircraft(std::vector<TADS_B_Aircraft>& list);

    void drawDisplay(TOpenGLPanel* panel);
    void onMouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    void onMouseMove(TShiftState Shift, int X, int Y);
    void onMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
};

#endif