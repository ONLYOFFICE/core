#include "../OfdFile.h"
#include <iostream>

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

int main()
{
	// Check system fonts
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	oWorker.m_bIsNeedThumbnails = false;

	if (!NSDirectory::Exists(oWorker.m_sDirectory))
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);

	NSFonts::IApplicationFonts* pFonts = oWorker.Check();

	COFDFile oOfdFile(pFonts);

	oOfdFile.SetTempDirectory(L"temp");

	if (oOfdFile.LoadFromFile(L"YOUR_PATH"))
		std::cout << "GOOD" << std::endl;
	else
		std::cout << "BAD" << std::endl;

	oOfdFile.ConvertToRaster(0, L"result.png", 4);

	pFonts->Release();

	return 0;
}
