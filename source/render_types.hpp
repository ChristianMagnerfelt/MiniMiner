#ifndef RENDER_TYPES_HPP
#define RENDER_TYPES_HPP

#include "math_types.hpp"

#include <vector>
#include <cstdint>

namespace MiniMiner
{
	// Data containing texture id and the position where to render the texture
	struct BufferData
	{
		uint32_t id;
		Vec2 position;
	};
	// Contains a ids of all textures in memory as well as a render buffer used for rendering textures
	struct RenderManager
	{
		std::vector<uint32_t> m_IDs;
		std::vector<BufferData> m_buffer;
	};
}
#endif