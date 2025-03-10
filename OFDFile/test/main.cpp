#include "../OfdFile.h"
#include <iostream>

int main()
{
	OFD::COfdFile oOfdFile;

	oOfdFile.SetTempDir(L"temp");

	if (oOfdFile.LoadFromFile(L"YOUR_PATH"))
		std::cout << "GOOD" << std::endl;
	else
		std::cout << "BAD" << std::endl;

	return 0;
}
