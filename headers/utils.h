#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <initializer_list>

namespace momo
{
	template<typename Lambda>
	class ScopeExitHandler
	{
		Lambda _lambda;
		bool active;
	public:
		ScopeExitHandler(Lambda&& lambda) : _lambda(std::move(lambda)), active(false) { }
		ScopeExitHandler(ScopeExitHandler&& handler) : _lambda(std::move(handler._lambda)), active(true) { }
		~ScopeExitHandler() { _lambda(); }
		ScopeExitHandler() = delete;
	};

	enum class ScopeExitHandlerHelper { };

	template<typename Lambda>
	ScopeExitHandler<Lambda> operator+ (ScopeExitHandlerHelper helper, Lambda&& lambda)
	{
		return ScopeExitHandler<Lambda>(std::move(lambda));
	}

	#define MOMO_INNER_CONCAT(value1, value2) value1##value2
	#define CONCAT(value1, value2) MOMO_INNER_CONCAT(value1, value2)
	#define ON_SCOPE_EXIT auto CONCAT(momo_ScopeExitHandler, __LINE__) = ScopeExitHandlerHelper() + [&] ()

	template<typename... Others>
	size_t print() 
	{
		std::cout << std::endl;
		return 0; 
	}

	template<typename First, typename... Others>
	size_t print(First arg, Others... args)
	{
		std::cout << arg << ' ';
		return 1 + print(args...);
	}

	template<typename... Others>
	size_t print(std::ostream& out) 
	{ 
		out << std::endl;
		return 0; 
	}

	template<typename First, typename... Others>
	size_t print(std::ostream& out, First arg, Others... args)
	{
		out << arg << ' ';
		return 1 + print(out, args...);
	}
	
	#ifdef MOMO_DEBUG
	template<typename... Args>
	size_t debug(Args... args)
	{
		return print(args...);
	}
	#else
	template<typename... Args>
	void debug(Args... args) { }
	#endif

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const std::vector<T>& _vec)
	{
		out << '[';
		if (!_vec.empty())
		{
			for (auto it = _vec.begin(); it != _vec.end() - 1; it++)
			{
				out << *it << ", ";
			}
			out << _vec.back();
		}
		out << ']';
		return out;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const std::set<T>& _set)
	{
		out << '(';
		if (!_set.empty())
		{
			decltype(_set.begin()) begin, end;
			for (begin = _set.begin(), end = --_set.end(); begin != end; begin++)
			{
				out << *begin << ", ";
			}
			out << *end;
		}
		out << ')';
		return out;
	}

	template<typename T, typename U>
	std::ostream& operator<<(std::ostream& out, const std::map<T, U>& _map)
	{
		out << '{';
		if (!_map.empty())
		{
			decltype(_map.begin()) begin, end;
			for (begin = _map.begin(), end = --_map.end(); begin != end; begin++)
			{
				out << begin->first << ": " << begin->second << ", ";
			}
			out << end->first << ": " << end->second;
		}
		out << '}';
		return out;
	}

	template<typename T, typename U>
	std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& _pair)
	{
		out << '[' << _pair.first << ", " << _pair.second << ']';
		return out;
	}

	template<typename T, typename Pr = std::less<T> >
	void sort(std::vector<T>& _vec, Pr pred = Pr())
	{
		std::sort(_vec.begin(), _vec.end(), pred);
	}
}