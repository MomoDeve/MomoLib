#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace momo
{
	namespace duration
	{
		enum
		{
			nanoseconds,
			microseconds,
			seconds,
			minutes
		};
	}

	__time64_t get_ctime();

	std::string getDate();

	std::string getTime();

	std::string getDay();

	template<typename T>
	long long functionTest(T(*f)(), int time = duration::nanoseconds)
	{
		auto start = steady_clock::now();
		f();
		auto end = steady_clock::now();
		switch (time)
		{
		case duration::minutes:
			return (long long)std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
		case duration::seconds:
			return (long long)std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		case duration::microseconds:
			return (long long)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		case duration::nanoseconds:
			return (long long)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		default:
			return (long long)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		}
	}
}
