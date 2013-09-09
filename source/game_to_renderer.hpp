///
///	Author:	Christian Magnerfelt
///
///	Description:	A collection of function for copying game data to a render manager for rendering
///
///
#ifndef GAME_TO_RENDERER_HPP
#define GAME_TO_RENDERER_HPP
#include "game_manager.hpp"
#include "render_manager.hpp"

namespace MiniMiner
{
	namespace gameToRenderer
	{
		void jewelsToDrawables(GameManager & gameManager, RenderManager & renderManager);
		void textsToTextDrawables(GameManager & gameManager, RenderManager & renderManager);
	};
}
#endif