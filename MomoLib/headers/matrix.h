#pragma once

#include <vector>
#include <iostream>
#include <exception>
#include <initializer_list>
#include <set>
#include <sstream>

/*
this is simple matrix class. I usually use it when solve 
programming contests, so you'd better install actual math library, glm for example
*/
namespace momo
{
	template <typename T>
	class matrix
	{
	private:
		size_t size_x, size_y;
	public:
		std::vector<std::vector<T> > vec;
		matrix();
		matrix(size_t);
		matrix(size_t, size_t);
		matrix(size_t, size_t, T);
		matrix(const matrix<T>&);
		matrix(matrix<T>&&);
		matrix(T**, size_t, size_t);
		matrix(const std::vector<std::vector<T> >&);
		matrix(std::initializer_list<std::initializer_list<T> >);
		matrix(std::initializer_list<T>);
		~matrix() = default;
		size_t xsize() const;
		size_t ysize() const;
		std::vector<std::vector<T> > getVectorCopy() const;
		matrix<T>& operator=(const matrix<T>&);
		matrix<T>& operator=(const std::vector<std::vector<T> >&);
		bool operator==(const matrix<T>&) const;
		bool operator!=(const matrix<T>&) const;
		matrix<T>& operator+=(const matrix<T>&);
		matrix<T>& operator-=(const matrix<T>&);
		matrix<T>& operator*=(const matrix<T>&);
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

		template<typename U>
		friend matrix<U> mult(const matrix<U>&, const matrix<U>&, U mod);
	};

	template<typename T>
	matrix<T> pow(const matrix<T>& M, long long power) {
		if (power == 1)
		{
			return M;
		}
		else if (power == 2)
		{
			return M * M;
		}

		if (power % 2 == 1)
			return pow(M, power - 1) * M;
		else {
			auto TMP = pow(M, power / 2);
			return TMP * TMP;
		}
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
	matrix<T>::matrix()
		: size_x(0), size_y(0) { }

	template<typename T>
	matrix<T>::matrix(size_t size)
		: matrix(size, size) { }

	template<typename T>
	matrix<T>::matrix(size_t xsize, size_t ysize)
		: matrix(xsize, ysize, T()) { }

	template<typename T>
	matrix<T>::matrix(size_t xsize, size_t ysize, T fill)
		: size_x(xsize), size_y(ysize), vec(xsize, std::vector<T>(ysize, fill)) { }

	template<typename T>
	matrix<T>::matrix(const matrix<T>& src)
		: size_x(src.size_x), size_y(src.size_y), vec(src.vec) { }

	template<typename T>
	matrix<T>::matrix(matrix<T>&& M)
		: size_x(M.size_x), size_y(M.size_y), vec(std::move(M.vec)) { }

	template<typename T>
	matrix<T>::matrix(T** src, size_t xsize, size_t ysize)
		: matrix(xsize, ysize)
	{
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
		: size_x(src.size()), size_y(src[0].size()), vec(src) { }

	template<typename T>
	matrix<T>::matrix(std::initializer_list<std::initializer_list<T> > src)
	{
		for (auto i : src) vec.push_back(i);
		size_x = vec.size();
		size_y = vec[0].size();
	}

	template<typename T>
	matrix<T>::matrix(std::initializer_list<T> src)
	{
		vec.resize(src.size());
		size_x = size_y = vec.size();
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			*it = src;
		}
	}

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
	matrix<T>& matrix<T>::operator+=(const matrix<T>& M)
	{
		if (size_x != M.size_x || size_y != M.size_y) return *this;
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
	matrix<T>& matrix<T>::operator-=(const matrix<T>& M)
	{
		if (size_x != M.size_x || size_y != M.size_y) return *this;
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
		matrix<T> res = *this;
		res += M;
		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator-(const matrix<T>& M) const
	{
		matrix<T> res = *this;
		res -= M;
		return res;
	}

	template<typename T>
	matrix<T>& matrix<T>::operator*=(const matrix<T>& M)
	{
		*this = *this * M;
		return *this;
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
	matrix<T> matrix<T>::operator*(const matrix<T>& M) const
	{
		if (this->size_y != M.size_x)
		{
			return matrix<T>(1, 1);
		}
		matrix<T> res(this->size_x, M.size_y);
		for (size_t i = 0; i < res.xsize(); i++)
		{
			for (size_t j = 0; j < res.ysize(); j++)
			{
				res.vec[i][j] = T();
				for (size_t k = 0; k < this->size_y; k++)
				{
					res.vec[i][j] += vec[i][k] * M.vec[k][j];
				}
			}
		}
		return res;
	}

	template<typename U>
	matrix<U> mult(const matrix<U>& M1, const matrix<U>& M2, U mod)
	{
		if (M1.size_y != M2.size_x)
		{
			return matrix<U>(1, 1);
		}
		matrix<U> res(M1.size_x, M2.size_y);
		for (size_t i = 0; i < res.xsize(); i++)
		{
			for (size_t j = 0; j < res.ysize(); j++)
			{
				res.vec[i][j] = U();
				for (size_t k = 0; k < M1.size_y; k++)
				{
					res.vec[i][j] += M1.vec[i][k] * M2.vec[k][j] % mod;
					res.vec[i][j] %= mod;
				}
			}
		}
		return res;
	}

	template<typename T>
	matrix<T> pow(const matrix<T>& M, long long power, T mod) {
		if (power == 1)
		{
			return M;
		}
		else if (power == 2)
		{
			return mult(M, M, mod);
		}

		if (power % 2 == 1)
			return mult(M, pow(M, power - 1, mod), mod);
		else 
		{
			auto X = mult(M, M, mod);
			return pow(X, power / 2, mod);
		}
	}
}
