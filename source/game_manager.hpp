///
///	Author:			Christian Magnerfelt
///
#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "game_timer.hpp"
#include "math_types.hpp"
#include "game_types.hpp"
#include "input_manager.hpp"

#include <cstdint>
#include <vector>

namespace MiniMiner
{
	struct GameManager
	{
		std::vector<uint8_t> m_uniqueTypes;			// The unique types of jewels
		std::vector<uint8_t> m_types;				// The type of the jewel at each grid section
		std::vector<Vec2> m_startPositions;			// The start position of each jewel
		std::vector<Vec2> m_positions;				// The current position of each jewel
		std::vector<Vec2> m_speed;					// The speed of each jewel used for animation
		std::vector<uint8_t> m_matches;				// The 3+ matches
		std::vector<uint8_t> m_matchCount;			// The number of matched sections per row
		std::vector<Vec2> m_targets;				// The target destination of each jewel used for animation
		std::vector<uint8_t> m_selectedIdx;			// A vector containg the selected indexes
		float animationSpeed;						// The animation speed for every jewel
		float dropSpeed;							// The speed of jewels that drop from above
		Rect m_gridContainer;						// Container of the grid	
		uint8_t m_stage;							// The current stage of the game
		float m_roundStart;
		float m_roundEnd;
	};
	namespace gameManager
	{
		bool init(GameManager & manager, const Rect & gridContainer, const uint8_t * types, uint32_t numTypes);
		bool update(GameManager & gameManager, InputManager & inputManager, GameTimer & gameTimer);
		bool createBoard(GameManager & manager);
		bool checkConditions(GameManager & manager, InputManager & inputManager);
		bool checkJewelSelection(GameManager & manager, InputManager & inputManager);
		bool animateJewelSwitch(GameManager & manager, GameTimer & gameTimer);
		bool checkMatches(GameManager & manager);
		bool generateJewels(GameManager & manager);
		bool updateJewelPositions(GameManager & manager);
		void setJewelSpeed(GameManager & manager, GameTimer & gameTimer, float speed);
		bool moveJewel(GameManager & manager, GameTimer & gameTimer);
	};
};

#endif