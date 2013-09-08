#include "game_to_renderer.hpp"

namespace MiniMiner
{
	namespace gameToRenderer
	{
		void jewelsToDrawables(GameManager & gameManager, RenderManager & renderManager)
		{		
			renderManager::copyToBuffer(renderManager, gameManager.m_types.data(), gameManager.m_positions.data(), gameManager.m_types.size());
		}
	};
};