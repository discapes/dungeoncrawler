#pragma once
#include <array>
#include "vec2.hpp"

enum Direction { NORTH, SOUTH, EAST, WEST, N_DIR };

constexpr std::array<ivec2, N_DIR> diroffsets = { { { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 } } };

static Direction reverse(Direction d)
{
	switch (d) {
	case NORTH: return SOUTH;
	case SOUTH: return NORTH;
	case EAST: return WEST;
	case WEST: return EAST;
	default: return N_DIR;
	}
}

static Direction wasd(char c)
{
	switch (c) {
	case 'w': return NORTH;
	case 'a': return WEST;
	case 's': return SOUTH;
	case 'd': return EAST;
	default: return N_DIR;
	}
}