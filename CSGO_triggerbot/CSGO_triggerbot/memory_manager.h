#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <iostream>
#include "windows.h"
#include "Psapi.h"

using namespace std;



const string CLIENT_MODULE_NAME = "client.dll";

const int RADAR_STATIC_POINTER_OFFSET = 0x04A5202C;
const int RADAR_DYNAMIC_POINTER_OFFSET = 0x20;

const int CROSSHAIR_STATIC_POINTER_OFFSET = 0x04B337F8;
const int CROSSHAIR_DYNAMIC_POINTER_OFFSET = 0x68;
const int CROSSHAIR_STRUCTURE_OFFSET = 0x860;


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

	bool init();
	void close();

	bool findNameInPath(string name, TCHAR* path, int nLetters);
	HMODULE findModuleHandle(string name);

	bool findRadarBaseAdress();
	bool findCrosshairAdress();

	int countPlayers();
	
	int readCrosshairTargetID();

	//Memory reading functions

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
	//Handle to window
	HWND window_;
	
	//Process info
	HANDLE process_;
	DWORD processID_;

	//Module info
	HMODULE clientModuleAdress_;

	
	//Adresses to radar structure
	int radarBaseAdress_;
	int crosshairAdress_;
};


#endif
