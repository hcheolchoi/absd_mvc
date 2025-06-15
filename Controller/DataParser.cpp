#pragma hdrstop
#include "DataParser.h"
#include <sstream>
#include <algorithm>
#pragma package(smart_init)

DataParser::DataParser() {}

std::vector<std::string> DataParser::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::optional<ParsedAircraftData> DataParser::parseSbsMessage(const std::string& sbsMessage)
{
    if (sbsMessage.empty()) {
        return std::nullopt;
    }
    std::vector<std::string> fields = split(sbsMessage, ',');
    if (fields.size() < 22 || fields[0] != "MSG") {
        return std::nullopt;
    }
    if (fields[4].empty() || fields[14].empty() || fields[15].empty() || fields[11].empty()) {
        return std::nullopt;
    }

    ParsedAircraftData data;
    try {
        data.icao24 = fields[4];
        if (!fields[10].empty()) {
            data.flightId = fields[10];
            data.flightId.erase(std::remove(data.flightId.begin(), data.flightId.end(), ' '), data.flightId.end());
        }
        data.latitude = std::stod(fields[14]);
        data.longitude = std::stod(fields[15]);
        data.altitude = std::stod(fields[11]);
        data.speed = std::stod(fields[12]);
        data.track = std::stod(fields[13]);
    } catch (const std::exception& e) {
        return std::nullopt;
    }
    return data;
}