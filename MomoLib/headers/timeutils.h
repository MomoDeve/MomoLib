#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#ifdef _MSC_VER
// MSVC does not like localtime
#pragma warning(disable : 4996)
#endif

/*
here is some time utils like get current date, time, day and etc.
also here is test_function() method to easily check how long any function runs
*/
namespace momo
{
	enum class duration
	{
		test_function,
		micro,
		sec,
		min
	};

	inline auto get_ctime()
	{
		auto t = std::chrono::system_clock::now();
		return std::chrono::system_clock::to_time_t(t);
	}

	inline std::string get_date()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
		return oss.str();
	}

	inline std::string get_time()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%X");
		return oss.str();
	}

	inline std::string get_day()
	{
		auto time = get_ctime();
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%Y-%m-%d");
		return oss.str();
	}

	template<typename function, typename... Args>
	int test_function(function& f, duration time = duration::test_function, Args&&... args)
	{
		using namespace std::chrono;

		auto start = steady_clock::now();
		f(std::forward<Args>(args)...);
		auto end = steady_clock::now();
		switch (time)
		{
		case duration::min:
			return duration_cast<minutes>(end - start).count();
		case duration::sec:
			return duration_cast<seconds>(end - start).count();
		case duration::micro:
			return duration_cast<microseconds>(end - start).count();
		case duration::test_function:
			return duration_cast<nanoseconds>(end - start).count();
		default:
			return duration_cast<nanoseconds>(end - start).count();
		}
	}
}