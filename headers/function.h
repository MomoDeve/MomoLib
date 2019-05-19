#pragma once

namespace momo
{
	template<typename Out, typename... In>
	class function
	{
		Out(*_function)(In...);

		typedef Out(*f_ptr)(In...);
	public:
		function();
		function(const function<Out, In...>& f);
		function(function<Out, In...>&& f);
		template<typename Func> function(Func f);

		template<typename Func> function<Out, In...>& operator=(Func f);
		function<Out, In...>& operator=(const function<Out, In...>& f);
		function<Out, In...>& operator=(function<Out, In...>&& f);

		Out operator()(In... args) const;
		bool operator==(const function<Out, In...>& f) const;
		bool operator!=(const function<Out, In...>& f) const;

		bool empty() const;
		f_ptr get() const;
		Out invoke(In... args) const;

		f_ptr reset();
		template<typename Func> function<Out, In...>& assign(Func f);
		function<Out, In...>& assign(const function<Out, In...>& f);
		void swap(function<Out, In...>& f);
	};

	template<typename Out, typename ...In>
	function<Out, In...>::function()
		: _function(nullptr) { }

	template<typename Out, typename ...In>
	function<Out, In...>::function(const function<Out, In...>& f)
		: _function(f._function) { }

	template<typename Out, typename ...In>
	function<Out, In...>::function(function<Out, In...>&& f)
		: _function(std::move(f._function)) { }

	template<typename Out, typename ...In>
	template<typename Func>
	function<Out, In...>& function<Out, In...>::operator=(Func f)
	{
		_function = f;
		return *this;
	}

	template<typename Out, typename ...In>
	function<Out, In...>& function<Out, In...>::operator=(const function<Out, In...>& f)
	{
		_function = f._function;
		return *this;
	}

	template<typename Out, typename ...In>
	function<Out, In...>& function<Out, In...>::operator=(function<Out, In...>&& f)
	{
		_function = std::move(f._function);
		return *this;
	}

	template<typename Out, typename ...In>
	inline Out function<Out, In...>::operator()(In... args) const
	{
		return _function(args...);
	}

	template<typename Out, typename ...In>
	inline bool function<Out, In...>::operator==(const function<Out, In...>& f) const
	{
		return _function == f._function;
	}

	template<typename Out, typename ...In>
	inline bool function<Out, In...>::operator!=(const function<Out, In...>& f) const
	{
		return _function != f._function;
	}

	template<typename Out, typename ...In>
	inline bool function<Out, In...>::empty() const
	{
		return _function != nullptr;
	}

	template<typename Out, typename ...In>
	inline typename function<Out, In...>::f_ptr function<Out, In...>::get() const
	{
		return _function;
	}

	template<typename Out, typename ...In>
	inline Out function<Out, In...>::invoke(In ...args) const
	{
		return _function(args...);
	}

	template<typename Out, typename ...In>
	typename function<Out, In...>::f_ptr function<Out, In...>::reset()
	{
		f_ptr res = _function;
		_function = nullptr;
		return res;
	}

	template<typename Out, typename ...In>
	template<typename Func>
	function<Out, In...>& function<Out, In...>::assign(Func f)
	{
		_function = f;
		return *this;
	}

	template<typename Out, typename ...In>
	function<Out, In...>& function<Out, In...>::assign(const function<Out, In...>& f)
	{
		_function = f._function;
		return *this;
	}

	template<typename Out, typename ...In>
	void function<Out, In...>::swap(function<Out, In...>& f)
	{
		f_ptr tmp = _function;
		_function = f._function;
		f._function = _function;
	}

	template<typename Out, typename ...In>
	template<typename Func>
	function<Out, In...>::function(Func f)
	{
		_function = f;
	}
}
