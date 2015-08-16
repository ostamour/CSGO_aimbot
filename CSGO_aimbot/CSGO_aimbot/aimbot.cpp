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


//Updates the data for all the players
void AimBot::updateData()
{
	//Have the right amount of players before updating
	updateSizeOfPlayers();
	//For every player, update info from the memory
	for (int i = 0; i < nPlayers_; i++)
	{
		if (players_[i] != 0)
		{
			players_[i]->posX = memoryManager_->readPosX(i);
			players_[i]->posY = memoryManager_->readPosY(i);
			players_[i]->posZ = memoryManager_->readPosZ(i);
			players_[i]->angleV = memoryManager_->readAngleV(i);
			players_[i]->angleH = memoryManager_->readAngleH(i);
			players_[i]->side = memoryManager_->readSide(i);
			players_[i]->shown = memoryManager_->readShown(i);
			players_[i]->health = memoryManager_->readHealth(i);
			players_[i]->playerID = memoryManager_->readID(i);
		}
	}

}

bool AimBot::checkDataUpdate()
{
	bool success = false;
	if (players_[0]->posX != memoryManager_->readPosX(0))
	{
		players_[0]->posX = memoryManager_->readPosX(0);
		success = true;
	}
	return success;
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
	//For every player in the game.
	for (int i = 0; i < nPlayers_; i++)
	{
		//If the player is shown as an enemy and is alive.	
		if (players_[i]->shown == true && players_[i]->health > 0 && players_[i] ->side != players_[0]->side)
		{
			//Check if he is the closest.
			if (calculateDistanceXYZ(i) < calculateDistanceXYZ(closestEnemy) || closestEnemy == 0)
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
	int playerID = findClosestEnemy();
	placeCrosshairH(playerID);
	placeCrosshairV(playerID);
}


//Place the crosshair at the right horizontal angle.
void AimBot::placeCrosshairH(int playerID)
{
	float angleH = calculateAngleH(playerID);
	if (playerID != 0)
	{
		memoryManager_->writeLocalAngleH(angleH);
	}
}

void AimBot::placeCrosshairV(int playerID)
{
	float angleV = calculateAngleV(playerID);
	if (playerID != 0)
	{
		memoryManager_->writeLocalAngleV(angleV);
	}
}

