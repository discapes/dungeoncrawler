#pragma once
#include "vec2.hpp"

template <typename T> constexpr T square(T x) { return x * x; }
template <typename T> constexpr T outersquare(T x) { return square(x) - square(x - 2); }

template <int N> constexpr std::array<ivec2, outersquare(N)> hollowSquare()
{
	std::array<ivec2, outersquare(N)> arr;
	int i = 0;
	int TL = -N / 2; // Max index
	int BR = TL + N - 1; // -1 because inclusive

	for (int x = TL; x <= BR; ++x, ++i) // top
		arr[i] = { x, TL };

	for (int y = TL + 1; y < BR; ++y, ++i) // left
		arr[i] = { TL, y };
	for (int y = TL + 1; y < BR; ++y, ++i) // right
		arr[i] = { BR, y };

	for (int x = TL; x <= BR; ++x, ++i) // bottom
		arr[i] = { x, BR };

	return arr;
}

template <typename T> static constexpr bool inArea(vec2<T> pos, vec2<T> topLeft, vec2<T> bottomRight)
{
	return pos.x >= topLeft.x && pos.y >= topLeft.y && pos.x <= bottomRight.x && pos.y <= bottomRight.y;
}

template <typename T> T average(T a, T b) { return (a + b) / 2; }