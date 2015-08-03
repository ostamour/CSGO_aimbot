#ifndef AIMBOT
#define AIMBOT

#include "memory_manager.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PI 3.14159

const int MAX_N_PLAYER = 24;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


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
	char shown;

	//The side of the player (t, ct or spectator)
	int side;
};

class AimBot
{
public:
	AimBot();
	AimBot(MemoryManager* memoryManager);
	~AimBot();
	
	void init();
	
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

	//Calculate the player's distance from the player 1 in the XYZ space.
	float calculateDistanceXYZ(int playerID);

	//Calculate the horizontal angle the player 1 must have to be aiming at an enemy player.
	float calculateAngleH(int playerID);

	//Calculate the vertical angle the player 1 must have to be aiming at an enemy player.
	float calculateAngleV(int playerID);

	
	float calculateMouseAngleRatio();

	//Check if the crosshair is on the player on the H axis
	bool crosshairIsOnHeadH(int playerID);

	//Check if the crosshair is on the player on the V axis
	bool crosshairIsOnHeadV(int playerID);
	
	

	//Place the crosshair at the right horizontal angle
	void placeCrosshairH(int playerID);

	//Place the crosshair at the right vertical angle
	void placeCrosshairV();

	void moveCursor(int posX, int posY);

	//Aim at the head of the closest enemy
	void aim();

private:
	MemoryManager* memoryManager_;
	Player* players_[MAX_N_PLAYER];
	int nPlayers_;
	int count_;

	POINT cursorPos_;
	float pixelAngleRation_;
	float oldDAngleH_;
	float oldAngleV_;
	INPUT mouseInput_;

};

#endif