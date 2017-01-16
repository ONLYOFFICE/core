/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "PPTXFormat/PPTXEvent.h"

#if defined(_WIN32) || defined (_WIN64)
    #include <shellapi.h>
    #include <shlobj.h>
    #include <shlwapi.h>
    #pragma comment( lib, "Rpcrt4.lib" )
    #pragma comment( lib, "shell32.lib" ) // добавить shell32.lib
    #pragma comment( lib, "Shlwapi.lib" )
#else
#endif

#include "Editor/PPTXWriter.h"

#include "PPTXFormat/PPTXEvent.h"
#include "Editor/PresentationDrawingsDef.h"

CPPTXFile::CPPTXFile(extract_to_directory fCallbackExtract, compress_from_directory fCallbackCompress, progress_operation fCallbackProgress, void* pCallbackArg)
{
#if defined(_WIN32) || defined (_WIN64)
    WCHAR buffer[4096];
    GetTempPathW(4096, buffer);
    m_strTempDir = std::wstring(buffer);

    GetLongPathName(m_strTempDir.c_str(), buffer, 4096);
    m_strTempDir = std::wstring(buffer) + std::wstring(L"_PPTX\\");
#else
    m_strTempDir = NSDirectory::GetTempPath();
    m_strTempDir = NSDirectory::GetLongPathName_(m_strTempDir) + std::wstring("_PPTX/");
#endif
	//
	m_strFontDirectory = _T("");
	m_strMediaDirectory = _T("");
    m_bIsUseSystemFonts = false;
	m_strEmbeddedFontsDirectory = _T("");

    m_strFolderThemes = _T("");

	//m_fCallbackResource = fCallbackResource;
	m_fCallbackExtract = fCallbackExtract;
	m_fCallbackCompress = fCallbackCompress;
	m_fCallbackProgress = fCallbackProgress;
	m_pCallbackArg = pCallbackArg;

	m_pFolder		= NULL;
	//m_pOfficeUtils	= NULL;

	//if (S_OK != CoCreateInstance(__uuidof(OfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(OfficeUtils::IOfficeUtils),(void**)&m_pOfficeUtils))
	//	return S_FALSE;

}
CPPTXFile::~CPPTXFile()
{
	RELEASEOBJECT(m_pFolder);
}
HRESULT CPPTXFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring sXMLOptions)
{
	std::wstring localTempDir(sDstPath);
    if(!localTempDir.empty())
	{
        bool res = NSDirectory::CreateDirectory(localTempDir);
        if (res == false) return S_FALSE;
        //int res = SHCreateDirectoryExW(NULL, localTempDir.GetString(), NULL);
        //if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
        //	return S_FALSE;
		put_TempDirectory(sDstPath);
	}
	else
	{
        bool res = NSDirectory::CreateDirectory(m_strTempDir);
        if (res == false) return S_FALSE;
    //	int res = SHCreateDirectoryExW(NULL, m_strTempDir, NULL);
    //	if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
    //		return S_FALSE;
	}
	localTempDir = m_strTempDir;

	/*
	SHFILEOPSTRUCTW shfos;
	ZeroMemory(&shfos, sizeof(shfos));
	shfos.wFunc = FO_DELETE;
    std::wstring _local = localTempDir + std::wstring(L"*.*");
	_local.AppendChar(0);
	_local.AppendChar(0);
	shfos.pFrom = _local.GetString();
	shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;
	if(SHFileOperationW(&shfos) != 0)
	return S_FALSE;
	*/
	std::wstring srcFileName = sSrcFileName;
	if (m_pCallbackArg)
	{
		if(!m_fCallbackExtract(m_pCallbackArg, srcFileName , localTempDir))
			return S_FALSE;
	}
	else
	{
		localTempDir = sSrcFileName;
	}

	RELEASEOBJECT(m_pFolder);
	m_pFolder = new PPTX::Folder();

	if(!m_pFolder->isValid(localTempDir))
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	m_pFolder->read(localTempDir, (PPTX::IPPTXEvent*)this);
	if(GetPercent() < 1000000)
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pFolder->get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
        NSDirectory::DeleteDirectory(m_strTempDir, false);
		return S_FALSE;
	}

    m_strDirectory = sSrcFileName;
    int nIndex = m_strDirectory.rfind(FILE_SEPARATOR_CHAR);
	if (-1 != nIndex)
		m_strDirectory = m_strDirectory.substr(0, nIndex);

	return S_OK;
}
HRESULT CPPTXFile::SaveToFile(std::wstring sDstFileName, std::wstring sSrcPath, std::wstring sXMLOptions)
{
	if (NULL == m_pFolder)
		return S_FALSE;

	OOX::CPath oPath;
	oPath.m_strFilename = std::wstring(sSrcPath);
	m_pFolder->write(oPath);

    std::wstring srcFilePath = sSrcPath;
    std::wstring dstFileName = sDstFileName;
	return m_fCallbackCompress ? (m_fCallbackCompress(m_pCallbackArg, srcFilePath, dstFileName) ? S_OK : S_FALSE) : S_OK;
}
HRESULT CPPTXFile::get_TempDirectory(std::wstring* pVal)
{
    *pVal = m_strTempDir;
	return S_OK;
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
        if(TempStr.Right(1) != _T("/"))
            TempStr += _T("/");
        m_strTempDir = TempStr;
    }
#endif
	return S_FALSE;
}
HRESULT CPPTXFile::GetDVDXml(std::wstring* pbstrPTTXml)
{
	return S_OK;
}
HRESULT CPPTXFile::GetBluRayXml(std::wstring* pbstrDVDXml)
{
	return S_OK;
}
HRESULT CPPTXFile::get_DrawingXml(std::wstring* pVal)
{
	if ((NULL == m_pFolder) || (NULL == pVal))
		return S_FALSE;

	return S_OK;
}

void CPPTXFile::SetEmbeddedFontsDirectory(std::wstring val)
{
    m_strEmbeddedFontsDirectory = val;
}


bool CPPTXFile::Progress(long ID, long Percent)
{
	percent = Percent;
	if (m_pCallbackArg == NULL)
	{
		if (Percent == 1000000 ) return true;
		return false;
	}

	return m_fCallbackProgress(m_pCallbackArg, ID, Percent);
}

// to PPTY
HRESULT CPPTXFile::SetMediaDir(std::wstring bsMediaDir) 
{
	m_strMediaDirectory = bsMediaDir;
	return S_OK;
}
HRESULT CPPTXFile::SetFontDir(std::wstring bsFontDir)
{
	m_strFontDirectory = bsFontDir;
	return S_OK;
}
HRESULT CPPTXFile::SetThemesDir(std::wstring bsDir)
{
	m_strFolderThemes = bsDir;
	return S_OK;
}
HRESULT CPPTXFile::SetUseSystemFonts(bool val)
{
    m_bIsUseSystemFonts = val;
	return S_OK;
}
HRESULT CPPTXFile::OpenFileToPPTY(std::wstring bsInput, std::wstring bsOutput)
{
    if (m_strTempDir.empty())
	{
        m_strTempDir = NSDirectory::GetTempPath();
	}

    NSDirectory::CreateDirectory(m_strTempDir);

    OOX::CPath pathLocalInputTemp = NSDirectory::CreateDirectoryWithUniqueName(m_strTempDir);

	bool notDeleteInput = false;

	if (m_fCallbackExtract)
	{
        std::wstring strInput = bsInput;
        std::wstring strOutput = pathLocalInputTemp.GetPath();

        if(!m_fCallbackExtract(m_pCallbackArg, strInput , strOutput))
		{
			pathLocalInputTemp = bsInput;
			notDeleteInput = true;
		}
	}
	else
	{
		pathLocalInputTemp = bsInput;
		notDeleteInput = true;
	}
	std::wstring bsLocalInputTemp= pathLocalInputTemp.GetPath();

	HRESULT hr = OpenDirectoryToPPTY(bsLocalInputTemp, bsOutput);

	if (notDeleteInput == false)
        NSDirectory::DeleteDirectory(pathLocalInputTemp.GetPath());
	
	return hr;
}
HRESULT CPPTXFile::OpenDirectoryToPPTY(std::wstring bsInput, std::wstring bsOutput)
{
	OOX::CPath pathInputDirectory = bsInput;

	RELEASEOBJECT(m_pFolder);
	m_pFolder = new PPTX::Folder();

	if (!m_pFolder->isValid(pathInputDirectory.GetPath())) // true ???
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}

	m_pFolder->read(pathInputDirectory.GetPath() + FILE_SEPARATOR_STR, (PPTX::IPPTXEvent*)this);
	if(GetPercent() < 1000000)
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pFolder->get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
        NSDirectory::DeleteDirectory(m_strTempDir, false);
		return S_FALSE;
	}

	m_strDirectory = pathInputDirectory.GetDirectory();

	NSBinPptxRW::CBinaryFileWriter oBinaryWriter;
	oBinaryWriter.m_pCommon->CheckFontPicker();
	oBinaryWriter.m_pCommon->m_pNativePicker->Init(m_strFontDirectory);

	OOX::CPath pathDstFileOutput = bsOutput;
	
	OOX::CPath pathMedia = pathDstFileOutput.GetDirectory() + FILE_SEPARATOR_STR + _T("media");

	m_strMediaDirectory = pathMedia.GetPath();
	oBinaryWriter.m_strMainFolder = pathDstFileOutput.GetDirectory();

 	oBinaryWriter.m_pCommon->m_pImageManager->m_strDstMedia = m_strMediaDirectory;
	oBinaryWriter.m_pCommon->m_pImageManager->SetFontManager(oBinaryWriter.m_pCommon->m_pNativePicker->m_pFontManager);

    NSDirectory::CreateDirectory(m_strMediaDirectory);

	if (_T("") != m_strEmbeddedFontsDirectory)
	{
        NSDirectory::CreateDirectory(m_strEmbeddedFontsDirectory);

		if (NULL != oBinaryWriter.m_pCommon->m_pFontPicker)
		{
            oBinaryWriter.m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts = true;
			oBinaryWriter.m_pCommon->m_pNativePicker->m_oEmbeddedFonts.m_strEmbeddedFontsFolder = m_strEmbeddedFontsDirectory;
		}
	}

	PPTX2EditorAdvanced::Convert(oBinaryWriter, *m_pFolder, m_strDirectory, pathDstFileOutput.GetPath());

	return S_OK;
}

HRESULT CPPTXFile::ConvertPPTYToPPTX(std::wstring bsInput, std::wstring bsOutput, std::wstring bsThemesFolder)//bsOutput и файл и директория может быть 
{
	OOX::CPath pathLocalTempDirectory;
	
	if (m_fCallbackCompress)//если компрессора нет - конвертим в назначеную директорию 
		pathLocalTempDirectory = m_strTempDir ;
	else
		pathLocalTempDirectory = bsOutput; //выходной файл - папка

#ifdef _DEBUG
	#if defined(_WIN32) || defined (_WIN64)
		if (m_fCallbackCompress)
			pathLocalTempDirectory = _T("C:\\PPTMemory\\PPTX_test");
	#endif
#endif


	NSBinPptxRW::CPPTXWriter oWriter;
	oWriter.Init(pathLocalTempDirectory.GetPath());

	CFile oFileBinary;
	oFileBinary.OpenFile((std::wstring)bsInput);	
		LONG lFileSize = (LONG)oFileBinary.GetFileSize();
		BYTE* pSrcBuffer = new BYTE[lFileSize];
		oFileBinary.ReadFile(pSrcBuffer, (DWORD)lFileSize);
	oFileBinary.CloseFile();
	
	std::wstring strBsInput = bsInput;
    std::wstring srcFolder = NSDirectory::GetFolderPath(strBsInput);

    oWriter.OpenPPTY(pSrcBuffer, lFileSize, srcFolder, bsThemesFolder);
	
	RELEASEARRAYOBJECTS(pSrcBuffer);
	HRESULT hRes = S_OK;

	if (m_fCallbackCompress)
	{
        std::wstring strOutput = bsOutput;
        std::wstring strInput = pathLocalTempDirectory.GetPath();

        hRes = m_fCallbackCompress(m_pCallbackArg, strInput, strOutput) ? S_OK : S_FALSE;

        NSDirectory::DeleteDirectory(strInput);
	}
	return hRes;
}
