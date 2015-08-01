#ifndef GAME
#define GAME

#include <vector>
#include "memory_manager.h"
#include <string>
#include <cmath>

#define PI 3.14159

const int MAX_N_PLAYER = 24;

using namespace std;

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
	void fire();
	//Stop virtual input
	void stopFire();
	//Calculate the vertical angle the player 1 must have to be aiming at an enemy player.
	float calculateVAngle();
	//Calculate the horizontal angle the player 1 must have to be aiming at an enemy player
	float calculateAngleH(float posX, float posY);

	int findClosestEnemyPlayer();
	
private:
	int crosshairTargetID_;

	Player* players_[MAX_N_PLAYER];
	int nPlayers_;

	MemoryManager* memoryManager_;

	INPUT mouseDown_;
	INPUT mouseUp_;
};

#endif