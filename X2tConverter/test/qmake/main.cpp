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

#define UNUSED_PARAM(x) (void)x

int main(int argc, char** argv)
{
	// warnings
	UNUSED_PARAM(argc);
	UNUSED_PARAM(argv);

	std::wstring curr_dir = NSFile::GetProcessDirectory();
	std::wstring wsep = FILE_SEPARATOR_STR;

   // std::wstring filename_in  = curr_dir + wsep + L"Diva readme.rtf";
    std::wstring filename_in  = curr_dir + wsep + L"CONTRACT.rtf";
    std::wstring filename_out = curr_dir + wsep + L"123.docx";
	std::wstring fonts_dir    = curr_dir + wsep + L"fonts";
	std::wstring xml          = curr_dir + wsep + L"params.xml";

	std::wstring tmp_dir      = NSDirectory::CreateDirectoryWithUniqueName(curr_dir);

	CheckFonts(fonts_dir);
    std::wstring fontname = L"Times New Roman";

	// GENERATE XML
	NSStringUtils::CStringBuilder oBuilder;

	oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	oBuilder.WriteString(L"<TaskQueueDataConvert>");

	// main
	oBuilder.WriteString(L"<m_sFileFrom>");
	oBuilder.WriteEncodeXmlString(filename_in);
	oBuilder.WriteString(L"</m_sFileFrom>");

	oBuilder.WriteString(L"<m_sFileTo>");
	oBuilder.WriteEncodeXmlString(filename_out);
	oBuilder.WriteString(L"</m_sFileTo>");

	oBuilder.WriteString(L"<m_nFormatTo>");
	int nFormat = COfficeFileFormatChecker::GetFormatByExtension(L"." + NSFile::GetFileExtention(filename_out));
	oBuilder.WriteString(std::to_wstring(nFormat));
	oBuilder.WriteString(L"</m_nFormatTo>");

	if (nFormat == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDFA)
		oBuilder.WriteString(L"<m_bIsPDFA>true</m_bIsPDFA>");

	oBuilder.WriteString(L"<m_sThemeDir>");
	oBuilder.WriteEncodeXmlString(curr_dir + L"/sdkjs/slide/themes");
	oBuilder.WriteString(L"</m_sThemeDir>");

	// changes
	oBuilder.WriteString(L"<m_bFromChanges>false</m_bFromChanges>");

	oBuilder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");

	// fonts
	oBuilder.WriteString(L"<m_sFontDir>");
	oBuilder.WriteEncodeXmlString(fonts_dir);
	oBuilder.WriteString(L"</m_sFontDir>");

	oBuilder.WriteString(L"<m_sAllFontsPath>");
	oBuilder.WriteString(fonts_dir + L"/AllFonts.js");
	oBuilder.WriteString(L"</m_sAllFontsPath>");

    oBuilder.WriteString(L"<DefaultFontName>");
    oBuilder.WriteEncodeXmlString(fontname);
    oBuilder.WriteString(L"</DefaultFontName>");
    oBuilder.WriteString(L"<DefaultFontSize>");
    oBuilder.WriteEncodeXmlString(L"24");
    oBuilder.WriteString(L"</DefaultFontSize>");
    // temp directory
	oBuilder.WriteString(L"<m_sTempDir>");
	oBuilder.WriteEncodeXmlString(tmp_dir);
	oBuilder.WriteString(L"</m_sTempDir>");

	// encrypt
	if (false)
	{
		oBuilder.WriteString(L"<m_sPassword>");
		oBuilder.WriteEncodeXmlString(L"111");
		oBuilder.WriteString(L"</m_sPassword>");

		oBuilder.WriteString(L"<m_sSavePassword>");
		oBuilder.WriteEncodeXmlString(L"222");
		oBuilder.WriteString(L"</m_sSavePassword>");
	}

	// docinfo (private rooms)
	if (false)
	{
		oBuilder.WriteString(L"<m_sDocumentID>");
		oBuilder.WriteEncodeXmlString(L"{data}");
		oBuilder.WriteString(L"</m_sDocumentID>");
	}

	// txt/csv
	oBuilder.WriteString(L"<m_nCsvTxtEncoding>");
	oBuilder.WriteEncodeXmlString(L"46");
	oBuilder.WriteString(L"</m_nCsvTxtEncoding>");

	oBuilder.WriteString(L"<m_nCsvDelimiter>");
	oBuilder.WriteEncodeXmlString(L"4");
	oBuilder.WriteString(L"</m_nCsvDelimiter>");

	// js params
	if (false)
	{
		oBuilder.WriteString(L"<m_sJsonParams>");
		oBuilder.WriteString(L"{");
		// *
		oBuilder.WriteString(L"}");
		oBuilder.WriteString(L"</m_sJsonParams>");
	}

	if (false)
	{
		// if need disable js engine cache
		oBuilder.WriteString(L"<m_nDoctParams>1</m_nDoctParams>");
	}

	// images
	if (true && (0 != (nFormat & AVS_OFFICESTUDIO_FILE_IMAGE)))
	{
		oBuilder.WriteString(L"<m_oThumbnail><first>false</first>");

		if (nFormat == AVS_OFFICESTUDIO_FILE_IMAGE_JPG)
			oBuilder.WriteString(L"<format>3</format>");

		oBuilder.WriteString(L"</m_oThumbnail>");
	}

	oBuilder.WriteString(L"</TaskQueueDataConvert>");

	// writing xml data into file
	if(NSFile::CFileBinary::Exists(xml))
		NSFile::CFileBinary::Remove(xml);

	NSFile::CFileBinary xml_file;
	xml_file.CreateFile(xml);
	xml_file.WriteStringUTF8(oBuilder.GetData());
	xml_file.CloseFile();

#if !defined(_WIN32) && !defined (_WIN64)
	std::string xmlDst = U_TO_UTF8(xml);
#else
	std::wstring xmlDst = xml;
#endif

	x2tchar* args[2];
	args[0] = NULL;
	args[1] = (x2tchar*)xmlDst.c_str();

	int nResultCode = X2T_Convert(2, args);
	NSDirectory::DeleteDirectory(tmp_dir);

	return nResultCode;
}
