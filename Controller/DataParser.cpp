// Controller/DataParser.cpp

#pragma hdrstop

#include "DataParser.h"
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#pragma package(smart_init)

std::vector<Aircraft> DataParser::parse(const std::string& jsonData) {
    std::vector<Aircraft> aircrafts;

    std::string states_key = "\"states\":[";
    size_t start_pos = jsonData.find(states_key);
    if (start_pos == std::string::npos) return aircrafts;

    start_pos += states_key.length();
    size_t end_pos = jsonData.find("]", start_pos);
    if (end_pos == std::string::npos) return aircrafts;

    std::string states_array_content = jsonData.substr(start_pos, end_pos - start_pos);
    size_t current_pos = 0;

    while ((start_pos = states_array_content.find('[', current_pos)) != std::string::npos) {
        end_pos = states_array_content.find(']', start_pos);
        if (end_pos == std::string::npos) break;

        std::string aircraft_data = states_array_content.substr(start_pos + 1, end_pos - start_pos - 1);
        current_pos = end_pos + 1;

        std::stringstream ss(aircraft_data);
        std::string field;
        int field_index = 0;

        // 임시 변수들
        unsigned int icao = 0;
        std::string callsign = "";
        double lon = 0.0, lat = 0.0, alt = 0.0, speed = 0.0, track = 0.0;
        bool on_ground = false;

        while (std::getline(ss, field, ',')) {
            size_t first = field.find_first_not_of(" \t\n\r\"");
            field = (std::string::npos == first) ? "" : field.substr(first, (field.find_last_not_of(" \t\n\r\"") - first + 1));

            try {
                if (field == "null" || field.empty()) {
                    field_index++;
                    continue;
                }
                switch (field_index) {
                    case 0: icao = std::stoul(field, nullptr, 16); break;
                    case 1: callsign = field.substr(0, field.find_last_not_of(' ') + 1); break;
                    case 5: lon = std::stod(field); break;
                    case 6: lat = std::stod(field); break;
                    case 7: alt = std::stod(field); break;
                    case 8: on_ground = (field == "true"); break;
                    case 9: speed = std::stod(field); break;
                    case 10: track = std::stod(field); break;
                }
            } catch (const std::exception& e) {
                // 변환 오류 무시
            }
            field_index++;
        }

        if (icao != 0) {
            // [수정] Aircraft 객체를 생성하고 메소드를 통해 데이터 업데이트
            Aircraft ac(icao);
            ac.setFlightId(callsign);
            ac.updatePosition(lat, lon, alt);
            ac.updateSpeedAndDirection(speed, track);
            ac.setOnGround(on_ground);
            aircrafts.push_back(ac);
        }
    }
    return aircrafts;
}