#include <render_manager.hpp>

#include <SDL.h>
#include <SDL_image.h>

#include <logger.hpp>

#include <cassert>

SDL_Window * g_window;
SDL_GLContext g_context;

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
void setup()
{
	Logger::LoggerManager::getInstance().init("debug.log");
	initializeSDL("RenderManagerTest", 800, 600, g_window, g_context);
}
void tearDown()
{
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
    SDL_Quit();
	Logger::LoggerManager::getInstance().finalize();
}
void testCopyToBuffer()
{
	MiniMiner::RenderManager manager;

	uint32_t a;
	MiniMiner::Vec2 b;
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, &a, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, &b, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 100));

	std::vector<uint32_t> ids(100, 1);
	MiniMiner::Vec2 pos = {10, 10};
	std::vector<MiniMiner::Vec2> positions(100, pos);
	std::size_t count = 100;
	assert(MiniMiner::renderManager::copyToBuffer(manager, ids.data(), positions.data(), count));
}
void testRenderBuffer()
{

}
void testReleaseTextures()
{

}
void testImageFileToGLTexture()
{
	MiniMiner::RenderManager manager;
	assert(MiniMiner::renderManager::imageFileToGLTexture(manager, "invalid") == 0);
	assert(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/BackGround.jpg") > 0);
}
int main( int argc, char* argv[])
{
	setup();
	testCopyToBuffer();
	testRenderBuffer();
	testReleaseTextures();
	testImageFileToGLTexture();
	tearDown();
	return 0;
}