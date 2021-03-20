#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <cmath>
#include <string>
#include <vector>

namespace vb01{
	typedef char s8;
	typedef short s16;
	typedef int s32;
	typedef long long s64;

	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int  u32;
	typedef unsigned long long u64;

	static const float PI = 4 * atan(1);

	void readFile(std::string, std::vector<std::string>&, int = 0, int = -1);
	void getLineData(std::string, std::string[], int, int = 0);
	int getCharId(std::string, char, bool = false);

	inline s64 getTime(){return s64(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));}
}

#endif
