#pragma once

#include <vector>
#include <iostream>
#include <exception>
#include <initializer_list>

#ifndef size_t
typedef unsigned int size_t;
#endif

namespace momo
{
	class calculationException : public std::exception
	{
		virtual const char* what() const throw();
	};

	template <typename T> class matrix
	{
	private:
		size_t size_x, size_y;
		void create(size_t, size_t);
	public:
		std::vector<std::vector<T> > vec;
		matrix();
		matrix(size_t);
		matrix(size_t, size_t);
		matrix(const matrix<T>&);
		matrix(T**, size_t, size_t);
		matrix(const std::vector<std::vector<T> >&);
		matrix(std::initializer_list<std::initializer_list<T> >);
		~matrix();
		size_t xsize() const;
		size_t ysize() const;
		std::vector<std::vector<T> > getVectorCopy() const;
		matrix<T>& operator=(const matrix<T>&);
		matrix<T>& operator=(const std::vector<std::vector<T> >&);
		bool operator==(const matrix<T>&) const;
		bool operator!=(const matrix<T>&) const;
		matrix<T>& operator+=(const matrix<T>);
		matrix<T>& operator-=(const matrix<T>);
		matrix<T>& operator*=(const matrix<T>);
		matrix<T>& operator+=(T);
		matrix<T>& operator-=(T);
		matrix<T>& operator*=(T);
		matrix<T>& operator/=(T);
		matrix<T> operator+(const matrix<T>&) const;
		matrix<T> operator-(const matrix<T>&) const;
		matrix<T> operator*(const matrix<T>&) const;
		matrix<T> operator+(T) const;
		matrix<T> operator-(T) const;
		matrix<T> operator*(T) const;
		matrix<T> operator/(T) const;
	};

	template<typename T>
	void display(const matrix<T>& src);

	template<typename T>
	matrix<T> pow(matrix<T> src, size_t pow);

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const matrix<T>& M);

	template<typename T>
	matrix<T> operator*(T value, const matrix<T>& M);
}
