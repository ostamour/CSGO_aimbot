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
				triggerBot->trigger();
				while (triggerBot->crosshairOnEnemy())
				{
					triggerBot->updateData();
				}
				triggerBot->stop();
			}
		}
	}
	return 0;
}

