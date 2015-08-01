#include "memory_manager.h"



MemoryManager::MemoryManager()
{

}



MemoryManager::~MemoryManager()
{
	close();
}



void MemoryManager::init()
{
	//Get a handle on the window of the game.
	window_ = FindWindow(0, "Counter-Strike: Global Offensive");
	if (window_ == 0)
	{
		MessageBox(0, "Error cannot find window.", "Error", MB_OK | MB_ICONERROR);
	}
	else
	{
		//Get the process ID from the window.
		GetWindowThreadProcessId(window_, &processID_);
		//Get a handle on the process from the process ID.
		process_ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID_);
		if (!process_)
		{
			MessageBox(0, "Could not open the process!", "Error!", MB_OK | MB_ICONERROR);
		}
		else
		{
			//Find a handle on the module we want from the process.
			clientModuleAdress_ = findModuleHandle(CLIENT_MODULE_NAME);
			if (clientModuleAdress_ == 0)
			{
				MessageBox(0, "Could not find client module adress!", "Error!", MB_OK | MB_ICONERROR);
			}
			else
			{
				//Find the radar base adress
				if (!findRadarBaseAdress())
				{
					MessageBox(0, "Could not find radar base adress!", "Error!", MB_OK | MB_ICONERROR);
				}
				//Find the crosshair adress
				if (!findCrosshairAdress())
				{
					MessageBox(0, "Could not find crosshair adress!", "Error!", MB_OK | MB_ICONERROR);
				}

			}
		}
	}
}

void MemoryManager::close()
{
	CloseHandle(process_);
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
		MessageBox(0, "Could not access process modules!", "Error!", MB_OK | MB_ICONERROR);
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
		MessageBox(0, "Could not read at static pointer adress!", "Error!", MB_OK | MB_ICONERROR);
		return false;
	}
	else
	{
		//Add the offset to get the base adress of the radar structure.
		dynamicPointerAdress += RADAR_DYNAMIC_POINTER_OFFSET;
		//Read the base adress of the radar structure.
		if (!ReadProcessMemory(process_, (LPCVOID)(dynamicPointerAdress), &radarBaseAdress_, sizeof(radarBaseAdress_), NULL))
		{ 
			MessageBox(0, "Could not read at dynamic pointer adress!", "Error!", MB_OK | MB_ICONERROR);
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
		MessageBox(0, "Could not read at static pointer adress!", "Error!", MB_OK | MB_ICONERROR);
		return false;
	}
	else
	{
		//Add the offset to get the base adress of the structure.
		dynamicPointerAdress += CROSSHAIR_DYNAMIC_POINTER_OFFSET;
		//Read the base adress of the structure.
		if (!ReadProcessMemory(process_, (LPCVOID)(dynamicPointerAdress), &crosshairAdress_, sizeof(crosshairAdress_), NULL))
		{
			MessageBox(0, "Could not read at dynamic pointer adress!", "Error!", MB_OK | MB_ICONERROR);
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
		MessageBox(0, "Could not read at crosshair adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return crosshairTargetID;
}

float MemoryManager::readPlayerPosX(int player)
{
	int playerPosXAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_X_OFFSET;
	float posX;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosXAdress), &posX, sizeof(posX), NULL))
	{
		MessageBox(0, "Could not read at position X adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return posX;
}



float MemoryManager::readPlayerPosY(int player)
{
	int playerPosYAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_Y_OFFSET;
	float posY;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosYAdress), &posY, sizeof(posY), NULL))
	{
		MessageBox(0, "Could not read at position Y adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return posY;
}



float MemoryManager::readPlayerPosZ(int player)
{
	int playerPosZAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + POSITION_Z_OFFSET;
	float posZ;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerPosZAdress), &posZ, sizeof(posZ), NULL))
	{
		MessageBox(0, "Could not read at position Z adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return posZ;
}



float MemoryManager::readPlayerAngleV(int player)
{
	int playerAngleVAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + ANGLE_V_OFFSET;
	float angleV;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerAngleVAdress), &angleV, sizeof(angleV), NULL))
	{
		MessageBox(0, "Could not read at angle V adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return angleV;
}



float MemoryManager::readPlayerAngleH(int player)
{
	//Get the adress where to read
	int playerAngleHAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + ANGLE_H_OFFSET;
	float angleH;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerAngleHAdress), &angleH, sizeof(angleH), NULL))
	{
		MessageBox(0, "Could not read at angle H adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return angleH;
}



int MemoryManager::readPlayerID(int player)
{
	int playerIDAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_ID_OFFSET;
	int playerID;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerIDAdress), &playerID, sizeof(playerID), NULL))
	{
		MessageBox(0, "Could not read at angle H adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return playerID;
}



int MemoryManager::readPlayerHealth(int player)
{
	int playerHealthAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_HEALTH_OFFSET;
	int playerHealth;
	if (!ReadProcessMemory(process_, (LPCVOID)(playerHealthAdress), &playerHealth, sizeof(playerHealth), NULL))
	{
		MessageBox(0, "Could not read at player health adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return playerHealth;
}



char* MemoryManager::readPlayerName(int player)
{
	int  playerNameAdress = radarBaseAdress_ + player * SIZE_OF_RADAR_STRUCTURE + PLAYER_NAME_OFFSET;
	
	char playerName[20];

	if (!ReadProcessMemory(process_, (LPCVOID)(playerNameAdress), &playerName, 20, NULL))
	{
		MessageBox(0, "Could not read at player name adress!", "Error!", MB_OK | MB_ICONERROR);
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
		MessageBox(0, "Could not read at playe side adress!", "Error!", MB_OK | MB_ICONERROR);
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
		MessageBox(0, "Could not read at player shown adress!", "Error!", MB_OK | MB_ICONERROR);
	}
	return playerShown;
}



