// Model/Aircraft.cpp

#pragma hdrstop
#include "Aircraft.h"

#pragma package(smart_init)

Aircraft::Aircraft()
    : icao24_(0), latitude_(0.0), longitude_(0.0), altitude_(0.0),
      speed_(0.0), track_(0.0), onGround_(false), selected(false)
{
    lastUpdateTime_ = time(0);
}


Aircraft::Aircraft(unsigned int icao24)
    : icao24_(icao24), latitude_(0.0), longitude_(0.0), altitude_(0.0),
      speed_(0.0), track_(0.0), onGround_(false), selected(false)
{
    lastUpdateTime_ = time(0);
}

void Aircraft::updatePosition(double lat, double lon, double alt) {
    latitude_ = lat;
    longitude_ = lon;
    altitude_ = alt;
    lastUpdateTime_ = time(0);
}

void Aircraft::updateSpeedAndDirection(double speed, double track) {
    speed_ = speed;
    track_ = track;
    lastUpdateTime_ = time(0);
}

void Aircraft::setFlightId(const std::string& flightId) {
    flightId_ = flightId;
}

void Aircraft::setOnGround(bool groundStatus) {
    onGround_ = groundStatus;
}

unsigned int Aircraft::getIcao24() const { return icao24_; }
std::string Aircraft::getFlightId() const { return flightId_; }
double Aircraft::getLatitude() const { return latitude_; }
double Aircraft::getLongitude() const { return longitude_; }
double Aircraft::getAltitude() const { return altitude_; }
double Aircraft::getSpeed() const { return speed_; }
double Aircraft::getTrack() const { return track_; }
bool Aircraft::isOnGround() const { return onGround_; }
time_t Aircraft::getLastUpdateTime() const { return lastUpdateTime_; }