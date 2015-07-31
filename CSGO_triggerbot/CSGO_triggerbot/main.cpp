#include <iostream>
#include <windows.h>
#include <string>
#include <winternl.h>
#include <Psapi.h>
#include "memory_manager.h"
#include "game.h"
using namespace std;




int main()
{

	MemoryManager* memoryManager = new MemoryManager();
	memoryManager->init();
	int nPlayers = memoryManager->countPlayers();
	Game* game = new Game(memoryManager);
	for (;;)
	{
		game->updatePlayers();
	}
	/*
	POINT p;

	GetCursorPos(&p);

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dx = 0;
	input.mi.dy = 0;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	INPUT input2;
	input2.type = INPUT_MOUSE;
	input2.mi.dx = 0;
	input2.mi.dy = 0;
	input2.mi.mouseData = 0;

	input2.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	Sleep(2000);

	SendInput(1, &input, sizeof(input));

	Sleep(10000);

	SendInput(1, &input2, sizeof(input2));
	*/
	
	return 0;
}