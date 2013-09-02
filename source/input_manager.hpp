///
///	Author:			Christian Magnerfelt
///
///	Description:	The Input manager handles player mouse clicks. The player can either click a grid which correnspond to a section 
///					containing a jewel or click a end button which reset the current round.
///
#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "math_types.hpp"

#include <cstdint>

namespace MiniMiner
{
	struct InputManager
	{
		Rect m_gridContainer;					// Container of the index grid
		uint8_t m_indexGrid [8][8];				// A grid that represent different sections where the player can click
		uint8_t m_selectedIndex;				// The index corresponing to the clicked section inside the grid
		Rect m_endRoundButtonContainer;			// Container of the end button
		uint8_t m_endRoundButtonClicked;		// True if player has clicked the end button otherwise false
	};

	namespace inputManager
	{
		bool init(InputManager & inputManager, Rect gridContainer, Rect endButtonContainer);
		void sendKeyDownEvent(InputManager & inputManager, float x, float y);
		void sendKeyUpEvent(InputManager & inputManager, float x, float y);
		bool retrieveSelectedIndex(InputManager & inputManager, uint8_t & index);
		bool endButtonClicked(InputManager & inputManager);
	};
};

#endif
