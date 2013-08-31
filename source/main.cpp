///
///	Author: Christian Magnerfelt
///	
///	Description: Mini Miner is a simple tile-matching puzzle game similar to Midas Miner
///
///
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "logger.hpp"

#include <cstdint>

bool initializeSDL(
	const char * windowTitle, 
	int32_t screenWidth, 
	int32_t screenHeight, 
	SDL_Window * & window,
	SDL_GLContext & context);
bool initializeOpenGL(int32_t width, int32_t height);
void render(SDL_Window * window);
GLuint imageFileToGLTex(const char * fileName);
void drawGLTexture(GLuint textureID, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

/// Main entry point
int main( int argc, char* argv[] )
{
	Logger::LoggerManager::getInstance().init("debug.log");

	const char * windowTitle = "MiniMiner";
	SDL_Window * window;
	SDL_GLContext context;
	int32_t screenWidth = 800;
	int32_t screenHeight = 600;
	bool isRunning = true;
	
	if(!initializeSDL(windowTitle, screenWidth, screenHeight, window, context))
		return 1;

	if(!initializeOpenGL(screenWidth, screenHeight))
		return 1;

	int32_t flags=IMG_INIT_JPG|IMG_INIT_PNG;
	IMG_Init(flags);

	GLuint bgId = imageFileToGLTex("assets/BackGround.jpg");

	// Enter main loop
	SDL_Event event;
	while(isRunning)
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				isRunning = false;
		}
		drawGLTexture(bgId, -1, -1, 2, 2);
		render(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
    SDL_Quit();

	Logger::LoggerManager::getInstance().finalize();
	return 0;
}
bool initializeSDL(
	const char * windowTitle, 
	int32_t screenWidth, 
	int32_t screenHeight, 
	SDL_Window * & window,
	SDL_GLContext & context)
{
	SDL_Init(SDL_INIT_VIDEO);

	// Create an application window with the following settings:
	window = SDL_CreateWindow( 
		windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_OPENGL
	);

	// Set framebuffer pizel format and attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);

	if(window == nullptr)
	{
		LOGGER_ERROR("Window creation failed.");
		return false;
	}
	// Create open gl context
	context = SDL_GL_CreateContext(window);

	// This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	return true;
}
bool initializeOpenGL(int32_t width, int32_t height)
{
	glClearColor ( 0.0, 0.0, 0.0, 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	return true;
}
void render(SDL_Window * window)
{
	SDL_GL_SwapWindow(window);
}
GLuint imageFileToGLTex(const char * fileName)
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
void drawGLTexture(GLuint textureID, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE);
}