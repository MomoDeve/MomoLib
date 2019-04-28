#pragma once
#include <ostream>
#include <cmath>

namespace momo
{
	namespace complex
	{
		class Complex
		{
			double Re, Im;
		public:
			static double Eps;
			static const Complex i;

			Complex();
			Complex(double Re);
			Complex(double Re, double Im);

			bool isReal() const;
			bool isImaginary() const;
			bool isComplex() const;

			bool operator==(const Complex& complex) const;
			bool operator!=(const Complex& complex) const;
			Complex operator-() const;
			Complex operator+() const;

			Complex operator+(const Complex& complex) const;
			Complex operator-(const Complex& complex) const;
			Complex operator*(const Complex& complex) const;
			Complex operator/(const Complex& complex) const;

			Complex operator+=(const Complex& complex);
			Complex operator-=(const Complex& complex);
			Complex operator*=(const Complex& complex);
			Complex operator/=(const Complex& complex);

			friend std::ostream& operator<<(std::ostream& out, const Complex& complex);
			friend double abs(const Complex& complex);
			friend double Re(const Complex& complex);
			friend double Im(const Complex& complex);
			friend Complex pow(const Complex& complex, unsigned power);
			friend Complex conjugate(const Complex& complex);
		};
	}
}
