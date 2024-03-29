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
		Rect m_gridContainer;						// Container of the grid	
		std::size_t m_gridSize;						// Number of elements in the grid
		std::vector<Vec2> m_positions;				// The positions of different elements, the first gridSize elements contains jewels
		std::vector<uint8_t> m_types;				// The id of different element, the id corresponds to a texture, the first gridSize contains jewels
		
		std::vector<Vec2> m_startPositions;			// The start position of each jewel
		std::vector<Vec2> m_speed;					// The speed of each jewel used for animation
		std::vector<uint8_t> m_matches;				// The 3+ matches

		std::vector<uint8_t> m_uniqueTypes;			// The unique types of jewels
		std::vector<uint8_t> m_matchCount;			// The number of matched sections per row
		std::vector<uint8_t> m_selectedIdx;			// A vector containing the selected indexes	
		std::vector<uint8_t> m_fireIDs;				// A vector containing fire texture ids
		float m_animationSpeed;						// The animation speed for every jewel
		float m_dropSpeed;							// The speed of jewels that drop from above
		float m_fireTimer;							// The fire timer
		float m_fireChangeDuration;					// How long it should take before the fire changes texture

		int32_t m_score;							// The player score
		float m_roundTimer;							// The timer of the round, when the timer reaches 0 the round ends
		uint8_t m_stage;							// The current stage of the game

		std::vector<Vec2> m_textPos;				// Text positions
		std::vector<Vec2> m_textScale;				// Text scale
		std::vector<char> m_textBuffer;				// A buffer that stores text
		std::vector<char*> m_texts;					// A vector string pointers
		bool m_doubleSwitch;						// Jewel switch animation flag
		bool m_switched;
		bool m_switchedBack;
	};
	namespace gameManager
	{
		bool init(
			GameManager & manager, 
			const Rect & gridContainer, 
			const uint8_t * types, 
			uint32_t numTypes, 
			uint8_t fire1ID,
			uint8_t fire2ID,
			uint8_t boardID);
		bool update(GameManager & gameManager, InputManager & inputManager, GameTimer & gameTimer);
		void animateFire(GameManager & manager, GameTimer & gameTimer);
		bool createBoard(GameManager & manager);
		bool checkConditions(GameManager & manager, InputManager & inputManager, GameTimer & gameTimer);
		bool checkJewelSelection(GameManager & manager, InputManager & inputManager);
		bool animateJewelSwitch(GameManager & manager, GameTimer & gameTimer);
		bool checkMatches(GameManager & manager);
		void calculateMatchCount(GameManager & manager);
		bool generateJewels(GameManager & manager);
		bool updateJewelPositions(GameManager & manager);
		void setJewelSpeed(GameManager & manager, GameTimer & gameTimer, float speed);
		bool moveJewel(GameManager & manager, GameTimer & gameTimer);
		void writeTextToBuffer(GameManager & manager);
	};
};

#endif