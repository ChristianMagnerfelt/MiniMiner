///
///	Author:			Christian Magnerfelt
///

#ifndef GAME_TYPES_HPP
#define GAME_TYPES_HPP

#include <cstdint>

namespace MiniMiner
{
	struct Match
	{
		uint8_t type;
		uint8_t count;
		uint32_t index;
	};
};

#endif