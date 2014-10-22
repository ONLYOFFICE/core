#include "ASCOfficePPTXFile.h"

#include "PPTXFormat/PPTXEvent.h"

#ifdef _WIN32
    #include <shellapi.h>
    #include <shlobj.h>
    #include <shlwapi.h>
    #pragma comment( lib, "shell32.lib" ) // добавить shell32.lib
#else
#endif

#include "Editor/PPTXWriter.h"

#include "PPTXFormat/PPTXEvent.h"

CPPTXFile::CPPTXFile(load_from_resource fCallbackResource, extract_to_directory fCallbackExtract, compress_from_directory fCallbackCompress, progress_operation fCallbackProgress, void* pCallbackArg)
{
#ifdef WIN32
    WCHAR buffer[4096];
    GetTempPathW(4096, buffer);
    m_strTempDir = CStringW(buffer);

    GetLongPathNameW(m_strTempDir.GetString(), buffer, 4096);
	m_strTempDir = CStringW(buffer) + CStringW("_PPTX\\");
#else
    m_strTempDir = FileSystem::Directory::GetTempPath();
    m_strTempDir = FileSystem::Directory::GetLongPathNameW(m_strTempDir) + CString("_PPTX\\");
#endif
	//
	m_strFontDirectory = _T("");
	m_strMediaDirectory = _T("");
	m_bIsUseSystemFonts = FALSE;
	m_strEmbeddedFontsDirectory = _T("");

	m_strFolderThemes = _T("");

	m_fCallbackResource = fCallbackResource;
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
    if(!m_fCallbackExtract(m_pCallbackArg, srcFileName , localTempDir))
		return S_FALSE;

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
		RemoveDirOrFile(m_strTempDir, false);
		return S_FALSE;
	}

	m_strDirectory = (CString)sSrcFileName;
	int nIndex = m_strDirectory.ReverseFind(TCHAR('\\'));
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
    return m_fCallbackCompress(m_pCallbackArg, srcFilePath, dstFileName) ? S_OK : S_FALSE;
}
HRESULT CPPTXFile::get_TempDirectory(BSTR* pVal)
{
#ifdef WIN32
    *pVal = m_strTempDir.AllocSysString();
#else
    *pVal = m_strTempDir;
#endif
	return S_OK;
}
HRESULT CPPTXFile::put_TempDirectory(BSTR newVal)
{
	CStringW TempStr(newVal);

#ifdef WIN32
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
        if(TempStr.Right(1) != _T("\\"))
            TempStr += _T("\\");
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
//	int res = SHCreateDirectoryExW(NULL, m_strTempDir, NULL);
//	if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
//		return S_FALSE;

    FileSystem::Directory::CreateDirectory(m_strTempDir); // security options ???

    CString strBsInput = bsInput;
    if(!m_fCallbackExtract(m_pCallbackArg, strBsInput , m_strTempDir))
		return S_FALSE;

	RELEASEOBJECT(m_pFolder);
	m_pFolder = new PPTX::Folder();

	if (!m_pFolder->isValid(m_strTempDir))
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	m_pFolder->read(m_strTempDir, (PPTX::IPPTXEvent*)this);
	if(GetPercent() < 1000000)
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pFolder->get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
		RemoveDirOrFile(m_strTempDir, false);
		return S_FALSE;
	}

	m_strDirectory = (CString)bsInput;
	int nIndex = m_strDirectory.ReverseFind(TCHAR('\\'));
	if (-1 != nIndex)
		m_strDirectory = m_strDirectory.Mid(0, nIndex);

	NSBinPptxRW::CBinaryFileWriter oBinaryWriter;
	oBinaryWriter.m_pCommon->CheckFontPicker();
	oBinaryWriter.m_pCommon->m_pNativePicker->Init(m_strFontDirectory);

	CString sDstFileOutput = bsOutput;
	m_strMediaDirectory = sDstFileOutput;
	nIndex = m_strMediaDirectory.ReverseFind(TCHAR('\\'));
	if (-1 != nIndex)
		m_strMediaDirectory = m_strMediaDirectory.Mid(0, nIndex);

	oBinaryWriter.m_strMainFolder = m_strMediaDirectory;
	m_strMediaDirectory = m_strMediaDirectory + _T("\\media");
	oBinaryWriter.m_pCommon->m_pImageManager->m_strDstMedia = m_strMediaDirectory;

    FileSystem::Directory::CreateDirectory(m_strMediaDirectory);

	if (_T("") != m_strEmbeddedFontsDirectory)
	{
        FileSystem::Directory::CreateDirectory(m_strEmbeddedFontsDirectory);

		if (NULL != oBinaryWriter.m_pCommon->m_pFontPicker)
		{
			oBinaryWriter.m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts = TRUE;
			oBinaryWriter.m_pCommon->m_pNativePicker->m_oEmbeddedFonts.m_strEmbeddedFontsFolder = m_strEmbeddedFontsDirectory;
		}
	}

	PPTX2EditorAdvanced::Convert(oBinaryWriter, *m_pFolder, m_strDirectory, sDstFileOutput);

	return S_OK;

}
HRESULT CPPTXFile::OpenDirectoryToPPTY(BSTR bsInput, BSTR bsOutput)
{
	RELEASEOBJECT(m_pFolder);
	m_pFolder = new PPTX::Folder();

	if (!m_pFolder->isValid((CString)bsInput))
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}

	m_pFolder->read((CString)bsInput, (PPTX::IPPTXEvent*)this);
	if(GetPercent() < 1000000)
	{
		RELEASEOBJECT(m_pFolder);
		return S_FALSE;
	}
	smart_ptr<PPTX::Presentation> presentation = m_pFolder->get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
	{
		RemoveDirOrFile(m_strTempDir, false);
		return S_FALSE;
	}

	m_strDirectory = (CString)bsInput;
	int nIndex = m_strDirectory.ReverseFind(TCHAR('\\'));
	if (-1 != nIndex)
		m_strDirectory = m_strDirectory.Mid(0, nIndex);

	NSBinPptxRW::CBinaryFileWriter oBinaryWriter;
	oBinaryWriter.m_pCommon->CheckFontPicker();
	oBinaryWriter.m_pCommon->m_pNativePicker->Init(m_strFontDirectory);

	CString sDstFileOutput = (CString)bsOutput;
	m_strMediaDirectory = sDstFileOutput;
	nIndex = m_strMediaDirectory.ReverseFind(TCHAR('\\'));
	if (-1 != nIndex)
		m_strMediaDirectory = m_strMediaDirectory.Mid(0, nIndex);

	oBinaryWriter.m_strMainFolder = m_strMediaDirectory;
	m_strMediaDirectory = m_strMediaDirectory + _T("\\media");
	oBinaryWriter.m_pCommon->m_pImageManager->m_strDstMedia = m_strMediaDirectory;

    FileSystem::Directory::CreateDirectory(m_strMediaDirectory);

	PPTX2EditorAdvanced::Convert(oBinaryWriter, *m_pFolder, m_strDirectory, sDstFileOutput);

	return S_OK;
}
HRESULT CPPTXFile::ConvertPPTYToPPTX(BSTR bsInput, BSTR bsOutput)
{
#ifdef _DEBUG
	m_strTempDir = _T("C:\\PPTMemory\\PPTX_test");
#endif

	int len = m_strTempDir.GetLength();
	while (len != 0 && m_strTempDir[len - 1] == (TCHAR)'\\')
	{
		m_strTempDir.Delete(len - 1);
		--len;
	}

	NSBinPptxRW::CPPTXWriter oWriter(m_fCallbackResource, m_pCallbackArg);
	oWriter.Init(m_strTempDir);

	CFile oFileBinary;
	oFileBinary.OpenFile((CString)bsInput);
	LONG lFileSize = (LONG)oFileBinary.GetFileSize();
	BYTE* pSrcBuffer = new BYTE[lFileSize];
	oFileBinary.ReadFile(pSrcBuffer, (DWORD)lFileSize);
	oFileBinary.CloseFile();
    CString srcFolder = FileSystem::Directory::GetFolderPath((CString)bsInput);
	oWriter.OpenPPTY(pSrcBuffer, lFileSize, srcFolder, m_strFolderThemes);
	RELEASEARRAYOBJECTS(pSrcBuffer);

    CString strBsOutput = bsOutput;
    HRESULT hRes = m_fCallbackCompress(m_pCallbackArg, m_strTempDir, strBsOutput) ? S_OK : S_FALSE;

	// нужно удалить темповую папку
	RemoveDirOrFile(m_strTempDir);

	return hRes;
}
int CPPTXFile::RemoveDirOrFile(CString sPath, bool bIsRemoveHead)
{
 #ifdef WIN32
    DWORD dwFileAttrib = ::GetFileAttributes( sPath );
	if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
	{
		DWORD dwResult = 0;
		if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
		{
			HANDLE Handle;
			WIN32_FIND_DATA FindData;
			DWORDLONG Result = 0;

			Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
			if ( Handle == INVALID_HANDLE_VALUE )
				return 0;
			do
			{
				BOOL bRes = TRUE;
				if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
					if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
						Result += RemoveDirOrFile( sPath + _T("\\") + FindData.cFileName ); //
					else
						bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
				if( FALSE == bRes )
					dwResult += 1;
			}
			while( FindNextFile( Handle, &FindData ) != 0 );
			FindClose( Handle );

			if (bIsRemoveHead)
			{
				BOOL bRes = RemoveDirectory( sPath );
				if( FALSE == bRes )
					dwResult += 1;
			}
		}
		else
		{
			if( FALSE == DeleteFile( sPath ) )
				dwResult = 1;
		}

		return dwResult;
    }// 0 - все закончилось хорошо
#else

#endif
	return 0;
}
