#include <game_manager.hpp>
#include <input_manager.hpp>

#include <cassert>

void testInit()
{
	MiniMiner::Rect gridContainer;
	MiniMiner::Vec2 pos;
	pos.x = 300;
	pos.y = 300;
	MiniMiner::Vec2 dim;
	dim.x = 256;
	dim.y = 256;
	gridContainer.pos = pos;
	gridContainer.dim = dim;
	MiniMiner::GameManager gameManager;
	assert(MiniMiner::gameManager::init(gameManager, gridContainer));

	
	for(auto i = 0; i < 8; ++i)
	{
		for(auto j = 0; j < 8; ++j)
		{
			pos.x = j * 32;
			pos.y = i * 32;
			assert(gameManager.m_positions[j * 8 + i] == pos);
		}
	}
}
void testUpdate()
{

}
void testCreateBoard()
{

}
void testCheckConditions()
{

}
void testCheckJewelSelection()
{

}
void testAnimateJewelSwitch()
{

}
void testGetMatches()
{

}
void testGenerateJewels()
{

}
void testMoveJewels()
{

}
int main()
{
	testInit();
	return 0;
}