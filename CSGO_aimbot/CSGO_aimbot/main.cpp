#include "aimbot.h"
#include "memory_manager.h"


int main()
{
	MemoryManager* memoryManager = new MemoryManager();
	AimBot* aimBot = new AimBot(memoryManager);
	if (!memoryManager->init())
	{
		cout << "Error cannot initialize memory manager." << endl;
	}
	else
	{
		Sleep(2000);
		aimBot->init();
		aimBot->updateData();
		for (;;)
		{
			aimBot->updateData();
			aimBot->aim();
		}
	}

	return 0;
}