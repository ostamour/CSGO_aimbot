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
		aimBot->updateData();
	
	}

	return 0;
}