// Controller/AppController.h

#ifndef AppController_h
#define AppController_h

#include <vector>
#include <string>

// View로 전달될 데이터 전송 객체 (DTO)
struct AircraftInfo
{
	double lat;
	double lon;
	double altitude;
	unsigned int icao;
	std::string callsign;
	bool selected;
};

// 전방 선언
class TForm1;
class AircraftModel;
class NetworkService;

class AppController
{
private:
	AircraftModel* theModel;
	NetworkService* networkService;
	TForm1* theView;

	void onDataReceived(const std::vector<char>& data);

public:
	AppController(TForm1* view);
	~AppController();

	void start();
	void connectToServer(const std::string& ip, int port);

	std::vector<AircraftInfo> getAllAircraftInfo();
	void selectAircraft(unsigned int icao);
	std::string getSelectedAircraftInfoText();
};

#endif