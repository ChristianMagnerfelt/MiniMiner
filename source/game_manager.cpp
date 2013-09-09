///
///	Author:			Christian Magnerfelt
///
#include "game_manager.hpp"

#include <cmath>

#ifdef _WIN32
#define copysign _copysign
#define snprintf _snprintf
#endif

namespace MiniMiner
{
	namespace gameManager
	{
		namespace internal 
		{
			void resetMatches(GameManager & manager)
			{
				auto & matches = manager.m_matches;
				for(int32_t i = 0; i < matches.size(); ++i)
				{
					matches[i] = 0;
				}
			}
			bool hasSpeed(GameManager & manager)
			{
				auto & speed = manager.m_speed;
				for(int32_t i = 0; i < speed.size(); ++i)
				{
					if(speed[i].x > 0) return true;
					if(speed[i].y > 0) return true;
				}
				return false;
			}
			bool hasMatches(GameManager & manager)
			{
				auto & matches = manager.m_matches;
				for(int32_t i = 0; i < matches.size(); ++i)
				{
					if(matches[i] == 1) return true;
				}
				return false;
			}
			void checkVerticalMatches(GameManager & manager, const uint8_t * types, uint8_t * matches)
			{
				uint8_t prev = -1;
				uint8_t current = 0;
				uint8_t count = 1;
				uint8_t offset;

				for(uint8_t i = 0; i < 8; ++i)
				{	
					for(uint8_t j = 0; j < 8; ++j)
					{
						offset = i * 8 + j;
						current = types[offset];
						// Type has changed, reset count and check if we have a match
						if(current != prev)
						{			
							if(count >= 3)
							{
								for(uint8_t k = offset - count; k < offset; ++k)
								{
									matches[k] |= 1;
								}
							}
							count = 1;
						}
						else
						{
							++count;
						}
						prev = current;
					}
					// If the the count is equal or greater than 3 when we exist row,
					// it means that we have a match up until the end of the row
					if(count >= 3)
					{
						for(uint8_t k = offset - count; k < offset; ++k)
						{
							matches[k] |= 1;
						}
					}
					count = 1;
					prev = - 1;
				}
			}
			void checkHorizontalMatches(GameManager & manager, const uint8_t * types, uint8_t * matches)
			{
				uint8_t prev = -1;
				uint8_t current = 0;
				uint8_t count = 1;
				uint8_t offset;

				for(uint8_t i = 0; i < 8; ++i)
				{	
					for(uint8_t j = 0; j < 8; ++j)
					{
						offset = j * 8 + i;
						current = types[offset];
						// Type has changed, reset count and check if we have a match
						if(current != prev)
						{			
							if(count >= 3)
							{
								for(uint8_t k = offset - count * 8; k < offset; k += 8)
								{
									matches[k] |= 1;
								}
							}
							count = 1;
						}
						else
						{
							++count;
						}
						prev = current;
					}
					// If the the count is equal or greater than 3 when we exist row,
					// it means that we have a match up until the end of the row
					if(count >= 3)
					{
						for(uint8_t k = offset - (count - 1) * 8; k <= offset; k += 8)
						{
							matches[k] |= 1;
						}
					}
					count = 1;
					prev = - 1;
				}
			}
		};
		bool init(
			GameManager & manager, 
			const Rect & gridContainer, 
			const uint8_t * types, 
			uint32_t numTypes, 
			uint8_t fire1Id, 
			uint8_t fire2Id, 
			uint8_t boardId)
		{
			manager.m_gridContainer = gridContainer;
			manager.m_gridSize = 64;
			manager.m_positions.resize(manager.m_gridSize + 2);	// jewels + fire + board
			manager.m_types.resize(manager.m_gridSize + 2, 0);
			manager.m_matches.resize(manager.m_gridSize, 0);
			manager.m_matchCount.resize(8, 0);

			Vec2 pos;
			pos.x = 0;
			pos.y = 0;
			manager.m_speed.resize(manager.m_gridSize, pos);

			manager.m_startPositions.resize(manager.m_gridSize);

			// Save the list of unique jewel types
			manager.m_uniqueTypes = std::vector<uint8_t>(types, types + numTypes);

			// Initialize start positions
			Vec2 gridOffset = manager.m_gridContainer.pos;	
			float width = manager.m_gridContainer.dim.x/8;
			float height = manager.m_gridContainer.dim.y/8;
			for(auto i = 0; i < 8; ++i)
			{
				for(auto j = 0; j < 8; ++j)
				{
					pos.x = gridOffset.x + i * width;
					pos.y = gridOffset.y + j * height;
					manager.m_positions[i * 8 + j] = pos;
					manager.m_startPositions[i * 8 + j] = pos;
				}
			}

			pos.x = 10;
			pos.y = 10;
			manager.m_positions[64] = pos;
			pos.x = 250;
			pos.y = 530;
			manager.m_positions[65] = pos;
			manager.m_types[64] = boardId;
			manager.m_types[65] = fire1Id;

			manager.m_fireIDs.push_back(fire1Id);
			manager.m_fireIDs.push_back(fire2Id);

			manager.m_fireTimer = 0.0f;
			manager.m_fireChangeDuration = 1.0f;
			manager.m_roundTimer = 65.0f;
			manager.m_score = 0;
			manager.m_stage = 0;
			manager.m_animationSpeed = 70.0f;
			manager.m_dropSpeed = 90.0f;

			manager.m_texts.resize(10, 0);
			manager.m_textBuffer.resize(512, 0);

			pos.x = 100;
			pos.y = 100;
			manager.m_textPos.push_back(pos);
			pos.x = 100;
			pos.y = 200;
			manager.m_textPos.push_back(pos);
			Vec2 scale;
			scale.x = 1;
			scale.y = 1;
			manager.m_textScale.push_back(scale);
			manager.m_textScale.push_back(scale);
			return true;
		}
		bool update(GameManager & gameManager, InputManager & inputManager, GameTimer & gameTimer)
		{
			gameManager::animateFire(gameManager, gameTimer);

			// Check timer and if end button is pressed
			if(!gameManager::checkConditions(gameManager, inputManager, gameTimer))
			{
				// End round
				gameManager::createBoard(gameManager);
				return true;
			}

			// Do some cool board randomizations
			if(gameManager.m_roundTimer > 60.0f)
			{
				gameManager::createBoard(gameManager);
				return true;
			}

			// Switch depending on game stage
			auto stage = gameManager.m_stage;
			switch (stage)
			{
				case 0:
					if(!gameManager::checkJewelSelection(gameManager, inputManager))
					{
						// We do not have a selection yet
						return true;
					}
					gameManager::animateJewelSwitch(gameManager, gameTimer);
				case 1:
					// Move the jewels which are switching place
					gameManager.m_stage = 1;
					gameManager::setJewelSpeed(gameManager, gameTimer, gameManager.m_animationSpeed);
					gameManager::moveJewel(gameManager, gameTimer);

					// When both jewels have stopped we continue to the next stage
					if(internal::hasSpeed(gameManager))
					{
						// Jewels are still moving
						return true;
					}
				case 2:
					// We have some 3 matches, remove the old and generate some new at the top
					gameManager::calculateMatchCount(gameManager);
					gameManager::generateJewels(gameManager);
					gameManager::updateJewelPositions(gameManager);
					gameManager::setJewelSpeed(gameManager, gameTimer, gameManager.m_dropSpeed);
				case 3:
					gameManager.m_stage = 3;
					// Move the jewels until they stop
					gameManager::moveJewel(gameManager, gameTimer);
					gameManager::setJewelSpeed(gameManager, gameTimer, gameManager.m_dropSpeed);
					if(internal::hasSpeed(gameManager))
						return true;
				case 5:
					// Jewels have finished moving, check if we got new matches and return to stage 2 if so 
					internal::resetMatches(gameManager);
					gameManager::checkMatches(gameManager);
					if(internal::hasMatches(gameManager))
					{
						gameManager.m_stage = 2;
						return true;
					}
					// We are finsihed, set up for the next switch
					gameManager.m_stage = 0;
			}
			return true;
		}
		/// Animates the fire
		void animateFire(GameManager & manager, GameTimer & gameTimer)
		{
			manager.m_fireTimer += gameTimer.getSmoothDeltaTime();
			if(manager.m_fireTimer >= manager.m_fireChangeDuration)
			{
				manager.m_fireTimer = 0;
				manager.m_types[65] = manager.m_fireIDs[0];
				std::swap(manager.m_fireIDs[0], manager.m_fireIDs[1]);
			}
		}
		/// Creates a random board from a unique set of jewels
		bool createBoard(GameManager & manager)
		{
			auto & types = manager.m_types;
			auto & uniques = manager.m_uniqueTypes;
			uint32_t size = manager.m_gridSize;
			uint32_t numUnique = manager.m_uniqueTypes.size();
			for(auto i = 0; i < size; ++i)
			{
				types[i] = uniques[rand() % numUnique];
			}
			return true;
		}
		/// Checks if condition are meet to end the round
		bool checkConditions(GameManager & manager, InputManager & inputManager, GameTimer & gameTimer)
		{
			manager.m_roundTimer -= gameTimer.getSmoothDeltaTime();
			if(inputManager::endButtonClicked(inputManager) || manager.m_roundTimer <= 0)
			{
				manager.m_roundTimer = 65;
				manager.m_stage = 0;
				return false;
			}
			return true;
		}
		/// Checks if we got 2 jewel selections and if it's possible to make a swap
		bool checkJewelSelection(GameManager & manager, InputManager & inputManager)
		{
			uint8_t index;
			auto & selectedIndex = manager.m_selectedIdx;
			auto & types = manager.m_types;
			if(inputManager::retrieveSelectedIndex(inputManager, index))
			{
				selectedIndex.push_back(index);
			}
			// Do we have 2 selections?
			if(selectedIndex.size() == 2)
			{

				auto first = selectedIndex[0];
				auto second = selectedIndex[1];

				if(first == second)
				{
					// We can't switch with ourself
					manager.m_selectedIdx.clear();
					manager.m_stage = 0;
					return false;
				}
				if(first + 1 == second)
				{
					std::swap(types[first], types[second]);
				}
				else if(first - 1 == second)
				{
					std::swap(types[first], types[second]);
				}
				else if(first + 8 == second)
				{
					std::swap(types[first], types[second]);
				}
				else if(first - 8 == second)
				{
					std::swap(types[first], types[second]);
				}
				else
				{
					// Other jewel is to far away
					manager.m_selectedIdx.clear();
					manager.m_stage = 0;
					return false;
				}

				// Check if we have a successful match
				internal::resetMatches(manager);
				gameManager::checkMatches(manager);
				if(internal::hasMatches(manager))
				{
					// We have a match, continue to the next stage
					return true;
				}
				else
				{
					// We do not have a successful switch, switch back
					manager.m_selectedIdx.clear();
					std::swap(types[first], types[second]);
					manager.m_stage = 0;
					return false;
				}
			}
			return false;
		}
		/// Animates the switch between two jewels
		bool animateJewelSwitch(GameManager & manager, GameTimer & gameTimer)
		{
			auto & positions = manager.m_positions;
			auto & selectedIdx = manager.m_selectedIdx;
			std::swap(positions[selectedIdx[0]], positions[selectedIdx[1]]);
			manager.m_selectedIdx.clear();
			return true;
		}
		/// Checks if we have a horizontal and/or a vertical 3+ match and store them in a matrix
		bool checkMatches(GameManager & manager)
		{
			auto & matches = manager.m_matches;
			auto & types = manager.m_types;
			internal::checkVerticalMatches(manager, types.data(), matches.data());
			internal::checkHorizontalMatches(manager, types.data(), matches.data());
			return true;
		}
		/// Updates the jewel positons so that matched jewels are moved to the top
		bool updateJewelPositions(GameManager & manager)
		{
			auto & matchCount = manager.m_matchCount;
			auto & matches = manager.m_matches;
			auto & positions = manager.m_positions;
			auto & types = manager.m_types;
			uint32_t height = manager.m_gridContainer.dim.y / 8;
			uint32_t offset = 0;
			uint32_t distance = 0;
			uint32_t count = 0;
			for(uint32_t i = 0; i < 8; ++i)
			{
				count = matchCount[i];

				// Move unmatched jewels to the bottom but keep their position
				int32_t limit = i * 8 + count;
				int32_t k = i * 8 + 7;
				int32_t l = k;
				while(l >= limit && k >= 0)
				{
					if(matches[k])
					{
						--k;
					}
					else
					{
						positions[l].y = positions[k].y;
						types[l] = types[k];
						if(l == limit)
							break;
						--l;
						--k;
					}
				}

				// Move all matched jewels to the top for this row
				for(int32_t j = 0; j < count; ++j)
				{
					offset = i * 8 + j;
					positions[offset].y -= height * count;
				}
				count = 0;
			}
			return true;
		}
		void calculateMatchCount(GameManager & manager)
		{
			auto & matches = manager.m_matches;
			auto & count = manager.m_matchCount;
			uint32_t offset = 0;
			for(uint32_t i = 0; i < 8; ++i)
			{
				count[i] = 0;
				for(int32_t j = 7; j >= 0; --j)
				{
					offset = i * 8 + j;
					count[i] += matches[offset];
				}
			}
		}
		/// Generate new jewels for all previously matched jewels
		bool generateJewels(GameManager & manager)
		{
			auto & matchCount = manager.m_matchCount;
			auto & types = manager.m_types;
			auto & uniqueTypes = manager.m_uniqueTypes;
			uint32_t numUnique = manager.m_uniqueTypes.size();
			uint32_t offset = 0;
			uint32_t count = 0;

			for(uint32_t i = 0; i < 8; ++i)
			{
				count = matchCount[i];	// Number of jewels that need to be generated for this row	
				for(uint32_t j = 0; j < count; ++j)
				{
					offset = i * 8 + j;
					types[offset] = uniqueTypes[rand() % numUnique];	// Select a random jewel
				}
			}
			return true;
		}
		/// Sets the speed of the jewels so that they can be moved, it also stops jewels that have reached their targets
		void setJewelSpeed(GameManager & manager, GameTimer & gameTimer, float speed)
		{
			auto & positions = manager.m_positions;
			auto & startPositions = manager.m_startPositions;
			auto & jewelSpeed = manager.m_speed;
			uint32_t size = manager.m_gridSize;
			float deltaTime = gameTimer.getSmoothDeltaTime();
			float deltaSpeed = speed * deltaTime;
			float deltaDistance = 0.0f;
			for(uint32_t i = 0; i < size; ++i)
			{
				// How far do we have left in the x direction?
				deltaDistance = startPositions[i].x - positions[i].x;

				// If we reach the target in the next step, we can set the position to the target position
				if(std::abs(deltaDistance) < deltaSpeed)
				{
					jewelSpeed[i].x = 0;
					positions[i].x = startPositions[i].x;
				}
				else
				{
					// Set speed so that the jewel move towards the start position
					jewelSpeed[i].x = copysign(speed, deltaDistance);
				}

				// How far do we have left in the y direction?
				deltaDistance = startPositions[i].y - positions[i].y;

				// If we reach the target in the next step, we can set the position to the target position
				if(std::abs(deltaDistance) < deltaSpeed)
				{
					jewelSpeed[i].y = 0;
					positions[i].y = startPositions[i].y;
				}
				else
				{
					// Set speed so that the jewel move towards the start position
					jewelSpeed[i].y = copysign(speed, deltaDistance);
				}
			}
		}
		/// Moves all jewels that have a speed
		bool moveJewel(GameManager & manager, GameTimer & gameTimer)
		{
			auto & positions = manager.m_positions;
			auto & speed = manager.m_speed;
			uint32_t size = manager.m_gridSize;
			float deltaTime = gameTimer.getSmoothDeltaTime();
			for(uint32_t i = 0; i < size; ++i)
			{
				positions[i].x += speed[i].x * deltaTime;
				positions[i].y += speed[i].y * deltaTime;			
			}
			return true;
		}
		void writeTextToBuffer(GameManager & manager)
		{
			auto buffer = manager.m_textBuffer.data();
			auto & texts = manager.m_texts;
			int32_t size =  manager.m_textBuffer.size();
			int32_t offset = 0;
			texts.clear();
			texts.push_back(buffer + offset);
			offset += snprintf(buffer + offset, size - offset, "Score : %d", manager.m_score);
			buffer[offset++] = '\0';
			texts.push_back(buffer + offset);
			offset += snprintf(buffer + offset, size - offset, "Timer : %.f", manager.m_roundTimer);
			buffer[offset++] = '\0';
		}
	};
};