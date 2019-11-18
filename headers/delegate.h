#pragma once

#include <functional>
#include <vector>
#include <initializer_list>

namespace momo
{
	template<typename Out, typename... In>
	class delegate
	{
		using function = std::function<Out(In...)>;

		std::vector<function> _functions;
	public:
		delegate() = default;
		delegate(const delegate<Out, In...>&) = default;
		delegate(delegate<Out, In...> &&) = default;
		delegate(const function& f);
		delegate(function&& f);
		template<typename Func> delegate(Func f);
		template<typename Func> delegate(const std::initializer_list<Func>& list);

		delegate<Out, In...>& operator=(const delegate<Out, In...>&) = default;
		delegate<Out, In...>& operator=(delegate<Out, In...>&&) = default;
		delegate<Out, In...>& operator=(const function& f);
		delegate<Out, In...>& operator=(function&& f);
		template<typename Func> delegate<Out, In...>& operator=(Func f);
		template<typename Func> delegate<Out, In...>& operator=(const std::initializer_list<Func>& list);

		Out operator()(In&&... args) const;
		
		Out invoke(In&&... args) const;
		bool empty() const;

		void clear();

		delegate<Out, In...>& operator+=(const delegate<Out, In...>& other);
		delegate<Out, In...>& operator+=(delegate<Out, In...>&& other);
		delegate<Out, In...>& operator+=(const function& f);
		delegate<Out, In...>& operator+=(function&& f);
		template<typename Func> delegate<Out, In...>& operator+=(Func f);
		template<typename Func> delegate<Out, In...>& operator+=(const std::initializer_list<Func>& list);

		delegate<Out, In...> operator+(const delegate<Out, In...>& other) const;
		delegate<Out, In...> operator+(const function& f) const;
		template<typename Func> delegate<Out, In...> operator+(Func f) const;
		template<typename Func> delegate<Out, In...> operator+(const std::initializer_list<Func>& list) const;
	};

	template<typename... Args> using action = delegate<void, Args...>;
	template<typename... Args> using predicate = delegate<bool, Args...>;

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(const function& f)
	{
		_functions.push_back(f);
	}

	template<typename Out, typename... In>
	delegate<Out, In...>::delegate(function&& f)
	{
		_functions.emplace_back(std::move(f));
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>::delegate(Func f)
		: _functions()
	{
		_functions.emplace_back(function(f));
	}

	template<typename Out, typename... In>
	template<typename Func> 
	delegate<Out, In...>::delegate(const std::initializer_list<Func>& list)
	{
		for (auto f : list)
		{
			_functions.emplace_back(std::move(function(f)));
		}
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(const function& f)
	{
		_functions.clear();
		_functions.push_back(f);
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator=(function&& f)
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
		_functions.emplace_back(std::move(function(f)));
		return *this;
	}

	template<typename Out, typename... In>
	template<typename Func>
	delegate<Out, In...>& delegate<Out, In...>::operator=(const std::initializer_list<Func>& list)
	{
		_functions.clear();
		for (auto f : list)
		{
			_functions.emplace_back(std::move(function(f)));
		}
		return *this;
	}

	template<typename Out, typename ...In>
	Out delegate<Out, In...>::operator()(In&&... args) const
	{
		for (int i = 0; i < (int)_functions.size() - 1; i++)
		{
			_functions[i](args...);
		}
		return _functions.back()(args...);
	}

	template<typename Out, typename ...In>
	Out delegate<Out, In...>::invoke(In&&... args) const
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
	delegate<Out, In...>& delegate<Out, In...>::operator+=(function&& f)
	{
		_functions.push_back(std::move(f));
		return *this;
	}

	template<typename Out, typename... In>
	delegate<Out, In...>& delegate<Out, In...>::operator+=(const function& f)
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
			_functions.emplace_back(std::move(function(f)));
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
	delegate<Out, In...> delegate<Out, In...>::operator+(const function& f) const
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
		return [&object, method](In... args) mutable { (object.*method)(args...); };
	}

	template<typename T, typename Out, typename... In>
	std::function<Out(In...)> make_method(const T& object, Out(T::* method)(In...) const)
	{
		return [&object, method](In... args) { (object.*method)(args...); };
	}
}