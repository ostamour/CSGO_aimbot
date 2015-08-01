#ifndef TRIGGERBOT
#define TRIGGERBOT

#include "memory_manager.h"

using namespace std;


const int MAX_N_PLAYER = 24;

struct Player
{
	//Position of the player on the map
	float posX;
	float posY;
	float posZ;

	//The angle the player is aiming
	float angleV;
	float angleH;
	
	//Player general info
	int playerID;
	int health;
	char* name;

	//If the player is shown on the radar (in line of sight)
	bool shown;

	//The side of the player (t, ct or spectator)
	int side;
};

class TriggerBot
{
public:
	TriggerBot();
	TriggerBot(MemoryManager* memoryManager);
	~TriggerBot();

	//Init inputs
	void initInputs();
	//Check the number of players in the game
	void updateSizeOfPlayers();
	//Updates the data for all the players
	void updateData();
	//If the crosshair is on an enemy
	bool crosshairOnEnemy();
	//Send click virtual input
	void trigger();
	//Stop virtual input
	void stop();
	
private:
	MemoryManager* memoryManager_;
	
	int crosshairTargetID_;

	Player* players_[MAX_N_PLAYER];
	int nPlayers_;

	INPUT mouseDown_;
	INPUT mouseUp_;
};

#endif