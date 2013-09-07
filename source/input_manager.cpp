#include "input_manager.hpp"

namespace MiniMiner
{

	namespace inputManager 
	{
		bool init(
			InputManager & inputManager, 
			const Rect & gridContainer, 
			const Rect & endButtonContainer)
		{
			inputManager.m_gridContainer = gridContainer;
			inputManager.m_endRoundButtonContainer = endButtonContainer;
			auto idx = 0;
			for(auto i = 0; i < 8; ++i)
			{
				for(auto j = 0; j < 8; ++j, ++idx)
				{
					inputManager.m_indexGrid[j][i] = idx;
				}
			}
			inputManager.m_endRoundButtonClicked = false;
			inputManager.m_selectedIndex = 0;
			return true;
		}
		void sendKeyDownEvent(InputManager & inputManager, const float x, const float y)
		{
			float gridX = inputManager.m_gridContainer.pos.x;
			float gridY = inputManager.m_gridContainer.pos.y;
			float gridWidth = inputManager.m_gridContainer.dim.x;
			float gridHeight = inputManager.m_gridContainer.dim.y;
			if(x > gridX && x <= (gridX + gridWidth) && y > gridY && y <= (gridY + gridHeight))
			{
				float startX = gridX;
				gridWidth /= 8;
				gridHeight /= 8;
				for(auto i = 0; i < 8; ++i)
				{
					for(auto j = 0; j < 8; ++j)
					{
						if(x > gridX && x <= (gridX + gridWidth) && y > gridY && y <= (gridY + gridHeight))
						{
							inputManager.m_gridClicked = true;
							inputManager.m_selectedIndex = i * 8 + j;
							return;
						}
						gridX += gridWidth;
					}
					gridX = startX;
					gridY += gridHeight;
				}
			}
			else
			{
				inputManager.m_gridClicked = false;
			}
		}
		void sendKeyUpEvent(InputManager & inputManager, const float x, const float y)
		{

		}
		bool retrieveSelectedIndex(InputManager & inputManager, uint8_t & index)
		{
			if(inputManager.m_gridClicked)
			{
				inputManager.m_gridClicked = false;
				index = inputManager.m_selectedIndex;
				return true;
			}
			return false;
		}
	};
};