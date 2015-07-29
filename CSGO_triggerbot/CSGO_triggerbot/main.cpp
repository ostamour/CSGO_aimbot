#include <iostream>
#include <windows.h>
#include <string>
#include <winternl.h>
#include <Psapi.h>
using namespace std;

#define BASE_POINTER_OFFSET 0x04A51FFC
#define RADAR_BASE_POINTER_OFFSET 0x20 



bool findWord(string word, TCHAR* path, int nLetters)
{
	bool letterFound = false;
	int i = 0;
	
	//For every character in the path
	for (int j = 0; j < nLetters; j++)
	{
		//if the backslashes are found
		if (path[j] == '\\')
		{
			j++;
			letterFound = true;
			//Check if this is the word we are looking for
			for (int k = 0; k < word.size() && letterFound == true; k++)
			{
				//If this is the right letter
				if (path[j + k] == word[k])
				{
					//For every letter of the word
					if (k == word.size() - 1)
					{
						//The word is found
						return true;
					}
				}
				//If the letter is not right go back at searching backslashes
				else
				{
					letterFound = false;
				}
			}
		}
	}
	return false;
}

HMODULE FindModuleHandle(HANDLE hProcess , string moduleName)
{
	HMODULE hModules[1024];
	DWORD nBytes;

	// Get a list of all the modules in this process.
	if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &nBytes))
	{
		MessageBox(0, "Could not access process modules!", "Error!", MB_OK | MB_ICONERROR);
	}		
	else
	{
		int nModules = nBytes / sizeof(HMODULE);
		for (int i = 0; i < nModules; i++)
		{
			TCHAR path[MAX_PATH];
			int nLetters = sizeof(path) / sizeof(TCHAR);
			bool moduleFound = false;

			// Get the full path to the module's file.
			if (GetModuleFileNameEx(hProcess, hModules[i], path, nLetters))
			{
				moduleFound = findWord(moduleName, path, nLetters);
				if (moduleFound == true)
				{
					//return the handle to the module found
					return hModules[i];
				}
			}
		}
	}
	return 0;
}



int main()
{
	HWND hWnd = FindWindow(0, "Counter-Strike: Global Offensive");
	if (hWnd == 0)
	{
		MessageBox(0, "Error cannot find window.", "Error", MB_OK | MB_ICONERROR);
	}
	else
	{
		DWORD processID;
	
		GetWindowThreadProcessId(hWnd, &processID);

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

		if (!hProcess)
		{
			MessageBox(0, "Could not open the process!", "Error!", MB_OK | MB_ICONERROR);
		}
		else
		{
			HMODULE moduleAdress = FindModuleHandle(hProcess, "client.dll");

			if (moduleAdress == 0)
			{
				MessageBox(0, "Could not find module adress!", "Error!", MB_OK | MB_ICONERROR);
			}
			LPTSTR imageFileName = 0;
			int newData = 0;
			DWORD newDataSize = sizeof(newData);
			int level1Pointer = (int)moduleAdress + BASE_POINTER_OFFSET;
			int level2Pointer = 0;
			int radarAdress = 0;
			ReadProcessMemory(hProcess, (LPCVOID)(level1Pointer), &level2Pointer, sizeof(level2Pointer), NULL);
			level2Pointer += RADAR_BASE_POINTER_OFFSET;
			ReadProcessMemory(hProcess, (LPCVOID)(level2Pointer), &radarAdress, sizeof(radarAdress), NULL);
			/*
			//Write
			WriteProcessMemory(hProcess, (LPVOID)0x132B0A3C, &newData, newDataSize, NULL)
			*/
			CloseHandle(hProcess);
		}
		
		
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
	}
	return 0;
}