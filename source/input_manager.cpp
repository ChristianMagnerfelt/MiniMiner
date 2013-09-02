#include "input_manager.hpp"

namespace MiniMiner {

	namespace inputManager {
		bool init(InputManager & inputManager, const Rect & gridContainer, const Rect & endButtonContainer)
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
	};
};