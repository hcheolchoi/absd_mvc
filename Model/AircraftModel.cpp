// Model/AircraftModel.cpp

#pragma hdrstop

#include "Model/AircraftModel.h"
#include "Utils/TimeFunctions.h"

// 생성자: 해시 테이블 초기화
AircraftModel::AircraftModel() {
    HashTable = ght_create(0);
}

// 소멸자: 프로그램 종료 시, 해시 테이블에 남아있는 모든 항공기 객체의 메모리를 해제
AircraftModel::~AircraftModel() {
    if (!HashTable) return;

    ght_iterator_t iterator;
    void *p_key;
    TADS_B_Aircraft *aircraft_ptr;

    // 해시 테이블을 순회하며 동적으로 할당된 모든 TADS_B_Aircraft 객체를 delete
    for (aircraft_ptr = (TADS_B_Aircraft *)ght_first(HashTable, &iterator, &p_key);
         aircraft_ptr != NULL;
         aircraft_ptr = (TADS_B_Aircraft *)ght_next(HashTable, &iterator, &p_key))
    {
        delete aircraft_ptr;
    }
    // 해시 테이블 자체의 메모리를 해제
    ght_finalize(HashTable);
}

// Observer(View) 등록
void AircraftModel::registerObserver(IModelObserver* observer)
{
    observers.push_back(observer);
}

// 모든 Observer에게 알림
void AircraftModel::notifyObservers()
{
    for (auto observer : observers) {
        if(observer) {
            observer->onModelUpdate();
        }
    }
}

// 항공기 추가 또는 업데이트 (메모리 누수 방지 로직 포함)
void AircraftModel::addOrUpdateAircraft(const TADS_B_Aircraft& aircraft)
{
    if (!HashTable) return;

    // 1. 새 항공기 데이터를 위해 동적 메모리 할당
    TADS_B_Aircraft* new_ac = new TADS_B_Aircraft(aircraft);
    new_ac->LastSeen = GetCurrentTimeInMsec(); // 마지막 수신 시간 갱신

    // 2. ght_insert를 호출하여 데이터를 삽입/갱신.
    //    키가 이미 존재하면 이전 데이터 포인터를 반환한다.
    TADS_B_Aircraft* old_ac = (TADS_B_Aircraft*)ght_insert(
        HashTable, new_ac, sizeof(uint32_t), (void*)&(new_ac->ICAO));

    // 3. 만약 기존 데이터가 덮어씌워졌다면, 이전 객체의 메모리를 해제하여 누수 방지
    if (old_ac) {
        delete old_ac;
    }

    // 데이터가 변경되었으므로 View에 통지
    notifyObservers();
}

// 오래된 항공기 제거
bool AircraftModel::updateAircraftStatus()
{
    if (!HashTable) return false;

    bool changed = false;
    __int64 currentTime = GetCurrentTimeInMsec();
    std::vector<void*> to_remove_keys;

    ght_iterator_t iterator;
    void *p_key;
    TADS_B_Aircraft *aircraft_ptr;

    // 1. 삭제할 항공기 키 목록 수집 (60초 이상 응답 없는 경우)
    for (aircraft_ptr = (TADS_B_Aircraft *)ght_first(HashTable, &iterator, &p_key);
         aircraft_ptr != NULL;
         aircraft_ptr = (TADS_B_Aircraft *)ght_next(HashTable, &iterator, &p_key))
    {
        if ((currentTime - aircraft_ptr->LastSeen) > 60000) {
            to_remove_keys.push_back(p_key);
        }
    }

    // 2. 수집된 키를 기반으로 실제 삭제 수행
    if (!to_remove_keys.empty()) {
        for (void* key : to_remove_keys) {
            // ght_remove는 제거된 데이터의 포인터를 반환
            TADS_B_Aircraft* removed_ac = (TADS_B_Aircraft*)ght_remove(HashTable, sizeof(uint32_t), key);
            if (removed_ac) {
                delete removed_ac; // 메모리 해제
            }
        }
        changed = true;
        notifyObservers(); // 데이터가 변경되었으므로 View에 통지
    }
    return changed;
}

void AircraftModel::getAllAircraft(std::vector<TADS_B_Aircraft>& list)
{
    list.clear(); // 리스트를 비우고 시작
    if (!HashTable) {
        return;
    }

    ght_iterator_t iterator;
    void *p_key;
    TADS_B_Aircraft *aircraft_ptr;

    for (aircraft_ptr = (TADS_B_Aircraft *)ght_first(HashTable, &iterator, &p_key);
         aircraft_ptr != NULL;
         aircraft_ptr = (TADS_B_Aircraft *)ght_next(HashTable, &iterator, &p_key))
    {
         list.push_back(*aircraft_ptr);
    }
}