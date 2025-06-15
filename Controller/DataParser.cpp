// Controller/DataParser.cpp

#pragma hdrstop

#include "Controller/DataParser.h"
#include <System.SysUtils.hpp> // VCL 유틸리티 함수 사용
#include <System.StrUtils.hpp> // Split 함수 사용
#include <vector>
#include <string>

// SBS 메시지 필드 인덱스 정의
enum SBSType {
	SBS_MESSAGE_TYPE      = 0,
	SBS_TRANSMISSION_TYPE = 1,
	SBS_SESSION_ID        = 2,
	SBS_AIRCRAFT_ID       = 3,
	SBS_HEX_INDENT        = 4,
	SBS_FLIGHT_ID         = 5,
	SBS_DATE_GENERATED    = 6,
	SBS_TIME_GENERATED    = 7,
	SBS_DATE_LOGGED       = 8,
	SBS_TIME_LOGGED       = 9,
	SBS_CALLSIGN          = 10,
	SBS_ALTITUDE          = 11,
	SBS_GROUND_SPEED      = 12,
	SBS_TRACK_HEADING     = 13,
	SBS_LATITUDE          = 14,
	SBS_LONGITUDE         = 15,
	SBS_VERTICAL_RATE     = 16,
	// ... 기타 필드들
};


DataParser::DataParser() {}

std::optional<TADS_B_Aircraft> DataParser::parse(const std::vector<char>& data)
{
	// char 벡터를 AnsiString으로 변환
	AnsiString sbs_msg_str(data.data(), data.size());

	// Split 함수에 문자열 배열을 전달하기 위해 ARRAYOFCONST 사용법 수정
	TStringDynArray fields = sbs_msg_str.Split(ARRAYOFCONST((",")));

	if (fields.Length < 17 || fields[SBS_MESSAGE_TYPE] != "MSG") {
		return std::nullopt; // 유효한 SBS 메시지가 아님
	}

	TADS_B_Aircraft aircraft = {}; // 항공기 구조체 초기화

	try
	{
		// ICAO 주소 파싱
		AnsiString hexIdent = fields[SBS_HEX_INDENT];
		if (!hexIdent.IsEmpty()) {
			aircraft.ICAO = StrToInt64Def("0x" + hexIdent, 0