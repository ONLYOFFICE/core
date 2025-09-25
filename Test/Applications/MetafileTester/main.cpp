#include <algorithm>
#include <iostream>
#include <string>

#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../DesktopEditor/common/Directory.h"

#define METAFILE_SUCCESSFUL_CONVERSION      0
#define METAFILE_UNSUCCESSFUL_CONVERSION   -1

#define METAFILE_INVALID_NUMBERS_ARGUMENTS -3
#define METAFILE_FILE_NOT_FOUND            -4
#define METAFILE_INVALID_VALUE_ARGUMENT    -5
#define METAFILE_INVALID_FORMAT_FILE       -6
#define METAFILE_OPENNING_ERROR            -7
#define METAFILE_FAILED_CREATE_FILE        -8

__ENUM_CXIMAGE_FORMATS IdentifyFileFormat(const std::wstring& wsFilePath)
{
	std::wstring::const_reverse_iterator itPointPos = std::find(wsFilePath.rbegin(), wsFilePath.rend(), L'.');

	if (wsFilePath.rend() == itPointPos || wsFilePath.rbegin() == itPointPos)
		return _CXIMAGE_FORMAT_UNKNOWN;

	std::wstring wsFileExtension = std::wstring(itPointPos.base(), wsFilePath.end());
	
	std::transform(wsFileExtension.begin(), wsFileExtension.end(), wsFileExtension.begin(), tolower);
	
	if (L"bmp" == wsFileExtension)
		return _CXIMAGE_FORMAT_BMP;
	else if (L"gif" == wsFileExtension)
		return _CXIMAGE_FORMAT_GIF;
	else if (L"jpg" == wsFileExtension)
		return _CXIMAGE_FORMAT_JPG;
	else if (L"png" == wsFileExtension)
		return _CXIMAGE_FORMAT_PNG;
	else if (L"ico" == wsFileExtension)
		return _CXIMAGE_FORMAT_ICO;
	else if (L"tif" == wsFileExtension)
		return _CXIMAGE_FORMAT_TIF;
	else if (L"tga" == wsFileExtension)
		return _CXIMAGE_FORMAT_TGA;
	else if (L"pcx" == wsFileExtension)
		return _CXIMAGE_FORMAT_PCX;
	else if (L"wmf" == wsFileExtension)
		return _CXIMAGE_FORMAT_WMF;
	else if (L"jp2" == wsFileExtension)
		return _CXIMAGE_FORMAT_JP2;
	else if (L"jpc" == wsFileExtension)
		return _CXIMAGE_FORMAT_JPC;
	else if (L"pgx" == wsFileExtension)
		return _CXIMAGE_FORMAT_PGX;
	else if (L"pnm" == wsFileExtension)
		return _CXIMAGE_FORMAT_PNM;
	else if (L"ras" == wsFileExtension)
		return _CXIMAGE_FORMAT_RAS;
	else if (L"jbg" == wsFileExtension)
		return _CXIMAGE_FORMAT_JBG;
	else if (L"mng" == wsFileExtension)
		return _CXIMAGE_FORMAT_MNG;
	else if (L"ska" == wsFileExtension)
		return _CXIMAGE_FORMAT_SKA;
	else if (L"raw" == wsFileExtension)
		return _CXIMAGE_FORMAT_RAW;
	else if (L"psd" == wsFileExtension)
		return _CXIMAGE_FORMAT_PSD;
	else if (L"emf" == wsFileExtension)
		return _CXIMAGE_FORMAT_EMF;
	else if (L"wb" == wsFileExtension)
		return _CXIMAGE_FORMAT_WB;
	else if (L"svm" == wsFileExtension)
		return _CXIMAGE_FORMAT_SVM;
	else if (L"svg" == wsFileExtension)
		return _CXIMAGE_FORMAT_SVG;
	
	return _CXIMAGE_FORMAT_UNKNOWN;
}

#if !defined(_WIN32) && !defined(_WIN64)
static std::wstring utf8_to_unicode(const char* src)
{
	return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)src, (LONG)strlen(src));
}

int main(int argc, char* argv[])
#else
int wmain(int argc, wchar_t* argv[])
#endif
{
	std::wcout << std::endl;
	std::wcout << L"-------------------------------------------------------------------------------" << std::endl;
	std::wcout << L"METAFILE CONVERSION TEST" << std::endl;
	std::wcout << L"-------------------------------------------------------------------------------" << std::endl;

	if (argc < 3)
	{
		std::wcout << L"INVALID NUMBER ARGUMENTS" << std::endl;
		std::wcout << "USAGE: metafile \"path_to_input_file\" \"path_to_output_file\"" << std::endl;
		std::wcout << L"WHERE:" << std::endl;
		std::wcout << L"\"path_to_input_file\" is the path to the metafile" << std::endl;
		std::wcout << L"\"path_to_output_file\" is the path to the final file with the specified format (.png, .jpeg, .jpg, .svg, ...)" << std::endl;

		return METAFILE_INVALID_NUMBERS_ARGUMENTS;
	}

	std::wstring wsInputFilePath, wsOutputFilePath;

#if !defined(_WIN32) && !defined(_WIN64)
	wsInputFilePath  = utf8_to_unicode(argv[1]);
	wsOutputFilePath = utf8_to_unicode(argv[2]);
#else
	wsInputFilePath  = std::wstring(argv[1]);
	wsOutputFilePath = std::wstring(argv[2]);
#endif

	if (!NSFile::CFileBinary::Exists(wsInputFilePath))
	{
		std::wcout << L"THE \"" << wsInputFilePath << L"\" FILE WAS NOT FOUND" << std::endl;
		return METAFILE_FILE_NOT_FOUND;
	}

	__ENUM_CXIMAGE_FORMATS enImageFormats = IdentifyFileFormat(wsOutputFilePath);

	if (_CXIMAGE_FORMAT_UNKNOWN == enImageFormats)
	{
		std::wcout << L"INVALID FILE FORMATS" << std::endl;
		std::wcout << L"The \"" << wsOutputFilePath << L"\" argument specifies an unsupported format" << std::endl;
		return METAFILE_INVALID_FORMAT_FILE;
	}

	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	oWorker.m_bIsNeedThumbnails = false;

	if (!NSDirectory::Exists(oWorker.m_sDirectory))
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);

	NSFonts::IApplicationFonts* pFonts = oWorker.Check();

	MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);

	int nResult;

	if (!pMetafile->LoadFromFile(wsInputFilePath.c_str()))
	{
		std::wcout << L"METAFILE OPENNING ERROR" << std::endl;
		nResult = METAFILE_OPENNING_ERROR;
	}
	else if (_CXIMAGE_FORMAT_SVG == enImageFormats)
	{
		const std::wstring wsSvg = pMetafile->ConvertToSvg();

		if (!wsSvg.empty())
		{
			NSFile::CFileBinary oFile;

			if (!oFile.CreateFileW(wsOutputFilePath))
			{
				std::wcout << L"FAILED TO CREATE FILE: \"" << wsOutputFilePath << L"\"" << std::endl;
				nResult = METAFILE_FAILED_CREATE_FILE;
			}
			else
			{
				oFile.WriteStringUTF8(wsSvg);
				oFile.CloseFile();
				nResult = METAFILE_SUCCESSFUL_CONVERSION;
			}
		}
		else
		{
			nResult = METAFILE_UNSUCCESSFUL_CONVERSION;
		}
	}
	else
	{
		double dX, dY, dWidth, dHeight;
		pMetafile->GetBounds(&dX, &dY, &dWidth, &dHeight);

		if (0. == dWidth)
		{
			dWidth = 1000.;
			dHeight = -1;
		}

		pMetafile->ConvertToRaster(wsOutputFilePath.c_str(), enImageFormats, dWidth, dHeight);
		nResult = METAFILE_SUCCESSFUL_CONVERSION;
	}

	pMetafile->Release();
	pFonts->Release();

	std::wcout << ((METAFILE_SUCCESSFUL_CONVERSION != nResult) ? L"UN" : L"") << L"SUCCESSFUL CONVERSION OF THE \"" << argv[1] << L"\" file to \"" << wsOutputFilePath << L"\"" << std::endl;

	return nResult;
}
