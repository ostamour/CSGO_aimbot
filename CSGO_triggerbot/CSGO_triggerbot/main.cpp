#include <iostream>
#include <windows.h>
#include <string>
#include <winternl.h>
#include <Psapi.h>
#include "memory_manager.h"
#include "triggerbot.h"

using namespace std;

int main()
{
	MemoryManager* memoryManager = new MemoryManager();
	TriggerBot* triggerBot = new TriggerBot(memoryManager);
	if (!memoryManager->init())
	{
		cout << "Error cannot initialize memory manager." << endl;
	}
	else
	{
		for (;;)
		{
			triggerBot->updateData();
			if (triggerBot->crosshairOnEnemy())
			{
				triggerBot->fire();
				while (triggerBot->crosshairOnEnemy())
				{
					triggerBot->updateData();
				}
				triggerBot->stopFire();
			}
		}
	}
	return 0;
}

