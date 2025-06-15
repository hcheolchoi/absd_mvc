#ifndef AircraftModel_H
#define AircraftModel_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "Aircraft.h"

class AircraftModel {
public:
    using Observer = std::function<void()>;

    void updateAircraft(const std::string& icao24, double lat, double lon, double alt, double speed, double track, const std::string& flightId);
    Aircraft* getAircraft(const std::string& icao24);
    std::vector<const Aircraft*> getAllAircraft() const;
    void addObserver(const Observer& observer);

private:
    void notifyObservers();

    std::map<std::string, Aircraft> aircrafts_;
    std::vector<Observer> observers_;
};

#endif // AircraftModel_H