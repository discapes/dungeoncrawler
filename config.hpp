#pragma once

constexpr int termW = 150;
constexpr int termH = 32;
constexpr bool followsPlayer = true;
constexpr int roomsize = 4;

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
