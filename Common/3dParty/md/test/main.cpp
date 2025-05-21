#include <iostream>

#include "../md2html.h"

int main(int argc, char *argv[])
{
	std::cout << (ConvertMdFileToHtml(L"YOUR_PATH", L"YOUR_PATH") ? "Good" : "Bad") << std::endl;

	return 0;
}
