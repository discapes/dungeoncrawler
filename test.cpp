#include <iostream>
#include <vector>
#include <memory>
using namespace std;

struct Room;

struct Door {
	shared_ptr<Room> a, b;
	bool locked;
};

struct Room {
	static uint32_t freeuid;
	uint64_t uid;
	vector<shared_ptr<Door>> doors;
	shared_ptr<Room> me;

	Room() 
	: me(this)
	{
		uid = freeuid++;
		int numDoors = rand() % 4 + 1; 
		for (int i = 0; i < numDoors; i++)
			this->doors.emplace_back(make_shared<Door>(me, shared_ptr<Room>(), true));
	}
};
uint32_t Room::freeuid = 0;

shared_ptr<Door> getInput(Room& room)
{
	cout << room.uid << " Please enter which room you would like to enter. Options are:\n    ";
	for (uint32_t i = 0; i < room.doors.size(); i++)
		cout << "\t(" << i << ')';
	cout << endl;
	int ans;
	cin >> ans;
	return room.doors.at(ans);
}

void run()
{
	Room initialRoom;
	shared_ptr<Room> currentRoom = initialRoom.me;
	while (true) {
		shared_ptr<Door> door = getInput(*currentRoom);
		if (!door->b) {
			shared_ptr<Room> newRoom = make_shared<Room>();
			door->b = newRoom;
			newRoom->doors.insert(newRoom->doors.begin(), door);
		}
		currentRoom = currentRoom == door->a ? door->b : door->a;
	}
	// MEMORY LEAK, CIRCULAR DEPENDENCIES MEAN STUFF WONT GET RELEASED
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
