#include "complex.h"

namespace momo
{
	namespace complex
	{
		double Complex::Eps = 1e-9;
		Complex const Complex::i(0.0, 1.0);

		Complex::Complex() : Complex(0.0, 0.0) { }
		Complex::Complex(double Re) : Complex(Re, 0.0) { }
		Complex::Complex(double Re, double Im) : Re(Re), Im(Im) { }

		bool Complex::isReal() const
		{
			return std::abs(Im) < Eps;
		}

		bool Complex::isImaginary() const
		{
			return std::abs(Re) < Eps;
		}

		bool Complex::isComplex() const
		{
			return !isReal() && !isImaginary();
		}

		bool Complex::operator==(const Complex& complex) const
		{
			return (std::abs(Re - complex.Re) < Eps) && (std::abs(Im - complex.Im) < Eps);
		}

		bool Complex::operator!=(const Complex& complex) const
		{
			return !(*this == complex);
		}

		Complex Complex::operator-() const
		{
			return Complex(-Re, -Im);
		}

		Complex Complex::operator+() const
		{
			return *this;
		}

		Complex Complex::operator+(const Complex& complex) const
		{
			return Complex(Re + complex.Re, Im + complex.Im);
		}

		Complex Complex::operator-(const Complex& complex) const
		{
			return Complex(Re - complex.Re, Im - complex.Im);
		}

		Complex Complex::operator*(const Complex& complex) const
		{
			Complex res;
			res.Re = Re * complex.Re - Im * complex.Im;
			res.Im = Re * complex.Im + Im * complex.Re;
			return res;
		}

		Complex Complex::operator/(const Complex& complex) const
		{
			Complex res;
			res.Re = (Re * complex.Re + Im * complex.Im) / (complex.Re * complex.Re + complex.Im * complex.Im);
			res.Im = (complex.Re * Im - complex.Im * Re) / (complex.Re * complex.Re + complex.Im * complex.Im);
			return res;
		}

		Complex Complex::operator+=(const Complex& complex)
		{
			*this = *this + complex;
			return *this;
		}

		Complex Complex::operator-=(const Complex& complex)
		{
			*this = *this - complex;
			return *this;
		}

		Complex Complex::operator*=(const Complex& complex)
		{
			*this = *this * complex;
			return *this;
		}

		Complex Complex::operator/=(const Complex& complex)
		{
			*this = *this / complex;
			return *this;
		}

		std::ostream& operator<<(std::ostream& out, const Complex& complex)
		{
			if (complex == 0)
			{
				out << 0;
				return out;
			}

			if (!complex.isImaginary())
			{
				out << complex.Re;
			}
			if (complex.isComplex())
			{
				out << (complex.Im > 0.0 ? " + " : " - ");
			}
			else if (complex.isImaginary() && complex.Im < 0.0)
			{
				out << "-";
			}

			if (!complex.isReal())
			{
				if (std::abs(complex.Im - 1.0) < Complex::Eps || std::abs(complex.Im + 1.0) < Complex::Eps)
				{
					out << "i";
				}
				else
				{
					out << std::abs(complex.Im) << "i";
				}
			}
			return out;
		}

		double abs(const Complex& complex)
		{
			return std::sqrt(complex.Re * complex.Re + complex.Im * complex.Im);
		}
		
		double Re(const Complex& complex)
		{
			return complex.Re;
		}

		double Im(const Complex& complex)
		{
			return complex.Im;
		}

		Complex pow(const Complex& complex, unsigned power)
		{
			if (power == 0)
			{
				return Complex(1.0);
			}
			else if (power % 2 == 1)
			{
				return pow(complex, power - 1) * complex;
			}
			else 
			{
				Complex c = pow(complex, power / 2);
				return c * c;
			}
		}
		
		Complex conjugate(const Complex& complex)
		{
			return Complex(complex.Re, -complex.Im);
		}
	}
}