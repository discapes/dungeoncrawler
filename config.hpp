#pragma once
//#define small

#ifdef tiny
constexpr int termW = 20;
constexpr int termH = 10;
#elif defined(small)
constexpr int termW = 180;
constexpr int termH = 15;
#else
constexpr int termW = 220;
constexpr int termH = 48;
#endif
constexpr bool followsPlayer = true;
constexpr int roomsize = 2;

namespace Chars {
    constexpr char wall = '#';
    constexpr char playericon = '?';
    constexpr char roomicon = '@';
    constexpr char empty = ' ';
    constexpr char doorlocked = 'O';
    constexpr char dooropen = ' ';
}

/********************************/
#include "vec2.hpp"
constexpr ivec2 termres = { (termW + 1) / 2, termH }; //cuz we have s p a c e s
