// Model/AircraftModel.cpp

#pragma hdrstop

// [수정] 누락된 헤더 파일들 추가
#include "AircraftModel.h"
#include "Aircraft.h"
#include <vector>

#pragma package(smart_init)

AircraftModel::AircraftModel() : selectedAircraftICAO(0) {}

AircraftModel::~AircraftModel() {}

void AircraftModel::addObserver(IModelObserver* observer) {
    observers.push_back(observer);
}

void AircraftModel::removeObserver(IModelObserver* observer) {
    for (auto it = observers.begin(); it != observers.end(); ) {
        if (*it == observer) {
            it = observers.erase(it);
        } else {
            ++it;
        }
    }
}

void AircraftModel::notifyObservers() {
    for (IModelObserver* observer : observers) {
        if (observer) {
            observer->onModelUpdate();
        }
    }
}

void AircraftModel::updateAircrafts(const std::vector<Aircraft>& newAircrafts) {
    bool modelChanged = false;
    if (newAircrafts.size() != aircrafts.size()) {
        modelChanged = true;
    }

    for (const auto& newAc : newAircrafts) {
        aircrafts[newAc.getIcao24()] = newAc;
    }

    if(modelChanged) {
        notifyObservers();
    }
}

int AircraftModel::getAircraftCount() const {
    return aircrafts.size();
}

std::vector<Aircraft> AircraftModel::getAllAircraft() const {
    std::vector<Aircraft> result;
    result.reserve(aircrafts.size());
    for (const auto& pair : aircrafts) {
        Aircraft ac = pair.second;
        ac.selected = (pair.first == selectedAircraftICAO);
        result.push_back(ac);
    }
    return result;
}

const Aircraft* AircraftModel::getSelectedAircraft() const {
    if (selectedAircraftICAO == 0) {
        return nullptr;
    }
    auto it = aircrafts.find(selectedAircraftICAO);
    if (it != aircrafts.end()) {
        return &(it->second);
    }
    return nullptr;
}

void AircraftModel::selectAircraft(unsigned int icao) {
    selectedAircraftICAO = icao;
    notifyObservers();
}