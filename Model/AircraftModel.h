// Model/AircraftModel.h

#ifndef AircraftModelH
#define AircraftModelH

#include <vector>
#include <string>
#include <unordered_map>
#include "IModelObserver.h"
#include "Aircraft.h"

class AircraftModel {
private:
    std::unordered_map<unsigned int, Aircraft> aircrafts;
    std::vector<IModelObserver*> observers;
    unsigned int selectedAircraftICAO;

    void notifyObservers();

public:
    AircraftModel();
    ~AircraftModel();

    void addObserver(IModelObserver* observer);
    void removeObserver(IModelObserver* observer);
    void updateAircrafts(const std::vector<Aircraft>& newAircrafts);
    int getAircraftCount() const;
    std::vector<Aircraft> getAllAircraft() const;
    const Aircraft* getSelectedAircraft() const;
    void selectAircraft(unsigned int icao);
};

#endif