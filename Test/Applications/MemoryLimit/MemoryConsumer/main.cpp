#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "../../../../Common/3dParty/misc/proclimits.h"
#include "../../../../DesktopEditor/common/StringExt.h"
using namespace std;

void test()
{
	bool res = false;
	long long nBytes;
	std::wstring sBytes;
	sBytes = L"1KB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"1MB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1000000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"1KiB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1024 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"11kb";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(11000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"4GiB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(4294967296 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"-1 kB ";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(-1000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;
}


int main(int argc, char* argv[])
{
	test();

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
