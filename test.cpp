#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <assert.h>
#include <initializer_list>
#include <unordered_map>
#include <tuple>
using namespace std;

using uint = uint32_t;

template <typename T> struct vec2 {
	T x = {};
	T y = {};

	friend constexpr bool operator==(const vec2& a, const vec2& b) noexcept = default;

	friend constexpr vec2 operator+(const vec2& a, const vec2& b) { return { a.x + b.x, a.y + b.y }; }
	friend constexpr vec2 operator-(const vec2& a, const vec2& b) { return { a.x - b.x, a.y - b.y }; }
	friend constexpr vec2 operator*(const vec2& a, const vec2& b) { return { a.x * b.x, a.y * b.y }; }
	friend constexpr vec2 operator/(const vec2& a, const vec2& b) { return { a.x / b.x, a.y / b.y }; }

	friend constexpr vec2 operator+(const vec2& a, const T& b) { return a + vec2<T>{ b, b }; }
	friend constexpr vec2 operator-(const vec2& a, const T& b) { return a - vec2<T>{ b, b }; }
	friend constexpr vec2 operator*(const vec2& a, const T& b) { return a * vec2<T>{ b, b }; }
	friend constexpr vec2 operator/(const vec2& a, const T& b) { return a / vec2<T>{ b, b }; }

	constexpr vec2& operator+=(const vec2& b) { return *this = *this + b; }
	constexpr vec2& operator-=(const vec2& b) { return *this = *this - b; }
	constexpr vec2& operator*=(const vec2& b) { return *this = *this * b; }
	constexpr vec2& operator/=(const vec2& b) { return *this = *this / b; }

	constexpr vec2& operator+=(const T& b) { return *this = *this + b; }
	constexpr vec2& operator-=(const T& b) { return *this = *this - b; }
	constexpr vec2& operator*=(const T& b) { return *this = *this * b; }
	constexpr vec2& operator/=(const T& b) { return *this = *this / b; }
};
using ivec2 = vec2<int>;

enum Direction { NORTH, WEST, EAST, SOUTH, N_DIR };
array<ivec2, N_DIR> diroffsets = { { { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 } } };
Direction reverse(Direction d) {
	switch(d) {
		case NORTH: return SOUTH;
		case SOUTH: return NORTH;
		case EAST: return WEST;
		case WEST: return EAST;
		default: return N_DIR;
	}
}


namespace std
{
template <> struct hash<ivec2> {
	size_t operator()(ivec2 v) const
	{
		size_t lhs = hash<int>{}(v.x);
		size_t rhs = hash<int>{}(v.y);
		lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
		return lhs;
	}
};
} // namespace std

struct Door;
struct Room {
	array<Door*, N_DIR> doors{};
};

struct Door {
	ivec2 from, to;
	char status = 'X';
	Door(ivec2 from, ivec2 to)
		: from(from)
		, to(to)
	{
	}
};

struct Level {
	ivec2 lowest, highest;
	unordered_map<ivec2, Room> rooms;
	vector<unique_ptr<Door> > doors;

	Room generateRoom(ivec2 pos)
	{
		Room room;
		for (uint i = 0; i < N_DIR; i++) {
			if (rooms.contains(pos + diroffsets[i]))
				room.doors[i] = rooms.at(pos + diroffsets[i]).doors[reverse((Direction)i)];
			if (!room.doors[i] && rand() % 2) {
				doors.push_back(make_unique<Door>(pos, pos + diroffsets[i]));
				room.doors[i] = doors.back().get();
			}
		}
		return room;
	}

	Room& through(ivec2 to)
	{
		if (rooms.contains(to))
			return rooms.at(to);
		unordered_map<ivec2, Room>::iterator room = rooms.insert(make_pair(to, generateRoom(to))).first;
		lowest.x = min(lowest.x, room->first.x);
		lowest.y = min(lowest.y, room->first.y);
		highest.x = max(highest.x, room->first.x);
		highest.y = max(highest.y, room->first.y);
		return room->second;
	}
};

template <typename T> constexpr T square(T x) { return x * x; }
template <typename T> constexpr T outersquare(T x) { return square(x) - square(x - 2); }

template <int N> constexpr array<ivec2, outersquare(N * 2 + 1)> hollowSquare()
{
	array<ivec2, outersquare(N * 2 + 1)> arr;
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

Direction wasd(char c)
{
	switch (c) {
	case 'w': return NORTH;
	case 'a': return WEST;
	case 's': return SOUTH;
	case 'd': return EAST;
	default: return N_DIR;
	}
}

void draw(const Level& level, ivec2 player)
{
	constexpr ivec2 roomsize{ 5, 5 };
	auto calcScreenPos = [&level, roomsize](ivec2 room){ 
		ivec2 relPosInRooms = room - level.lowest;
		ivec2 screenPos = relPosInRooms * roomsize + roomsize / 2;
		return screenPos;
	};

	ivec2 areaInRooms = level.highest + 1 - level.lowest;
	ivec2 screenSize = areaInRooms * roomsize;

	using col = vector<char>;
	vector<col> screen(screenSize.x + sizeof('\0'));

	for (col& col : screen) {
		col.resize(screenSize.y);
		fill(col.begin(), col.end(), ' ');
	}

	for (const pair<ivec2, Room>& room : level.rooms) {
		ivec2 screenPos = calcScreenPos(room.first);
		for (ivec2 i : hollowSquare<2>())
			screen[screenPos.x + i.x][screenPos.y + i.y] = '#';

		const array<Door*, N_DIR>& doors = room.second.doors;
		for (int i = 0; i < N_DIR; i++) {
			if (doors[i])
				screen[screenPos.x + diroffsets[i].x * 2][screenPos.y + diroffsets[i].y * 2] =
					doors[i]->status;
		}
	}

	screen[calcScreenPos(player).x][calcScreenPos(player).y] = '?';

	for (int y = 0; y < screenSize.y; y++) {
		for (int x = 0; x < screenSize.x; x++)
			cout << ' ' << screen[x][y];
		cout << '\n';
	}

	cout << endl;
}

Door* input(Room& room)
{
	auto ask = []() -> Direction {
		char c;
		cin >> c;
		return wasd(c);
	};

	while (true) {
		Direction d = ask();
		if (d == N_DIR) {
			continue;
		}
		if (!room.doors[d]) {
			continue;
		}
		return room.doors[d];
	}
}

void run()
{
	Level level;
	ivec2 pos = { 0, 0 };
	Room& initial = level.through(pos);
	Room* room = &initial;
	while (true) {
		draw(level, pos);
		Door* d = input(*room);
		d->status = 'O';
		ivec2 otherside = d->from == pos ? d->to : d->from;
		pos = otherside;
		room = &level.through(otherside);
	}
}

int main(void)
{
	srand(time(0));
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << endl;
	run();
	cout << endl;
	return 0;
}