#pragma once
#include "vec2.hpp"

template <typename T> constexpr T square(T x) { return x * x; }
template <typename T> constexpr T outersquare(T x) { return square(x) - square(x - 2); }

template <int N> constexpr std::array<ivec2, outersquare(N * 2 + 1)> hollowSquare()
{
	std::array<ivec2, outersquare(N * 2 + 1)> arr;
	int i = 0;

	for (int x = -N; x <= N; ++x, ++i) // top
		arr[i] = { x, -N };

	for (int y = -N + 1; y < N; ++y, ++i) // left
		arr[i] = { -N, y };
	for (int y = -N + 1; y < N; ++y, ++i) // right
		arr[i] = { N, y };

	for (int x = -N; x <= N; ++x, ++i) // bottom
		arr[i] = { x, N };

	return arr;
}

static constexpr bool inSquare(ivec2 pos, ivec2 topLeft, ivec2 bottomRight)
{
	return pos.x >= topLeft.x && pos.y >= topLeft.y && pos.x <= bottomRight.x && pos.y <= bottomRight.y;
}