// FB2File.cpp : Implementation of CWSHtmlFile

#include "stdafx.h"
#include "FB2File.h"
#include "FB2Document/FB2Document.h"
#include "DocxDocument/DocxDocument.h"
#include "Converter/Converter.h"

#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>


// CFB2File

HRESULT CFB2File::FinalConstruct()
{
	m_pOfficeUtils = NULL;
	return CoCreateInstance(__uuidof(ASCOfficeUtils::COfficeUtils),
													NULL, 
													CLSCTX_INPROC_SERVER, 
													__uuidof(ASCOfficeUtils::IOfficeUtils), 
													(void **)&(m_pOfficeUtils));
}


void CFB2File::FinalRelease()
{
	if (m_pOfficeUtils != NULL )
	{
		m_pOfficeUtils->Release();
		m_pOfficeUtils = NULL;
	}
}


STDMETHODIMP CFB2File::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{	
	//надо ли вставлять содержание
	bool bGenerateContent = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr pXmlOpt( L"Msxml2.DOMDocument");
		if (pXmlOpt->loadXML(sXMLOptions) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodePtr pXmlGenerateContent = pXmlOpt->selectSingleNode(L"Options/FB2Options/GenerateContent/@val");
			if(NULL != pXmlGenerateContent )
			{
				CString sGenerateContent = pXmlGenerateContent->Gettext();
				sGenerateContent.MakeLower();
				if( _T("0") == sGenerateContent || _T("false") ==sGenerateContent )
					bGenerateContent = false;
				else
					bGenerateContent = true;
			}
		}
	}
	catch(...)
	{
	}
	CString fb2File	 = sSrcFileName;
	CString docxPath = sDstPath;
	CString fb2Path	 = docxPath + _T("/FB2");
	
	CreateDirectoryW( docxPath, NULL );
	CreateDirectoryW( fb2Path, NULL );
	CopyFileW( fb2File, fb2Path + _T("\\1"), FALSE );

	CString fb2FileInDir;
	fb2FileInDir = (fb2Path + _T("\\1"));
	CString origin = docxPath + _T("/Origin");
	createOriginDocx(origin);

	bool SetContent = true;

	try
	{
		FB2Converter::FB2Document fb2;
		FB2Converter::DocxDocument docx(origin, SetContent,bGenerateContent);
		FB2Converter::Converter converter(bGenerateContent);

		bool res;
		res = fb2.Read(fb2FileInDir);
		m_sMetadata = fb2.m_strMeta;
		res = docx.Read();

		res = converter.Convert(fb2, docx);
		res = docx.Write(docxPath);
		//копируем footer1.xml
		CopyFileW(origin + _T("\\word\\footer1.xml"), docxPath + _T("\\word\\footer1.xml"), FALSE);
	}
	catch(...)
	{
		RemoveDir(fb2Path);
		RemoveDir(origin);
		return S_FALSE;
	}

	RemoveDir(fb2Path);
	RemoveDir(origin);

	return S_OK;
}


STDMETHODIMP CFB2File::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	return S_OK;
}
STDMETHODIMP CFB2File::SetAdditionalParam( BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP CFB2File::GetAdditionalParam( BSTR ParamName, VARIANT* ParamValue)
{
	CComBSTR ParamStr = ParamName;
	if (ParamStr == L"Metadata")
	{
		(*ParamValue).vt = VT_BSTR;
		(*ParamValue).bstrVal = m_sMetadata.AllocSysString();
	}
	;
	return S_OK;
}
void CFB2File::createOriginDocx(const CString path) const
{
	CString docxFile = path + _T("/origin.docx");
	CreateDirectoryW( path, NULL );

	LoadFromResource(MAKEINTRESOURCE(IDR_DOCX_TEMPLATE), L"Document", _bstr_t(docxFile));
	m_pOfficeUtils->ExtractToDirectory(_bstr_t(docxFile), _bstr_t(path), NULL, 0);
	DeleteFileW(docxFile);
}


const unsigned long CFB2File::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
{
	HMODULE hMod = GetModuleHandle(_T("ASCOfficeFB2File.dll"));
	if (hMod)
	{
		HRSRC hRes = FindResource(hMod, lpResName, lpResType);
		if (hRes)
		{
			HGLOBAL hGlob = LoadResource(hMod, hRes);
			if (hGlob)
			{
				BYTE *lpbArray = (BYTE*)LockResource(hGlob);
				if (lpbArray)
				{
					const DWORD dwFileSize = SizeofResource(hMod, hRes);
					if (dwFileSize != 0)
					{
						int hFile = 0;
						if (!_wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE))
						{
							_write(hFile, lpbArray, dwFileSize);
							_close(hFile);
						}
					}
				}
			}
		}
	}
	return GetLastError();
}


int CFB2File::RemoveDir(const CString sPath )
{
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
						Result += RemoveDir( sPath + _T("\\") + FindData.cFileName ); //
					else
						bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
				if( FALSE == bRes )
					dwResult += 1;
				}
				while( FindNextFile( Handle, &FindData ) != 0 );
				FindClose( Handle );
				BOOL bRes = RemoveDirectory( sPath );
				if( FALSE == bRes )
					dwResult += 1;
			}
			else
			{
				if( FALSE == DeleteFile( sPath ) )
					dwResult = 1;
			}

			return dwResult;
		}// 0 - все закончилось хорошо
		return 0;
}

