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
				engineModuleAdress_ = findModuleHandle(ENGINE_MODULE_NAME);
				if (engineModuleAdress_ == 0)
				{
					cout << "Could not find engine module adress!" << endl;
					success = false;
				}
				else
				{
					findEntityListAdress();
					if (!findPlayerBaseAdresses())
					{
						cout << "Could not find playerbase adresses!" << endl;
						success = false;
					}
					else
					{
						if(!findBoneMatrixAdresses())
						{
							cout << "Could not find bone matrix adresses!" << endl;
							success = false;
						}
						else
						{
							if (!findClientStateAdress())
							{
								cout << "Could not find client state adress!" << endl;
								success = false;
							}
						}
					}
				}
			}
		}
	}
	return success;
}



void MemoryManager::close()
{

}



bool MemoryManager::findNameInPath(string name, TCHAR* path, int nLetters)
{
	bool letterFound = false;
	int nRightLetters = 0;

	//For every character in the path.
	for (int i = 0; i < nLetters; i++)
	{
		//If the backslashes are found.
		if (path[i] == '\\')
		{
			i++;
			letterFound = true;
			//Check if this is the name we are looking for.
			for (int j = 0; j < name.size() && letterFound == true; j++)
			{
				//If this is the right letter.
				if (path[i + j] == name[j])
				{
					nRightLetters++;
					//If all the letters in the name are right.
					if (nRightLetters == name.size())
					{
						//The name is found.
						return true;
					}
				}
				//If the letter is not right, go back at searching backslashes.
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
				//Search for the right module with the path.
				if (findNameInPath(name, path, nLetters))
				{
					return modules[i];
				}				
			}
		}
	}
	return 0;
}



int MemoryManager::countPlayers()
{
	int count = 0;
	//Check the next pointer until it's null (end of the list).
	while (readPlayerBaseAdress(count) != 0)
	{
		count++;
	}
	return count;
}



float MemoryManager::calculateRefreshRate(int adress)
{
	int value = readAdressInMemory(adress);
	LARGE_INTEGER tick1, tick2;
	long long time;
	bool changed = false;

	//Check the time at the start of the scan.
	QueryPerformanceCounter(&tick1);
	for (int i = 0; i < 100; i++)
	{
		while (!changed)
		{
			//If the value has changed
			if (value != readAdressInMemory(adress))
			{
				changed = true;
			}
			value = readAdressInMemory(adress);
		}
		changed = false;
	}
	//Check the time at the end of the scan.
	QueryPerformanceCounter(&tick2);
	//Get the time average in microseconds.
	time = (tick2.QuadPart - tick1.QuadPart) / 100.0;
	//Convert the time in seconds (1 000 000 us / s).
	float timeS = time / 1000000.0;
	//Return the frequency (f = 1 / t).
	return 1.0 / timeS;
}



void MemoryManager::findEntityListAdress()
{
	entityListAdress_ = (int)clientModuleAdress_ + ENTITY_LIST_OFFSET;
}



bool MemoryManager::findPlayerBaseAdresses()
{
	bool success = false;
	int nPlayers = countPlayers();
	for (int i = 0; i < nPlayers; i++)
	{
		playerBaseAdresses_[i] = readPlayerBaseAdress(i);
		success = true;
	}
	return success;
}

bool MemoryManager::findBoneMatrixAdresses()
{
	bool success = false;
	int nPlayers = countPlayers();
	for (int i = 0; i < nPlayers; i++)
	{
		boneMatrixAdresses_[i] = readBoneMatrixAdress(i);
		success = true;
	}
	return success;
}

bool MemoryManager::findClientStateAdress()
{
	bool success = true;
	int ClientStatePointerAdress = (int)engineModuleAdress_ + CLIENT_STATE_OFFSET;
	if (!ReadProcessMemory(process_, (LPCVOID)ClientStatePointerAdress, &clientStateAdress_, sizeof(clientStateAdress_), NULL))
	{
		cout << "Could not read at client state pointer adress!" << endl;
		success = false;
	}
	return success;
}



int MemoryManager::readPlayerBaseAdress(int playerID)
{
	int playerBasePointerAdress = entityListAdress_ + PLAYERBASE_OFFSET * playerID;
	int playerBaseAdress = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)playerBasePointerAdress, &playerBaseAdress, sizeof(playerBaseAdress), NULL))
	{
		cout << "Could not read at static pointer adress!" << endl;
		return 0;
	}
	return playerBaseAdress;

}



int MemoryManager::readBoneMatrixAdress(int playerID)
{
	int boneMatrixPointerAdress = playerBaseAdresses_[playerID] + BONE_MATRIC_OFFSET;
	int boneMatrixAdress = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)boneMatrixPointerAdress, &boneMatrixAdress, sizeof(boneMatrixAdress), NULL))
	{
		cout << "Could not read at bone matrix pointer adress" << endl;
		return 0;
	}
	return boneMatrixAdress;
}



float MemoryManager::readPosX(int player)
{
	int posXAdress = playerBaseAdresses_[player] + POSITION_X_OFFSET;
	float posX;
	if (!ReadProcessMemory(process_, (LPCVOID)posXAdress, &posX, sizeof(posX), NULL))
	{
		cout << "Could not read at position X adress!" << endl;
	}
	return posX;
}



float MemoryManager::readPosY(int player)
{
	int posYAdress = playerBaseAdresses_[player] + POSITION_Y_OFFSET;
	float posY;
	if (!ReadProcessMemory(process_, (LPCVOID)posYAdress, &posY, sizeof(posY), NULL))
	{
		cout << "Could not read at position Y adress!" << endl;
	}
	return posY;
}



float MemoryManager::readPosZ(int player)
{
	int posZAdress = playerBaseAdresses_[player]  + POSITION_Z_OFFSET;
	float posZ;
	if (!ReadProcessMemory(process_, (LPCVOID)posZAdress, &posZ, sizeof(posZ), NULL))
	{
		cout << "Could not read at position Z adress!" << endl;
	}
	return posZ;
}



float MemoryManager::readHeadPosX(int player)
{
	int headPosXAdress = boneMatrixAdresses_[player] + ((BONE_STRUCT_SIZE * HEAD_BONE_ID) + HEAD_POSITION_X_OFFSET);
	float headPosX;
	if (!ReadProcessMemory(process_, (LPCVOID)headPosXAdress, &headPosX, sizeof(headPosX), NULL))
	{
		cout << "Could not read at position Z adress!" << endl;
	}
	return headPosX;
}



float MemoryManager::readHeadPosY(int player)
{
	int headPosYAdress = boneMatrixAdresses_[player] +((BONE_STRUCT_SIZE * HEAD_BONE_ID) + HEAD_POSITION_Y_OFFSET);
	float headPosY;
	if (!ReadProcessMemory(process_, (LPCVOID)headPosYAdress, &headPosY, sizeof(headPosY), NULL))
	{
		cout << "Could not read at position Z adress!" << endl;
	}
	return headPosY;
}



float MemoryManager::readHeadPosZ(int player)
{
	int headPosZAdress = boneMatrixAdresses_[player] + ((BONE_STRUCT_SIZE * HEAD_BONE_ID) + HEAD_POSITION_Z_OFFSET);
	float headPosZ;
	if (!ReadProcessMemory(process_, (LPCVOID)headPosZAdress, &headPosZ, sizeof(headPosZ), NULL))
	{
		cout << "Could not read at position Z adress!" << endl;
	}
	return headPosZ;
}



float MemoryManager::readAngleV(int player)
{
	int angleVAdress = playerBaseAdresses_[player] + ANGLE_V_OFFSET;
	float angleV;
	if (!ReadProcessMemory(process_, (LPCVOID)angleVAdress, &angleV, sizeof(angleV), NULL))
	{
		cout << "Could not read at angle V adress!" << endl;
	}
	return angleV;
}



float MemoryManager::readAngleH(int player)
{
	int angleHAdress = playerBaseAdresses_[player] + ANGLE_H_OFFSET;
	float angleH;
	if (!ReadProcessMemory(process_, (LPCVOID)angleHAdress, &angleH, sizeof(angleH), NULL))
	{
		cout << "Could not read at angle H adress!" << endl;
	}
	return angleH;
}



float MemoryManager::readLocalAngleV()
{
	int angleVAdress = clientStateAdress_ + VIEW_ANGLES_V_OFFSET;
	float angleV = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)angleVAdress, &angleV, sizeof(angleV), NULL))
	{
		cout << "Could not read at angle V adress!" << endl;
	}
	return angleV;
}



float MemoryManager::readLocalAngleH()
{
	int angleHAdress = clientStateAdress_ + VIEW_ANGLES_H_OFFSET;
	float angleH = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)angleHAdress, &angleH, sizeof(angleH), NULL))
	{
		cout << "Could not read at angle H adress!" << endl;
	}
	return angleH;
}


//[-89.00, 89.00]
bool MemoryManager::writeLocalAngleV(float angleV)
{
	bool success = true;
	int angleVAdress = clientStateAdress_ + VIEW_ANGLES_V_OFFSET;
	if (!WriteProcessMemory(process_, (LPVOID)angleVAdress, &angleV, sizeof(angleV), NULL))
	{
		cout << "Could not read at angle V adress!" << endl;
		success = false;
	}
	return success;
}



bool MemoryManager::writeLocalAngleH(float angleH)
{
	bool success = true;
	int angleHAdress = clientStateAdress_ + VIEW_ANGLES_H_OFFSET;
	if (!WriteProcessMemory(process_, (LPVOID)angleHAdress, &angleH, sizeof(angleH), NULL))
	{
		cout << "Could not read at angle V adress!" << endl;
		success = false;
	}
	return success;
}



int MemoryManager::readID(int player)
{
	int playerIDAdress = playerBaseAdresses_[player] + PLAYER_ID_OFFSET;
	int playerID;
	if (!ReadProcessMemory(process_, (LPCVOID)playerIDAdress, &playerID, sizeof(playerID), NULL))
	{
		cout << "Could not read at player ID adress!" << endl;
	}
	return playerID;
}



int MemoryManager::readHealth(int player)
{
	int healthAdress = playerBaseAdresses_[player] + HEALTH_OFFSET;
	int health;
	if (!ReadProcessMemory(process_, (LPCVOID)healthAdress, &health, sizeof(health), NULL))
	{
		cout << "Could not read at player health adress!" << endl;
	}
	return health;
}


//t = 0xD, ct = 0x11
int MemoryManager::readSide(int player)
{
	int  playerSideAdress = playerBaseAdresses_[player]  + SIDE_OFFSET;
	char playerSide;
	if (!ReadProcessMemory(process_, (LPCVOID)playerSideAdress, &playerSide, sizeof(playerSide), NULL))
	{
		cout << "Could not read at playe side adress!" << endl;
	}
	return playerSide;
}


// 1 = true, 0 = false, 9 = ally
char MemoryManager::readShown(int player)
{
	int  playerShownAdress = playerBaseAdresses_[player] + SHOWN_OFFSET;
	char playerShown;
	if (!ReadProcessMemory(process_, (LPCVOID)playerShownAdress, &playerShown, sizeof(playerShown), NULL))
	{
		cout << "Could not read at player shown adress!" << endl;
	}
	return playerShown;
}



int MemoryManager::readCrosshairTargetID(int player)
{
	int crosshairIDAdress = playerBaseAdresses_[player] + CROSSHAIR_ID_OFFSET;
	int crosshairTargetID = 0;
	if (!ReadProcessMemory(process_, (LPCVOID)crosshairIDAdress, &crosshairTargetID, sizeof(crosshairTargetID), NULL))
	{
		cout << "Could not read at crosshair adress!" << endl;
	}
	return crosshairTargetID;
}



int MemoryManager::readAdressInMemory(int adress)
{
	int value;
	ReadProcessMemory(process_, (LPCVOID)adress, &value, sizeof(value), NULL);
	return value;
}


