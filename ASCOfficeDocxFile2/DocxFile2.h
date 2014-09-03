#pragma once

#include "resource.h"       // main symbols
#include "DocWrapper/DocxSerializer.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IAVSOfficeDocxFile2
[ object, uuid("98B1ECA8-9575-4eec-B327-8F8BA3FA232C"), dual, pointer_default(unique) ]
__interface IAVSOfficeDocxFile2: IDispatch
{
	[id(1)] HRESULT OpenFile([in] BSTR bsInputDir, [in] BSTR bsFileDst);
	[id(2)] HRESULT GetJfdoc([in] BSTR bsInputDir, [out] BSTR* bsJfdoc);

	[id(3)] HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(4)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(5)] HRESULT SetUseSystemFonts([in] BOOL useSystemFonts);
	[id(6)] HRESULT GetBinaryContent([in] BSTR bsTxContent, [out, satype("BYTE")] SAFEARRAY** ppBinary);

	[id(10)] HRESULT Write([in] BSTR bstrFileIn, [in] BSTR bstrFileOut);
	[id(20)] HRESULT GetXmlContent([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [out] BSTR* bsXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// _IAVSOfficeDocxFile2Events
[uuid("C3CF75C3-28FE-4b2c-A770-5952ADF4EAC2"), dispinterface]
__interface _IAVSOfficeDocxFile2Events
{
};

// CAVSOfficeDocxFile2
[ coclass, default(IAVSOfficeDocxFile2), threading(apartment), event_source(com), vi_progid("DocxFile2"), progid("DocxFile2.Rend.1"), version(1.0), uuid("CD07583A-6362-454f-A14E-542AE706FFBC") ]
class ATL_NO_VTABLE CAVSOfficeDocxFile2 : public IAVSOfficeDocxFile2
{
public:
	__event __interface _IAVSOfficeDocxFile2Events;

private:
	BinDocxRW::CDocxSerializer m_oCDocxSerializer;
	IUnknown* m_pThis;
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CAVSOfficeDocxFile2()
	{
		m_pThis = NULL;
	}
	~CAVSOfficeDocxFile2()
	{
		RELEASEINTERFACE(m_pThis);
	}

public:
	STDMETHOD(OpenFile)(BSTR bsInputDir, BSTR bsFileDst)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oCDocxSerializer.setComInterface(m_pThis);
		}
		bool bRes = m_oCDocxSerializer.saveToFile(BstrToStdString(bsFileDst), BstrToStdString(bsInputDir), std::wstring(_T("")));
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(GetJfdoc)(BSTR bsInputDir, BSTR* bsJfdoc)
	{
		return S_OK;
	}
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir)
	{
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		m_oCDocxSerializer.setFontDir(BstrToStdString(bsFontDir));
		return S_OK;
	}

	STDMETHOD(SetUseSystemFonts)(BOOL useSystemFonts)
	{
		//DocWrapper::FontProcessor::useSystemFonts = (useSystemFonts == TRUE);
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_oCDocxSerializer.setEmbeddedFontsDir(BstrToStdString(ParamValue.bstrVal));
			return S_OK;
		}
		else if (_T("FontDir") == sParamName && ParamValue.vt == VT_BSTR)
		{
			m_oCDocxSerializer.setFontDir(BstrToStdString(ParamValue.bstrVal));
		}
		else if (_T("SaveChartAsImg") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_oCDocxSerializer.setSaveChartAsImg(VARIANT_FALSE != ParamValue.boolVal);
		}
		else if (_T("NoBase64Save") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_oCDocxSerializer.setIsNoBase64Save(VARIANT_FALSE != ParamValue.boolVal);
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetBinaryContent)(BSTR bsTxContent, SAFEARRAY** ppBinary)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oCDocxSerializer.setComInterface(m_pThis);
		}
		unsigned char* pData = NULL;
		long lDataSize = 0;
		bool bRes = m_oCDocxSerializer.GetBinaryContent(BstrToStdString(bsTxContent), &pData, lDataSize);
		if(NULL != pData && lDataSize > 0)
		{
			SAFEARRAYBOUND	rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = lDataSize;
			LPSAFEARRAY pArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

			BYTE* pDataD = (BYTE*)pArray->pvData;
			BYTE* pDataS = pData;
			memcpy(pDataD, pDataS, lDataSize);

			*ppBinary = pArray;
		}
		RELEASEARRAYOBJECTS(pData);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(Write)(BSTR bstrFileIn, BSTR bstrDirectoryOut)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oCDocxSerializer.setComInterface(m_pThis);
		}
		CString sDirectoryOut = bstrDirectoryOut;
		CString sThemePath;
		CString sMediaPath;
		CreateDocument(sDirectoryOut, sThemePath, sMediaPath);
		bool bRes = m_oCDocxSerializer.loadFromFile(BstrToStdString(bstrFileIn), BstrToStdString(bstrDirectoryOut), std::wstring(_T("")), std::wstring(sThemePath.GetString()), std::wstring(sMediaPath.GetString()));
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(GetXmlContent)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR* bsXml)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oCDocxSerializer.setComInterface(m_pThis);
		}
		std::wstring sRes;
		bool bRes = m_oCDocxSerializer.GetXmlContent((BYTE*)pBinaryObj->pvData, pBinaryObj->rgsabound[0].cElements, lStart, lLength, sRes);
		(*bsXml) = CString(sRes.c_str()).AllocSysString();
		return bRes ? S_OK : S_FALSE;
	}
private:
	void CreateDocument(CString strDirectory, CString& sThemePath, CString& sMediaPath)
	{
		HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

		// rels
		CString strRels = strDirectory + _T("\\_rels");
		CreateDirectory(strRels, NULL);

		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_RELS), _T("DOCXWR"), strRels + _T("\\.rels"));



		// docProps
		CString strDocProps = strDirectory + _T("\\docProps");
		CreateDirectory(strDocProps, NULL);

		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_APP),  _T("DOCXWR"), strDocProps + _T("\\app.xml"));
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CORE), _T("DOCXWR"), strDocProps + _T("\\core.xml"));

		// word
		CString strWord = strDirectory + _T("\\word");
		CreateDirectory(strWord, NULL);

		sMediaPath = strWord + _T("\\media");

		//LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_SETTINGS),		_T("DOCXWR"), strWord + _T("\\settings.xml"));
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_WEBSETTINGS),	_T("DOCXWR"), strWord + _T("\\webSettings.xml"));

		// theme
		CString strTheme = strWord + _T("\\theme");
		CreateDirectory(strTheme, NULL);
		CString strThemeRels = strTheme + _T("\\_rels");
		CreateDirectory(strThemeRels, NULL);
		sThemePath = strTheme + _T("\\theme1.xml");
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_THEME),	_T("DOCXWR"), sThemePath);

		// documentRels
		CreateDirectory(strWord + _T("\\_rels"), NULL);
	}
	void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
	{
		HRSRC hrRes = FindResource(hInst, sResName, sResType);
		if (!hrRes)
			return;

		HGLOBAL hGlobal = LoadResource(hInst, hrRes);
		DWORD sz = SizeofResource(hInst, hrRes);
		void* ptrRes = LockResource(hGlobal);

		CFile oFile;
		oFile.CreateFile(strDstFile);
		oFile.WriteFile(ptrRes, sz);

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}	
	std::wstring BstrToStdString(BSTR sVal)
	{
		return std::wstring(sVal, SysStringLen(sVal));
	}
};