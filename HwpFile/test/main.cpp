#include "../HWPFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include <iostream>

int main()
{
	CHWPFile oFile;

	// if (oFile.OpenHWPX(L"YOUR_PATH"))
	if (oFile.OpenHWP(L"YOUR_PATH"))
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
	oFile.ConvertToOOXML(L"result.docx");
	oFile.Close();
}
