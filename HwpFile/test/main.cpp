#include "../HWPFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include <iostream>

int main()
{
	CHWPFile oFile(L"YOUR_PATH");

	if (oFile.Open())
	{
		std::cout << "Successful" << std::endl;
	}
	else
	{
		std::cout << "Unsuccessful" << std::endl;
		oFile.Close();
		return -1;
	}
	std::wstring wsTempDir = NSFile::GetProcessDirectory() + L"/temp";

	NSDirectory::DeleteDirectory(wsTempDir);
	NSDirectory::CreateDirectory(wsTempDir);
	oFile.SetTempDirectory(wsTempDir);
	oFile.ConvertToOOXML(L"YOUR_PATH.docx");
}
