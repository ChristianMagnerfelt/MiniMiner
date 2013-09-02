#include "math_types.hpp"

#include <cstring>

namespace MiniMiner {
	bool operator==(const Vec2 & a, const Vec2 & b)
	{
		return (memcmp(&a, &b,sizeof(Vec2)))?false:true;
	}
	bool operator==(const Rect & a, const Rect & b)
	{
		return (memcmp(&a, &b,sizeof(Rect)))?false:true;
	}
};