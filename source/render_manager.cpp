///
///	Author:			Christian Magnerfelt
///
#include "render_manager.hpp"

#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>
#include <SDL_ttf.h>

#include "logger.hpp"

#include <cstdint>
#include <cstdio>
#include <algorithm>

#ifdef _WIN32
#define snprintf _snprintf
#endif

namespace MiniMiner {

	namespace renderManager {

		namespace internal {
			bool renderTexts(RenderManager & manager)
			{
				glPushMatrix();
				glScalef(0.5,0.5,0.5);

				int32_t count = manager.m_textOffsets.size();
				uint32_t * offsets = manager.m_textOffsets.data();
				const char * texts = manager.m_textBuffer.data();
				auto & textDrawables = manager.m_textDrawables;

				const char * text = nullptr;
				Vec2 pos;
				int idx = 0;
				float x = 0;
				float y = 0;
				int posX = 0;
				int posY = 0;
				int textLength = 0;
				int offset = 0;
				for(auto i = 0; i < count; ++i)
				{
					offset = offsets[i];
					text = &(texts[offset]);
					textLength = strlen(text);
					if(textLength == 0)
					{
						LOGGER_DEBUG("String is empty");
						continue;
					}
					pos = textDrawables[i].position;
					posX = pos.x;
					posY = pos.y;
					for(auto i = 0; i < textLength; ++i)
					{
						// Transform char offset to atlas offset
						idx = text[i] - 32;			

						//Convert offset to texture coordinates
						y = (idx / 10) * 0.1f;
						x = (idx % 10) * 0.1f;

						// Draw the glyph
						glBegin(GL_QUADS);
						glTexCoord2f(x, y + 0.02f); glVertex3f(posX, posY, 0);
						glTexCoord2f(x, y + 0.1f); glVertex3f(posX, posY + 32, 0);
						glTexCoord2f(x + 0.1f, y + 0.1f); glVertex3f(posX + 32, posY + 32, 0);
						glTexCoord2f(x + 0.1f, y); glVertex3f(posX + 32, posY, 0);
						glEnd();
						posX += 25;
					}
				}

				glPopMatrix();
				return true;
			}
			bool createGlyphsTexture(TTF_Font * font, uint32_t & textureID)
			{
				// Write text to surface
				SDL_Surface* temp = SDL_CreateRGBSurface(0, 320, 320, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
				SDL_SetSurfaceBlendMode(temp, SDL_BLENDMODE_BLEND);
				if(temp == nullptr)
				{
					LOGGER_ERROR("Could not create RGBSurface");
					return false;
				}
				SDL_Surface * glyphs[93];

				SDL_Color textColor = {255, 255, 255, 255};		//Default color is white
				SDL_Rect src;
				SDL_Rect dst;

				char c = 32;	// Don't inlude control characters

				// Render each glyph to a sdl surface
				for(auto i = 0; i < 93; ++i)
				{
					glyphs[i] = TTF_RenderGlyph_Blended(font, c, textColor);
					SDL_SetSurfaceBlendMode(glyphs[i], SDL_BLENDMODE_BLEND);
					++c;
				}

				// Combine all glyphs to a single texture atlas
				int x = 0;
				int y = 0;
				int minx = 0;
				int maxx = 0;
				int miny = 0;
				int maxy = 0; 
				int advance = 0;
				c = 32;
				for(auto i = 0; i < 93; ++i)
				{
					TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance);

					// We place all glyph in a matrix with 10 columns
					if((x + 1) % 11 == 0)
					{
						// We reached the end of a row, increase y offset by 1
						x = 0;
						++y;
					}
					src.x = 0;
					src.y = 0;
					src.w = glyphs[i]->w;
					src.h = glyphs[i]->h;
					dst.x = x * 32;
					dst.y = y * 32;
					dst.w = temp->w;
					dst.h = temp->h;
					SDL_BlitSurface(glyphs[i], &src, temp, &dst);
					++x;
					++c;
				}

				// Free all glyphs surfaces
				for(auto i = 0; i < 93; ++i)
				{
					SDL_FreeSurface(glyphs[i]);
				}

				// Generate a OpenGL texture from sdl surface
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);
 
				int Mode = GL_RGB;
				if(temp->format->BytesPerPixel == 4) {
					Mode = GL_RGBA;
				}
	
				auto texWidth = temp->w;
				auto texHeight = temp->h;
				glTexImage2D(GL_TEXTURE_2D, 0, Mode, texWidth, texHeight, 0, Mode, GL_UNSIGNED_BYTE, temp->pixels);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				SDL_FreeSurface(temp);
				return true;
			}
			bool initSDLttf(RenderManager & manager, uint32_t fontSize)
			{
				// Load a font
				TTF_Init();
				TTF_Font * font;
				const char * fontFilePath = "assets/FreeSans.ttf";
				font = TTF_OpenFont(fontFilePath, 24);
				if(font == nullptr)
				{
					LOGGER_ERROR("Could not open font file : %s", fontFilePath);
					return false;
				}

				// Generate glyph texture from font
				uint32_t textureID = 0; 
				if(!internal::createGlyphsTexture(font, textureID))
				{
					LOGGER_ERROR("Could not create glyphs texture");
					return false;
				}
				manager.m_textAtlas = textureID;
				TTF_CloseFont(font);
				return true;
			}
		};
		bool init(RenderManager & manager, int32_t width, int32_t height)
		{
			// Setup SDL ttf
			if(!internal::initSDLttf(manager, 24))
			{
				LOGGER_ERROR("Could not intialize tff");
				return false;
			}

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
		bool finalize(RenderManager & manager)
		{
			TTF_Quit();
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
			glDeleteTextures(1, &manager.m_bgID);
			glDeleteTextures(1, &manager.m_textAtlas);
			glDeleteTextures(manager.m_IDs.size(), manager.m_IDs.data());
			manager.m_IDs.clear();
			manager.m_texDimensions.clear();
			return true;
		}
		bool copyToBuffer(RenderManager & manager, uint8_t * IDs, Vec2 * positions, std::size_t count)
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
		bool renderDrawables(RenderManager & manager)
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
			

			// Render texts
			glBlendFunc(GL_ONE, GL_ZERO);
			glBindTexture(GL_TEXTURE_2D, manager.m_textAtlas);

			internal::renderTexts(manager);
			
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);	
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