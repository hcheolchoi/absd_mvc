// Model/AircraftModel.h

#ifndef AircraftModelH
#define AircraftModelH

#include <vector>
#include "IModelObserver.h"
#include "Aircraft.h" // 'TADS_B_Aircraft' or 'Aircraft' unknown type name 오류 해결

class AircraftModel {
private:
	// TADS_B_Aircraft -> Aircraft 로 클래스명이 변경되었을 수 있으므로 확인 필요
	std::vector<Aircraft> aircrafts;
	std::vector<IModelObserver*> observers;
	Aircraft* selectedAircraft;

	void notifyObservers();

public:
	AircraftModel();
	void addObserver(IModelObserver* observer);
	void removeObserver(IModelObserver* observer);

	void updateAircrafts(const std::vector<Aircraft>& newAircrafts);
	int getAircraftCount() const;
	const Aircraft* getAircraft(int index) const;

	void selectAircraft(unsigned int icao);
	const Aircraft* getSelectedAircraft() const;
};

#endif