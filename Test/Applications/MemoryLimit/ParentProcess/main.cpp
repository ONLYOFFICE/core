#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

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

	cout << "Start exec:" <<proc.c_str()<< endl;
	pid_t pid = fork();
	if (pid == 0) {
		execlp(proc.c_str(), proc.c_str(), (char*)NULL);
		_exit(127);
	}
	int status = 0;
	waitpid(pid, &status, 0);
	cout << "End exec:" <<proc.c_str()<< endl;
	cout << "Allocated:" <<strlen(alloc)<< endl;
	cout << "End" << endl;
	return 0;
}
