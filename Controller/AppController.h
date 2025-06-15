// Controller/AppController.h

#ifndef AppController_h
#define AppController_h

#include "AircraftModel.h"
#include "NetworkService.h"
#include "DataParser.h"
#include <vector>
#include <string>

// View(TForm1)에 데이터 렌더링을 위해 전달될 간단한 항공기 정보 구조체 (DTO)
struct AircraftInfo
{
	double lat;
	double lon;
	double altitude;
	unsigned int icao;
	std::string callsign;
	bool selected;
};


class TForm1; // Forward declaration (TMainForm -> TForm1)

class AppController
{
private:
	AircraftModel* theModel;
	NetworkService* networkService;
	TForm1* theView; // Controller가 View를 알고 있도록 참조를 추가 (TMainForm* -> TForm1*)

	void loadAircraftData();

public:
	AppController(TForm1* view); // 생성자 인자 변경 (TMainForm* -> TForm1*)
	~AppController();

	void start();
	void onTimer(); // View의 타이머 이벤트가 호출할 함수

	// --- View가 호출할 함수들 ---
	int getAircraftCount();
	AircraftInfo getAircraftInfo(int index);
	void selectAircraft(unsigned int icao);
	std::string getSelectedAircraftInfoText();


};

#endif