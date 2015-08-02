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
		aimBot->init();
		Sleep(2000);
		for (;;)
		{
			aimBot->updateData();
			aimBot->aim();
		}
	}

	return 0;
}