// Controller/AppController.cpp

#pragma hdrstop

#include "AppController.h"
#include "MainView.h" // TForm1의 전체 정의를 위해 필요
#include "DataParser.h"
#include <sstream>
#include <iomanip>

//---------------------------------------------------------------------------
#pragma package(smart_init)

AppController::AppController(TForm1* view) // 생성자 인자 변경 (TMainForm* -> TForm1*)
: theView(view), networkService(nullptr)
{
	theModel = new AircraftModel();
	networkService = new NetworkService();

	// Model에 View(Observer)를 등록하는 작업을 Controller가 수행
	theModel->addObserver(theView);
}

AppController::~AppController()
{
	theModel->removeObserver(theView);
	delete theModel;
	delete networkService;
}

void AppController::start()
{
	loadAircraftData();
}

void AppController::onTimer()
{
    // 주기적으로 데이터를 로드
	loadAircraftData();
}


void AppController::loadAircraftData()
{
	std::string jsonData = networkService->fetchData();
	if (!jsonData.empty())
	{
		std::vector<Aircraft> aircrafts = DataParser::parse(jsonData);
		theModel->updateAircrafts(aircrafts);
	}
}

// --- View가 호출할 함수들의 구현부 ---

int AppController::getAircraftCount()
{
	if(theModel)
	{
		return theModel->getAircraftCount();
	}
	return 0;
}

AircraftInfo AppController::getAircraftInfo(int index)
{
	AircraftInfo info = {}; // 0으로 초기화
	if (theModel)
	{
		const Aircraft* ac = theModel->getAircraft(index);
		if (ac)
		{
			info.lat = ac->lat;
			info.lon = ac->lon;
			info.altitude = ac->altitude;
			info.icao = ac->icao24;
			info.callsign = ac->callsign;
			info.selected = ac->selected;
		}
	}
	return info;
}

void AppController::selectAircraft(unsigned int icao)
{
	if(theModel)
	{
		theModel->selectAircraft(icao);
	}
}

std::string AppController::getSelectedAircraftInfoText()
{
	if (!theModel) return "";

	const Aircraft* ac = theModel->getSelectedAircraft();
	if (ac)
	{
		std::stringstream ss;
		ss << "Callsign: " << ac->callsign << "\n"
		   << "ICAO24: " << std::hex << ac->icao24 << std::dec << "\n"
		   << "Origin: " << ac->origin_country << "\n"
		   << "Altitude: " << static_cast<int>(ac->altitude) << " m\n"
		   << "Latitude: " << std::fixed << std::setprecision(4) << ac->lat << "\n"
		   << "Longitude: " << std::fixed << std::setprecision(4) << ac->lon;
		return ss.str();
	}
	return "No aircraft selected.";
}