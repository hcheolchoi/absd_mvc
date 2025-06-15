#pragma hdrstop
#include "AircraftModel.h"
#pragma package(smart_init)

void AircraftModel::updateAircraft(const std::string& icao24, double lat, double lon, double alt, double speed, double track, const std::string& flightId) {
    auto it = aircrafts_.find(icao24);
    if (it == aircrafts_.end()) {
        it = aircrafts_.emplace(icao24, Aircraft(icao24)).first;
    }
    it->second.updatePosition(lat, lon, alt);
    it->second.updateSpeedAndDirection(speed, track);
    if (!flightId.empty()) {
        it->second.setFlightId(flightId);
    }
    notifyObservers();
}

Aircraft* AircraftModel::getAircraft(const std::string& icao24) {
    auto it = aircrafts_.find(icao24);
    return (it != aircrafts_.end()) ? &(it->second) : nullptr;
}

std::vector<const Aircraft*> AircraftModel::getAllAircraft() const {
    std::vector<const Aircraft*> result;
    result.reserve(aircrafts_.size());
    for (const auto& pair : aircrafts_) {
        result.push_back(&pair.second);
    }
    return result;
}

void AircraftModel::addObserver(const Observer& observer) {
    observers_.push_back(observer);
}

void AircraftModel::notifyObservers() {
    for (const auto& observer : observers_) {
        if (observer) {
            observer();
        }
    }
}