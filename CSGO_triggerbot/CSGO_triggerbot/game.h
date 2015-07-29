#ifndef GAME
#define GAME

#include <vector>
#include "memory_manager.h"

using namespace std;

struct Player
{
	//Position of the player on the map
	float posX;
	float posY;

	//The angle the player is aiming
	float angleH;
	float angleV;

	//The side of the player (t, ct or spectator)
	int side;

	//If the player is shown on the radar (in line of sight)
	bool shown;
};

class Game
{
public:

	Game();
	Game(MemoryManager* memoryManager);
	~Game();
	//Updates the data for all the players
	void updatePlayers();

private:
	vector<Player*> players_;
	MemoryManager* memoryManager_;
};

#endif