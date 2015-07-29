#include "game.h"

Game::Game()
{
	memoryManager_ = 0;
}

Game::Game(MemoryManager* memoryManager)
{
	memoryManager_ = memoryManager;
}
Game::~Game()
{

}

void Game::updatePlayers()
{


}