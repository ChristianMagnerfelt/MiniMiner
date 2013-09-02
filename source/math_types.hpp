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
};

#endif