#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "render_types.hpp"
#include "math_types.hpp"

#include <cstdint>

namespace MiniMiner
{
	namespace renderManager
	{
		uint32_t imageFileToGLTexture(RenderManager & manager, const char * fileName);
		bool releaseTextures(RenderManager & manager);
		bool copyToBuffer(RenderManager & manager, uint32_t * IDs, Vec2 * positions, std::size_t count);
		bool renderBuffer(RenderManager & manager);
	};
};

#endif