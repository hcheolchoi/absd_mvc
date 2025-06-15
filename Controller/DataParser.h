// Controller/DataParser.h

#ifndef DataParserH
#define DataParserH

#include <string>
#include <vector>
#include "Aircraft.h" // 'TADS_B_Aircraft' or 'Aircraft' unknown type name 오류 해결

class DataParser {
public:
	// TADS_B_Aircraft -> Aircraft 로 클래스명이 변경되었을 수 있으므로 확인 필요
    static std::vector<Aircraft> parse(const std::string& jsonData);
};

#endif