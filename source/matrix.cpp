#pragma once
#pragma once

#include <vector>
#include <iostream>
#include <exception>
#include <initializer_list>
#include "matrix.h"

namespace momo
{
	const char* calculationException::what() const throw()
	{
		return "Exception occured: matrices could not be calculated";
	}

	template<typename T>
	void display(const matrix<T>& src)
	{
		for (size_t i = 0; i < src.xsize(); i++)
		{
			for (size_t j = 0; j < src.ysize(); j++)
			{
				std::cout << src.vec[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

	template<typename T>
	matrix<T> pow(matrix<T> src, size_t pow)
	{
		matrix<T> res(src.vec);
		while (pow)
		{
			if (pow & 1) res *= src;
			src *= src;
			pow >>= 1;
		}
		return res;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const matrix<T>& M)
	{
		for (size_t i = 0; i < M.xsize(); i++)
		{
			for (size_t j = 0; j < M.ysize(); j++)
			{
				os << M.vec[i][j] << " ";
			}
			os << "\n";
		}
		return os;
	}

	template<typename T>
	matrix<T> operator*(T value, const matrix<T>& M)
	{
		matrix<T> res(M.xsize(), M.ysize());
		for (size_t i = 0; i < res.xsize(); i++)
		{
			for (size_t j = 0; j < res.ysize(); j++)
			{
				res.vec[i][j] = value * M.vec[i][j];
			}
		}
		return res;
	}

	template<typename T>
	void matrix<T>::create(size_t xsize, size_t ysize)
	{
		size_x = xsize;
		size_y = ysize;
		vec.resize(size_x);
		for (size_t i = 0; i < size_x; i++)
		{
			vec[i].resize(size_y);
		}
	}

	template<typename T>
	matrix<T>::matrix()
	{
		size_x = 0;
		size_y = 0;
	}

	template<typename T>
	matrix<T>::matrix(size_t size)
	{
		create(size, size);
	}

	template<typename T>
	matrix<T>::matrix(size_t xsize, size_t ysize)
	{
		create(xsize, ysize);
	}

	template<typename T>
	matrix<T>::matrix(const matrix<T>& src)
	{
		size_x = src.size_x;
		size_y = src.size_y;
		vec = src.vec;
	}

	template<typename T>
	matrix<T>::matrix(T** src, size_t xsize, size_t ysize)
	{
		create(xsize, ysize);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] = src[i][j];
			}
		}
	}

	template<typename T>
	matrix<T>::matrix(const std::vector<std::vector<T> >& src)
	{
		size_x = src.size();
		size_y = src[0].size();
		vec = src;
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<std::initializer_list<T> > src)
	{
		for (auto i : src) vec.push_back(i);
		size_x = vec.size();
		size_y = vec[0].size();
	}

	template<typename T>
	matrix<T>::~matrix() {}

	template<typename T>
	size_t matrix<T>::xsize() const
	{
		return size_x;
	}

	template<typename T>
	size_t matrix<T>::ysize() const
	{
		return size_y;
	}

	template<typename T>
	std::vector<std::vector<T> > matrix<T>::getVectorCopy() const
	{
		return vec;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator=(const matrix<T>& src)
	{
		size_x = src.size_x;
		size_y = src.size_y;
		vec = src.vec;
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator=(const std::vector<std::vector<T> >& src)
	{
		size_x = src.size();
		size_y = src[0].size();
		vec = src;
		return *this;
	}

	template<typename T>
	bool matrix<T>::operator==(const matrix<T>& src) const
	{
		return vec == src.vec;
	}

	template<typename T>
	bool matrix<T>::operator!=(const matrix<T>& src) const
	{
		return vec != src.vec;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator+=(const matrix<T> M)
	{
		if (size_x != M.size_x || size_y != M.size_y) throw new calculationException;
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] += M.vec[i][j];
			}
		}
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator-=(const matrix<T> M)
	{
		if (size_x != M.size_x || size_y != M.size_y) throw new calculationException;
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] -= M.vec[i][j];
			}
		}
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator-=(T value)
	{
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] = vec[i][j] - value;
			}
		}
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator+=(T value)
	{
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] = vec[i][j] + value;
			}
		}
		return *this;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(const matrix<T>& M) const
	{
		if (size_x != M.size_x || size_y != M.size_y) throw new calculationException;
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] + M.vec[i][j];
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator-(const matrix<T>& M) const
	{
		if (size_x != M.size_x || size_y != M.size_y) throw new calculationException;
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] - M.vec[i][j];
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const matrix<T>& M) const
	{
		if (size_y != M.size_x) throw new calculationException();
		matrix<T> res(size_x, M.size_y);
		for (size_t i = 0; i < res.xsize(); i++)
		{
			for (size_t j = 0; j < res.ysize(); j++)
			{
				res.vec[i][j] = 0;
				for (size_t k = 0; k < size_y; k++)
				{
					res.vec[i][j] += vec[i][k] * M.vec[k][j];
				}
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(T value) const
	{
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] + value;
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator-(T value) const
	{
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] - value;
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(T value) const
	{
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] * value;
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator/(T value) const
	{
		matrix<T> res(size_x, size_y);
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				res.vec[i][j] = vec[i][j] / value;
			}
		} // no check for devision-by-zero
		return res;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(T value)
	{
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] = value * vec[i][j];
			}
		}
		return *this;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator/=(T value)
	{
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				vec[i][j] = vec[i][j] / value;
			}
		} // no check for devision-by-zero
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(const matrix<T> M)
	{
		if (size_y != M.size_x) throw new calculationException();
		matrix<T> res(size_x, M.size_y);
		for (size_t i = 0; i < res.xsize(); i++)
		{
			for (size_t j = 0; j < res.ysize(); j++)
			{
				res.vec[i][j] = 0;
				for (size_t k = 0; k < size_y; k++)
				{
					res.vec[i][j] += vec[i][k] * M.vec[k][j];
				}
			}
		}
		(*this) = res;
		return *this;
	}
}
