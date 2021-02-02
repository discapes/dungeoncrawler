#include <iostream>
#include <memory>
#include <math.h>
#include "dir.hpp"
#include "math.hpp"
#include "config.hpp"
#include "getch.h"
using namespace std;
#define once(x)                           \
	{                                 \
		static bool done = false; \
		if (!done) {              \
			x done = true;    \
		}                         \
	}

struct Door {
	char status = Chars::doorlocked;
};

struct Room {
	static constexpr ivec2 size = { roomsize + 1, roomsize + 1 };
	static constexpr auto shape = hollowSquare<Room::size.x + 1>();
};

struct Level {
	unordered_map<ivec2, Room> rooms;
	unordered_map<ivec2, Door> doors;

	Level() {}

	Room& get(ivec2 pos)
	{
		if (rooms.contains(pos))
			return rooms.at(pos);

		Room& room = rooms.emplace(pos, Room{}).first->second;
		for (int i = 0; i < N_DIR; i++) {
			ivec2 relNextRoom = diroffsets[i] * Room::size;
			if (!rooms.contains(pos + relNextRoom) && rand() % 2) {
				ivec2 wallmod = { abs(diroffsets[i].y), abs(diroffsets[i].x) };
				int wallLen = Room::size.x - 1;
				dvec2 walloffset = (dvec2)wallmod * ((rand() % wallLen) - wallLen / 2);
				dvec2 doorpos = (dvec2)relNextRoom / 2.;
				doors.emplace(pos + (ivec2)floor(doorpos + walloffset), Door{});
			}
		}
		return room;
	}
	void draw(ivec2 player, ivec2 rp)
	{
		static vector<vector<char> > screen(termres.x);
		once({
			for (vector<char>& col : screen)
				col.resize(termres.y);
		});
		for (vector<char>& col : screen)
			fill(col.begin(), col.end(), Chars::empty);

		const ivec2 topleft = player - termres / 2;
		const ivec2 bottomright = topleft + termres - 1;
		const auto set = [&](ivec2 pos, char c) {
			if (inArea(pos, topleft, bottomright)) {
				ivec2 screenPos = pos - topleft;
				screen[screenPos.x][screenPos.y] = c;
			}
		};

		set(rp, Chars::roomicon);

		for (pair<const ivec2, Room>& item : rooms)
			for (ivec2 i : Room::shape)
				set(item.first + i, Chars::wall);

		for (pair<const ivec2, Door>& item : doors)
			set(item.first, item.second.status);

		set(player, Chars::playericon);

		for (int y = 0; y < termres.y; y++) {
			cout << screen[0][y];
			for (int x = 1; x < termres.x; x++)
				cout << ' ' << screen[x][y];
			cout << '\n';
		}
		cout << endl;
	}
};

Direction input()
{
	char c = getch();
	Direction d = wasd(c);
	return d;
}

Direction vectodir(dvec2 v)
{
	if (abs(v.x) >= abs(v.y))
		if (v.x >= 0)
			return EAST;
		else
			return WEST;
	else if (v.y > 0)
		return SOUTH;
	else
		return NORTH;
}

bool inwall(ivec2 pos, ivec2 rp)
{
	for (ivec2 i : Room::shape)
		if (pos == rp + i)
			return true;
	return false;
}

void run()
{
	Level level;
	ivec2 pos = { 0, 0 };
	Room* room = &level.get(pos);
	ivec2 rp = pos;
	while (true) {
		level.draw(pos, rp);
		Direction d = input();
		if (d == N_DIR)
			continue;
		ivec2 prev = pos;
		pos += diroffsets[d];
		dvec2 actualCenterOfRoom = (dvec2)rp - (Room::size.x % 2 == 0 ? 0 : 0.5);
		Direction fromRoom = vectodir((dvec2)pos - (dvec2)actualCenterOfRoom);
		if (level.doors.contains(pos)) {
			Door& door = level.doors.at(pos);
			// unlock door
			door.status = Chars::dooropen;
			rp += diroffsets[fromRoom] * Room::size;
			room = &level.get(rp);
		} else if (inwall(pos, rp)) {
			pos -= diroffsets[d];
		} else if (bool movedAwayFromRoom = abs(rp - prev).x < abs(rp - pos).x ||
						    abs(rp - prev).y < abs(rp - pos).y;
			   movedAwayFromRoom && level.doors.contains(prev)) {
			rp += diroffsets[fromRoom] * Room::size;
			room = &level.get(rp);
		}
		room = &level.get(rp);
	}
}

int main(void)
{
	//srand(time(0));
	srand(0);
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << endl;
	run();
	cout << endl;
	return 0;
}
