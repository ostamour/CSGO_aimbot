#ifndef DUMPER
#define DUMPER

#include "memory_manager.h"

class Dumper
{
public:
	Dumper();
	Dumper(MemoryManager* memoryManager_);
	~Dumper();
private:
	MemoryManager* memoryManager_;
};

#endif