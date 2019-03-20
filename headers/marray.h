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
	class marray : public std::vector<T>
	{
	public:
		marray();
		marray(size_t size);
		marray(size_t size, const T& filler);
		marray(size_t size, T beginFiller, T endFiller);
		marray(std::initializer_list<T>);
		marray(const std::vector<T>&);
		marray(const std::string&);
		marray<T>& operator+= (const T& element);
		marray<T>& operator+= (const marray<T>& other);
		marray<T>& operator*= (const T& element);
		marray<T>& operator*= (const marray<T>& other);
		marray<T>& operator/= (const T& element);
		marray<T>& operator/= (const marray<T>& other);
		marray<T>& operator-= (const T& element);
		marray<T>& operator-= (const marray<T>& other);
		marray<T> operator+ (const T& element) const;
		marray<T> operator- (const T& element) const;
		marray<T> operator* (const T& element) const;
		marray<T> operator/ (const T& element) const;
		marray<T>& append(const marray<T>& other);
		marray<T>& append(const T& element);
		T pop();
		T push(const T& element);
		int find(const T& element) const;
		bool remove(const T& element);
		bool removeAll(const T& element);
		marray<T> slice(int begin, int end = -1, int step = 1) const;
		marray<T>& reverse();
		marray<T>& sort();
		marray<T>& makeUnique();
		marray<T> reversed() const;
		marray<T> sorted() const;
		marray<T> unique() const;

		template<typename U>
		friend std::ostream& operator<<(std::ostream& out, const marray<U>& src);

		template<typename Function>
		marray<T>& apply(Function f);

		template<typename Function>
		marray<T> transfromed(Function f);
	};

	template<typename T>
	marray<T>::marray() : std::vector<T>() { }

	template <typename T>
	marray<T>::marray(size_t size) : std::vector<T>(size) { } 

	template <typename T>
	marray<T>::marray(size_t size, const T& filler) : std::vector<T>(size, filler) { }

	template <typename T>
	marray<T>::marray(size_t size, T beginFiller, T endFiller) : std::vector<T>(size)
	{
		if (beginFiller < endFiller)
		{
			T step = (endFiller - beginFiller) / size;
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] = beginFiller;
				beginFiller += step;
			}
		}
		else
		{
			T step = (beginFiller - endFiller) / size;
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] = beginFiller;
				beginFiller -= step;
			}
		}
	}

	template <typename T>
	marray<T>::marray(std::initializer_list<T> src) : std::vector<T>(src) { }

	template <typename T>
	marray<T>::marray(const std::vector<T>& src) : std::vector<T>(src) { }

	template<typename T>
	marray<T>::marray(const std::string& src) : std::vector<T>(src.begin(), src.end()) { }

	template<typename T>
	inline marray<T>& marray<T>::reverse()
	{
		std::reverse(std::vector<T>::begin(), std::vector<T>::end());
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::sort()
	{
		std::stable_sort(std::vector<T>::begin(), std::vector<T>::end());
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::makeUnique()
	{
		sort();
		auto it = std::unique(std::vector<T>::begin(), std::vector<T>::end());
		std::vector<T>::erase(it, std::vector<T>::end());
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
	inline marray<T> marray<T>::unique() const
	{
		marray<T> res(*this);
		return res.make_unique();
	}

	template<typename T>
	inline marray<T>& marray<T>::operator+=(const T & element)
	{
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			*it += element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator+=(const marray<T>& other)
	{
		size_t end = std::min(std::vector<T>::size(), other.size());
		for (size_t i = 0; i < end; i++)
		{
			(*this)[i] += other[i];
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator*=(const T & element)
	{
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			*it = (*it) * element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator*=(const marray<T>& other)
	{
		size_t end = std::min(std::vector<T>::size(), other.size());
		for (size_t i = 0; i < end; i++)
		{
			(*this)[i] *= other[i];
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator/=(const T & element)
	{
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			*it = (*it) * element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator/=(const marray<T>& other)
	{
		size_t end = std::min(std::vector<T>::size(), other.size());
		for (size_t i = 0; i < end; i++)
		{
			(*this)[i] /= other[i];
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator-=(const T & element)
	{
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			*it = (*it) - element;
		}
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::operator-=(const marray<T>& other)
	{
		size_t end = std::min(std::vector<T>::size(), other.size());
		for (size_t i = 0; i < end; i++)
		{
			(*this)[i] -= other[i];
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
		std::vector<T>::insert(std::vector<T>::end(), other.begin(), other.end());
		return *this;
	}

	template<typename T>
	inline marray<T>& marray<T>::append(const T& element)
	{
		std::vector<T>::push_back(element);
		return *this;
	}

	template<typename T>
	inline T marray<T>::pop()
	{
		T element = std::vector<T>::back();
		std::vector<T>::pop_back();
		return element;
	}

	template<typename T>
	inline T marray<T>::push(const T & element)
	{
		std::vector<T>::push_back(element);
		return element;
	}

	template<typename T>
	inline int marray<T>::find(const T& element) const
	{
		for (size_t i = 0; i < std::vector<T>::size(); i++)
		{
			if ((*this)[i] == element) return i;
		}
		return -1;
	}

	template<typename T>
	inline bool marray<T>::remove(const T& element)
	{
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			if (*it == element)
			{
				std::vector<T>::erase(it);
				return true;
			}
		}
		return false;
	}

	template<typename T>
	inline bool marray<T>::removeAll(const T & element)
	{
		marray<T> res;
		for (auto it = std::vector<T>::begin(); it != std::vector<T>::end(); it++)
		{
			if (*it != element)
			{
				res.push(*it);
			}
		}
		bool removedAny = res.size() == std::vector<T>::size();
		*this = res;
		return removedAny;
	}

	template<typename T>
	inline marray<T> marray<T>::slice(int begin, int end, int step) const
	{
		marray<T> res;
		if (end < 0) end += std::vector<T>::size();
		if (begin < 0) begin += std::vector<T>::size();
		if (step > 0)
		{
			if(begin > end) std::swap(begin, end);
			for (int i = begin; i <= end; i += step)
			{
				res.push((*this)[i]);
			}
		}
		else
		{
			if (begin < end) std::swap(begin, end);
			for (int i = begin; i >= end; i += step)
			{
				res.push((*this)[i]);
			}
		}
		return res;
	}

	template<typename U>
	std::ostream& operator<<(std::ostream & out, const marray<U>& src)
	{
		for (U i : src)
		{
			out << i << " ";
		}
		return out;
	}
	template<typename T>
	template<typename Function>
	inline marray<T>& marray<T>::apply(Function f)
	{
		std::transform(std::vector<T>::begin(), std::vector<T>::end(), std::vector<T>::begin(), f);
		return *this;
	}
	template<typename T>
	template<typename Function>
	inline marray<T> marray<T>::transfromed(Function f)
	{
		marray<T> res(*this);
		return res.apply(f);
	}
}
