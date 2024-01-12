#pragma once
#include <random>
#include <map>
#include <sstream>
#include <iostream>
#include <math.h>
#include <chrono>
#include <random>
#include <iterator>
#include "../Math/Vector3D.hpp"
#include "../Math/Vector4D.hpp"
#include "Memory.hpp"
#include "../Core/Offsets.hpp"

namespace Conversion {
    float ToGameUnits(float Meters) {
        return 39.37007874 * Meters;
    }

    float ToMeters(float GameUnits) {
        return GameUnits / 39.37007874;
    }
};

struct RGBA2 {
	int R;
	int G;
	int B;
	int A;
};
std::map<int, RGBA2> teamColors;
//Main Map Radar Color
typedef struct
{

	uint32_t R;
	uint32_t G;
	uint32_t B;
	uint32_t A;
}RGBA;

namespace Utils {
    inline float RandomFloat() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        return dis(gen);
    }

    inline float RandomRange(float min, float max) {
        if (min > max) {
            float Temp = min;
            min = max;
            max = Temp;
        }
        return RandomFloat() * (max - min) + min;
    }

    template <typename T>
    std::string convertNumberToString(const T a_value)
    {
        std::ostringstream out;
        out.precision(6);
        out << std::fixed << a_value;
        return out.str();
    }
    // trim from start (in place)
    static inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

    std::vector<std::string> static inline split(std::string s)
    {
        std::stringstream ss(s);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> tokens(begin, end);
        return tokens;
    }
    bool toBool(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::istringstream is(str);
        bool b;
        is >> std::boolalpha >> b;
        return b;
    }
    void clearScreen() {
        printf("\e[H\e[2J\e[3J");
    }
};