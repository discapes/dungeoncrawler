#pragma once
#define small

#ifdef small 
constexpr int termW = 181;
constexpr int termH = 15;
#else
constexpr int termW = 220;
constexpr int termH = 48;
#endif
constexpr bool followsPlayer = true;


/********************************/
#include "vec2.hpp"
constexpr ivec2 termres = { (termW + 1) / 2, termH }; //cuz we have s p a c e s
