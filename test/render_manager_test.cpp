#include <render_manager.hpp>

#include <cassert>

void testCopyToBuffer()
{
	MiniMiner::RenderManager manager;

	uint32_t a;
	MiniMiner::Vec2 b;
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, &a, nullptr, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, &b, 0));
	assert(!MiniMiner::renderManager::copyToBuffer(manager, nullptr, nullptr, 100));

	std::vector<uint32_t> ids(100, 1);
	MiniMiner::Vec2 pos = {10, 10};
	std::vector<MiniMiner::Vec2> positions(100, pos);
	std::size_t count = 100;
	assert(MiniMiner::renderManager::copyToBuffer(manager, ids.data(), positions.data(), count));
}


int main()
{
	testCopyToBuffer();
	return 0;
}