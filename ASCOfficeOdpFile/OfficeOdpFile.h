// OfficeODPFile3.h : Declaration of the CODPFile3
#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "..\Common\OfficeFileTemplate.h"

#include "Odp.h"
#include "Odp2DrawingXML/Converter.h"
#include "RSA\XMLEncoder.h"

#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment( lib, "shell32.lib" ) // добавить shell32.lib

// IAVSOfficeODPFile
[object, uuid("803CCF2C-1A65-4574-9C68-2242AE80EABE"), dual, pointer_default(unique)]
__interface IAVSOfficeODPFile:  IAVSOfficeFileTemplate
{
	[id(100 + 0), propget] HRESULT DrawingXml( [out, retval] BSTR* pbstrXml );
   	[id(100 + 1), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
   	[id(100 + 1), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
	
	[id(1000 + 0)] HRESULT Is_ODPFile([in] BSTR fileName, [out, retval] VARIANT_BOOL* Result);
	
	[id(1000 + 1)] HRESULT GetDVDXml([out, retval] BSTR* bstrPTTXml);
	[id(1000 + 2)] HRESULT GetBluRayXml([out, retval] BSTR* bstrDVDXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IAVSOfficeODPFileEvents
[dispinterface,	uuid("3BE5A1B6-D05C-4e8e-9522-16647B9EE2E9")]
__interface _IAVSOfficeODPFileEvents
{
	[id(1)] HRESULT ProcessVideoFile([in] LONG ID, [in] LONG Percent, [in, out, ref] SHORT* Cancel);
};	

// CAVSOfficeODPFile
[coclass, uuid("D36BCFE7-5AF8-4df1-BB71-9F9504E16A17"), event_source(com), threading(apartment), vi_progid("AVSOfficeOdpFile.OfficeOdpFile"), progid("AVSOfficeOdpFile.OfficeOdpFile.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficeODPFile : 
	public IAVSOfficeODPFile//, 
{
public:
	CAVSOfficeODPFile()
	{
		WCHAR buffer[4096];
		GetTempPathW(4096, buffer);
		m_strTempDirectory = CStringW(buffer);
		GetLongPathNameW(m_strTempDirectory.GetString(), buffer, 4096);
		m_strTempDirectory = CStringW(buffer) + CStringW("_ODP\\");
	}

	__event __interface _IAVSOfficeODPFileEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		Folder = NULL;
		m_pOfficeUtils = NULL;
		return CoCreateInstance( __uuidof(OfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(OfficeUtils::IOfficeUtils), (void **)&(this->m_pOfficeUtils) );
	}

	void FinalRelease()
	{
		if ( this->m_pOfficeUtils != NULL )
		{
			this->m_pOfficeUtils->Release();
			this->m_pOfficeUtils = NULL;
		}
		if ( this->Folder != NULL )
		{
			delete Folder;
			Folder = NULL;
		}
	}

private:
	Odp::Folder* Folder;
	CStringW m_strTempDirectory;
	OfficeUtils::IOfficeUtils* m_pOfficeUtils;
	CXMLEncoder m_oEncoder;

	NSPresentationEditor::CDocument m_oDocument;
public:
	STDMETHOD(get_DrawingXml)(BSTR* pbstrXml)
	{
		if(Folder == NULL)
			return S_FALSE;

		return S_OK;
	}

	STDMETHOD(get_TempDirectory)(BSTR* pbstrDir)
	{
		*pbstrDir = m_strTempDirectory.AllocSysString();
		return S_OK;
	}

	STDMETHOD(put_TempDirectory)(BSTR bstrDir)
	{
		CStringW TempStr(bstrDir);
		if(PathIsDirectoryW(TempStr.GetString()))
		{
			if(TempStr.Right(1) != L"\\")
				TempStr += L"\\";
			m_strTempDirectory = TempStr;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(Is_ODPFile)(BSTR fileName, VARIANT_BOOL* Result)
	{
		return S_OK;
	}

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		CStringW localTempDir(sDstPath);
		if((sDstPath != NULL) || (localTempDir != ""))
		{
			int res = SHCreateDirectoryExW(NULL, localTempDir.GetString(), NULL);
			if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
				return S_FALSE;
			put_TempDirectory(sDstPath);
		}
		else
		{
			int res = SHCreateDirectoryExW(NULL, m_strTempDirectory.GetString(), NULL);
			if((res != ERROR_SUCCESS) && (res != ERROR_ALREADY_EXISTS) && (res != ERROR_FILE_EXISTS))
				return S_FALSE;
		}
		localTempDir = m_strTempDirectory;

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

		HRESULT hr = m_pOfficeUtils->ExtractToDirectory( sSrcFileName, localTempDir.AllocSysString(), NULL, 0);
		if(hr!=S_OK)
			return hr;

//#ifdef NDEBUG
//		try{
//#endif
			if(Folder != NULL)
				delete Folder;
			Folder = new Odp::Folder;

			if(!Folder->isValid(localTempDir.GetString()))
			{
				delete Folder;
				Folder = NULL;
				return S_FALSE;
			}
			Folder->read(localTempDir.GetString());
//#ifdef NDEBUG
//		}
//		catch(...){
//			return S_FALSE;
//		}
//#endif
/*
#ifndef NDEBUG
		if(Folder == NULL)
			return S_FALSE;

		Odp2DrawingXML::Converter Converter;

		CStringW xml;
		Converter.Convert(*Folder, xml);

		HANDLE XMLFile = ::CreateFileW(L"C:\\VideoSourceODP.xml", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		DWORD BytesWritten;
		::WriteFile(XMLFile, xml.GetString(), xml.GetLength()*sizeof(WCHAR), &BytesWritten, NULL);
		::CloseHandle(XMLFile);
#endif
*/
		Odp2DrawingXML::Converter Converter;
		Converter.Convert(*Folder, m_oDocument);
		return S_OK;
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
//#ifdef NDEBUG
//		try{
//#endif
			if(Folder == NULL)
				return S_FALSE;
			Folder->write(sSrcPath);
//#ifdef NDEBUG
//		}
//		catch(...){
//			return S_FALSE;
//		}
//#endif

		return m_pOfficeUtils->CompressFileOrDirectory( sSrcPath, sDstFileName, -1 );
	}
	
	// для конвертации с поддержкой интерактива...	
	STDMETHOD(GetDVDXml)(BSTR* bstrDVDXml)
	{
		return S_OK;
	}
	STDMETHOD(GetBluRayXml)(BSTR* bstrBluRayXml)
	{
		return S_OK;
	}
#pragma region
//-----------------------------AdditionalParams--------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
			ParamValue->punkVal = m_oEncoder.GetBlowfishKey();
		}
		else if (_T("EditorXml") == sParamName)
		{
			CString xml = m_oDocument.ToXmlEditor2();
			ParamValue->bstrVal = xml.AllocSysString();
		}
		return S_OK;
	}
#pragma endregion
};

