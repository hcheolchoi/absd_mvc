#ifndef AppController_H
#define AppController_H

#include <memory>
#include <string>
#include <vector>

// 전방 선언
class TForm1; // TMainForm 대신 TForm1으로 선언
class AircraftModel;
class DataParser;
class NetworkService;
class Aircraft;

class AppController {
public:
    AppController(TForm1* view); // 파라미터 타입을 TForm1* 로 변경
    ~AppController();

    void toggleConnection();
    void selectAircraft(const std::string& icao);
    void checkForData();

private:
    void onModelUpdate();
    void processReceivedData(const std::string& data);
    void checkForCPAAlerts();

    TForm1* view_; // 멤버 변수 타입도 TForm1* 로 변경
    std::unique_ptr<AircraftModel> model_;
    std::unique_ptr<DataParser> parser_;
    std::unique_ptr<NetworkService> networkService_;
};

#endif // AppController_H