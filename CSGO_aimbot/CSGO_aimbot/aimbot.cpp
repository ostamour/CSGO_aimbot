#include "aimbot.h"

AimBot::AimBot()
{
	memoryManager_ = 0;
	nPlayers_ = 0;
}


AimBot::AimBot(MemoryManager* memoryManager)
{
	memoryManager_ = memoryManager;
	nPlayers_ = 0;
}



AimBot::~AimBot()
{
	for (int i = 0; i < nPlayers_; i++)
	{
		if (players_[i] != 0)
		{
			delete players_[i];
		}
		players_[i] = 0;
	}
}

void AimBot::init()
{
	
	
	mouseRight_.type = INPUT_MOUSE;
	mouseRight_.mi.mouseData = 0;
	mouseRight_.mi.dx = 1;
	mouseRight_.mi.dy = 0;
	mouseRight_.mi.dwFlags = MOUSEEVENTF_MOVE;

	mouseLeft_.type = INPUT_MOUSE;
	mouseLeft_.mi.mouseData = 0;
	mouseLeft_.mi.dx = -1;
	mouseLeft_.mi.dy = 0;
	mouseLeft_.mi.dwFlags = 0;

	mouseUp_.type = INPUT_MOUSE;
	mouseUp_.mi.mouseData = 0;
	mouseUp_.mi.dx = 0;
	mouseUp_.mi.dy = -1;
	mouseUp_.mi.dwFlags = 0;

	mouseDown_.type = INPUT_MOUSE;
	mouseDown_.mi.mouseData = 0;
	mouseDown_.mi.dx = 0;
	mouseDown_.mi.dy = 1;
	mouseDown_.mi.dwFlags = 0;

	mouseStop_.type = INPUT_MOUSE;
	mouseStop_.mi.mouseData = 0;
	mouseStop_.mi.dx = 0;
	mouseStop_.mi.dy = 0;
	mouseStop_.mi.dwFlags = 0;
}

//Updates the data for all the players
void AimBot::updateData()
{
	//Have the right amoount of players before updating
	updateSizeOfPlayers();
	//For every player, update info from the memory
	for (int i = 0; i < nPlayers_; i++)
	{
		if (players_[i] != 0)
		{
			players_[i]->posX = memoryManager_->readPlayerPosX(i);
			players_[i]->posY = memoryManager_->readPlayerPosY(i);
			players_[i]->posZ = memoryManager_->readPlayerPosZ(i);
			players_[i]->angleV = memoryManager_->readPlayerAngleV(i);
			players_[i]->angleH = memoryManager_->readPlayerAngleH(i);
			players_[i]->name = memoryManager_->readPlayerName(i);
			players_[i]->side = memoryManager_->readPlayerSide(i);
			players_[i]->shown = memoryManager_->readPlayerShown(i);
			players_[i]->health = memoryManager_->readPlayerHealth(i);
			players_[i]->playerID = memoryManager_->readPlayerID(i);
		}
	}
}



void AimBot::updateSizeOfPlayers()
{
	//Get the difference in the number of players from the memory and the container
	int dPlayers = memoryManager_->countPlayers() - nPlayers_;
	//Add the players missing in the container
	for (int i = 0; i < dPlayers; i++)
	{
		players_[nPlayers_] = new Player();
		nPlayers_++;
	}
}


int AimBot::findClosestEnemy()
{
	int closestEnemy = 0;
	//For every player in the game
	for (int i = 0; i < nPlayers_; i++)
	{
		//If the player is shown as an enemy and is alive	
		if (players_[i]->shown == true && players_[i]->health > 0)
		{
			//Check if is the closest
			if (calculateDistanceXYZ(i) < calculateDistanceXYZ(closestEnemy))
			{
				closestEnemy = i;
			}
		}
	}
	return closestEnemy;
}



float AimBot::calculateDistanceX(int playerID)
{
	return players_[0]->posX - players_[playerID]->posX;
}



float AimBot::calculateDistanceY(int playerID)
{
	return players_[0]->posY - players_[playerID]->posY;
}



float AimBot::calculateDistanceZ(int playerID)
{
	return players_[0]->posZ - players_[playerID]->posZ;
}


float AimBot::calculateDistanceXY(int playerID)
{
	float dPosX = calculateDistanceX(playerID);
	float dPosY = calculateDistanceY(playerID);
	//Pythagorean theorem : c^2 = a^2 + b^2.
	double distanceSquared = dPosX * dPosX + dPosY * dPosY;

	return sqrt(distanceSquared);
}

float AimBot::calculateDistanceXYZ(int playerID)
{
	float dPosX = calculateDistanceX(playerID);
	float dPosY = calculateDistanceY(playerID);
	float dPosZ = calculateDistanceZ(playerID);
	//Pythagorean theorem
	double distanceSquared = dPosX * dPosX + dPosY * dPosY + dPosZ * dPosZ;
	return sqrt(distanceSquared);
}

float AimBot::calculateAngleH(int playerID)
{
	float dPosX = calculateDistanceX(playerID);
	float dPosY = calculateDistanceY(playerID);

	float angle = atan(dPosY / dPosX) * 180 / PI;
	//Angle corrections because the atan function gives the angle closest to the x axis
	if (dPosX > 0 && dPosY < 0)
	{
		angle += 180;
	}
	if (dPosX > 0 && dPosY > 0)
	{
		angle -= 180;
	}
	
	if (dPosX < 0 && dPosY == 0)
	{
		angle = 179.99;
	}

	if (dPosX == 0)
	{
		angle = -angle;
	}
	
	return angle;
}



float AimBot::calculateAngleV(int playerID)
{
	float distance = calculateDistanceXY(playerID);
	float dPosZ = calculateDistanceZ(playerID);

	return atan(dPosZ / distance) * 180 / PI;
}

void AimBot::aim()
{	

}


//Place the crosshair at the right horizontal angle
void AimBot::placeCrosshairH(int playerID)
{
	float dAngle = players_[0]->angleH - calculateAngleH(playerID);
	
	if ( dAngle < -0.5)
	{
		if (!SendInput(1, &mouseLeft_, sizeof(mouseLeft_)))
		{
			cout << GetLastError() << endl;
		}
	}
	else if (dAngle > 0.5)
	{
		if (!SendInput(1, &mouseRight_, sizeof(mouseLeft_)))
		{
			cout << GetLastError() << endl;
		}
	}
	SendInput(1, &mouseStop_, sizeof(mouseStop_));
}