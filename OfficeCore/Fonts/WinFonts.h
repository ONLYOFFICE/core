// FontManager.h : Declaration of the CFontManager

#pragma once

#include "./File.h"
#include "./WinFont.h"
#include "./WinFontStorage.h"

[object, uuid("F30AE253-88EF-4ae2-81B6-D9E1502082FF"), dual, pointer_default(unique)]
__interface IWinFonts : IDispatch
{
	[id(1001)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CWinFonts
[
	coclass,
	default(IWinFonts),
	threading(apartment),
	vi_progid("OfficeCore.WinFonts"),
	progid("OfficeCore.WinFonts.1"),
	version(1.0),
	uuid("86C455AD-20D4-4071-A431-06C79C913B39")
]
class ATL_NO_VTABLE CWinFonts : public IWinFonts
{
private:
	WinFontsStatusStorage    m_oSS;
	
	CWinFontsInfoStorage    *m_pInfoStorage;
	CWinFontsStatusStorage  *m_pStatusStorage;

	CString m_strInputFontDirectory;
	CString	m_strDumpFontSelectionFile;

	CWinFontList			m_oList;

public:
	CWinFonts() : m_oList(_T(""))
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

				FT_Library pLibrary = NULL;
				BYTE* pData = NULL;
				LONG lLen2 = 0;
				
				if (!FT_Init_FreeType( &pLibrary ))
				{
					if (_T("") == m_strInputFontDirectory)
					{
						pData = CWinFontList::GetWinFontsData( pLibrary, lLen2 );
					}
					else
					{
						CWinFontList oList(pLibrary, m_strInputFontDirectory);
						oList.ToBuffer(&pData, &lLen2);
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
		
		if (!FT_Init_FreeType( &pLibrary ))
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

public:

	//----- IAVSGraphicsBase ----------------------------------------------------------------------------
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

	STDMETHOD(GetWinFontByParams)(BSTR bsFontParams, BSTR *pbsFontPath, long *plIndex)
	{
		CWinFontInfo *pFontInfo = m_oList.GetByParams( (CString)bsFontParams );
		if ( NULL == pFontInfo )
		{
			*pbsFontPath = CStringW( _T("") ).AllocSysString();
			*plIndex     = 0;

			return S_FALSE;
		}

		*pbsFontPath = pFontInfo->m_wsFontPath.AllocSysString();
		*plIndex     = pFontInfo->m_lIndex;

		return S_OK;
	}
};