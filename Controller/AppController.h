//---------------------------------------------------------------------------
#ifndef AppControllerH
#define AppControllerH
//---------------------------------------------------------------------------
#include "View/MainView.h"
#include "Model/AircraftModel.h"
#include "Controller/NetworkService.h"
#include "Controller/DataParser.h"

// Forward declaration
class TMainViewForm;
class AircraftModel;

class AppController
{
private:
    TMainViewForm* mainView;
    AircraftModel* aircraftModel;
    NetworkService* networkService;
    DataParser* dataParser;

public:
    AppController();
    ~AppController();

    void Run();

    // View로부터 UI 이벤트를 처리하기 위한 함수들
    void HandleMapPan(int deltaX, int deltaY);
    void HandleMapZoom(int delta);
    void HandleAircraftSelection(int screenX, int screenY);
};

#endif