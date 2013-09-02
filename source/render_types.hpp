#ifndef RENDER_TYPES_HPP
#define RENDER_TYPES_HPP

#include "math_types.hpp"

#include <vector>
#include <cstdint>

namespace MiniMiner
{
	// Data containing texture id and the position where to render the texture
	struct Drawable
	{
		uint32_t id;
		Vec2 position;
	};
}
#endif