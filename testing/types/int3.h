#pragma once
#include "../global/Global.h"

class int3
{
public:
	i32 x;
	i32 y;
	i32 z;

	//c-tor: x, y, z initialized to 0
	constexpr int3() : x(0), y(0), z(0) {} // I think that x, y, z should be left uninitialized.
	//c-tor: x, y, z initialized to X, Y, Z
	constexpr int3(const i32 X, const i32 Y, const i32 Z) : x(X), y(Y), z(Z) {}

	int3 operator+(const int3 & i) const { return int3(x + i.x, y + i.y, z + i.z); }
	int3 operator+(const int & i) const { return int3(x + i, y + i, z + i); }

	int3 operator-(const int3 & i) const { return int3(x - i.x, y - i.y, z - i.z); }
	int3 operator-(const int & i) const { return int3(x - i, y - i, z - i); }

	int3 operator*(const float i) const {return int3(x * i, y * i, z);}
	int3 operator/(const float i) const {return int3(x / i, y / i, z);}


	int3& operator+=(const int3 & i)
	{
		x += i.x;
		y += i.y;
		z += i.z;

		return *this;
	}
	int3& operator+=(const int & i)
	{
		x += i;
		y += i;
		z += i;
		
		return *this;
	}
	int3& operator-=(const int3 & i)
	{
		x -= i.x;
		y -= i.y;
		z -= i.z;

		return *this;
	}
	int3& operator-=(const int & i)
	{
		x -= i;
		y -= i;
		z -= i;
		
		return *this;
	}

	float distance2DSQ(const int3& other) const
	{
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
	}
	float distance2DMH(const int3& other) const
	{
		return std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
	}

	std::string toString() const
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}
};