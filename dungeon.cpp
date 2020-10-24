#include <iostream>
#include <memory>
#include "dir.hpp"
#include "math.hpp"
#include "config.hpp"
using namespace std;

struct Door {
	ivec2 from, to;
	char status = 'X';
	Door(ivec2 from, ivec2 to)
		: from(from)
		, to(to)
	{}
};

struct Room {
	array<Door*, N_DIR> doors{};
	static constexpr ivec2 size{ 4, 4 }; // actual room size may vary (its actually 5x5 but we have connected walls)
	bool operator==(const Room& other) { return this == &other; }
};

class Level {
    public:
	ivec2 topLeft, bottomRight;
	unordered_map<ivec2, Room> rooms;
	vector<unique_ptr<Door> > doors;

	Room& get(ivec2 pos)
	{
		if (rooms.contains(pos))
			return rooms.at(pos);
		unordered_map<ivec2, Room>::iterator room = rooms.insert(make_pair(pos, generateRoom(pos))).first;
		topLeft.x = min(topLeft.x, room->first.x);
		topLeft.y = min(topLeft.y, room->first.y);
		bottomRight.x = max(bottomRight.x, room->first.x);
		bottomRight.y = max(bottomRight.y, room->first.y);
		return room->second;
	}
	ivec2 move(Room*& room, ivec2 prev, ivec2 pos)
	{
		for (int i = 0; i < N_DIR; i++) {
			Door* door = room->doors[i];
			if (door) {
				ivec2 dir = door->to - door->from;
				ivec2 doorpos = door->from * Room::size + (Room::size / 2) * dir;
				bool cameback = (pos == (prev + diroffsets[i])) && prev == doorpos;
				if (pos == doorpos || cameback) { // (pos+1) so every square is translated correctly
					door->status = ' ';
					Room& to = get(door->to);
					room = &(*room == to ? get(door->from) : to);
					return pos;
				}
			}
		}
		auto square = hollowSquare<Room::size.x / 2>();
		for (ivec2 i : square)
			if (pos % Room::size == i) {
				return prev;
			}
		return pos;
	}

    private:
	Room generateRoom(ivec2 pos)
	{
		Room room;
		for (uint i = 0; i < N_DIR; i++) {
			if (rooms.contains(pos + diroffsets[i])) {
				room.doors[i] = rooms[pos + diroffsets[i]].doors[reverse((Direction)i)];
			} else if (!room.doors[i] && rand() % 2) {
				doors.push_back(make_unique<Door>(pos, pos + diroffsets[i]));
				room.doors[i] = doors.back().get();
			}
		}
		return room;
	}
};

using col = vector<char>;
class Screen : public vector<col> {
	ivec2 fullsize() { return bottomRight + 1 - topLeft; }

    public:
	ivec2 topLeft; // inclusive
	ivec2 bottomRight; // inclusive
	void set(ivec2 pos, char c)
	{
		if (!followsPlayer || inSquare(pos, topLeft, bottomRight))
			(*this)[pos.x - topLeft.x][pos.y - topLeft.y] = c;
	}
	void clear()
	{
		for (col& col : *this)
			fill(col.begin(), col.end(), ' ');
	}
	void print()
	{
		cout << string(50, '\n');
		for (int y = 0; y < fullsize().y; y++) {
			cout << (*this)[0][y];
			for (int x = 1; x < fullsize().x; x++)
				cout << ' ' << (*this)[x][y];
			cout << '\n';
		}
		cout << endl;
	}
};

#define once(x)                           \
	{                                 \
		static bool done = false; \
		if (!done) {              \
			x done = true;    \
		}                         \
	}

Screen& getScreen(const Level& level, ivec2 player)
{
	static Screen screen;
	if (followsPlayer) {
		screen.topLeft = player - termres / 2;
		screen.bottomRight = screen.topLeft + termres - 1; // -1 because inclusive
		once({
			screen.resize(termres.x);
			for (col& col : screen)
				col.resize(termres.y);
		})
	} else {
		screen.topLeft = level.topLeft * Room::size - Room::size / 2;
		screen.bottomRight =
			screen.topLeft + (level.bottomRight + 1 - level.topLeft) * Room::size - 1 + 1; // +1 so walls
		screen.resize(screen.bottomRight.x + 1 - screen.topLeft.x);
		for (col& col : screen)
			col.resize(screen.bottomRight.y + 1 - screen.topLeft.y);
	}
	return screen;
}

void draw(const Level& level, ivec2 player)
{
	constexpr char wall = '#';
	constexpr char playericon = '?';

	Screen& screen = getScreen(level, player);
	screen.clear();

	for (const pair<ivec2, Room>& room : level.rooms) {
		ivec2 pos = room.first * Room::size;
		for (ivec2 i : hollowSquare<Room::size.x / 2>())
			screen.set(pos + i, wall);
	}

	for (const unique_ptr<Door>& door : level.doors) {
		ivec2 dir = door->to - door->from;
		ivec2 pos = door->from * Room::size + (Room::size / 2) * dir;
		screen.set(pos, door->status);
	}

	screen.set(player, playericon);

	screen.print();
}

Direction input()
{
	Direction d;
	do {
		char c;
		cin >> c;
		d = wasd(c);
	} while (d == N_DIR);
	return d;
}

void run()
{
	Level level;
	ivec2 pos = { 0, 0 };
	ivec2 prev = pos;
	Room& initial = level.get(pos);
	Room* room = &initial;
	while (true) {
		draw(level, pos);
		Direction d = input();
		pos += diroffsets[d];
		pos = level.move(room, prev, pos);
		prev = pos;
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