#pragma once

#include "delegate.h"

namespace momo
{
	template<typename... In>
	class event
	{
		delegate<void, In...> _delegate;
		event& operator=(const event& other) = delete;
		event& operator=(event&& other) = delete;
	public:
		event();
		void invoke(In... args) const;
		bool empty() const;
		void operator()(In... args) const;
		template<typename Func> event<In...>& operator+= (Func f);
	};

	template<typename ...In>
	event<In...>::event() { }

	template<typename ...In>
	void event<In...>::invoke(In... args) const
	{
		_delegate.invoke(args...);
	}

	template<typename... In>
	bool event<In...>::empty() const
	{
		return _delegate.empty();
	}

	template<typename... In>
	void event<In... >::operator()(In ...args) const
	{
		_delegate.invoke(args...);
	}

	template<typename... In>
	template<typename Func>
	event<In...>& event<In...>::operator+=(Func f)
	{
		_delegate += f;
		return *this;
	}
}
