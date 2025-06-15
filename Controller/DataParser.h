// Controller/DataParser.h

#ifndef DataParserH
#define DataParserH

#include <vector>
#include <optional>
#include "Model/Aircraft.h"

class DataParser {
public:
    DataParser();
    std::optional<TADS_B_Aircraft> parse(const std::vector<char>& data);
};

#endif