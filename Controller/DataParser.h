#ifndef DataParser_H
#define DataParser_H

#include <string>
#include <vector>
#include <optional>

struct ParsedAircraftData {
    std::string icao24;
    std::string flightId;
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    double speed = 0.0;
    double track = 0.0;
};

class DataParser {
public:
    DataParser();
    std::optional<ParsedAircraftData> parseSbsMessage(const std::string& sbsMessage);
private:
    std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif // DataParser_H