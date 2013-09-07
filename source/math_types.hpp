///
///	Author:			Christian Magnerfelt
///
#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

namespace MiniMiner
{
	struct Vec2
	{
		float x;
		float y;
	};

	struct Rect
	{
		Vec2 dim;	// Width and height
		Vec2 pos;	// Position x and y
	};

	bool operator==(const Vec2 & a, const Vec2 & b);
	bool operator==(const Rect & a, const Rect & b);
};

#endif