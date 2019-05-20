#pragma once
#include <ostream>
#include <cmath>

namespace momo
{
	template<typename T>
	class complex
	{
		T Re, Im;
	public:
		static const complex<T> i;

		complex();
		complex(T Re);
		complex(T Re, T Im);

		bool isReal() const;
		bool isImaginary() const;
		bool isComplex() const;

		bool operator==(const complex<T>& c) const;
		bool operator!=(const complex<T>& c) const;
		complex<T> operator-() const;
		complex<T> operator+() const;

		complex<T> operator+(const complex<T>& c) const;
		complex<T> operator-(const complex<T>& c) const;
		complex<T> operator*(const complex<T>& c) const;
		complex<T> operator/(const complex<T>& c) const;

		complex<T>& operator+=(const complex<T>& c);
		complex<T>& operator-=(const complex<T>& c);
		complex<T>& operator*=(const complex<T>& c);
		complex<T>& operator/=(const complex<T>& c);

		template<typename U>
		friend std::ostream& operator<<(std::ostream& out, const complex<U>& c);

		template<typename U>
		friend U abs(const complex<U>& c);

		template<typename U>
		friend U Re(const complex<U>& c);

		template<typename U>
		friend U Im(const complex<U>& c);

		template<typename U>
		friend complex<U> pow(complex<U> c, size_t power);

		template<typename U>
		friend complex<U> conjugate(const complex<U>& c);

		template<typename U>
		friend complex<U> operator+(U d, const complex<U>& c);

		template<typename U>
		friend complex<U> operator-(U d, const complex<U>& c);

		template<typename U>
		friend complex<U> operator*(U d, const complex<U>& c);

		template<typename U>
		friend complex<U> operator/(U d, const complex<U>& c);
	};
	
	template<typename T>
	complex<T> const complex<T>::i(0, 1);

	template<typename T>
	complex<T>::complex() : complex<T>(0, 0) { }

	template<typename T>
	complex<T>::complex(T Re) : complex<T>(Re, 0) { }

	template<typename T>
	complex<T>::complex(T Re, T Im) : Re(Re), Im(Im) { }

	template<typename T>
	bool complex<T>::isReal() const
	{
		return Im == 0;
	}

	template<typename T>
	bool complex<T>::isImaginary() const
	{
		return Re == 0;
	}

	template<typename T>
	bool complex<T>::isComplex() const
	{
		return !isReal() && !isImaginary();
	}

	template<typename T>
	bool complex<T>::operator==(const complex<T>& c) const
	{
		return (Re == c.Re) && (Im == c.Im);
	}

	template<typename T>
	bool complex<T>::operator!=(const complex<T>& c) const
	{
		return !(*this == c);
	}

	template<typename T>
	complex<T> complex<T>::operator-() const
	{
		return Complex(-Re, -Im);
	}

	template<typename T>
	complex<T> complex<T>::operator+() const
	{
		return *this;
	}

	template<typename T>
	complex<T> complex<T>::operator+(const complex<T>& c) const
	{
		return complex<T>(Re + c.Re, Im + c.Im);
	}

	template<typename T>
	complex<T> complex<T>::operator-(const complex<T>& c) const
	{
		return complex<T>(Re - c.Re, Im - c.Im);
	}

	template<typename T>
	complex<T> complex<T>::operator*(const complex<T>& c) const
	{
		complex<T> res;
		res.Re = Re * c.Re - Im * c.Im;
		res.Im = Re * c.Im + Im * c.Re;
		return res;
	}

	template<typename T>
	complex<T> complex<T>::operator/(const complex<T>& c) const
	{
		complex<T> res;
		res.Re = (Re * c.Re + Im * c.Im) / (c.Re * c.Re + c.Im * c.Im);
		res.Im = (c.Re * Im - c.Im * Re) / (c.Re * c.Re + c.Im * c.Im);
		return res;
	}

	template<typename T>
	complex<T>& complex<T>::operator+=(const complex<T>& c)
	{
		*this = *this + c;
		return *this;
	}

	template<typename T>
	complex<T>& complex<T>::operator-=(const complex<T>& c)
	{
		*this = *this - c;
		return *this;
	}

	template<typename T>
	complex<T>& complex<T>::operator*=(const complex<T>& c)
	{
		*this = *this * c;
		return *this;
	}

	template<typename T>
	complex<T>& complex<T>::operator/=(const complex<T>& c)
	{
		*this = *this / c;
		return *this;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const complex<T>& c)
	{
		if (c.Im == 0 && c.Re == 0)
		{
			out << 0;
			return out;
		}

		if (!c.isImaginary())
		{
			out << c.Re;
		}
		if (c.isComplex())
		{
			out << (c.Im > 0 ? " + " : " - ");
		}
		else if (c.isImaginary() && c.Im < 0)
		{
			out << "-";
		}

		if (!c.isReal())
		{
			if (c.Im == 1 || c.Im == -1)
			{
				out << "i";
			}
			else
			{
				out << std::abs(c.Im) << "i";
			}
		}
		return out;
	}

	template<typename U>
	U abs(const complex<U>& c)
	{
		return std::sqrt(c.Re * c.Re + c.Im * c.Im);
	}
	
	template<typename U>
	U Re(const complex<U>& c)
	{
		return complex.Re;
	}

	template<typename U>
	U Im(const complex<U>& c)
	{
		return c.Im;
	}

	template<typename U>
	complex<U> pow(complex<U> c, size_t power)
	{
		if (power == 0)
		{
			return complex<U>(1);
		}
		else if (power % 2 == 1)
		{
			return pow(c, power - 1) * c;
		}
		else
		{
			c = pow(c, power / 2);
			return c * c;
		}
	}

	template<typename U>
	complex<U> conjugate(const complex<U>& c)
	{
		return complex<U>(c.Re, -c.Im);
	}

	template<typename U>
	complex<U> operator+(U d, const complex<U>& c)
	{
		return complex<U>(d + c.Re, c.Im);
	}

	template<typename U>
	complex<U> operator-(U d, const complex<U>& c)
	{
		return complex<U>(d - c.Re, c.Im);
	}

	template<typename U>
	complex<U> operator*(U d, const complex<U>& c)
	{
		return complex<U>(d * c.Re, d * c.Im);
	}

	template<typename U>
	complex<U> operator/(U d, const complex<U>& c)
	{
		return complex<U>(d) / c;
	}
}
