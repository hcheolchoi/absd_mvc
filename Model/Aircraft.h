// Model/Aircraft.h

#ifndef Aircraft_H
#define Aircraft_H

#include <string>
#include <vector>
#include <ctime>

class Aircraft {
public:
    // [수정] ICAO 타입을 unsigned int로 변경
    Aircraft(unsigned int icao24);
    Aircraft(); // 기본 생성자 추가

    void updatePosition(double lat, double lon, double alt);
    void updateSpeedAndDirection(double speed, double track);
    void setFlightId(const std::string& flightId);
    void setOnGround(bool groundStatus);

    // [수정] ICAO 타입을 unsigned int로 변경
    unsigned int getIcao24() const;
    std::string getFlightId() const;
    double getLatitude() const;
    double getLongitude() const;
    double getAltitude() const;
    double getSpeed() const;
    double getTrack() const;
    bool isOnGround() const;
    time_t getLastUpdateTime() const;
    
    // MVC 패턴에서 선택 여부를 표시하기 위해 추가
    bool selected;


private:
    unsigned int icao24_; // [수정] std::string -> unsigned int
    std::string flightId_;
    double latitude_;
    double longitude_;
    double altitude_;
    double speed_;
    double track_;
    bool onGround_;
    time_t lastUpdateTime_;
};

#endif // Aircraft_H