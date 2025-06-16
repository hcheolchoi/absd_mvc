//---------------------------------------------------------------------------
#pragma hdrstop

#include "Aircraft.h"
#include "TimeFunctions.h"
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

// 함수 선언
static int cprModFunction(int a, int b);
static int cprNLFunction(double lat);
static double cprDlonFunction(double lat, int isodd);
static void decodeCPR(TADS_B_Aircraft *a);

//---------------------------------------------------------------------------
// 항상 양수 MOD 연산 (CPR 디코딩에 사용)
static int cprModFunction(int a, int b) {
    int res = a % b;
    if (res < 0) res += b;
    return res;
}

//---------------------------------------------------------------------------
// 미리 계산된 테이블을 사용하는 NL 함수
static int cprNLFunction(double lat) {
    if (lat < 0) lat = -lat;
    if (lat < 10.47047130) return 59;
    if (lat < 14.82817437) return 58;
    if (lat < 18.18626357) return 57;
    if (lat < 21.02939493) return 56;
    if (lat < 23.54504487) return 55;
    if (lat < 25.82924707) return 54;
    if (lat < 27.93898710) return 53;
    if (lat < 29.91135686) return 52;
    if (lat < 31.77209708) return 51;
    if (lat < 33.53993436) return 50;
    if (lat < 35.22899437) return 49;
    if (lat < 36.85025108) return 48;
    if (lat < 38.41241892) return 47;
    if (lat < 39.92256684) return 46;
    if (lat < 41.38651832) return 45;
    if (lat < 42.80914012) return 44;
    if (lat < 44.19454951) return 43;
    if (lat < 45.54626723) return 42;
    if (lat < 46.86733222) return 41;
    if (lat < 48.16039128) return 40;
    if (lat < 49.42776439) return 39;
    if (lat < 50.67150166) return 38;
    if (lat < 51.89325451) return 37;
    if (lat < 53.09440656) return 36;
    if (lat < 54.27611746) return 35;
    if (lat < 55.43926421) return 34;
    if (lat < 56.58468481) return 33;
    if (lat < 57.71311135) return 32;
    if (lat < 58.82525736) return 31;
    if (lat < 59.92181155) return 30;
    if (lat < 61.00346359) return 29;
    if (lat < 62.07089922) return 28;
    if (lat < 63.12480161) return 27;
    if (lat < 64.16584218) return 26;
    if (lat < 65.19468497) return 25;
    if (lat < 66.21196169) return 24;
    if (lat < 67.21824574) return 23;
    if (lat < 68.21404343) return 22;
    if (lat < 69.19983451) return 21;
    if (lat < 70.17601673) return 20;
    if (lat < 71.14296790) return 19;
    if (lat < 72.10103578) return 18;
    if (lat < 73.05051995) return 17;
    if (lat < 73.99168568) return 16;
    if (lat < 74.92476577) return 15;
    if (lat < 75.84992237) return 14;
    if (lat < 76.76725763) return 13;
    if (lat < 77.67688536) return 12;
    if (lat < 78.57894227) return 11;
    if (lat < 79.47351113) return 10;
    if (lat < 80.36061908) return 9;
    if (lat < 81.24029281) return 8;
    if (lat < 82.11252771) return 7;
    if (lat < 82.97729982) return 6;
    if (lat < 83.83459382) return 5;
    if (lat < 84.68434354) return 4;
    if (lat < 85.52643576) return 3;
    if (lat < 86.36069925) return 2;
    return 1;
}

//---------------------------------------------------------------------------
static double cprDlonFunction(double lat, int isodd) {
    return 360.0 / cprNLFunction(lat);
}

//---------------------------------------------------------------------------
// CPR 위치 디코딩
static void decodeCPR(TADS_B_Aircraft *a) {
    const double AirDlat0 = 360.0 / 60.0;
    const double AirDlat1 = 360.0 / 59.0;
    int j;
    double lat0, lat1;
    double lon0, lon1;

    // 위도 디코딩
    j = floor(((double)a->even_cprlat / 131072.0) * 59.0 + 0.5);
    lat0 = AirDlat0 * (cprModFunction(j, 60) + (double)a->even_cprlat / 131072.0);

    j = floor(((double)a->odd_cprlat / 131072.0) * 60.0 + 0.5);
    lat1 = AirDlat1 * (cprModFunction(j, 59) + (double)a->odd_cprlat / 131072.0);

    if (lat0 >= 270) lat0 -= 360;
    if (lat1 >= 270) lat1 -= 360;

    // 두 위도가 같은 구역에 있는지 확인
    if (cprNLFunction(lat0) != cprNLFunction(lat1)) return;

    // 경도 디코딩
    if (a->even_cprtime > a->odd_cprtime) { // even frame이 더 최신
        double dlon = cprDlonFunction(lat0, 0);
        int m = floor(((double)a->even_cprlon / 131072.0 - (double)a->odd_cprlon / 131072.0) + 0.5);
        lon0 = dlon * (cprModFunction(m, cprNLFunction(lat0)) + (double)a->even_cprlon / 131072.0);
        a->Latitude = lat0;
        a->Longitude = lon0;
    } else { // odd frame이 더 최신
        double dlon = cprDlonFunction(lat1, 1);
        int m = floor(((double)a->even_cprlon / 131072.0 - (double)a->odd_cprlon / 131072.0) + 0.5);
        lon1 = dlon * (cprModFunction(m, cprNLFunction(lat1)-1) + (double)a->odd_cprlon / 131072.0);
        a->Latitude = lat1;
        a->Longitude = lon1;
    }
    a->HaveLatLon = true;
}

//---------------------------------------------------------------------------
// Raw 메시지를 Aircraft 구조체로 변환
void RawToAircraft(modeS_message *mm, TADS_B_Aircraft *ADS_B_Aircraft) {
    __int64 CurrentTime = GetCurrentTimeInMsec();
    ADS_B_Aircraft->ICAO = mm->addr;
    ADS_B_Aircraft->LastSeen = CurrentTime;

    if (mm->msg_type == 5) { // 고도 응답
        if (mm->altitude > 0) {
            ADS_B_Aircraft->Altitude = mm->altitude;
            ADS_B_Aircraft->HaveAltitude = true;
        }
    } else if (mm->msg_type == 17) { // ADS-B 메시지
        if (mm->ME_type >= 1 && mm->ME_type <= 4) { // 항공기 식별
            memcpy(ADS_B_Aircraft->FlightNum, mm->flight, sizeof(ADS_B_Aircraft->FlightNum));
            ADS_B_Aircraft->HaveFlightNum = true;
        } else if (mm->ME_type >= 9 && mm->ME_type <= 18) { // 항공기 위치
            ADS_B_Aircraft->Altitude = mm->altitude;
            ADS_B_Aircraft->HaveAltitude = true;

            if (mm->odd_flag) {
                ADS_B_Aircraft->odd_cprlat = mm->raw_latitude;
                ADS_B_Aircraft->odd_cprlon = mm->raw_longitude;
                ADS_B_Aircraft->odd_cprtime = CurrentTime;
            } else {
                ADS_B_Aircraft->even_cprlat = mm->raw_latitude;
                ADS_B_Aircraft->even_cprlon = mm->raw_longitude;
                ADS_B_Aircraft->even_cprtime = CurrentTime;
            }

            // 두 CPR 데이터의 시간 차이가 10초 이내면 위치 계산
            if (abs(ADS_B_Aircraft->even_cprtime - ADS_B_Aircraft->odd_cprtime) <= 10000) {
                decodeCPR(ADS_B_Aircraft);
            }
        }
    }
}