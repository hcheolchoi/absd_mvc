// Model/AircraftModel.h

#ifndef AircraftModelH
#define AircraftModelH

#include <vector>
#include "Model/Aircraft.h"
#include "Model/IModelObserver.h"
#include "HashTable/Lib/ght_hash_table.h"

class AircraftModel {
private:
    ght_hash_table_t* HashTable;
    std::vector<IModelObserver*> observers;
    void notifyObservers();

public:
    AircraftModel();
    ~AircraftModel();

    void registerObserver(IModelObserver* observer);
    void addOrUpdateAircraft(const TADS_B_Aircraft& aircraft);
    bool updateAircraftStatus();

    // === 변경된 부분: 벡터를 참조로 받아 내용을 채워줌 ===
    void getAllAircraft(std::vector<TADS_B_Aircraft>& list);
};

#endif