#pragma once
#include <stdint.h>
#include <math.h>
#include <functional>

template <typename T> struct vec2 {
	T x = {};
	T y = {};

	template <typename T2> constexpr operator vec2<T2>() const { return { (T2)x, (T2)y }; }
	friend constexpr vec2 abs(const vec2& a) { return { abs(a.x), abs(a.y) }; }
	friend constexpr vec2 ceil(const vec2& a) { return { ceil(a.x), ceil(a.y) }; }

	friend constexpr bool operator==(const vec2& a, const vec2& b) noexcept = default;
	friend constexpr bool operator==(const vec2& a, const T& b) noexcept { return a == vec2{ b, b }; }

	friend constexpr vec2 operator+(const vec2& a, const vec2& b) { return { a.x + b.x, a.y + b.y }; }
	friend constexpr vec2 operator-(const vec2& a, const vec2& b) { return { a.x - b.x, a.y - b.y }; }
	friend constexpr vec2 operator*(const vec2& a, const vec2& b) { return { a.x * b.x, a.y * b.y }; }
	friend constexpr vec2 operator/(const vec2& a, const vec2& b) { return { a.x / b.x, a.y / b.y }; }
	friend constexpr vec2 operator%(const vec2& a, const vec2& b) { return { a.x % b.x, a.y % b.y }; }

	friend constexpr vec2 operator+(const vec2& a, const T& b) { return a + vec2{ b, b }; }
	friend constexpr vec2 operator-(const vec2& a, const T& b) { return a - vec2{ b, b }; }
	friend constexpr vec2 operator*(const vec2& a, const T& b) { return a * vec2{ b, b }; }
	friend constexpr vec2 operator/(const vec2& a, const T& b) { return a / vec2{ b, b }; }
	friend constexpr vec2 operator%(const vec2& a, const T& b) { return a % vec2{ b, b }; }

	constexpr vec2& operator+=(const vec2& b) { return *this = *this + b; }
	constexpr vec2& operator-=(const vec2& b) { return *this = *this - b; }
	constexpr vec2& operator*=(const vec2& b) { return *this = *this * b; }
	constexpr vec2& operator/=(const vec2& b) { return *this = *this / b; }
	constexpr vec2& operator%=(const vec2& b) { return *this = *this % b; }

	constexpr vec2& operator+=(const T& b) { return *this = *this + b; }
	constexpr vec2& operator-=(const T& b) { return *this = *this - b; }
	constexpr vec2& operator*=(const T& b) { return *this = *this * b; }
	constexpr vec2& operator/=(const T& b) { return *this = *this / b; }
	constexpr vec2& operator%=(const T& b) { return *this = *this % b; }
};
using cvec2 = vec2<char>;
using ucvec2 = vec2<unsigned char>;
using scvec2 = vec2<signed char>;
using ivec2 = vec2<int>;
using dvec2 = vec2<double>;
using fvec2 = vec2<float>;
using uvec2 = vec2<unsigned>;
using lvec2 = vec2<long>;
using llvec2 = vec2<long long>;
using ulvec2 = vec2<unsigned long>;
using ullvec2 = vec2<unsigned long long>;
using ldvec2 = vec2<long double>;

namespace std
{
template <typename T> struct hash<vec2<T> > {
	size_t operator()(vec2<T> v) const
	{
		size_t lhs = std::hash<int>{}(v.x);
		size_t rhs = std::hash<int>{}(v.y);
		lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
		return lhs;
	}
};
}