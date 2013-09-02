#include <input_manager.hpp>

#include <cassert>
#include <iostream>

MiniMiner::Rect g_gridRect;
MiniMiner::Rect g_endRndBtnRect;

void setup()
{
	MiniMiner::Vec2 gridPos;
	MiniMiner::Vec2 gridDim;
	gridPos.x = 300;
	gridPos.y = 300;
	gridDim.x = 100;
	gridDim.y = 100;
	g_gridRect.pos = gridPos;
	g_gridRect.dim = gridDim;
	MiniMiner::Vec2 endBtnPos;
	MiniMiner::Vec2 endBtnDim;
	endBtnPos.x = 10;
	endBtnPos.y = 800;
	endBtnDim.x = 100;
	endBtnDim.y = 100;
	g_endRndBtnRect.pos = gridPos;
	g_endRndBtnRect.dim = gridDim;
}
void testInit()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::InputManager inputManager;
	MiniMiner::inputManager::init(inputManager, g_gridRect, g_endRndBtnRect);
	auto idx = 0;
	for(auto i = 0; i < 8; ++i)
	{
		for(auto j = 0; j < 8; ++j, ++idx)
		{
			assert(inputManager.m_indexGrid[j][i] == idx);
		}
	}
	assert(inputManager.m_selectedIndex == 0);
	assert(inputManager.m_endRoundButtonClicked == false);
	assert(inputManager.m_gridContainer == g_gridRect);
	assert(inputManager.m_endRoundButtonContainer == g_endRndBtnRect);
}
void testSendKeyDownEvent()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::InputManager inputManager;
	MiniMiner::inputManager::init(inputManager, g_gridRect, g_endRndBtnRect);
}
void testSendKeyUpEvent()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::InputManager inputManager;
	MiniMiner::inputManager::init(inputManager, g_gridRect, g_endRndBtnRect);
}
void testRetrieveSelectedIndex()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::InputManager inputManager;
	MiniMiner::inputManager::init(inputManager, g_gridRect, g_endRndBtnRect);
}
void testEndButtonClicked()
{
	std::cout << __FUNCTION__ << std::endl;
	MiniMiner::InputManager inputManager;
	MiniMiner::inputManager::init(inputManager, g_gridRect, g_endRndBtnRect);
}
int main()
{
	setup();
	testInit();
	testSendKeyDownEvent();
	testSendKeyUpEvent();
	testRetrieveSelectedIndex();
	testEndButtonClicked();
	return 0;
}