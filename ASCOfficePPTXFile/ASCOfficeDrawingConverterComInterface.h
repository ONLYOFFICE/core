// AVSOfficeDrawingConverter.h : Declaration of the CAVSOfficeDrawingConverter.h

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "../Common/OfficeFileTemplate.h"
#include "ASCOfficeDrawingConverter.h"

// IAVSODObjectProps
[object, uuid("44B693E1-F4F9-4547-ACEF-0AE037C84485"), dual, pointer_default(unique)]
__interface IAVSODObjectProps : IDispatch
{
	[id(1)]			HRESULT GetProperty([in] LONG lId, [out, retval] VARIANT* pProp);
	[id(2)]			HRESULT SetProperty([in] LONG lId, [in] VARIANT prop);	
	
	[id(1000)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1001)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// IAVSOfficeDrawingConverter
[object, uuid("8E0FBC40-9B34-40bf-B68A-0FA320E1B004"), dual, pointer_default(unique)]
__interface IAVSOfficeDrawingConverter : IDispatch
{
	[id(1)]				HRESULT SetMainDocument([in] IUnknown* pDocument);
	[id(2)]				HRESULT SetRelsPath([in] BSTR bsRelsPath);
	[id(3)]				HRESULT	SetMediaDstPath([in] BSTR bsMediaPath);
	
	[id(9)]				HRESULT AddShapeType([in] BSTR bsXml);
	[id(10)]			HRESULT AddObject([in] BSTR bsXml, [out] BSTR* pMainProps, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(11)]			HRESULT SaveObject([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsMainProps, [out] BSTR* bsXml);
	[id(12)]			HRESULT SaveObjectEx([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsMainProps, [in] LONG lDocType, [out] BSTR* bsXml);

	[id(13)]			HRESULT GetRecordBinary([in] LONG lRecordType, [in] BSTR bsXml, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(14)]			HRESULT GetRecordXml([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] LONG lRecType, [in] LONG lDocType, [out] BSTR* bsXml);
		
	[id(20)]			HRESULT AddObject2([in] BSTR bsXml, [in, satype("BYTE")] SAFEARRAY* pBinaryObj, [out] BSTR* pXmlOutput);
	
	[id(30)]			HRESULT GetThemeBinary([in] BSTR bsThemeFilePath, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(31)]			HRESULT SaveThemeXml([in, satype("BYTE")] SAFEARRAY* pBinaryTheme, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsThemePath);

	[id(40)]			HRESULT SetDstContentRels();
	[id(41)]			HRESULT SaveDstContentRels([in] BSTR bsRelsPath);
	[id(42)]			HRESULT WriteRels([in] BSTR bsType, [in] BSTR bsTarget, [in] BSTR bsTargetMode, [out] LONG* lId);

	[id(50)]			HRESULT LoadClrMap([in] BSTR bsXml);

	[id(60)]			HRESULT GetTxBodyBinary([in] BSTR bsXml, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(61)]			HRESULT GetTxBodyXml([in, satype("BYTE")] SAFEARRAY* pBinary, [in] LONG lStart, [in] LONG lLength, BSTR* pbstrXml);
	[id(62)]			HRESULT SetFontDir([in] BSTR bsFontDir);
	
	[id(1000)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1001)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CAVSODObjectProps
[coclass, uuid("4AB04382-4B51-4674-A691-BE2691A5F387"), threading(apartment), vi_progid("AVSOfficePPTXFile.ODObjectProps"), progid("AVSOfficePPTXFile.ODObjectProps.1"), version(1.0), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSODObjectProps 
	:	public IAVSODObjectProps
{
private:
	CElementProps m_oProps;

public:

	CAVSODObjectProps() : m_oProps()
	{
	}

	~CAVSODObjectProps()
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
	STDMETHOD(GetProperty)(LONG lId, VARIANT* pProp)
	{
		return m_oProps.GetProperty(lId, pProp);
	}
	STDMETHOD(SetProperty)(LONG lId, VARIANT prop)
	{
		return m_oProps.SetProperty(lId, prop);
	}
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
};

// CAVSOfficeDrawingConverter
[coclass, uuid("BA240E3F-CFE4-45d7-96BB-97CDD73F63C3"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTXFile.ODConverter"), progid("AVSOfficePPTXFile.ODConverter.1"), version(1.0), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSOfficeDrawingConverter 
	:	public IAVSOfficeDrawingConverter
{
private:
	NSBinPptxRW::CDrawingConverter m_oDrawingConverter;
public:

	__event __interface _IAVSOfficeFileTemplateEvents2;

	CAVSOfficeDrawingConverter()
	{
	}

	~CAVSOfficeDrawingConverter()
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
	STDMETHOD(SetMainDocument)(IUnknown* pDocument)
	{

		return m_oDrawingConverter.SetMainDocument(pDocument);
	}
	STDMETHOD(SetRelsPath)(BSTR bsRelsPath)
	{
		return m_oDrawingConverter.SetRelsPath(bsRelsPath);
	}
	STDMETHOD(SetMediaDstPath)(BSTR bsMediaPath)
	{
		return m_oDrawingConverter.SetMediaDstPath(bsMediaPath);
	}
	
	STDMETHOD(AddShapeType)(BSTR bsXml)
	{
		return m_oDrawingConverter.AddShapeType(bsXml);
	}
	STDMETHOD(AddObject)(BSTR bsXml, BSTR* pMainProps, SAFEARRAY** ppBinary)
	{
		return m_oDrawingConverter.AddObject(bsXml, pMainProps, ppBinary);
	}
	STDMETHOD(AddObject2)(BSTR bsXml, SAFEARRAY* pBinaryObj, BSTR* pXmlOutput)
	{
		return m_oDrawingConverter.AddObject2(bsXml, pBinaryObj, pXmlOutput);
	}

	STDMETHOD(GetThemeBinary)(BSTR bsThemeFilePath, SAFEARRAY** ppBinary)
	{
		return m_oDrawingConverter.GetThemeBinary(bsThemeFilePath, ppBinary);
	}

	STDMETHOD(SaveThemeXml)(SAFEARRAY* pBinaryTheme, LONG lStart, LONG lLength, BSTR bsThemePath)
	{
		return m_oDrawingConverter.SaveThemeXml(pBinaryTheme, lStart, lLength, bsThemePath);
	}
	STDMETHOD(SaveObject)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsMainProps, BSTR* bsXml)
	{
		return m_oDrawingConverter.SaveObject(pBinaryObj, lStart, lLength, bsMainProps, bsXml);
	}
	STDMETHOD(SaveObjectEx)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsMainProps, LONG lDocType, BSTR* bsXml)
	{
		return m_oDrawingConverter.SaveObjectEx(pBinaryObj, lStart, lLength, bsMainProps, lDocType, bsXml);
	}

	STDMETHOD(GetRecordBinary)(LONG lRecordType, BSTR bsXml, SAFEARRAY** ppBinary)
	{
		return m_oDrawingConverter.GetRecordBinary(lRecordType, bsXml, ppBinary);
	}
	STDMETHOD(GetRecordXml)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, LONG lRecType, LONG lDocType, BSTR* bsXml)
	{
		return m_oDrawingConverter.GetRecordXml(pBinaryObj, lStart, lLength, lRecType, lDocType, bsXml);
	}

	STDMETHOD(SetDstContentRels)()
	{
		return m_oDrawingConverter.SetDstContentRels();
	}
	STDMETHOD(SaveDstContentRels)(BSTR bsRelsPath)
	{
		return m_oDrawingConverter.SaveDstContentRels(bsRelsPath);
	}
	STDMETHOD(WriteRels)(BSTR bsType, BSTR bsTarget, BSTR bsTargetMode, LONG* lId)
	{
		return m_oDrawingConverter.WriteRels(bsType, bsTarget, bsTargetMode, lId);
	}

	STDMETHOD(LoadClrMap)(BSTR bsXml)
	{
		return m_oDrawingConverter.LoadClrMap(bsXml);
	}

	STDMETHOD(GetTxBodyBinary)(BSTR bsXml, SAFEARRAY** ppBinary)
	{
		return m_oDrawingConverter.GetTxBodyBinary(bsXml, ppBinary);
	}
	STDMETHOD(GetTxBodyXml)(SAFEARRAY* pBinary, LONG lStart, LONG lLength, BSTR *pbstrXml)
	{
		return m_oDrawingConverter.GetTxBodyXml(pBinary, lStart, lLength, pbstrXml);
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		return m_oDrawingConverter.SetFontDir(bsFontDir);
	}
		
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return m_oDrawingConverter.SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return m_oDrawingConverter.GetAdditionalParam(ParamName, ParamValue);
	}
};
