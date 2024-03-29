#include <render_manager.hpp>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <logger.hpp>

#include <cassert>

SDL_Window * g_window;
SDL_GLContext g_context;
int32_t g_width = 800;
int32_t g_height = 600;

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
	initializeSDL("RenderManagerTest", g_width, g_height, g_window, g_context);
}
void tearDown()
{
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
    SDL_Quit();
	Logger::LoggerManager::getInstance().finalize();
}
void testInit()
{
	MiniMiner::RenderManager manager;
	assert(MiniMiner::renderManager::init(manager, g_width, g_height));
}
void testFinalize()
{
	MiniMiner::RenderManager manager;
	assert(MiniMiner::renderManager::init(manager, g_width, g_height));
	assert(MiniMiner::renderManager::finalize(manager));
}
void testCopyToBuffer()
{
	MiniMiner::RenderManager manager;
	MiniMiner::renderManager::init(manager, g_width, g_height);

	uint8_t a;
	MiniMiner::Vec2 b;
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, &a, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, &b, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 100));

	std::vector<uint8_t> ids(100, 1);
	MiniMiner::Vec2 pos = {10, 10};
	std::vector<MiniMiner::Vec2> positions(100, pos);
	std::size_t count = 100;
	assert(MiniMiner::renderManager::copyToBuffer(manager, ids.data(), positions.data(), count));
	assert(manager.m_buffer.size() == positions.size());

	assert(MiniMiner::renderManager::finalize(manager));
}
void testImageFileToGLTextureAndRelease()
{
	MiniMiner::RenderManager manager;
	MiniMiner::renderManager::init(manager, g_width, g_height);

	auto invalid = MiniMiner::renderManager::imageFileToGLTexture(manager, "invalid");
	auto bg = MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/BackGround.jpg");
	auto blue = MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Blue.png");

	assert(invalid == 0);
	assert(bg > 0);	
	assert(blue > 0);
	assert(manager.m_IDs.size() == 2);
	assert(manager.m_texDimensions.size() == 2);

	MiniMiner::renderManager::releaseTextures(manager);
	assert(manager.m_IDs.size() == 0);
	assert(manager.m_texDimensions.size() == 0);

	assert(MiniMiner::renderManager::finalize(manager));
}
void testCopyToBuffer(MiniMiner::RenderManager & manager)
{
	std::vector<uint32_t> colors;
	colors.push_back(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Blue.png"));
	colors.push_back(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Green.png"));
	colors.push_back(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Purple.png"));
	colors.push_back(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Red.png"));
	colors.push_back(MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/Yellow.png"));

	std::vector<uint8_t> ids(100);
	for(std::size_t i = 0; i < ids.size(); ++i)
	{
		ids[i] = colors[rand() % colors.size()];
	}
	float x = 0.0f;
	float y = 0.0f;
	std::vector<MiniMiner::Vec2> positions(100);
	for(std::size_t i = 0; i < positions.size(); ++i)
	{
		positions[i].x = x;
		x = static_cast<int>(x + 50.0f) % 500;
		y = static_cast<int>((i) / 10.0f);
		positions[i].y = y * 50;
		
	}
	MiniMiner::renderManager::copyToBuffer(manager, ids.data(), positions.data(), ids.size());
}
void testSetBackground(MiniMiner::RenderManager & manager)
{
	// Load background and push to render manager buffer
	GLuint bgID = MiniMiner::renderManager::imageFileToGLTexture(manager, "assets/BackGround.jpg");
	MiniMiner::Vec2 bgPos;
	bgPos.x = 0.0f;
	bgPos.y = 0.0f;
	MiniMiner::renderManager::setBackground(manager, bgID, bgPos);
	assert(manager.m_bgID == bgID && manager.m_bgPos == bgPos);
}
void testRenderDrawables(MiniMiner::RenderManager & manager)
{
	MiniMiner::renderManager::renderDrawables(manager);
}
void testCopyTextsToBuffer(MiniMiner::RenderManager & manager)
{
	std::vector<char *> textStrings;
	textStrings.push_back("This is a long string");
	textStrings.push_back("!!%!%!#");
	textStrings.push_back("1234567890");
	textStrings.push_back("------------");
	textStrings.push_back("");

	MiniMiner::Vec2 scale;
	scale.x = 1;
	scale.y = 1;
	std::vector<MiniMiner::Vec2> scales(5, scale); 
	std::vector<MiniMiner::Vec2> positions;
	MiniMiner::Vec2 pos;
	pos.x = 100;
	pos.y = 100;
	positions.push_back(pos);
	pos.y = 200;
	positions.push_back(pos);
	pos.y = 300;
	positions.push_back(pos);
	pos.y = 400;
	positions.push_back(pos);
	pos.y = 500;
	positions.push_back(pos);

	assert(MiniMiner::renderManager::copyTextsToBuffer(manager, textStrings.data(), positions.data(), scales.data(), textStrings.size()));
	assert(manager.m_textDrawables.size() == 5);
}
void testReleaseTextures(MiniMiner::RenderManager & manager)
{
	MiniMiner::renderManager::releaseTextures(manager);
}

int main( int argc, char* argv[])
{
	setup();
	testInit();
	testFinalize();
	testImageFileToGLTextureAndRelease();
	testCopyToBuffer();

	MiniMiner::RenderManager manager;
	MiniMiner::renderManager::init(manager, g_width, g_height);
	testCopyToBuffer(manager);
	testSetBackground(manager);
	testCopyTextsToBuffer(manager);

	bool isRunning = true;
	SDL_Event event;
	while(isRunning)
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT : 
				isRunning = false;
		}
		testRenderDrawables(manager);
		SDL_GL_SwapWindow(g_window);
	}
	testReleaseTextures(manager);
	MiniMiner::renderManager::finalize(manager);
	tearDown();
	return 0;
}