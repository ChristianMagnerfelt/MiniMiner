///
///	Author:			Christian Magnerfelt
///
#include "game_manager.hpp"

namespace MiniMiner
{
	namespace gameManager
	{
		namespace internal 
		{
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
			for(auto i = 0; i < 8; ++i)
			{
				for(auto j = 0; j < 8; ++j)
				{
					pos.x = gridOffset.x + i * 32;
					pos.y = gridOffset.y + j * 32;
					manager.m_positions[i * 8 + j] = pos;
					manager.m_startPositions[i * 8 + j] = pos;
				}
			}
			pos.x = 0;
			pos.y = 0;
			manager.m_targets.resize(64, pos);
			manager.m_speed.resize(64, pos);
			manager.m_types.resize(64, 0);
			return true;
		}
		bool update(GameManager & gameManager, InputManager & inputManager)
		{
			
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

			return true;
		}
		bool checkJewelSelection(GameManager & manager)
		{

			return true;
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
			uint32_t offset = 0;
			uint32_t distance = 0;
			for(uint32_t i = 0; i < 8; ++i)
			{
				for(int32_t j = 7; j >= 0; --j)
				{
					offset = i * 8 + j;
					distance += matches[offset];
					positions[offset].y -= 32 * distance;
				}
				distance = 0;
			}
			return true;
		}
		bool generateJewels(GameManager & manager)
		{

			return true;
		}
		bool moveJewels(GameManager & manager, GameTimer & gameTimer)
		{

			return true;
		}
	};
};