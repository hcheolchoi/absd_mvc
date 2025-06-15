// Controller/DataParser.cpp

#pragma hdrstop

#include "DataParser.h"
#include <vector>
#include <string>
#include <sstream> // std::stringstream 사용을 위해 추가
#include <stdexcept> // std::stod, std::stoul 예외 처리를 위해 추가

//---------------------------------------------------------------------------
#pragma package(smart_init)

// opensky-network.org에서 받은 JSON 데이터를 파싱하는 함수
std::vector<Aircraft> DataParser::parse(const std::string& jsonData) {
    std::vector<Aircraft> aircrafts;

    // 1. JSON 데이터에서 "states" 키워드를 찾아 배열의 시작 위치를 찾음
    std::string states_key = "\"states\":[";
    size_t start_pos = jsonData.find(states_key);

    if (start_pos == std::string::npos) {
        // "states" 배열이 없으면 빈 벡터를 반환
        return aircrafts;
    }
    // 실제 배열 데이터의 시작 위치로 포인터 이동
    start_pos += states_key.length();

    // 2. "states" 배열의 끝(']')을 찾음
    size_t end_pos = jsonData.find("]", start_pos);
    if (end_pos == std::string::npos) {
        // 배열이 제대로 닫히지 않았으면 빈 벡터 반환
        return aircrafts;
    }

    // 3. 전체 state 배열에 해당하는 문자열만 추출
    std::string states_array_content = jsonData.substr(start_pos, end_pos - start_pos);
    size_t current_pos = 0;

    // 4. 개별 항공기 정보 배열 "[...]"을 순회
    while ((start_pos = states_array_content.find('[', current_pos)) != std::string::npos) {
        end_pos = states_array_content.find(']', start_pos);
        if (end_pos == std::string::npos) {
            break; // 더 이상 파싱할 배열이 없으면 종료
        }

        // 개별 항공기 데이터 "[...]"에서 괄호를 제외한 순수 데이터만 추출
        std::string aircraft_data = states_array_content.substr(start_pos + 1, end_pos - start_pos - 1);
        current_pos = end_pos + 1;

        Aircraft ac;
        std::stringstream ss(aircraft_data);
        std::string field;
        int field_index = 0;

        // 5. 쉼표(,)를 기준으로 필드를 하나씩 분리하여 처리
        while (std::getline(ss, field, ',')) {
            // 필드 양쪽의 공백과 따옴표 제거
            // "   \"value\"   " -> "value"
            size_t first = field.find_first_not_of(" \t\n\r\"");
            if (std::string::npos == first) {
                 field = "";
            } else {
                size_t last = field.find_last_not_of(" \t\n\r\"");
                field = field.substr(first, (last - first + 1));
            }


            try {
                 // OpenSky-Network API의 state vector 순서에 따라 데이터 할당
                switch (field_index) {
                    case 0: // icao24 (16진수 문자열)
                        if (field != "null" && !field.empty())
                            ac.icao24 = std::stoul(field, nullptr, 16);
                        break;
                    case 1: // callsign
                        if (field != "null") {
                            // 호출 부호 뒤에 오는 공백 제거
                            size_t last_char = field.find_last_not_of(' ');
                            ac.callsign = (last_char == std::string::npos) ? "" : field.substr(0, last_char + 1);
                        }
                        break;
                    case 2: // origin_country
                        if (field != "null") ac.origin_country = field;
                        break;
                    case 5: // longitude
                        if (field != "null" && !field.empty()) ac.lon = std::stod(field);
                        break;
                    case 6: // latitude
                        if (field != "null" && !field.empty()) ac.lat = std::stod(field);
                        break;
                    case 7: // baro_altitude
                        if (field != "null" && !field.empty()) ac.altitude = std::stod(field);
                        break;
                }
            } catch (const std::invalid_argument& ia) {
                // 데이터 변환 실패 시 로그를 남기거나 무시할 수 있음
            } catch (const std::out_of_range& oor) {
                // 데이터 범위 초과 시 처리
            }

            field_index++;
        }

        // 유효한 ICAO 코드가 있는 항공기만 리스트에 추가
        if (ac.icao24 != 0) {
            aircrafts.push_back(ac);
        }
    }

    return aircrafts;
}