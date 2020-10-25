#include <iostream>
#include <memory>
#include <math.h>
#include "dir.hpp"
#include "math.hpp"
#include "config.hpp"
using namespace std;
#define once(x)                           \
	{                                 \
		static bool done = false; \
		if (!done) {              \
			x done = true;    \
		}                         \
	}

struct Door {
	ivec2 from, to, pos;
	char status = 'X';
	Door(ivec2 from, ivec2 to)
		: from(from)
		, to(to)
		, pos(average(from, to))
	{}
};

struct Room {
	static constexpr ivec2 size = { 5, 5 };
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
		for (uint i = 0; i < N_DIR; i++) {
			ivec2 relNextRoom = diroffsets[i] * Room::size;
			if (!rooms.contains(pos + relNextRoom) && rand() % 2)
				doors.emplace(pos + (ivec2)ceil((dvec2)relNextRoom / 2. - 0.5), Door{ pos, pos + relNextRoom });
		}
		return room;
	}
	void draw(ivec2 player, ivec2 rp)
	{
		constexpr char wall = '#';
		constexpr char playericon = '?';
		constexpr char empty = ' ';

		static vector<vector<char> > screen(termres.x);
		once({
			for (vector<char>& col : screen)
				col.resize(termres.y);
		});
		for (vector<char>& col : screen)
			fill(col.begin(), col.end(), empty);

		const ivec2 topleft = player - termres / 2;
		const ivec2 bottomright = topleft + termres - 1;
		const auto set = [&](ivec2 pos, char c) {
			if (inArea(pos, topleft, bottomright)) {
				ivec2 screenPos = pos - topleft;
				screen[screenPos.x][screenPos.y] = c;
			}
		};

		for (pair<const ivec2, Room>& item : rooms)
			for (ivec2 i : Room::shape)
				set(item.first + i, wall);

		for (pair<const ivec2, Door>& item : doors)
			set(item.first, item.second.status);

		set(rp, 'h');
		set(player, playericon);

		cout << string(50, '\n');
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
	Direction d;
	char c;
	c = cin.get();
	cin.clear();
	d = wasd(c);
	return d;
}

void run()
{
	Level level;
	ivec2 pos = { 0, 0 }, prev = pos;
	Room* room = &level.get(pos);
	ivec2 rp = pos;
	while (true) {
		level.draw(pos, rp);
		Direction d = input();
		if (d == N_DIR) continue;
		pos += diroffsets[d];
		bool isOndoor = level.doors.contains(pos);
		bool wasAtDoor = level.doors.contains(prev);
		bool turnedAroundAtDoor = false;
		if (wasAtDoor) {
			ivec2 distFromRoom = abs(pos - rp);
			turnedAroundAtDoor = distFromRoom.x > Room::size.x/2 || distFromRoom.y > Room::size.y/2; 
		}
		if (isOndoor || turnedAroundAtDoor) {
			Door* door;
			if (isOndoor) door = &level.doors.at(pos);
			else door = &level.doors.at(prev);
			door->status = ' ';
			rp = door->from == rp ? door->to : door->from;
			room = &level.get(rp);
		} else {
			for (ivec2 i : Room::shape)
				if (pos == rp + i)
					pos = prev;
		}
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
