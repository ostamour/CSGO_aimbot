#ifndef AIMBOT
#define AIMBOT

#include "memory_manager.h"
#include <cmath>

#define PI 3.14159

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

class AimBot
{
public:
	AimBot();
	AimBot(MemoryManager* memoryManager);
	~AimBot();
	
	//Updates the data for all the players
	void updateData();

	//Check the number of players in the game
	void updateSizeOfPlayers();

	//Find the closest enemy
	int findClosestEnemy();

	//Calculate the player's distance from the player 1 on the X axis.
	float calculateDistanceX(int playerID);
	//Calculate the player's distance from the player 1 on the Y axis.
	float calculateDistanceY(int playerID);
	//Calculate the player's distance from the player 1 on the Z axis.
	float calculateDistanceZ(int playerID);
	//Calculate the player's distance from the player 1 on the XY plan.
	float calculateDistanceXY(int playerID);

	//Calculate the horizontal angle the player 1 must have to be aiming at an enemy player.
	float calculateAngleH(int playerID);

	//Calculate the vertical angle the player 1 must have to be aiming at an enemy player.
	float calculateAngleV(int playerID);

	//Place the crosshair at the right horizontal angle
	void placeCrosshairH();
	//Place the crosshair at the right vertical angle
	void placeCrosshairV();

	//Aim at the head of the closest enemy
	void aim();

private:
	MemoryManager* memoryManager_;
	Player* players_[MAX_N_PLAYER];
	int nPlayers_;
};

#endif