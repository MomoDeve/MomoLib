#pragma once

#include <vector>
#include <initializer_list>
#include <iostream>
#include <algorithm>

#ifndef size_t
typedef unsigned int size_t;
#endif

namespace momo
{
	template <typename T>
	class marray
	{
	private:
		std::vector<T> vec;
	public:
		marray() = default;
		marray(size_t size);
		marray(size_t size, const T& filler);
		marray(size_t size, T beginFiller, T endFiller);
		marray(std::initializer_list<T>);
		marray(const std::vector<T>&);
		size_t size() const;
		T& operator[] (int index);
		marray<T>& operator+= (const T& element);
		marray<T>& operator+= (const marray<T>& other);
		marray<T>& operator+= (const std::vector<T>& other);
		marray<T>& operator*= (const T& element);
		marray<T>& operator/= (const T& element);
		marray<T>& operator-= (const T& element);
		marray<T> operator+ (const T& element) const;
		marray<T> operator- (const T& element) const;
		marray<T> operator* (const T& element) const;
		marray<T> operator/ (const T& element) const;
		marray<T>& append(const marray<T>& other);
		marray<T>& append(const T& element);
		marray<T>& append(std::initializer_list<T> src);
		marray<T>& append(const std::vector<T>& src);
		T pop();
		T push(const T& element);
		marray<T> slice(int begin, int end = -1, int step = 1) const;
		marray<T>& reverse();
		marray<T>& sort();
		marray<T> reversed() const;
		marray<T> sorted() const;

		template<typename U>
		friend std::ostream& operator<<(std::ostream& out, const marray<U>& src);
	};

	template <typename T>
	marray<T>::marray(size_t size)
	{
		vec.resize(size, 0);
	}

	template <typename T>
	marray<T>::marray(size_t size, const T& filler)
	{
		vec.resize(size, filler);
	}

	template <typename T>
	marray<T>::marray(size_t size, T beginFiller, T endFiller)
	{
		vec.resize(size);
		if (beginFiller < endFiller)
		{
			T step = (endFiller - beginFiller) / size;
			for (size_t i = 0; i < size; i++)
			{
				vec[i] = beginFiller;
				beginFiller += step;
			}
		}
		else
		{
			T step = (beginFiller - endFiller) / size;
			for (size_t i = 0; i < size; i++)
			{
				vec[i] = beginFiller;
				beginFiller -= step;
			}
		}
	}

	template <typename T>
	marray<T>::marray(std::initializer_list<T> list)
	{
		vec = list;
	}

	template <typename T>
	marray<T>::marray(const std::vector<T>& src)
	{
		vec = src;
	}

	template<typename T>
	inline size_t marray<T>::size() const
	{
		return vec.size();
	}

	template<typename T>
	inline marray<T>& marray<T>::reverse()
	{
		std::reverse(vec.begin(), vec.end());
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::sort()
	{
		std::stable_sort(vec.begin(), vec.end());
		return *this;
	}

	template<typename T>
	inline marray<T> marray<T>::reversed() const
	{
		marray<T> res(*this);
		return res.reverse();
	}

	template<typename T>
	inline marray<T> marray<T>::sorted() const
	{
		marray<T> res(*this);
		return res.sort();
	}

	template<typename T>
	inline T & marray<T>::operator[](int index)
	{
		if (index < 0) index += size();
		return vec[index];
	}

	template<typename T>
	inline marray<T>& marray<T>::operator+=(const T & element)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			*it += element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator+=(const marray<T>& other)
	{
		size_t limit = std::min(size(), other.size());
		for (size_t i = 0; i < limit; i++)
		{
			vec[i] += other.vec[i];
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator+=(const std::vector<T>& other)
	{
		size_t limit = std::min(size(), other.size());
		for (size_t i = 0; i < limit; i++)
		{
			vec[i] += other[i];
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator*=(const T & element)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			*it = (*it) * element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator/=(const T & element)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			*it = (*it) * element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator-=(const T & element)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			*it = (*it) - element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T> marray<T>::operator+(const T & element) const
	{
		marray<T> res(*this);
		for (auto it = res.vec.begin(); it != res.vec.end(); it++) 
		{
			*it = (*it) + element;
		}
		return res;
	}

	template<typename T>
	inline marray<T> marray<T>::operator-(const T & element) const
	{
		marray<T> res(*this);
		for (auto it = res.vec.begin(); it != res.vec.end(); it++)
		{
			*it = (*it) - element;
		}
		return res;
	}

	template<typename T>
	inline marray<T> marray<T>::operator*(const T & element) const
	{
		marray<T> res(*this);
		for (auto it = res.vec.begin(); it != res.vec.end(); it++)
		{
			*it = (*it) * element;
		}
		return res;
	}

	template<typename T>
	inline marray<T> marray<T>::operator/(const T & element) const
	{
		marray<T> res(*this);
		for (auto it = res.vec.begin(); it != res.vec.end(); it++)
		{
			*it = (*it) / element;
		}
		return res;
	}

	template<typename T>
	inline marray<T>& marray<T>::append(const marray<T>& other)
	{
		vec.insert(vec.end(), other.vec.begin(), other.vec.end());
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::append(const T& element)
	{
		vec.push_back(element);
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::append(std::initializer_list<T> src)
	{
		vec.insert(vec.end(), src);
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::append(const std::vector<T>& src)
	{
		vec.insert(vec.end(), src);
		return *this;
	}

	template<typename T>
	inline T marray<T>::pop()
	{
		T element = vec.back();
		vec.pop_back();
		return element;
	}

	template<typename T>
	inline T marray<T>::push(const T & element)
	{
		vec.push_back(element);
		return element;
	}

	template<typename T>
	inline marray<T> marray<T>::slice(int begin, int end, int step) const
	{
		marray<T> res;
		if (end < 0) end += size();
		if (begin < 0) begin += size();
		if (step > 0)
		{
			if(begin > end) std::swap(begin, end);
			for (int i = begin; i <= end; i += step)
			{
				res.push(vec[i]);
			}
		}
		else
		{
			if (step < 0 && begin < end)
			{
				std::swap(begin, end);
			}
			for (int i = begin; i >= end; i += step)
			{
				res.push(vec[i]);
			}
		}
		return res;
	}

	template<typename U>
	std::ostream& operator<<(std::ostream & out, const marray<U>& src)
	{
		for (U i : src.vec)
		{
			out << i << " ";
		}
		return out;
	}
}
