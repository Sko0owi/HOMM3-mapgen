
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

	float distance2DSQ(const int3& other) const
	{
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
	}
	float distance2DMH(const int3& other) const
	{
		return std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
	}
};