#include <cmath>
#include <iostream>
#include <vector>

namespace momo
{
	constexpr long double PI   = 3.14159265358979323846; // pi
	constexpr long double PI_2 = 1.57079632679489661923; // pi/2
	constexpr long double PI_4 = 0.785398163397448309616; // pi/4

	using ValueType = int;

	struct vector2
	{
		using T = ValueType;

		T x, y;

		inline vector2 operator+(const vector2& v) const { return { x + v.x, y + v.y }; }
		inline vector2 operator-(const vector2& v) const { return { x - v.x, y - v.y }; }
		inline vector2 operator*(T val) const { return { x * val, y * val }; }
		inline vector2 operator/(T val) const { return { x * val, y * val }; }
		inline const vector2& operator+=(const vector2& v) { x += v.x; y += v.y; return *this; }
		inline const vector2& operator-=(const vector2& v) { x -= v.x; y -= v.y; return *this; }
		inline const vector2& operator*=(T val) { x *= val; y *= val; return *this; }
		inline const vector2& operator/=(T val) { x /= val; y /= val; return *this; }
		inline T dot(const vector2& v) const { return x * v.x + y * v.y; }
		inline T cross(const vector2& v) const { return x * v.y + y * v.x; }
		inline T angle() const { auto angle = std::atan2(y, x); return (angle < 0 ? angle + 2 * PI : angle); }
		inline T length() const { return std::sqrt(x * x + y * y); }
	};

	inline std::ostream& operator<<(std::ostream& out, vector2& v)
	{
		out << '[' << v.x << ", " << v.y << ']';
		return out;
	}

	struct interval2
	{
		using T = ValueType;
		using Vec = vector2;

		union
		{
			struct
			{
				Vec first;
				Vec second;
			};
			struct
			{
				T x1, y1;
				T x2, y2;
			};
		};
	};

	inline std::ostream& operator<<(std::ostream& out, interval2& v)
	{
		out << '{' << v.first << ", " << v.second << '}';
		return out;
	}

	inline ValueType lengthSq(const vector2& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	inline ValueType length(const vector2& v)
	{
		return (ValueType)std::sqrt(lengthSq(v));
	}

	inline ValueType lengthSq(const vector2& v1, const vector2& v2)
	{
		return lengthSq(v2 - v1);
	}

	inline ValueType length(const vector2& v1, const vector2& v2)
	{
		return length(v2 - v1);
	}

	inline ValueType lengthSq(const interval2& v)
	{
		return lengthSq(v.first, v.second);
	}

	inline ValueType length(const interval2& v)
	{
		return length(v.first, v.second);
	}

	inline ValueType polarAngle(const vector2& v)
	{
		return v.angle();
	}

	inline ValueType dot(const vector2& v1, const vector2& v2)
	{
		return v1.dot(v2);
	}

	inline ValueType cross(const vector2& v1, const vector2& v2)
	{
		return v1.cross(v2);
	}

	inline ValueType angle(const vector2& v1, const vector2& v2)
	{
		return (ValueType)std::acos(dot(v1, v2) / (length(v1) * length(v2)));
	}

	inline void precision(std::streamsize digits = 12) { std::cout.precision(digits); }

	struct polygon
	{
		using Vertex = vector2;

		inline polygon() { }
		inline polygon(size_t size) : points(size) { }

		std::vector<Vertex> points;

		inline Vertex& operator[] (int index) { return points[(index + (int)points.size()) % points.size()]; }
		inline const Vertex& operator[] (int index) const { return points[(index + (int)points.size()) % points.size()]; }

		inline std::vector<Vertex>::iterator begin() { return points.begin(); }
		inline std::vector<Vertex>::const_iterator begin() const  { return points.end(); }
		inline std::vector<Vertex>::iterator end() { return points.end(); }
		inline std::vector<Vertex>::const_iterator end() const { return points.end(); }
	};

	inline ValueType trianglePerim(const vector2& p1, const vector2& p2, const vector2& p3)
	{
		return length(p1, p2) + length(p1, p3) + length(p2, p3);
	}

	inline ValueType trianglePerim(const polygon& poly)
	{
		return trianglePerim(poly[0], poly[1], poly[2]);
	}

	inline ValueType trianglePerimHalf(const vector2& p1, const vector2& p2, const vector2& p3)
	{
		return trianglePerim(p1, p2, p3) * 0.5;
	}

	inline ValueType trianglePerimHalf(const polygon& poly)
	{
		return trianglePerim(poly) * 0.5;
	}

	inline ValueType triangleArea(const vector2& p1, const vector2& p2, const vector2& p3)
	{
		auto a = length(p1 - p2), b = length(p1 - p3), c = length(p2 - p3);
		auto p = (a + b + c) * 0.5;
		return std::sqrt(p * (p - a) * (p - b) * (p - c));
	}

	inline ValueType triangleArea(const polygon& poly)
	{
		return triangleArea(poly[0], poly[1], poly[2]);
	}

	struct line2
	{
		using T = ValueType;
		T A, B, C;

		inline vector2 norm() const { return { A, B }; }
		inline vector2 pointX(T x) { return { x, (A * x + C) / -B }; }
		inline vector2 pointY(T y) { return { (B * y + C) / -A, y }; }
		inline line2 translated(T dist) { return { A, B, -1 * dist * (T)std::sqrt(A * A + B * B) + C }; }
		inline bool is_parallel(const line2& line)
		{
			return
				(A == 0 && line.A == 0) ||
				(B == 0 && line.B == 0) ||
				(A / line.A == B / line.B);

		}
	};

	inline line2 toLine(const interval2& v) { return { v.y1 - v.y2, v.x2 - v.x1, v.x1 * v.y2 - v.x2 * v.y1 }; }

	inline line2 toLineNorm(const vector2& norm, const vector2& p = { 0, 0 })
	{
		return { norm.x, norm.y, (norm.x * p.x + norm.y * p.y) * -1 };
	}
}

using namespace std;
using namespace momo;

int main()
{
	vector<vector<int>>Gr;
	vector<momo::line2> lines;
	lines.reserve(3000);
	Gr.reserve(3000);

	int T;
	cin >> T;

	for (int t = 0; t < T; t++)
	{
		lines.clear();
		Gr.clear();
		int n;
		cin >> n;
		for (int i = 0; i < n; i++)
		{
			momo::line2 l;
			cin >> l.A >> l.B >> l.C;
			lines.push_back(l);
			Gr.emplace_back();
			int idx = lines.size() - 1;
			for (int i = 0; i < idx; i++)
			{
				auto& line = lines[i];
				if (line.is_parallel(l) || line.pointX(0).y != line.pointY(0).y)
				{
					Gr[i].push_back(idx);
					Gr[idx].push_back(i);
				}
			}
		}
	}
}