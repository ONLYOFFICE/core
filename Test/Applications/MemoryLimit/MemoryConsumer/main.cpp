#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "../proclimits.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Start" << endl;
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

	if(lMemoryLimit > 0)
	{
		limit_memory(lMemoryLimit);
	}
	cout << "limit_memory:" <<lMemoryLimit<< endl;
	vector<unsigned char*> consumer;
	int index = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << "sleep_for:" << ++index<< endl;
		consumer.push_back(new unsigned char[lAllocSize]);
	}
	cout << "End" << endl;
	return 0;
}
