#include "triggerbot.h"

TriggerBot::TriggerBot()
{
	memoryManager_ = 0;
	nPlayers_ = 0;
}



TriggerBot::TriggerBot(MemoryManager* memoryManager)
{
	memoryManager_ = memoryManager;
	initInputs();
}



TriggerBot::~TriggerBot()
{	
	for (int i = 0; i < nPlayers_; i++)
	{
		if (players_[i] != 0)
		{
			delete players_[i];
		}
		players_[i] = 0;
	}
	initInputs();
}



void TriggerBot::initInputs()
{
	mouseDown_.type = INPUT_MOUSE;
	mouseDown_.mi.mouseData = 0;
	mouseDown_.mi.dx = 0;
	mouseDown_.mi.dy = 0;
	mouseDown_.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	mouseUp_.type = INPUT_MOUSE;
	mouseUp_.mi.mouseData = 0;
	mouseUp_.mi.dx = 0;
	mouseUp_.mi.dy = 0;
	mouseUp_.mi.dwFlags = MOUSEEVENTF_LEFTUP;
}



void TriggerBot::updateSizeOfPlayers()
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



void TriggerBot::updateData()
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
	crosshairTargetID_ = memoryManager_->readCrosshairTargetID();
}



bool TriggerBot::crosshairOnEnemy()
{
	int mySide = players_[0]->side;

	//If my crosshair is aiming another player
	if (crosshairTargetID_ > 0 && crosshairTargetID_< 24)
	{
		//And this player is on the enemmy side
		if (players_[crosshairTargetID_ - 1]->side != mySide)
		{
			return true;
		}
	}
	return false;
}



void TriggerBot::fire()
{
	//Send left mouse button down input
	SendInput(1, &mouseDown_, sizeof(mouseDown_));
}



void TriggerBot::stopFire()
{
	//Send left mouse button up input
	SendInput(1, &mouseUp_, sizeof(mouseUp_));
}



float TriggerBot::calculateAngleH(float posX, float posY)
{
	float mPosX = players_[0]->posX;
	float mPosY = players_[0]->posY;
	float dPosX = posX - mPosX;
	float dPosY = posY - mPosY;
	
	float angleH = atan(dPosY / dPosX) * 180 / PI;
	
	return angleH;
}