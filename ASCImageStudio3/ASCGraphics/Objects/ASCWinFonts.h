// FontManager.h : Declaration of the CFontManager

#pragma once

#include "..\Interfaces\IASCWinFonts.h"
#include "Font\WinFont.h"
#include "Font\WinFontStorage.h"

#include "..\Interfaces\BaseThread.h"
#include "../../../Common/OfficeDrawing/File.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CASCWinFonts

[
	coclass,
	default(IASCWinFonts),
	threading(apartment),
	vi_progid("AVSOfficeWinFonts.WinFonts"),
	progid("AVSOfficeWinFonts.WinFonts.1"),
	version(1.0),
	uuid("051DCF8B-2932-4f04-A50F-5BDDC1DF3384"),
	helpstring("WinFonts Class")
]
class ATL_NO_VTABLE CASCWinFonts : public IASCWinFonts
{

private:

	WinFontsStatusStorage    m_oSS;
	
	CWinFontsInfoStorage    *m_pInfoStorage;
	CWinFontsStatusStorage  *m_pStatusStorage;

	CString m_strInputFontDirectory;
	CString	m_strDumpFontSelectionFile; // "debug" teamlab server fonts

public:
	CASCWinFonts()
	{
		m_strDumpFontSelectionFile = _T("");
		m_strInputFontDirectory = _T("");
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_pStatusStorage = NULL;
		m_pInfoStorage   = NULL;

		
		m_pStatusStorage = new CWinFontsStatusStorage( STATUS_STORAGE_NAME );
		do
		{
			bool bGetMaster = false;
			m_pStatusStorage->GetStatus( &bGetMaster, &m_oSS );

			if ( STIF_BROKEN == m_oSS.m_sStatus || STIF_ERROR == m_oSS.m_sStatus )
			{
				m_oSS.m_sStatus = STIF_CREATING;
				m_pStatusStorage->WriteStruct( &m_oSS );

				//FT_Library pLibrary = NULL;
				//CString sXml;

				//if( !FT_Init_FreeType( &pLibrary ) )
				//{
				//	sXml = CWinFontList::GetWinFontsXml( pLibrary );
				//	FT_Done_FreeType( pLibrary );
				//}

				//m_oSS.m_sStatus   = STIF_AVAILABLE;
				//WinFontsInfoStorage oInfo;
				//BSTR bsXml = sXml.AllocSysString();
				//oInfo.GenerateInfo( bsXml );
				//LONG64 lLen = oInfo.GetLength();

				//oInfo.m_lCount  = 1;
				//m_oSS.m_lLength = lLen;

				FT_Library pLibrary = NULL;
				BYTE* pData;
				LONG lLen2;

				
				if( !FT_Init_FreeType( &pLibrary ) )
				{
					if (_T("") == m_strInputFontDirectory)
					{
						pData = CWinFontList::GetWinFontsData( pLibrary, lLen2 );
					}
					else
					{
						CWinFontList oList(pLibrary, m_strInputFontDirectory);
						oList.ToBuffer(&pData, & lLen2);
					}
					FT_Done_FreeType( pLibrary );
				}

				m_oSS.m_sStatus   = STIF_AVAILABLE;
				WinFontsInfoStorage oInfo;
				oInfo.GenerateInfo( pData, lLen2 );

				if (_T("") != m_strDumpFontSelectionFile)
				{
					CFile oFile;
					if (S_OK == oFile.CreateFile(m_strDumpFontSelectionFile))
					{
						oFile.WriteFile((BYTE*)pData, (DWORD)lLen2);
						oFile.CloseFile();
					}
				}
				
				LONG64 lLen = lLen2;

				oInfo.m_lCount  = 1;
				m_oSS.m_lLength = lLen;


				RELEASEOBJECT(m_pInfoStorage);
				m_pInfoStorage = new CWinFontsInfoStorage( STATUS_STORAGE_NAME, lLen );

				m_pInfoStorage->WriteStruct( &oInfo );
				m_pStatusStorage->WriteStruct( &m_oSS );

				RELEASEARRAYOBJECTS(pData);
			}
			else if ( STIF_CREATING == m_oSS.m_sStatus )
			{
				Sleep ( 100 );
			}
			else if ( STIF_AVAILABLE == m_oSS.m_sStatus )
			{
				RELEASEOBJECT(m_pInfoStorage);
				m_pInfoStorage = new CWinFontsInfoStorage( STATUS_STORAGE_NAME, m_oSS.m_lLength );

				WinFontsInfoStorage oInfo;
				m_pInfoStorage->ReadStruct( &oInfo );
			}
		}
		while ( STIF_CREATING == m_oSS.m_sStatus );

		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEOBJECT(m_pStatusStorage);
		RELEASEOBJECT(m_pInfoStorage);
	}

	void ReInit()
	{
		FT_Library pLibrary = NULL;
		BYTE* pData;
		LONG lLen2;
		
		if( !FT_Init_FreeType( &pLibrary ) )
		{
			if (_T("") == m_strInputFontDirectory)
			{
				pData = CWinFontList::GetWinFontsData( pLibrary, lLen2 );
			}
			else
			{
				CWinFontList oList(pLibrary, m_strInputFontDirectory);
				oList.ToBuffer(&pData, &lLen2, m_strInputFontDirectory);
			}
			FT_Done_FreeType( pLibrary );
		}

		if (_T("") != m_strDumpFontSelectionFile)
		{
			CFile oFile;
			if (S_OK == oFile.CreateFile(m_strDumpFontSelectionFile))
			{
				oFile.WriteFile((BYTE*)pData, (DWORD)lLen2);
				oFile.CloseFile();
			}
		}
	}

private:

public:

	//----- IASCGraphicsBase ----------------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName = ParamName;
		if ( _T("DumpBinaryPath") == sParamName && VT_BSTR == ParamValue.vt )
		{
			m_strDumpFontSelectionFile = (CString)ParamValue.bstrVal;
			ReInit();
		}
		if ( _T("InputFontsPath") == sParamName && VT_BSTR == ParamValue.vt )
		{
			m_strInputFontDirectory = (CString)ParamValue.bstrVal;
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IASCGraphicsBase** ppGraphicsBase)
	{
		return S_OK;
	}

};

class CThreadFonts : public CBaseThread
{
private:
	IASCWinFonts* m_pFonts;

public:
	CThreadFonts() : CBaseThread(0)
	{
		m_pFonts = NULL;
		StartWork(THREAD_PRIORITY_NORMAL);
	}
	~CThreadFonts()
	{
		StopWork();
		RELEASEINTERFACE(m_pFonts);
	}

protected:
	virtual DWORD ThreadProc()
	{
		CoInitialize(NULL);
		CoCreateInstance(__uuidof(CASCWinFonts), NULL, CLSCTX_INPROC, __uuidof(IASCWinFonts), (void**)&m_pFonts);
		CoUninitialize();

		m_bRunThread = FALSE;
		return 0;
	}
};

