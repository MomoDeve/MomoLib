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
}
