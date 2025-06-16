//---------------------------------------------------------------------------
#ifndef AircraftModelH
#define AircraftModelH
//---------------------------------------------------------------------------
#include <vector>
#include "Model/IModelObserver.h"
#include "HashTable/Lib/ght_hash_table.h"
#include "Model/Aircraft.h" // <<-- 이 줄을 추가해야 합니다!

class AircraftModel
{
private:
    std::vector<IModelObserver*> observers;
    ght_hash_table_t* aircraftHashTable;
    uint32_t selectedAircraftICAO;

    void NotifyObservers();

public:
    AircraftModel();
    ~AircraftModel();

    void AddObserver(IModelObserver* observer);

    void AddOrUpdateAircraft(TADS_B_Aircraft* aircraft);
    void RemoveStaleAircraft();

    TADS_B_Aircraft* GetAircraft(uint32_t icao);
    std::vector<TADS_B_Aircraft*> GetAllAircraft();

    void SetSelectedAircraft(uint32_t icao);
    uint32_t GetSelectedAircraftICAO();
    uint32_t FindAircraftAtPos(double lat, double lon);
};

#endif