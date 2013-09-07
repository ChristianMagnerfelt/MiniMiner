///
///	Author:			Christian Magnerfelt
///
#include "game_manager.hpp"

namespace MiniMiner
{
	namespace gameManager
	{
		bool init(GameManager & manager, const Rect & gridContainer, const uint8_t * types, uint32_t numTypes)
		{
			manager.m_uniqueTypes = std::vector<uint8_t>(types, types + numTypes);
			manager.m_positions.resize(8 * 8);
			Vec2 gridOffset = gridContainer.pos;
			Vec2 pos;
			for(auto i = 0; i < 8; ++i)
			{
				for(auto j = 0; j < 8; ++j)
				{
					pos.x = gridOffset.x + j * 32;
					pos.y = gridOffset.y + i * 32;
					manager.m_positions[j * 8 + i] = pos;
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
		bool animateJewelSwitch(GameManager & manager)
		{

			return true;
		}
		bool getMatches(GameManager & manager)
		{

			return true;
		}
		bool generateJewels(GameManager & manager)
		{

			return true;
		}
		bool moveJewels(GameManager & manager)
		{

			return true;
		}
	};
};