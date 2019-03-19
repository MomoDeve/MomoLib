#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace momo
{
	typedef enum
	{
		nano,
		micro,
		sec,
		min
	} duration;

	__time64_t get_ctime();

	std::string getDate();

	std::string getTime();

	std::string getDay();

	template<typename function>
	int functionTest(function* f, duration time = duration::nano)
	{
		using namespace std::chrono;

		auto start = steady_clock::now();
		f();
		auto end = steady_clock::now();
		switch (time)
		{
		case duration::min:
			return duration_cast<minutes>(end - start).count();
		case duration::sec:
			return duration_cast<seconds>(end - start).count();
		case duration::micro:
			return duration_cast<microseconds>(end - start).count();
		case duration::nano:
			return duration_cast<nanoseconds>(end - start).count();
		default:
			return duration_cast<nanoseconds>(end - start).count();
		}
	}
}
