#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "render_types.hpp"
#include "math_types.hpp"

#include <cstdint>

namespace MiniMiner
{
	struct RenderManager
	{
		std::vector<uint32_t> m_IDs;			// Textures IDs
		std::vector<uint32_t> m_IDTmp;			// Texture ID temp vector
		std::vector<Vec2> m_texDimensions;		// Texture dimensions
		std::vector<Drawable> m_buffer;			// Render data buffer
		uint32_t m_bgID;						// Background id
		Vec2 m_bgPos;							// Background position
	};

	namespace renderManager
	{
		bool init(RenderManager & manager, int32_t width, int32_t height);
		uint32_t imageFileToGLTexture(RenderManager & manager, const char * fileName);
		bool releaseTextures(RenderManager & manager);
		bool copyToBuffer(RenderManager & manager, uint32_t * IDs, Vec2 * positions, std::size_t count);
		bool setBackground(RenderManager & manager, uint32_t bgID, Vec2 position);
		bool renderBuffer(RenderManager & manager);
	};
};

#endif