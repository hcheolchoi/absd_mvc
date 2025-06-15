// Controller/AppController.cpp

#pragma hdrstop

#include "AppController.h"
#include "MainView.h"
#include "DataParser.h"
#include "Model/AircraftModel.h"
#include "NetworkService.h"
#include <sstream>
#include <iomanip>
#include <vector>

#pragma package(smart_init)

AppController::AppController(TForm1* view)
: theView(view), networkService(nullptr), theModel(nullptr)
{
	theModel = new AircraftModel();
	networkService = new NetworkService();
	theModel->addObserver(theView);

	networkService->setDataCallback([this](const std::vector<char>& data) {
		this->onDataReceived(data);
	});
}

AppController::~AppController()
{
	if(networkService) networkService->disconnect();
	if(theModel) theModel->removeObserver(theView);
	delete theModel;
	delete networkService;
}

void AppController::start()
{
	// 초기화 로직
}

void AppController::connectToServer(const std::string& ip, int port)
{
	if(networkService) {
		networkService->connect(ip.c_str(), port);
	}
}

void AppController::onDataReceived(const std::vector<char>& data)
{
    std::string jsonData(data.begin(), data.end());
	if (!jsonData.empty())
	{
		std::vector<Aircraft> aircrafts = DataParser::parse(jsonData);
		if(theModel) theModel->updateAircrafts(aircrafts);
	}
}

std::vector<AircraftInfo> AppController::getAllAircraftInfo()
{
	std::vector<AircraftInfo> result;
	if (theModel)
	{
		std::vector<Aircraft> all_ac = theModel->getAllAircraft();
		result.reserve(all_ac.size());

		for(const auto& ac : all_ac)
		{
			AircraftInfo info;
			info.lat = ac.getLatitude();
			info.lon = ac.getLongitude();
			info.altitude = ac.getAltitude();
			info.icao = ac.getIcao24();
			info.callsign = ac.getFlightId();
			info.selected = ac.selected;
			result.push_back(info);
		}
	}
	return result;
}

void AppController::selectAircraft(unsigned int icao)
{
	if (theModel)
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
		ss << "Callsign: " << ac->getFlightId() << "\n"
		   << "ICAO24: " << std::hex << ac->getIcao24() << std::dec << "\n"
		   << "Altitude: " << static_cast<int>(ac->getAltitude()) << " m\n"
           << "Speed: " << ac->getSpeed() * 3.6 << " km/h\n"
		   << "Latitude: " << std::fixed << std::setprecision(4) << ac->getLatitude() << "\n"
		   << "Longitude: " << std::fixed << std::setprecision(4) << ac->getLongitude();
		return ss.str();
	}
	return "No aircraft selected.";
}