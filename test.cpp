#include <iostream>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
using namespace std;

enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	DIRECTIONS,
};

Direction reverse(Direction d)
{
	return (Direction)(d > 1 ? d - 2 : d + 2);
}

struct Door;

struct Room
{
	vector<Door> doors;
	Door *backdoor = nullptr;
	int id;
	inline Room(Door *prev);
	friend bool operator==(const Room &a, const Room &b);
};
bool operator==(const Room &a, const Room &b)
{
	return &a == &b;
}

struct Door
{
	char c = 'X';
	Room &origin;
	unique_ptr<Room> dest;
	bool locked = true;
	Direction dir;
	Door(Room &origin, Direction dir)
		: origin(origin), dir(dir)
	{
	}
	Room &through(Room &from)
	{
		unlock();
		if (!dest)
			dest = make_unique<Room>(this);
		return from == origin ? *dest.get() : origin;
	}
	Direction relaDir(Room &room)
	{
		return room == origin ? dir : reverse(dir);
	}
	friend bool operator==(const Door &a, const Door &b);
	void unlock()
	{
		locked = false;
		c = 'O';
	}
};
bool operator==(const Door &a, const Door &b)
{
	return &a == &b;
}

inline Room::Room(Door *prev)
{
	static int nextID = 0;
	id = nextID++;

	vector<int> dirs(DIRECTIONS);
	iota(dirs.begin(), dirs.end(), 0);
	if (prev)
		dirs.erase(dirs.begin() + reverse(prev->dir));
	random_shuffle(dirs.begin(), dirs.end());

	if (prev)
		this->backdoor = prev;

	int doors = rand() % (prev ? 3 : 4) + 1;
	for (int i = 0; i < doors; i++)
	{
		this->doors.emplace_back(*this, (Direction)dirs[i]);
	}
}

Direction wasd(char c)
{
	switch (c)
	{
	case 'w':
		return NORTH;
	case 'a':
		return WEST;
	case 's':
		return SOUTH;
	case 'd':
		return EAST;
	default:
		return DIRECTIONS;
	}
}

Door *askInput(Room &room)
{
	Door *doors[DIRECTIONS]{};
	if (room.backdoor)
		doors[room.backdoor->relaDir(room)] = room.backdoor;
	for (Door &door : room.doors)
		doors[door.relaDir(room)] = &door;

	array<array<char, 10>, 5> screen =
		{
			"/=======\\",
			"|       |",
			"|       |",
			"|       |",
			"\\=======/",
		};
	screen[2][4] = '0' + room.id % 10;
	if (doors[NORTH])
		screen[0][4] = doors[NORTH]->c;
	if (doors[SOUTH])
		screen[4][4] = doors[SOUTH]->c;
	if (doors[WEST])
		screen[2][0] = doors[WEST]->c;
	if (doors[EAST])
		screen[2][8] = doors[EAST]->c;

	for (array<char, 10> row : screen)
		cout << "  " << row.data() << '\n';
	cout << flush;

	char ans;
	cin >> ans;
	if (ans == 'q')
		exit(0);
	if (wasd(ans) == DIRECTIONS)
		return nullptr;
	return doors[wasd(ans)];
}

Door &getInput(Room &room)
{
	Door *door;
	while (!(door = askInput(room)))
		;
	return *door;
}

void run()
{
	Room lobby(nullptr);
	Door exit(lobby, NORTH);
	exit.c = ' ';
	Room init(&exit), *room = &init;
	while (true)
	{
		Door &door = getInput(*room);
		if (door == exit)
			return;
		room = &door.through(*room);
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
