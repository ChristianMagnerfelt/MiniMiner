#include <game_manager.hpp>
#include <input_manager.hpp>

#include <cassert>
#include <iostream>

void setup(MiniMiner::GameManager & gameManager, MiniMiner::Rect & gridContainer, std::vector<uint8_t> & types, uint32_t numTypes)
{
	MiniMiner::Vec2 pos;
	MiniMiner::Vec2 dim;
	for(uint32_t i = 0; i < numTypes; ++i)
	{
		types.push_back(i);
	}

	pos.x = 300;
	pos.y = 300;	
	dim.x = 256;
	dim.y = 256;
	gridContainer.pos = pos;
	gridContainer.dim = dim;
}
void testInit()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::GameManager gameManager;
	MiniMiner::Rect gridContainer;
	std::vector<uint8_t> types;	
	uint32_t numTypes = 6;
	setup(gameManager, gridContainer, types, numTypes);

	assert(MiniMiner::gameManager::init(gameManager, gridContainer, types.data(), numTypes));
	assert(gameManager.m_positions.size() == 64);
	assert(gameManager.m_targets.size() == 64);
	assert(gameManager.m_speed.size() == 64);
	assert(gameManager.m_types.size() == 64);

	MiniMiner::Vec2 pos;
	for(auto i = 0; i < 8; ++i)
	{
		for(auto j = 0; j < 8; ++j)
		{
			pos.x = 300 + j * 32;
			pos.y = 300 + i * 32;
			assert(gameManager.m_positions[j * 8 + i] == pos);
			std::cout << "(" << pos.x << "," << pos.y << ")";
		}
		std::cout << std::endl;
	}
	
	pos.x = 0;
	pos.y = 0;
	for(auto i = 0; i < 64; ++i)
	{
		assert(gameManager.m_targets[i] == pos);
		assert(gameManager.m_speed[i] == pos);
	}
}
void testUpdate()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testCreateBoard()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::GameManager gameManager;
	MiniMiner::Rect gridContainer;
	std::vector<uint8_t> types;	
	uint32_t numTypes = 6;
	setup(gameManager, gridContainer, types, numTypes);

	assert(MiniMiner::gameManager::init(gameManager, gridContainer, types.data(), numTypes));
	assert(MiniMiner::gameManager::createBoard(gameManager));
}
void testCheckConditions()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testCheckJewelSelection()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testAnimateJewelSwitch()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testGetMatches()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testGenerateJewels()
{
	std::cout << __FUNCTION__ << std::endl;
}
void testMoveJewels()
{
	std::cout << __FUNCTION__ << std::endl;
}
int main()
{
	testInit();
	testUpdate();
	testCreateBoard();
	testCheckConditions();
	testCheckJewelSelection();
	testAnimateJewelSwitch();
	testGetMatches();
	testGenerateJewels();
	testMoveJewels();
	system("pause");
	return 0;
}