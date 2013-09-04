#include "render_manager.hpp"

#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>

#include "logger.hpp"

#include <cstdint>
#include <cstdio>
#include <algorithm>

#ifdef _WIN32
#define snprintf _snprintf
#endif

namespace MiniMiner {

	namespace renderManager {
		bool init(RenderManager & manager, int32_t width, int32_t height)
		{
			// Allocate buffer storage
			manager.m_textOffsets.resize(16);
			manager.m_textDrawables.resize(16);
			manager.m_textBuffer.resize(1024);

			glClearColor ( 0.0, 0.0, 0.0, 1.0 );
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, width, height, 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			return true;
		}
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
			dim.x = static_cast<float>(surface->w);
			dim.y = static_cast<float>(surface->h);
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
		bool setBackground(RenderManager & manager, uint32_t bgID, Vec2 position)
		{
			manager.m_bgID = bgID;
			manager.m_bgPos = position;
			return true;
		}
		bool renderBuffer(RenderManager & manager)
		{
			uint32_t id;
			uint32_t idOffset;
			Vec2 pos;
			float x = 0.0f;
			float y = 0.0f;
			float width = 0.0f;
			float height = 0.0f;

			// Render background first
			id = manager.m_bgID;
			pos = manager.m_bgPos;
			x = pos.x;
			y = pos.y;
			{
				auto it = std::find(manager.m_IDs.begin(), manager.m_IDs.end(), id);
				idOffset = std::distance(manager.m_IDs.begin(), it);
				width = manager.m_texDimensions[idOffset].x;
				height = manager.m_texDimensions[idOffset].y;
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, id);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, 0.0f);
			glEnd();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Render everything in data buffer
			auto & buffer = manager.m_buffer;
			if(buffer.size() == 0)
				return false;

			// Sort texture ids so we don't have to bind texture so often
			std::sort(buffer.begin(), buffer.end(), [](Drawable & dataA, Drawable & dataB)
			{
				if(dataA.id > dataB.id) return false;
				else return true;
			});

			uint32_t current = 0;
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
				x = pos.x;
				y = pos.y;

				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, 0.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, 0.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, 0.0f);
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			return true;
		}
		bool copyTextsToBuffer(RenderManager & manager, char ** text, Vec2 * position, Vec2 * scale, uint32_t textCount)
		{
			// Copy text drawable data to text drawable buffer
			manager.m_textDrawables.resize(textCount);
			uint32_t atlasID = manager.m_textAtlas;
			Drawable * drawables = manager.m_textDrawables.data();
			Drawable drawable;
			for(uint32_t i = 0; i < textCount; ++i)
			{			
				drawable.id = atlasID;
				drawable.position = position[i];
				drawable.scale = scale[i];
				drawables[i] = drawable;
			}

			// Copy text to text buffer and store offsets for string
			manager.m_textOffsets.resize(textCount);
			uint32_t * textOffsets = manager.m_textOffsets.data();
			uint32_t maxSize = manager.m_textBuffer.size();
			uint32_t offset = 0;
			uint32_t sizeUsed = 0;
			
			char * buffer = manager.m_textBuffer.data();

			for(uint32_t i = 0; i < textCount; ++i)
			{
				sizeUsed = std::min(offset, maxSize);
				textOffsets[i] = offset;
				offset += snprintf(buffer + offset, maxSize - sizeUsed, "%s", text[i]);
				buffer[offset++] = '\0';
			}		
			return true;
		}
	};
};