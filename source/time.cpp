#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "time.h"

#pragma warning(disable : 4996)

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

	__time64_t get_ctime()
	{
		auto t = std::chrono::system_clock::now();
		return std::chrono::system_clock::to_time_t(t);
	}

	std::string getDate()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
		return oss.str();
	}

	std::string getTime()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%X");
		return oss.str();
	}

	std::string getDay()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%Y-%m-%d");
		return oss.str();
	}

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
