
#include "../global/Global.h"

class float3
{
public:
	f32 x;
	f32 y;
	i32 z;

	constexpr float3() : x(0), y(0), z(0) {} // I think that x, y, z should be left uninitialized.
	constexpr float3(const f32 X, const f32 Y, const f32 Z) : x(X), y(Y), z(Z) {}

	float3 operator+(const float3 & i) const { return float3(x + i.x, y + i.y, z + i.z); }
	float3 operator+(const float & i) const { return float3(x + i, y + i, z + i); }

	float3 operator-(const float3 & i) const { return float3(x - i.x, y - i.y, z - i.z); }
	float3 operator-(const float & i) const { return float3(x - i, y - i, z - i); }

	float3 operator*(const float i) const {return float3(x * i, y * i, z);}
	float3 operator/(const float i) const {return float3(x / i, y / i, z);}


	float3& operator+=(const float3 & i)
	{
		x += i.x;
		y += i.y;
		z += i.z;

		return *this;
	}
	float3& operator+=(const float & i)
	{
		x += i;
		y += i;
		z += i;
		
		return *this;
	}
	float3& operator-=(const float3 & i)
	{
		x -= i.x;
		y -= i.y;
		z -= i.z;

		return *this;
	}
	float3& operator-=(const float & i)
	{
		x -= i;
		y -= i;
		z -= i;
		
		return *this;
	}

	float distance2DSQ(const float3& other) const
	{
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
	}

	float distance2DMH(const float3& other) const
	{
		return std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
	}

	
};