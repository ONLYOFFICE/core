#include <iostream>
#include <cstdlib>

#include "../../../../Common/3dParty/misc/proclimits.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Start" << endl;
	std::string proc;
	long lMemoryLimit = 5 * 1024 * 1024;
	long lAllocSize = 1 * 1024 * 1024;
	if(argc > 1)
	{
		lMemoryLimit = atol(argv[1]) * 1024 * 1024;
	}
	if(argc > 2)
	{
		lAllocSize = atol(argv[2]) * 1024 * 1024;
	}
	if(argc > 3)
	{
		proc = std::string(argv[3]);
	}
	if(lMemoryLimit > 0)
	{
		limit_memory(lMemoryLimit);
	}
	cout << "limit_memory:" <<lMemoryLimit<< endl;
	char* alloc;
	if(lAllocSize > 0)
	{
		alloc = new char[lAllocSize];
	}
	cout << "Allocated:" <<strlen(alloc)<< endl;

	cout << "Start system:" <<proc.c_str()<< endl;
	std::system(proc.c_str());
	cout << "End system:" <<proc.c_str()<< endl;
	cout << "Allocated:" <<strlen(alloc)<< endl;
	cout << "End" << endl;
	return 0;
}
