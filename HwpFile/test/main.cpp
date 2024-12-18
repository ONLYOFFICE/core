#include "../HWPFile.h"
#include <iostream>

int main()
{
	CHWPFile oFile(L"C:/ONLYOFFICE/Files/hwp/example.hwp");

	if (oFile.Open())
	{
		std::cout << "Successful" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Unsuccessful" << std::endl;
		return -1;
	}

	oFile.Close();
}
