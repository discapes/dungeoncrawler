#include <iostream>
#include <vector>
#include <memory>
using namespace std;

struct Room;
struct Door {
	Room* const origin;
	unique_ptr<Room> dest;
	bool locked = true;
	Door(Room* origin)
	: origin(origin)
	{}
};

struct Room {
	vector<Door> children;
	Door* backdoor = nullptr;
	int id;
	Room() 
	{
		static int nextID = 0;
		id = nextID++;

		int doors = rand() % 4 + 1;
		for (int i = 0; i < doors; i++)
			this->children.emplace_back(this);
	}
};

Door* getInput(Room& room)
{
	while (true) {
		cout << "\n { " << room.id << " } Please enter which room you would like to enter. Options are:\n";
		cout << "\t[" << 0 << ']';
		for (uint32_t i = 1; i <= room.children.size(); i++)
			cout << "\t(" << i << ')';
		cout << endl;
		uint32_t ans;
		cin >> ans;
		if (ans > room.children.size())
			continue;
		return ans == 0 ? 0 : &room.children[ans - 1];
	}
}

void run()
{
	Room init, *room = &init;
    while (true) {
        Door* door = getInput(*room);
        if (!door) {
			if (!room->backdoor) return;
			room = room->backdoor->origin;
			continue;
		} else if (!door->dest) {
            door->dest = make_unique<Room>();
            door->dest->backdoor = door;
        }
		room = door->dest.get();
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
