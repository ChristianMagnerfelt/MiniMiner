#include "render_manager.hpp"

#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>

#include "logger.hpp"

#include <cstdint>

namespace MiniMiner {

	namespace renderManager {
		uint32_t imageFileToGLTexture(RenderManager manager, const char * fileName)
		{
			GLuint id = 0;
			auto surface = IMG_Load(fileName);
	
			if(surface == nullptr)
			{
				LOGGER_ERROR("%s", IMG_GetError());
				LOGGER_ERROR("Could not load image : %s", fileName);
				return 0;
			}

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			GLint mode = GL_RGB;
			if(surface->format->BytesPerPixel == 4)
				mode = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			SDL_FreeSurface(surface);
			return id;
		}
		bool releaseTextures(RenderManager manager)
		{
			
			return true;
		}
		bool copyToBuffer(RenderManager manager, uint32_t * IDs, Vec2 * positions, std::size_t count)
		{
			if(IDs == nullptr || positions == nullptr || count == 0)
				return false;

			auto & buffer = manager.m_buffer;
			if(buffer.size() < count)
				buffer.resize(count);

			for(std::size_t i = 0; i < count; ++i)
			{
				auto & data = buffer[i];
				data.id = IDs[i];
				data.position = positions[i];
			}
			return true;
		}
		bool renderBuffer(RenderManager manager)
		{

			return true;
		}
	};
};