#ifndef Aircraft_H
#define Aircraft_H

#include <string>
#include <vector>
#include <ctime>

class Aircraft {
public:
    Aircraft(const std::string& icao24);

    void updatePosition(double lat, double lon, double alt);
    void updateSpeedAndDirection(double speed, double track);
    void setFlightId(const std::string& flightId);
    void setOnGround(bool groundStatus);

    std::string getIcao24() const;
    std::string getFlightId() const;
    double getLatitude() const;
    double getLongitude() const;
    double getAltitude() const;
    double getSpeed() const;
    double getTrack() const;
    bool isOnGround() const;
    time_t getLastUpdateTime() const;

private:
    std::string icao24_;
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