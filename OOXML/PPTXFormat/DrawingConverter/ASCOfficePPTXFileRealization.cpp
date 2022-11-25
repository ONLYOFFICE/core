/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "ASCOfficePPTXFile.h"

#include "../PPTXEvent.h"

#if defined(_WIN32) || defined (_WIN64)
    #include <windows.h>
    #include <shellapi.h>
    #include <shlobj.h>
    #include <shlwapi.h>
    #pragma comment( lib, "Rpcrt4.lib" )
    #pragma comment( lib, "shell32.lib" ) // добавить shell32.lib
    #pragma comment( lib, "Shlwapi.lib" )
#else
#endif

#include "../../Binary/Presentation/PPTXWriter.h"

#include "../PPTXEvent.h"
#include "../../../Common/OfficeFileErrorDescription.h"

CPPTXFile::CPPTXFile()
{
#if defined(_WIN32) || defined (_WIN64)
    WCHAR buffer[4096];
    GetTempPathW(4096, buffer);
    m_strTempDir = std::wstring(buffer);

    GetLongPathName(m_strTempDir.c_str(), buffer, 4096);
    m_strTempDir = std::wstring(buffer) + std::wstring(L"_PPTX\\");
#else
    m_strTempDir = NSDirectory::GetTempPath() + L"_PPTX/";
#endif

    m_bIsUseSystemFonts = false;
	m_bIsNoBase64 = false;
	m_bIsMacro = false;

	m_pPptxDocument		= NULL;
}

CPPTXFile::~CPPTXFile()
{
	RELEASEOBJECT(m_pPptxDocument);
}
_UINT32 CPPTXFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring sXMLOptions)
{
	std::wstring localTempDir(sDstPath);
    if(!localTempDir.empty())
	{
        bool res = NSDirectory::CreateDirectory(localTempDir);
        if (res == false) return AVS_FILEUTILS_ERROR_CONVERT;

		put_TempDirectory(sDstPath);
	}
	else
	{
        bool res = NSDirectory::CreateDirectory(m_strTempDir);
        if (res == false) return AVS_FILEUTILS_ERROR_CONVERT;
	}
	localTempDir = m_strTempDir;

	std::wstring srcFileName = sSrcFileName;

	localTempDir = sSrcFileName;

	RELEASEOBJECT(m_pPptxDocument);
	m_pPptxDocument = new PPTX::Document();

	if(!m_pPptxDocument->isValid(localTempDir))
	{
		RELEASEOBJECT(m_pPptxDocument);
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	if(false == m_pPptxDocument->read(localTempDir))
	{
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pPptxDocument->Get(OOX::Presentation::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
        NSDirectory::DeleteDirectory(m_strTempDir, false);
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

    m_strDirectory = sSrcFileName;
    size_t nIndex = m_strDirectory.rfind(FILE_SEPARATOR_CHAR);
	if (std::wstring::npos != nIndex)
		m_strDirectory = m_strDirectory.substr(0, nIndex);

	return 0;
}
_UINT32 CPPTXFile::SaveToFile(std::wstring sDstFileName, std::wstring sSrcPath, std::wstring sXMLOptions)
{
	if (NULL == m_pPptxDocument)
		return AVS_FILEUTILS_ERROR_CONVERT;

	OOX::CPath oPath;
	oPath.m_strFilename = std::wstring(sSrcPath);
	m_pPptxDocument->write(oPath);

	return 0;
}
void CPPTXFile::get_TempDirectory(std::wstring* pVal)
{
    *pVal = m_strTempDir;
}
HRESULT CPPTXFile::put_TempDirectory(std::wstring newVal)
{
    std::wstring TempStr(newVal);

#if defined(_WIN32) || defined (_WIN64)
    if(PathIsDirectoryW(TempStr.c_str()))
	{
        if(TempStr.substr(TempStr.length() - 2, 1) != L"\\")
			TempStr += L"\\";
		m_strTempDir = TempStr;
		return S_OK;
	}
#else
    if(NSDirectory::PathIsDirectory(TempStr))
    {
        if(TempStr[TempStr.length() - 1] != L'/')
            TempStr += L"/";
        m_strTempDir = TempStr;
    }
#endif
	return S_FALSE;
}
void CPPTXFile::SetEmbeddedFontsDirectory(std::wstring val)
{
    m_strEmbeddedFontsDirectory = val;
}
void CPPTXFile::SetMediaDir(std::wstring bsMediaDir) 
{
	m_strMediaDirectory = bsMediaDir;
}
void CPPTXFile::SetFontDir(std::wstring bsFontDir)
{
	m_strFontDirectory = bsFontDir;
}
void CPPTXFile::SetThemesDir(std::wstring bsDir)
{
	m_strFolderThemes = bsDir;
}
void CPPTXFile::SetUseSystemFonts(bool val)
{
    m_bIsUseSystemFonts = val;
}
void CPPTXFile::SetIsNoBase64(bool val)
{
	m_bIsNoBase64 = val;
}
void CPPTXFile::SetMacroEnabled(bool val)
{
	m_bIsMacro = val;
}
_UINT32 CPPTXFile::OpenFileToPPTY(std::wstring bsInput, std::wstring bsOutput)
{
    if (m_strTempDir.empty())
	{
        m_strTempDir = NSDirectory::GetTempPath();
	}

	if (true == NSDirectory::CreateDirectory(m_strTempDir))
	{
		_UINT32 hr = OpenDirectoryToPPTY(bsInput, bsOutput);

		return hr;
	}
	else
		return S_FALSE;
}
_UINT32 CPPTXFile::OpenDirectoryToPPTY(std::wstring bsInput, std::wstring bsOutput)
{
	OOX::CPath pathInputDirectory = bsInput;

	RELEASEOBJECT(m_pPptxDocument);
	m_pPptxDocument = new PPTX::Document();

	if (!m_pPptxDocument->isValid(pathInputDirectory.GetPath())) // true ???
	{
		RELEASEOBJECT(m_pPptxDocument);
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

	bool res = m_pPptxDocument->read(pathInputDirectory.GetPath() + FILE_SEPARATOR_STR);
	if (false == res)
	{
		RELEASEOBJECT(m_pPptxDocument);
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pPptxDocument->Get(OOX::Presentation::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
        NSDirectory::DeleteDirectory(m_strTempDir, false);
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

	m_strDirectory = pathInputDirectory.GetDirectory();

	NSBinPptxRW::CBinaryFileWriter oBinaryWriter;
	oBinaryWriter.m_pCommon->CheckFontPicker();
	oBinaryWriter.m_pCommon->m_pNativePicker->Init(m_strFontDirectory);

	OOX::CPath pathDstFileOutput = bsOutput;
	
	OOX::CPath pathMedia = pathDstFileOutput.GetDirectory() + FILE_SEPARATOR_STR + _T("media");

	m_strMediaDirectory = pathMedia.GetPath();
	oBinaryWriter.m_strMainFolder = pathDstFileOutput.GetDirectory();

 	oBinaryWriter.m_pCommon->m_pMediaManager->m_strDstMedia = m_strMediaDirectory;
	oBinaryWriter.m_pCommon->m_pMediaManager->SetFontManager(oBinaryWriter.m_pCommon->m_pNativePicker->m_pFontManager);

    NSDirectory::CreateDirectory(m_strMediaDirectory);

    if (!m_strEmbeddedFontsDirectory.empty())
	{
        NSDirectory::CreateDirectory(m_strEmbeddedFontsDirectory);

		if (NULL != oBinaryWriter.m_pCommon->m_pFontPicker)
		{
            oBinaryWriter.m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts = true;
			oBinaryWriter.m_pCommon->m_pNativePicker->m_oEmbeddedFonts.m_strEmbeddedFontsFolder = m_strEmbeddedFontsDirectory;
		}
	}

	PPTX2EditorAdvanced::Convert(oBinaryWriter, *m_pPptxDocument, m_strDirectory, pathDstFileOutput.GetPath(), m_bIsNoBase64);

	return S_OK;
}

_UINT32 CPPTXFile::ConvertPPTYToPPTX(std::wstring bsInput, std::wstring bsOutput, std::wstring bsThemesFolder)//bsOutput и файл и директория может быть 
{
	OOX::CPath pathLocalTempDirectory = bsOutput; //выходной файл - папка
	
	NSBinPptxRW::CPPTXWriter oWriter;
	oWriter.Init(pathLocalTempDirectory.GetPath(), m_bIsMacro);

	NSFile::CFileBinary oFileBinary;
	oFileBinary.OpenFile((std::wstring)bsInput);	
		LONG lFileSize = (LONG)oFileBinary.GetFileSize();
		BYTE* pSrcBuffer = new BYTE[lFileSize];
		oFileBinary.ReadFile(pSrcBuffer, (DWORD)lFileSize);
	oFileBinary.CloseFile();
	
	std::wstring strBsInput = bsInput;
    std::wstring srcFolder = NSDirectory::GetFolderPath(strBsInput);

	_UINT32 hRes = S_OK;
	
	try
	{
		oWriter.OpenPPTY(pSrcBuffer, lFileSize, srcFolder, bsThemesFolder);
	}
	catch(...)
	{
		hRes = S_FALSE;
	}
	
	RELEASEARRAYOBJECTS(pSrcBuffer);
	
	return hRes;
}
