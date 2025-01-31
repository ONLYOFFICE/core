#include "../HWPFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include <iostream>

int main()
{
	CHWPFile oFile;

	if (oFile.OpenHWPX(L"C:/ONLYOFFICE/Files/hwpx/test_multiple_elements.hwpx"))
	// if (oFile.OpenHWP(L"C:/ONLYOFFICE/Files/hwp/Examples/test_rect.hwp"))
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
}
