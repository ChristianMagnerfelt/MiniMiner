///
///	Author:			Christian Magnerfelt
///
#include "game_manager.hpp"

#include <cmath>

#define copysign _copysign

namespace MiniMiner
{
	namespace gameManager
	{
		namespace internal 
		{
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
						for(uint8_t k = offset - count * 8; k < offset; k += 8)
						{
							matches[k] |= 1;
						}
					}
					count = 1;
					prev = - 1;
				}
			}
		};
		bool init(GameManager & manager, const Rect & gridContainer, const uint8_t * types, uint32_t numTypes)
		{
			manager.m_uniqueTypes = std::vector<uint8_t>(types, types + numTypes);
			manager.m_positions.resize(8 * 8);
			manager.m_startPositions.resize(8 * 8);
			Vec2 gridOffset = gridContainer.pos;
			Vec2 pos;
			float width = gridContainer.dim.x/8;
			float height = gridContainer.dim.y/8;
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
			pos.x = 0;
			pos.y = 0;
			manager.m_targets.resize(64, pos);
			manager.m_speed.resize(64, pos);
			manager.m_types.resize(64, 0);
			manager.m_stage = 0;
			manager.m_gridContainer = gridContainer;
			return true;
		}
		bool update(GameManager & gameManager, InputManager & inputManager, GameTimer & gameTimer)
		{
			// Check timer and if end button is pressed
			if(!gameManager::checkConditions(gameManager, inputManager))
			{
				// End round
				gameManager::createBoard(gameManager);
				return true;
			}
			auto stage = gameManager.m_stage;
			switch (stage)
			{
				case 0:
					if(!gameManager::checkJewelSelection(gameManager, inputManager))
					{
						return true;
					}
				case 1: 
					if(!gameManager::animateJewelSwitch(gameManager, gameTimer))
					{
						return true;
					} 
				case 2:
					gameManager::generateJewels(gameManager);
					gameManager::updateJewelPositions(gameManager);
					gameManager::setJewelSpeed(gameManager, gameTimer, 1.0f);
					gameManager.m_stage = 0;
				case 3:
					gameManager::moveJewel(gameManager, gameTimer);
				case 5:
					gameManager::checkMatches(gameManager);
			}
			return true;
		}
		bool createBoard(GameManager & manager)
		{
			auto & types = manager.m_types;
			auto & uniques = manager.m_uniqueTypes;
			uint32_t size = manager.m_types.size();
			uint32_t numUnique = manager.m_uniqueTypes.size();
			for(auto i = 0; i < size; ++i)
			{
				types[i] = uniques[rand() % numUnique];
			}
			return true;
		}
		bool checkConditions(GameManager & manager, InputManager & inputManager)
		{
			if(inputManager::endButtonClicked(inputManager))
			{
				manager.m_stage = 0;
				return false;
			}
			return true;
		}
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

				// Make switch and check match
				gameManager::checkMatches(manager);
				if(internal::hasMatches(manager))
				{
					manager.m_selectedIdx.clear();
					return true;
				}
				else
				{
					manager.m_selectedIdx.clear();
					std::swap(types[first], types[second]);
					manager.m_stage = 0;
					return false;
				}
			}
			return false;
		}
		bool animateJewelSwitch(GameManager & manager, GameTimer & gameTimer)
		{

			return true;
		}
		bool checkMatches(GameManager & manager)
		{
			auto & matches = manager.m_matches;
			auto & types = manager.m_types;
			matches.resize(64, 0);
			internal::checkVerticalMatches(manager, types.data(), matches.data());
			internal::checkHorizontalMatches(manager, types.data(), matches.data());
			return true;
		}
		bool updateJewelPositions(GameManager & manager)
		{
			auto & matches = manager.m_matches;
			auto & positions = manager.m_positions;
			uint32_t height = manager.m_gridContainer.dim.y / 8;
			uint32_t offset = 0;
			uint32_t distance = 0;
			for(uint32_t i = 0; i < 8; ++i)
			{
				for(int32_t j = 7; j >= 0; --j)
				{
					offset = i * 8 + j;
					distance += matches[offset];
					positions[offset].y -= height * distance;
				}
				distance = 0;
			}
			return true;
		}
		bool generateJewels(GameManager & manager)
		{

			return true;
		}
		void setJewelSpeed(GameManager & manager, GameTimer & gameTimer, float speed)
		{
			auto & positions = manager.m_positions;
			auto & startPositions = manager.m_startPositions;
			auto & jewelSpeed = manager.m_speed;
			uint32_t size = positions.size();
			float deltaTime = gameTimer.getSmoothDeltaTime();
			float deltaSpeed = speed * deltaTime;
			float deltaDistance = 0.0f;
			for(uint32_t i = 0; i < size; ++i)
			{
				deltaDistance = startPositions[i].x - positions[i].x;
				if(deltaDistance < deltaSpeed)
				{
					jewelSpeed[i].x = 0;
					positions[i].x = startPositions[i].x;
				}
				else
				{
					// Set speed so that the jewel move towards the start position
					jewelSpeed[i].x = copysign(speed, deltaDistance);
				}

				deltaDistance = startPositions[i].y - positions[i].y;
				if(deltaDistance < deltaSpeed)
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
		bool moveJewel(GameManager & manager, GameTimer & gameTimer)
		{
			auto & positions = manager.m_positions;
			auto & speed = manager.m_speed;
			uint32_t size = positions.size();
			float deltaTime = gameTimer.getSmoothDeltaTime();
			for(uint32_t i = 0; i < size; ++i)
			{
				positions[i].x += speed[i].x * deltaTime;
				positions[i].y += speed[i].y * deltaTime;			
			}
			return true;
		}
	};
};