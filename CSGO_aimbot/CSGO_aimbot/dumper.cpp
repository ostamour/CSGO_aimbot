#include "dumper.h"

Dumper::Dumper()
{
	memoryManager_ = 0;
}



Dumper::Dumper(MemoryManager* memoryManager)
{
	memoryManager_ = memoryManager;
}



Dumper::~Dumper()
{
	memoryManager_ = 0;
}