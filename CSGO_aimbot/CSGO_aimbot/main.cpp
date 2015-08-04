#include "aimbot.h"
#include "memory_manager.h"



int main()
{
	LARGE_INTEGER time, test1, test2;
	int count = 0;
	
	MemoryManager* memoryManager = new MemoryManager();
	AimBot* aimBot = new AimBot(memoryManager);
	if (!memoryManager->init())
	{
		cout << "Error cannot initialize memory manager." << endl;
	}
	else
	{
		aimBot->init();
		Sleep(2000);
		aimBot->updateData();
	
		float test = memoryManager->calculateRefreshRate(0xaef897C);
		int t = 0;

		/*
		for (;;)
		{
			aimBot->updateData();
			
			aimBot->aim();
		}
		*/
		
		
		
	}
	
	return 0;
}