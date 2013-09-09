#include "game_to_renderer.hpp"

namespace MiniMiner
{
	namespace gameToRenderer
	{
		void jewelsToDrawables(GameManager & gameManager, RenderManager & renderManager)
		{		
			renderManager::copyToBuffer(renderManager, gameManager.m_types.data(), gameManager.m_positions.data(), gameManager.m_types.size());
		}
		void textsToTextDrawables(GameManager & gameManager, RenderManager & renderManager)
		{
			renderManager::copyTextsToBuffer(
				renderManager, 
				gameManager.m_texts.data(), 
				gameManager.m_textPos.data(), 
				gameManager.m_textScale.data(), 
				gameManager.m_texts.size());
		}
	};
};