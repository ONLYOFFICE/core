#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../Common/OfficeFileFormatChecker.h"
#include "../../src/dylib/x2t.h"

void CheckFonts(const std::wstring& fontsDir, bool isUseSystem = true, const std::vector<std::wstring>& addtitionalFontsDirs = {})
{
	CApplicationFontsWorker fonts_worker;

	fonts_worker.m_sDirectory = fontsDir;
	if (!NSDirectory::Exists(fonts_worker.m_sDirectory))
		NSDirectory::CreateDirectory(fonts_worker.m_sDirectory);

	fonts_worker.m_bIsUseSystemFonts = isUseSystem;

	for (const auto& additional : addtitionalFontsDirs)
	{
		std::wstring sFolder = additional;
		if (0 == sFolder.find(L"."))
			sFolder = NSFile::GetProcessDirectory() + FILE_SEPARATOR_STR + sFolder;
		fonts_worker.m_arAdditionalFolders.push_back(sFolder);
	}

	fonts_worker.m_bIsNeedThumbnails = false;
	NSFonts::IApplicationFonts* pFonts = fonts_worker.Check();
	RELEASEINTERFACE(pFonts);
}

int main(int argc, char** argv)
{
	std::wstring curr_dir = NSFile::GetProcessDirectory();
	std::wstring wsep = FILE_SEPARATOR_STR;

	std::wstring filename_in  = curr_dir + wsep + L"123.docx";
	std::wstring filename_out = curr_dir + wsep + L"123.txt";
	std::wstring fonts_dir    = curr_dir + wsep + L"fonts";
	std::wstring xml          = curr_dir + wsep + L"params.xml";

	CheckFonts(fonts_dir);

	NSStringUtils::CStringBuilder oBuilder;

	// GENERATE XML

#if !defined(_WIN32) && !defined (_WIN64)
	std::string xmlDst = U_TO_UTF8(xml);
#else
	std::wstring xmlDst = xml;
#endif

	x2tchar* args[2];
	args[0] = NULL;
	args[1] = (x2tchar*)xmlDst.c_str();

	return X2T_Convert(2, args);
}
