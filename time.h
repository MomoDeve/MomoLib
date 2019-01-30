#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace momo
{
	__time64_t get_ctime();

	std::string getDate();

	std::string getTime();

	std::string getDay();

	template<typename T>
	long long functionTest(T(*)(), int);
}