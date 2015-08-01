#include "memory_manager.h"



MemoryManager::MemoryManager()
{

}



MemoryManager::~MemoryManager()
{
	CloseHandle(process_);
}



bool MemoryManager::init()
{
	bool success = true;
	//Get a handle on the window of the game.
	window_ = FindWindow(0, "Counter-Strike: Global Offensive");
	if (window_ == 0)
	{
		cout << "Error cannot find window." << endl;
		success = false;
	}
	else
	{
		//Get the process ID from the window.
		GetWindowThreadProcessId(window_, &processID_);
		//Get a handle on the process from the process ID.
		process_ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID_);
		if (!process_)
		{
			cout << "Could not open the process!" << endl;
			success = false;
		}
		else
		{
			//Find a handle on the module we want from the process.
			clientModuleAdress_ = findModuleHandle(CLIENT_MODULE_NAME);
			if (clientModuleAdress_ == 0)
			{
				cout << "Could not find client module adress!" << endl;
				success = false;
			}
			else
			{
				//Find the radar base adress
				if (!findRadarBaseAdress())
				{
					cout << "Could not find radar base adress!" << endl;
					success = false;
				}
				//Find the crosshair adress
				if (!findCrosshairAdress())
				{
					cout << "Could not find crosshair adress!" << endl;
					success = false;
				}

			}
		}
	}
	return success;
}



bool MemoryManager::findNameInPath(string name, TCHAR* path, int nLetters)
{
	bool letterFound = false;
	int i = 0;

	//For every character in the path,
	for (int j = 0; j < nLetters; j++)
	{
		//if the backslashes are found,
		if (path[j] == '\\')
		{
			j++;
			letterFound = true;
			//check if this is the name we are looking for.
			for (int k = 0; k < name.size() && letterFound == true; k++)
			{
				//If this is the right letter.
				if (path[j + k] == name[k])
				{
					//If the letter is the same as the last letter
					if (path[j+k] == name[name.size() - 1])
					{
						//The name is found.
						return true;
					}
				}
				//If the letter is not right, go back at searching backslashes
				else
				{
					letterFound = false;
				}
			}
		}
	}
	return false;
}



HMODULE MemoryManager::findModuleHandle(string name)
{
	HMODULE modules[1024];
	DWORD nBytes;
	// Get a list of all the modules in this process.
	if (!EnumProcessModules(process_, modules, sizeof(modules), &nBytes))
	{
		cout << "Could not access process modules!" << endl;
	}
	else
	{
		int nModules = nBytes / sizeof(HMODULE);
		TCHAR path[MAX_PATH];
		int nLetters = sizeof(path) / sizeof(TCHAR);
		bool moduleFound = false;

		for (int i = 0; i < nModules; i++)
		{
			// Get the full path to the module's file.
			if (GetModuleFileNameEx(process_, modules[i], path, nLetters))
			{
				if (findNameInPath(name, path, nLetters))
				{
					return modules[i];
				}
			}
		}
	}
	return 0;
}



bool MemoryManager::findRadarBaseAdress()
{
	int staticPointerAdress = (int)clientModuleAdress_ + RADAR_STATIC_POINTER_OFFSET;
	int dynamicPointerAdress;
	//Read the adress of the dynamic pointer.
	if (!ReadProcessMemory(process_, (LPCVOID)(staticPointerAdress), &dynamicPointerAdress, sizeof(dynamicPointerAdress), NULL))
	{
		cout << "Could not read at static pointer adress!" << endl;
		return false;
	}
	else
	{
		//Add the offset to get the base adress of the radar structure.
		dynamicPointerAdress += RADAR_DYNAMIC_POINTER_OFFSET;
		//Read the base adress of the radar structure.
		if (!ReadProcessMemory(process_, (LPCVOID)(dynamicPointerAdress), &radarBaseAdress_, sizeof(radarBaseAdress_), NULL))
		{ 
			cout << "Could not read at dynamic pointer adress!" << endl;
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool MemoryManager::findCrosshairAdress()
{
	int staticPointerAdress = (int)clientModuleAdress_ + CROSSHAIR_STATIC_POINTER_OFFSET;
	int dynamicPointerAdress;
	if (!ReadProcessMemory(process_, (LPCVOID)(staticPointerAdress), &dynamicPointerAdress, sizeof(dynamicPointerAdress), NULL))
	{
		cout << "Could not read at static pointer adress!" << endl;
		return false;
	}
	else
	{
		//Add the offset to get the base adress of the structure.
		dynamicPointerAdress += CROSSHAIR_DYNAMIC_POINTER_OFFSET;
		//Read the base adress of the structure.
		if (!ReadProcessMemory(process_, (LPCVOID)(dynamicPointerAdress), &crosshairAdress_, sizeof(crosshairAdress_), NULL))
		{
			cout << "Could not read at dynamic pointer adress!" << endl;
			return false;
		}
		else
		{
			//Add the offset to get the crosshair adress
			crosshairAdress_ += CROSSHAIR_STRUCTURE_OFFSET;
			return true;
		}
	}
}

int MemoryManager::countPlayers()
{
	int count = 0;
	//For every player in the game
	while (readPlayerID(count) != 0)
	{
		//If the player is an enemy
		count++;
	}
	return count;

}

int MemoryManager::readCrosshairTargetID()
{
	int crosshairTargetID = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)(crosshairAdress_), &crosshairTargetID, sizeof(crosshairTargetID), NULL))
	{
		cout << "Could not read at crosshair adress!" << endl;
	}
	return crosshairTargetID;
}

float MemoryManager::readPlayerPosX(int player)
{
	int playerPosXAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_X_OFFSET;
	float posX;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosXAdress), &posX, sizeof(posX), NULL))
	{
		cout << "Could not read at position X adress!" << endl;
	}
	return posX;
}



float MemoryManager::readPlayerPosY(int player)
{
	int playerPosYAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_Y_OFFSET;
	float posY;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosYAdress), &posY, sizeof(posY), NULL))
	{
		cout << "Could not read at position Y adress!" << endl;
	}
	return posY;
}



float MemoryManager::readPlayerPosZ(int player)
{
	int playerPosZAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_Z_OFFSET;
	float posZ;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosZAdress), &posZ, sizeof(posZ), NULL))
	{
		cout << "Could not read at position Z adress!" << endl;
	}
	return posZ;
}



float MemoryManager::readPlayerAngleV(int player)
{
	int playerAngleVAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + ANGLE_V_OFFSET;
	float angleV;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerAngleVAdress), &angleV, sizeof(angleV), NULL))
	{
		cout << "Could not read at angle V adress!" << endl;
	}
	return angleV;
}



float MemoryManager::readPlayerAngleH(int player)
{
	int playerAngleHAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + ANGLE_H_OFFSET;
	float angleH;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerAngleHAdress), &angleH, sizeof(angleH), NULL))
	{
		cout << "Could not read at angle H adress!" << endl;
	}
	return angleH;
}



int MemoryManager::readPlayerID(int player)
{
	int playerIDAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_ID_OFFSET;
	int playerID;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerIDAdress), &playerID, sizeof(playerID), NULL))
	{
		cout << "Could not read at angle H adress!" << endl;
	}
	return playerID;
}



int MemoryManager::readPlayerHealth(int player)
{
	int playerHealthAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_HEALTH_OFFSET;
	int playerHealth;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerHealthAdress), &playerHealth, sizeof(playerHealth), NULL))
	{
		cout << "Could not read at player health adress!" << endl;
	}
	return playerHealth;
}



char* MemoryManager::readPlayerName(int player)
{
	int  playerNameAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_NAME_OFFSET;
	
	char playerName[20];

	if (!ReadProcessMemory(process_, (LPCVOID)(playerNameAdress), &playerName, 20, NULL))
	{
		coiut << "Could not read at player name adress!" << endl;
	}
	return playerName;
}


//t = 0xD, ct = 0x11
int MemoryManager::readPlayerSide(int player)
{
	int  playerSideAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_SIDE_OFFSET;
	char playerSide;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerSideAdress), &playerSide, sizeof(playerSide), NULL))
	{
		cout << "Could not read at playe side adress!" << endl;
	}
	return playerSide;
}


// 1 = true, 0 = false, 9 = ally
char MemoryManager::readPlayerShown(int player)
{
	int  playerShownAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_SHOWN_OFFSET;
	char playerShown;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerShownAdress), &playerShown, sizeof(playerShown), NULL))
	{
		cout << "Could not read at player shown adress!" << endl;
	}
	return playerShown;
}



