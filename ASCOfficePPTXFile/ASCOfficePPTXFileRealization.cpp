#include "ASCOfficePPTXFile.h"

#include "PPTXFormat/PPTXEvent.h"

#if defined(_WIN32) || defined (_WIN64)
    #include <shellapi.h>
    #include <shlobj.h>
    #include <shlwapi.h>
    #pragma comment( lib, "shell32.lib" ) // добавить shell32.lib
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
    m_strTempDir = CStringW(buffer);

    GetLongPathName(m_strTempDir.GetString(), buffer, 4096);
	m_strTempDir = CStringW(buffer) + CStringW("_PPTX\\");
#else
    m_strTempDir = FileSystem::Directory::GetTempPath();
    m_strTempDir = FileSystem::Directory::GetLongPathName_(m_strTempDir) + CString("_PPTX/");
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
HRESULT CPPTXFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	CStringW localTempDir(sDstPath);
    if((sDstPath != NULL) || (localTempDir != _T("")))
	{
        bool res = FileSystem::Directory::CreateDirectory(localTempDir);
        if (res == false) return S_FALSE;
        //int res = SHCreateDirectoryExW(NULL, localTempDir.GetString(), NULL);
        //if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
        //	return S_FALSE;
		put_TempDirectory(sDstPath);
	}
	else
	{
        bool res = FileSystem::Directory::CreateDirectory(m_strTempDir);
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
	CStringW _local = localTempDir + CStringW(L"*.*");
	_local.AppendChar(0);
	_local.AppendChar(0);
	shfos.pFrom = _local.GetString();
	shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;
	if(SHFileOperationW(&shfos) != 0)
	return S_FALSE;
	*/
    CString srcFileName = sSrcFileName;
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
		FileSystem::Directory::DeleteDirectory(m_strTempDir, false);
		return S_FALSE;
	}

	m_strDirectory = (CString)sSrcFileName;
	int nIndex = m_strDirectory.ReverseFind(FILE_SEPARATOR_CHAR);
	if (-1 != nIndex)
		m_strDirectory = m_strDirectory.Mid(0, nIndex);

	return S_OK;
}
HRESULT CPPTXFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	if (NULL == m_pFolder)
		return S_FALSE;

	OOX::CPath oPath;
	oPath.m_strFilename = CString(sSrcPath);
	m_pFolder->write(oPath);

    CString srcFilePath = sSrcPath;
    CString dstFileName = sDstFileName;
	return m_fCallbackCompress ? (m_fCallbackCompress(m_pCallbackArg, srcFilePath, dstFileName) ? S_OK : S_FALSE) : S_OK;
}
HRESULT CPPTXFile::get_TempDirectory(BSTR* pVal)
{
#if defined(_WIN32) || defined (_WIN64)
    *pVal = m_strTempDir.AllocSysString();
#else
    *pVal = m_strTempDir;
#endif
	return S_OK;
}
HRESULT CPPTXFile::put_TempDirectory(BSTR newVal)
{
	CStringW TempStr(newVal);

#if defined(_WIN32) || defined (_WIN64)
	if(PathIsDirectoryW(TempStr.GetString()))
	{
		if(TempStr.Right(1) != L"\\")
			TempStr += L"\\";
		m_strTempDir = TempStr;
		return S_OK;
	}
#else
    if(FileSystem::Directory::PathIsDirectory(TempStr))
    {
        if(TempStr.Right(1) != _T("/"))
            TempStr += _T("/");
        m_strTempDir = TempStr;
    }
#endif
	return S_FALSE;
}
HRESULT CPPTXFile::GetDVDXml(BSTR* pbstrPTTXml)
{
	return S_OK;
}
HRESULT CPPTXFile::GetBluRayXml(BSTR* pbstrDVDXml)
{
	return S_OK;
}
HRESULT CPPTXFile::get_DrawingXml(BSTR* pVal)
{
	if ((NULL == m_pFolder) || (NULL == pVal))
		return S_FALSE;

	return S_OK;
}

HRESULT CPPTXFile::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName; sParamName = ParamName;
	if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
	{		
		m_strEmbeddedFontsDirectory = ParamValue.bstrVal;
		return S_OK;
	}		
	return S_OK;
}

HRESULT CPPTXFile::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	if (NULL == ParamValue)
		return S_FALSE;

	return S_OK;
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
HRESULT CPPTXFile::SetMediaDir(BSTR bsMediaDir) 
{
	m_strMediaDirectory = bsMediaDir;
	return S_OK;
}
HRESULT CPPTXFile::SetFontDir(BSTR bsFontDir)
{
	m_strFontDirectory = bsFontDir;
	return S_OK;
}
HRESULT CPPTXFile::SetThemesDir(BSTR bsDir)
{
	m_strFolderThemes = bsDir;
	return S_OK;
}
HRESULT CPPTXFile::SetUseSystemFonts(VARIANT_BOOL useSystemFonts) 
{
	m_bIsUseSystemFonts = (VARIANT_TRUE == useSystemFonts);
	return S_OK;
}
HRESULT CPPTXFile::OpenFileToPPTY(BSTR bsInput, BSTR bsOutput)
{
	if (m_strTempDir.GetLength() < 1)
	{
        m_strTempDir = FileSystem::Directory::GetTempPath();
	}

	FileSystem::Directory::CreateDirectory(m_strTempDir);

	OOX::CPath pathLocalInputTemp = FileSystem::Directory::CreateDirectoryWithUniqueName(m_strTempDir);

	bool notDeleteInput = false;

	if (m_fCallbackExtract)
	{
        CString strInput = bsInput;
        CString strOutput = pathLocalInputTemp.GetPath();

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
	BSTR bsLocalInputTemp;
	
#if defined(_WIN32) || defined (_WIN64)
	bsLocalInputTemp= pathLocalInputTemp.GetPath().AllocSysString();
#else
	bsLocalInputTemp= pathLocalInputTemp.GetPath();
#endif

	HRESULT hr = OpenDirectoryToPPTY(bsLocalInputTemp, bsOutput);

	if (notDeleteInput == false)
		FileSystem::Directory::DeleteDirectory(pathLocalInputTemp.GetPath());
	
#if defined(_WIN32) || defined (_WIN64)
	if (bsLocalInputTemp)SysFreeString(bsLocalInputTemp);
#endif
	return hr;
}
HRESULT CPPTXFile::OpenDirectoryToPPTY(BSTR bsInput, BSTR bsOutput)
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
		FileSystem::Directory::DeleteDirectory(m_strTempDir, false);
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

    FileSystem::Directory::CreateDirectory(m_strMediaDirectory);

	if (_T("") != m_strEmbeddedFontsDirectory)
	{
        FileSystem::Directory::CreateDirectory(m_strEmbeddedFontsDirectory);

		if (NULL != oBinaryWriter.m_pCommon->m_pFontPicker)
		{
            oBinaryWriter.m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts = true;
			oBinaryWriter.m_pCommon->m_pNativePicker->m_oEmbeddedFonts.m_strEmbeddedFontsFolder = m_strEmbeddedFontsDirectory;
		}
	}

	PPTX2EditorAdvanced::Convert(oBinaryWriter, *m_pFolder, m_strDirectory, pathDstFileOutput.GetPath());

	return S_OK;
}

HRESULT CPPTXFile::ConvertPPTYToPPTX(BSTR bsInput, BSTR bsOutput, BSTR bsThemesFolder)//bsOutput и файл и директория может быть 
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
	oFileBinary.OpenFile((CString)bsInput);	
		LONG lFileSize = (LONG)oFileBinary.GetFileSize();
		BYTE* pSrcBuffer = new BYTE[lFileSize];
		oFileBinary.ReadFile(pSrcBuffer, (DWORD)lFileSize);
	oFileBinary.CloseFile();
	
	CString strBsInput = bsInput;
    CString srcFolder = FileSystem::Directory::GetFolderPath(strBsInput);

    oWriter.OpenPPTY(pSrcBuffer, lFileSize, srcFolder, bsThemesFolder);
	
	RELEASEARRAYOBJECTS(pSrcBuffer);
	HRESULT hRes = S_OK;

	if (m_fCallbackCompress)
	{
        CString strOutput = bsOutput;
        CString strInput = pathLocalTempDirectory.GetPath();

        hRes = m_fCallbackCompress(m_pCallbackArg, strInput, strOutput) ? S_OK : S_FALSE;

        FileSystem::Directory::DeleteDirectory(strInput);
	}
	return hRes;
}
