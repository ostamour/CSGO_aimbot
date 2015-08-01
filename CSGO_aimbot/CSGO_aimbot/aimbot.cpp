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
			players_[i]->angleV = memoryManager_->readPlayerAngleH(i);
			players_[i]->name = memoryManager_->readPlayerName(i);
			players_[i]->side = memoryManager_->readPlayerSide(i);
			players_[i]->shown = memoryManager_->readPlayerShown(i);
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


int findClosestEnemy()
{
	return 0;

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
	double cSquared = dPosX * dPosX + dPosY * dPosY;

	return sqrt(cSquared);
}


float AimBot::calculateAngleH(int playerID)
{
	float dPosX = calculateDistanceX(playerID);
	float dPosY = calculateDistanceY(playerID);

	return atan(dPosY / dPosX) * 180 / PI;
}



float AimBot::calculateAngleV(int playerID)
{
	float distance = calculateDistanceXY(playerID);
	float dPosZ = calculateDistanceZ(playerID);

	return atan(dPosZ / distance) * 180 / PI;
}