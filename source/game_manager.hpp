///
///	Author:			Christian Magnerfelt
///
#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

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
		std::vector<Vec2> m_positions;				// The current position of each jewel
		std::vector<Vec2> m_speed;					// The speed of each jewel used for animation
		std::vector<uint8_t> m_matches;				// The 3+ matches
		std::vector<Vec2> m_targets;				// The target destination of each jewel used for animation
		uint8_t m_stage;							// The current stage of the game
		float m_roundStart;
		float m_roundEnd;
	};
	namespace gameManager
	{
		bool init(GameManager & manager, const Rect & gridContainer, const uint8_t * types, uint32_t numTypes);
		bool update(GameManager & gameManager, InputManager & inputManager);
		bool createBoard(GameManager & manager);
		bool checkConditions(GameManager & manager, InputManager & inputManager);
		bool checkJewelSelection(GameManager & manager);
		bool animateJewelSwitch(GameManager & manager);
		bool checkMatches(GameManager & manager);
		bool generateJewels(GameManager & manager);
		bool moveJewels(GameManager & manager);
	};
};

#endif