#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <iostream>
#include "windows.h"

using namespace std;



class MemoryManager
{
public:

	MemoryManager();
	~MemoryManager();

	void findWordInPath(string word, TCHAR* path, int nLetters);

	void init();
	
	float readPlayerPosX(int playerID);
	float readPlayerPosY(int playerID);

	float readPlayerAngleH(int playerID);
	float readPlayerAngleV(int playerID);

	float readPlayerShown(int playerID);

	float readPlayerSide(int playerID);



private:
	//Process info
	HANDLE process_;
	DWORD processID_;

	//Module info
	string moduleName_;
	HMODULE moduleAdress_;
	
	//Adresses to radar structure
	int radarBasePointerAdress_;
	int radarBaseAdress_;
};


#endif
