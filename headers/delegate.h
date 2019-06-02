#pragma once

#include <functional>
#include <vector>
#include <initializer_list>

namespace momo
{
	template<typename Out, typename... In>
	class delegate
	{
		template<typename T> using function = std::function<T>;

		std::vector<function<Out(In...)> > _functions;
	public:
		delegate();
		delegate(const function<Out(In...)>& f);
		delegate(function<Out(In...)>&& f);
		delegate(const delegate<Out, In...>& other);
		delegate(delegate<Out, In...>&& other);
		template<typename Func> delegate(Func f);
		template<typename Func> delegate(const std::initializer_list<Func>& list);

		delegate<Out, In...>& operator=(const delegate<Out, In...>& other);
		delegate<Out, In...>& operator=(delegate<Out, In...>&& other);
		delegate<Out, In...>& operator=(const function<Out(In...)>& f);
		delegate<Out, In...>& operator=(function<Out(In...)>&& f);
		template<typename Func> delegate<Out, In...>& operator=(Func f);
		template<typename Func> delegate<Out, In...>& operator=(const std::initializer_list<Func>& list);

		Out operator()(In... args) const;
		
		Out invoke(In... args) const;
		bool empty() const;

		void clear();

		delegate<Out, In...>& operator+=(const delegate<Out, In...>& other);
		delegate<Out, In...>& operator+=(delegate<Out, In...>&& other);
		delegate<Out, In...>& operator+=(const function<Out(In...)>& f);
		delegate<Out, In...>& operator+=(function<Out(In...)>&& f);
		template<typename Func> delegate<Out, In...>& operator+=(Func f);
		template<typename Func> delegate<Out, In...>& operator+=(const std::initializer_list<Func>& list);

		delegate<Out, In...> operator+(const delegate<Out, In...>& other) const;
		delegate<Out, In...> operator+(const function<Out(In...)>& f) const;
		template<typename Func> delegate<Out, In...> operator+(Func f) const;
		template<typename Func> delegate<Out, In...> operator+(const std::initializer_list<Func>& list) const;
	};

	template<typename... Args> using action = delegate<void, Args...>;
	template<typename... Args> using predicate = delegate<bool, Args...>;

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate() { }

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(const function<Out(In...)>& f)
	{
		_functions.push_back(f);
	}

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(function<Out(In...)>&& f)
	{
		_functions.emplace_back(std::move(f));
	}
	
	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(const delegate<Out, In...>& other)
		: _functions(other._functions) { }

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(delegate<Out, In...>&& other)
		: _functions(std::move(other._functions)) { }

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>::delegate(Func f)
		: _functions()
	{
		_functions.emplace_back(function<Out(In...)>(f));
	}

	template<typename Out, typename... In>
	template<typename Func> 
	delegate<Out, In...>::delegate(const std::initializer_list<Func>& list)
	{
		for (auto f : list)
		{
			_functions.emplace_back(std::move(function<Out(In...)>(f)));
		}
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(const delegate<Out, In...>& other)
	{
		_functions = other._functions;
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(delegate<Out, In...>&& other)
	{
		_functions = std::move(other._functions);
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(const function<Out(In...)>& f)
	{
		_functions.clear();
		_functions.push_back(f);
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(function<Out(In...)>&& f)
	{
		_functions.clear();
		_functions.emplace_back(std::move(f));
		return *this;
	}

	template<typename Out, typename... In>
	template<typename Func> 
	delegate<Out, In...>& delegate<Out, In...>::operator=(Func f)
	{
		_functions.clear();
		_functions.emplace_back(std::move(function<Out(In...)>(f)));
		return *this;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>& delegate<Out, In...>::operator=(const std::initializer_list<Func>& list)
	{
		_functions.clear();
		for (auto f : list)
		{
			_functions.emplace_back(std::move(function<Out(In...)>(f)));
		}
		return *this;
	}

	template<typename Out, typename ...In>
	Out delegate<Out, In...>::operator()(In... args) const
	{
		for (int i = 0; i < (int)_functions.size() - 1; i++)
		{
			_functions[i](args...);
		}
		return _functions.back()(args...);
	}

	template<typename Out, typename ...In>
	Out delegate<Out, In...>::invoke(In ...args) const
	{
		for (int i = 0; i < (int)_functions.size() - 1; i++)
		{
			_functions[i](args...);
		}
		return _functions.back()(args...);
	}

	template<typename Out, typename ...In>
	inline bool delegate<Out, In...>::empty() const
	{
		return _functions.empty();
	}

	template<typename Out, typename ...In>
	inline void delegate<Out, In...>::clear()
	{
		_functions.clear();
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(const delegate<Out, In...>& other)
	{
		_functions.insert(_functions.end(), other._functions.begin(), other._functions.end());
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(delegate<Out, In...>&& other)
	{
		_functions.insert(
			_functions.end(),
			std::make_move_iterator(other._functions.begin()), 
			std::make_move_iterator(other._functions.end()));
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(function<Out(In...)>&& f)
	{
		_functions.push_back(std::move(f));
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(const function<Out(In...)>& f)
	{
		_functions.push_back(f);
		return *this;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(Func f)
	{
		_functions.emplace_back(f);
		return *this;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(const std::initializer_list<Func>& list)
	{
		for (auto f : list)
		{
			_functions.emplace_back(std::move(function<Out(In...)>(f)));
		}
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...> delegate<Out, In...>::operator+(const delegate<Out, In...>& other) const
	{
		delegate<Out, In...> res = *this;
		res += other;
		return res;
	}

	template<typename Out, typename... In>
	delegate<Out, In...> delegate<Out, In...>::operator+(const function<Out(In...)>& f) const
	{
		delegate<Out, In...> res = *this;
		res += f;
		return res;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...> delegate<Out, In...>::operator+(Func f) const
	{
		delegate<Out, In...> res = *this;
		res += f;
		return res;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...> delegate<Out, In...>::operator+(const std::initializer_list<Func>& list) const
	{
		delegate<Out, In...> res = *this;
		res += list;
		return res;
	}

	template<typename T, typename Out, typename... In>
	std::function<Out(In...)> make_method(T&& object, Out(T::*method)(In...) const)
	{
		return[object{ std::move(object) }, method](In... args) { (object.*method)(args...); };
	}

	template<typename T, typename Out, typename... In>
	std::function<Out(In...)> make_method(T& object, Out(T::*method)(In...) const)
	{
		return[&object, method](In... args) { (object.*method)(args...); };
	}

	template<typename T, typename Out, typename... In>
	std::function<Out(In...)> make_method(T&& object, Out (T::*method)(In...))
	{
		return [object{ std::move(object) }, method](In... args) mutable { (object.*method)(args...); };
	}

	template<typename T, typename Out, typename... In>
	std::function<Out(In...)> make_method(T& object, Out(T::*method)(In...))
	{
		return[&object, method](In... args) mutable { (object.*method)(args...); };
	}
}