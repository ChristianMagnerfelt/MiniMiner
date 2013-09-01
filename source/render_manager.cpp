#include "render_manager.hpp"

#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>

#include "logger.hpp"

#include <cstdint>
#include <algorithm>

namespace MiniMiner {

	namespace renderManager {
		uint32_t imageFileToGLTexture(RenderManager & manager, const char * fileName)
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
			manager.m_IDs.push_back(id);
			Vec2 dim;
			dim.x = surface->w;
			dim.y = surface->h;
			manager.m_texDimensions.push_back(dim);
			return id;
		}
		bool releaseTextures(RenderManager & manager)
		{
			glDeleteTextures(manager.m_IDs.size(), manager.m_IDs.data());
			manager.m_IDs.clear();
			manager.m_texDimensions.clear();
			return true;
		}
		bool copyToBuffer(RenderManager & manager, uint32_t * IDs, Vec2 * positions, std::size_t count)
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
		bool renderBuffer(RenderManager & manager)
		{
			auto & buffer = manager.m_buffer;
			if(buffer.size() == 0)
				return false;

			// Sort texture ids so we don't have to bind texture so often
			std::sort(buffer.begin(), buffer.end(), [](BufferData & dataA, BufferData & dataB)
			{
				if(dataA.id > dataB.id) return false;
				else return true;
			});

			glEnable(GL_TEXTURE_2D);
			uint32_t current = 0;
			uint32_t id;
			uint32_t idOffset;
			Vec2 pos;
			float width = 0;
			float height = 0;
			for(std::size_t i = 0; i < buffer.size(); ++i)
			{
				id = buffer[i].id;
				// Bind new texture if tex id differs
				if(id != current)
				{
					glBindTexture(GL_TEXTURE_2D, id);
					auto it = std::find(manager.m_IDs.begin(), manager.m_IDs.end(), id);
					idOffset = std::distance(manager.m_IDs.begin(), it);
					width = manager.m_texDimensions[idOffset].x;
					height = manager.m_texDimensions[idOffset].y;
					current = id;
				}

				pos = buffer[i].position;
				float x = pos.x;
				float y = pos.y;


				glBegin(GL_QUADS);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, 0.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, 0.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, 0.0f);
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);
			return true;
		}
	};
};