///
///	Author:			Christian Magnerfelt
///
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
		std::vector<Drawable> m_buffer;			// Buffer of drawables
		uint32_t m_bgID;						// Background id
		Vec2 m_bgPos;							// Background position
		uint32_t m_textAtlas;					// Atlas texture containing text characters
		std::vector<char> m_textBuffer;			// Buffer that contains all strings to render
		std::vector<uint32_t> m_textOffsets;	// Offsets to beginning och each string
		std::vector<Drawable> m_textDrawables;	// Buffer of text drawables
	};

	namespace renderManager
	{
		bool init(RenderManager & manager, int32_t width, int32_t height);
		bool finalize(RenderManager & manager);
		uint32_t imageFileToGLTexture(RenderManager & manager, const char * fileName);
		bool releaseTextures(RenderManager & manager);
		bool copyToBuffer(RenderManager & manager, uint8_t * IDs, Vec2 * positions, std::size_t count);
		bool setBackground(RenderManager & manager, uint32_t bgID, Vec2 position);
		bool renderDrawables(RenderManager & manager);
		bool copyTextsToBuffer(RenderManager & manager, char ** text, Vec2 * position, Vec2 * scale, uint32_t count);  
	};
};

#endif