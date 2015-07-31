#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <iostream>
#include "windows.h"
#include "Psapi.h"

using namespace std;



const string MODULE_NAME = "client.dll";
const int STATIC_POINTER_OFFSET = 0x04A5202C;
const int DYNAMIC_POINTER_OFFSET = 0x20;
const int POSITION_X_OFFSET = 0x1C0;
const int POSITION_Y_OFFSET = 0x1C4;
const int POSITION_Z_OFFSET = 0x1C8;
const int ANGLE_V_OFFSET = 0x1CC;
const int ANGLE_H_OFFSET = 0x1D0;
const int PLAYER_ID_OFFSET = 0x208;
const int PLAYER_HEALTH_OFFSET = 0x20C;
const int PLAYER_NAME_OFFSET = 0x210;
const int PLAYER_SIDE_OFFSET = 0x314;
const int PLAYER_SHOWN_OFFSET = 0x325;
const int SIZE_OF_RADAR_STRUCTURE = 0x1EC;



class MemoryManager
{
public:

	MemoryManager();
	~MemoryManager();

	void init();
	void close();

	bool findNameInPath(TCHAR* path, int nLetters);
	HMODULE findModuleHandle();

	bool findRadarBaseAdress();

	int countPlayers();
	
	float readPlayerPosX(int player);
	float readPlayerPosY(int player);
	float readPlayerPosZ(int player);

	float readPlayerAngleV(int player);
	float readPlayerAngleH(int player);

	int readPlayerID(int player);
	int readPlayerHealth(int player);
	char* readPlayerName(int player);


	int readPlayerSide(int player);

	char readPlayerShown(int player);

	



private:
	HWND window_;
	
	//Process info
	HANDLE process_;
	DWORD processID_;

	//Module info
	HMODULE moduleAdress_;
	
	//Adresses to radar structure
	int radarBaseAdress_;
};


#endif
