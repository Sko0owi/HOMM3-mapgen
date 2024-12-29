
#include "../global/global.h"

class float3
{
public:
	f32 x;
	f32 y;
	i32 z;

	//c-tor: x, y, z initialized to 0
	constexpr float3() : x(0), y(0), z(0) {} // I think that x, y, z should be left uninitialized.
	//c-tor: x, y, z initialized to X, Y, Z
	constexpr float3(const f32 X, const f32 Y, const f32 Z) : x(X), y(Y), z(Z) {}


};