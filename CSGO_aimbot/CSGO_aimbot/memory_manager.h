#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <iostream>
#include "windows.h"
#include "Psapi.h"

using namespace std;


const int MAX_N_PLAYER = 24;

const string CLIENT_MODULE_NAME = "client.dll";
const string ENGINE_MODULE_NAME = "engine.dll";

const int HEAD_BONE_ID = 11;

const int ENTITY_LIST_OFFSET = 0x04A22594;

const int PLAYERBASE_OFFSET = 0x10;

const int POSITION_X_OFFSET = 0x0134;
const int POSITION_Y_OFFSET = 0x0138;
const int POSITION_Z_OFFSET = 0x013C;

const int BONE_MATRIC_OFFSET = 0x0A78;

const int ANGLE_V_OFFSET = 0x158C;
const int ANGLE_H_OFFSET = 0x1590;

const int PLAYER_ID_OFFSET = 0x0064;

const int HEALTH_OFFSET = 0x00FC;
const int ARMOR_OFFSET = 0x23B0;

const int SIDE_OFFSET = 0x00F0;
const int SHOWN_OFFSET = 0x0935;

const int CROSSHAIR_ID_OFFSET = 0x2410;

const int BONE_MATRIX_OFFSET = 0xA78;

const int BONE_STRUCT_SIZE = 0x30;

const int CLIENT_STATE_OFFSET = 0x005D0224;

const int VIEW_ANGLES_V_OFFSET = 0x00004CE0;
const int VIEW_ANGLES_H_OFFSET = 0x00004CE4;



class MemoryManager
{
public:

	MemoryManager();
	~MemoryManager();

	bool init();
	void close();

	bool findNameInPath(string name, TCHAR* path, int nLetters);
	HMODULE findModuleHandle(string name);

	int countPlayers();
	float calculateRefreshRate(int adress);


	void findEntityListAdress();
	bool findPlayerBaseAdresses();
	bool findBoneMatrixAdresses();
	bool findClientStateAdress();
	
	//Memory reading functions

	int readPlayerBaseAdress(int playerID);
	int readBoneMatrixAdress(int playerID);
	

	float readPosX(int player);
	float readPosY(int player);
	float readPosZ(int player);

	float readHeadPosX(int player);
	float readHeadPosY(int player);
	float readHeadPosZ(int player);

	float readAngleV(int player);
	float readAngleH(int player);

	float readLocalAngleV();
	float readLocalAngleH();
	
	bool writeLocalAngleV(float angleV);
	bool writeLocalAngleH(float angleH);

	int readID(int player);
	int readHealth(int player);

	int readSide(int player);
	char readShown(int player);

	int readCrosshairTargetID(int player);

	int readAdressInMemory(int adress);
	

private:
	//Handle to window
	HWND window_;
	
	//Process info
	HANDLE process_;
	DWORD processID_;

	//Module info
	HMODULE clientModuleAdress_;
	HMODULE engineModuleAdress_;
	
	//Adresses
	int entityListAdress_;
	int clientStateAdress_;
	int playerBaseAdresses_[MAX_N_PLAYER];
	int boneMatrixAdresses_[MAX_N_PLAYER];
};


#endif
