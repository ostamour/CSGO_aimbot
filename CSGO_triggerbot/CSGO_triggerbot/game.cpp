#include "game.h"

Game::Game()
{
	memoryManager_ = 0;
}



Game::Game(MemoryManager* memoryManager)
{
	memoryManager_ = memoryManager;
	for (int i = 0; i < memoryManager->countPlayers(); i++)
	{
		players_.push_back(new Player());
	}
}



Game::~Game()
{
	

}



void Game::updatePlayers()
{
	
	int dPlayers = memoryManager_->countPlayers() - players_.size();

	if (dPlayers != 0)
	{
		for (int i = 0; i < dPlayers; i++)
		{
			players_.push_back(new Player());
		}
	}

	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->posX = memoryManager_->readPlayerPosX(i);
		players_[i]->posY = memoryManager_->readPlayerPosY(i);
		players_[i]->posZ = memoryManager_->readPlayerPosZ(i);
		players_[i]->angleV = memoryManager_->readPlayerAngleV(i);
		players_[i]->angleV = memoryManager_->readPlayerAngleH(i);
		players_[i]->name = memoryManager_->readPlayerName(i);
	}
}