#pragma once
#include "resource.h" 
#include "../Common/OfficeFileTemplate.h"
#include "DocWrapper/XlsxSerializer.h"
#include "../XlsxSerializerCom/Common/Common.h"
#include "../XlsxSerializerCom/Common/BinReaderWriterDefines.h"
#include "../DesktopEditor/common/Directory.h"

[
	object,
	uuid("B7AD8AA6-A103-4260-82BE-338C32543B90"),
	dual,	helpstring("IAVSOfficeXlsxSerizer Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeXlsxSerizer : public IAVSOfficeFileTemplate
{
	[id(20)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(30)] HRESULT LoadChart([in] BSTR bsFilename, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(31)] HRESULT SaveChart([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsFilename, [in] BSTR bsContentTypePath, [out] BSTR* bsContentTypeElement);
	[id(32)] HRESULT SetDrawingConverter([in] IUnknown* pDocument);

	[id(120)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(130)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[
	coclass,
	default(IAVSOfficeXlsxSerizer),
	event_receiver(native),
	threading(apartment),
	vi_progid("ASCOfficeXlsxSerizer.AVSDocume"),
	progid("ASCOfficeXlsxSerizer.AVSDocu.1"),
	version(1.0),
	uuid("BDE1A2A6-2EE0-4507-BD2E-0C42DA4101C2"),
	helpstring("ASCOfficeXlsxSerizer Class")
]
class ATL_NO_VTABLE CAVSOfficeXlsxSerizer : public IAVSOfficeXlsxSerizer
{
private:
	BinXlsxRW::CXlsxSerializer m_oXlsxSerializer;
	IUnknown* m_pThis;
public:
	CAVSOfficeXlsxSerizer()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR bstrXMLOptions)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oXlsxSerializer.setComInterface(m_pThis);
		}

		CString sMediaDir = sDstPath;
		sMediaDir.Append(_T("\\xl\\media"));
		// File Type (Можно парсить не два раза, а один, если передавать в ReadFile не опции, а параметры)
		BYTE fileType;
		UINT nCodePage;
		WCHAR wcDelimiter;
		SerializeCommon::ReadFileType(CString(bstrXMLOptions), fileType, nCodePage, wcDelimiter);

		if (BinXlsxRW::c_oFileTypes::CSV != fileType)
		{
			CString sXlDir;sXlDir.Format(_T("%s\\xl"), sDstPath);
			CString sThemeDir;sThemeDir.Format(_T("%s\\xl\\%s"), sDstPath, OOX::FileTypes::Theme.DefaultDirectory());
			CString sThemePath;sThemePath.Format(_T("%s\\%s"), sThemeDir, OOX::FileTypes::Theme.DefaultFileName());
			CString sThemeRelsPath;sThemeRelsPath.Format(_T("%s\\_rels"), sThemeDir);

			NSDirectory::CreateDirectory(string2std_string(sXlDir));
			NSDirectory::CreateDirectory(string2std_string(sThemeDir));
			NSDirectory::CreateDirectory(string2std_string(sThemeRelsPath));
			NSDirectory::CreateDirectory(string2std_string(sMediaDir));

			CreateTheme(sThemePath);
		}
		bool bRes = m_oXlsxSerializer.loadFromFile(CString(sSrcFileName), CString(sDstPath), CString(bstrXMLOptions), sMediaDir);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oXlsxSerializer.setComInterface(m_pThis);
		}
		bool bRes = m_oXlsxSerializer.saveToFile(CString(sDstFileName), CString(sSrcPath), CString(sXMLOptions)) ? S_OK : S_FALSE;
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(LoadChart)(BSTR bsFilename, SAFEARRAY** ppBinary)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oXlsxSerializer.setComInterface(m_pThis);
		}
		unsigned char* pData = NULL;
		long lDataSize = 0;
		bool bRes = m_oXlsxSerializer.loadChart(CString(bsFilename), &pData, lDataSize);
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
	STDMETHOD(SaveChart)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsFilename, BSTR bsContentTypePath, BSTR* bsContentTypeElement)
	{
		if(NULL == m_pThis)
		{
			this->QueryInterface( __uuidof(IUnknown), (void**)&m_pThis );
			m_oXlsxSerializer.setComInterface(m_pThis);
		}
		CString* sContentTypeElement = NULL;
		bool bRes = m_oXlsxSerializer.saveChart(pBinaryObj, lStart, lLength, CString(bsFilename), CString(bsContentTypePath), &sContentTypeElement);
		if(NULL != sContentTypeElement)
			*bsContentTypeElement = sContentTypeElement->AllocSysString();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(SetDrawingConverter)(IUnknown* pDocument)
	{
		m_oXlsxSerializer.setDrawingConverter(pDocument);
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		m_oXlsxSerializer.setFontDir(CString(bsFontDir));
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_oXlsxSerializer.setEmbeddedFontsDir(CString(ParamValue.bstrVal));
			return S_OK;
		}		
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
private:
	void CreateTheme(CString sThemePath)
	{
		HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_XLSX_THEME), _T("XLSXSER"), sThemePath);
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
};