#pragma once
#include "MobiReader.h"
#include "..\..\Common\OfficeFileTemplate.h"
#include "..\..\AVSOfficeFile\OfficeEventsTemplate.h"
#include "..\..\Common\OfficeFileFormats.h"
#include "..\..\Common\OfficeFileErrorDescription.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

template <class T, class Reciever>
class COfficeHtmlEvent : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeHtmlEvent(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgress( LONG nID, LONG nPercent )
	{
		m_pReciever->OnProgress( nID, nPercent );
		return S_OK;
	}
	HRESULT OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel )
	{
		m_pReciever->OnProgressEx( nID, nPercent, Cancel );
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		VARIANTARG varg2;
		switch(dispIdMember) {
		case 1:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			OnProgress( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		case 2:
			VariantInit(&varg0);
			VariantInit(&varg1);
			VariantInit(&varg2);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			hr = DispGetParam(pDispParams,2,VT_I2,&varg2,puArgErr);		
			OnProgressEx( varg0.lVal, varg1.lVal, &varg2.iVal );
			if (pDispParams->rgvarg[0].vt & VT_BYREF)	// implemented now for last VT_I2 arg only
			{
				SHORT* p = (SHORT*)pDispParams->rgvarg[0].byref;
				if (p) *p = varg2.iVal;
			}
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};

// IOfficeMobiFile
[
	object,
	uuid("A603595B-F537-4dbd-A616-4DFFF11613F0"),
	dual,	helpstring("IOfficeMobiFile Interface"),
	pointer_default(unique)
]
__interface IOfficeMobiFile : IAVSOfficeFileTemplate
{
	[id(10)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(20)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// COfficeMobiFile
[
	coclass,
	default(IOfficeMobiFile),
	threading(apartment),
	event_source(com),
	vi_progid("Ebook.Mobi"),
	progid("Ebook.Mobi.1"),
	version(1.0),
	uuid("CB6B5C2C-9F3B-47d8-A184-187919722F88"),
	helpstring("COfficeMobiFile Class")
]
class ATL_NO_VTABLE COfficeMobiFile : public IOfficeMobiFile
{
public:
	COfficeMobiFile()
	{
	}
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
	STDMETHOD(LoadFromFile)( BSTR bstrSrcFileName,  BSTR bstrDstPath,  BSTR bstrXMLOptions)
	{
		CString strSrcFileName = (CString)bstrSrcFileName;
		if (!IsMobiFile(strSrcFileName))
			return S_FALSE;

		NSMobi::CMobiReader oReader;
		oReader.m_oTempManager.m_strTempDirectory = _T("");
		oReader.m_bIsCorrectHtml = true;

		m_strMetadata = _T("");
		bool bIsDRM = false;
		bool bIsMobi = oReader.OpenFile(strSrcFileName, bIsDRM);
		m_strMetadata = oReader.m_strMeta;

		if (bIsDRM)
		{
			oReader.CloseFile();
			return AVS_ERROR_DRM;
		}

		CString strHtmlFile = oReader.m_oTempManager.m_strTempDirectory + _T("\\index.html");
		CFile oFile;
		oFile.OpenFile(strHtmlFile);
		if (0 == oFile.GetFileSize())
		{
			oFile.CloseFile();
			return S_FALSE;
		}

		HRESULT hrRes = S_FALSE;
		if (bIsMobi)
		{
			OfficeHtmlFile::IHtmlFilePtr piHtmlFile;
			piHtmlFile.CreateInstance(__uuidof(OfficeHtmlFile::CHtmlFile));

			//подцепл€емс€ к событи€м
			COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeMobiFile >* oHtmlEvents = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeMobiFile >( this );
			oHtmlEvents->AddRef();
			oHtmlEvents->Advise( piHtmlFile );
			COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeMobiFile >* oHtmlEvents2 = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeMobiFile >( this );
			oHtmlEvents2->AddRef();
			oHtmlEvents2->Advise( piHtmlFile );

			BSTR bsSrc = strHtmlFile.AllocSysString();
			CString sOptions;
			sOptions.AppendFormat( _T("<Options><FileType>%d</FileType></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML );
			BSTR bstrOptions = sOptions.AllocSysString();
			hrRes = piHtmlFile->LoadFromFile(bsSrc, bstrDstPath, bstrOptions);
			SysFreeString(bsSrc);
			SysFreeString(bstrOptions);

			//отцепл€емс€ от событий
			oHtmlEvents->UnAdvise( piHtmlFile );
			RELEASEINTERFACE( oHtmlEvents );
			oHtmlEvents2->UnAdvise( piHtmlFile );
			RELEASEINTERFACE( oHtmlEvents2 );
		}
		else
		{
			OfficeTxtFile::ITxtFilePtr piHtmlFile;
			piHtmlFile.CreateInstance(__uuidof(OfficeTxtFile::CTxtFile));

			//подцепл€емс€ к событи€м
			COfficeHtmlEvent< OfficeTxtFile::_IAVSOfficeFileTemplateEvents, COfficeMobiFile >* oHtmlEvents = new COfficeHtmlEvent< OfficeTxtFile::_IAVSOfficeFileTemplateEvents, COfficeMobiFile >( this );
			oHtmlEvents->AddRef();
			oHtmlEvents->Advise( piHtmlFile );
			COfficeHtmlEvent< OfficeTxtFile::_IAVSOfficeFileTemplateEvents2, COfficeMobiFile >* oHtmlEvents2 = new COfficeHtmlEvent< OfficeTxtFile::_IAVSOfficeFileTemplateEvents2, COfficeMobiFile >( this );
			oHtmlEvents2->AddRef();
			oHtmlEvents2->Advise( piHtmlFile );

			strHtmlFile.Replace(_T("\\\\"), _T("\\"));
			BSTR bsSrc = strHtmlFile.AllocSysString();
			CString sOptions;
			sOptions.AppendFormat( _T("<Options><FileType>%d</FileType></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT );
			BSTR bstrOptions = sOptions.AllocSysString();
			hrRes = piHtmlFile->LoadFromFile(bsSrc, bstrDstPath, bstrOptions);
			SysFreeString(bsSrc);
			SysFreeString(bstrOptions);

			//отцепл€емс€ от событий
			oHtmlEvents->UnAdvise( piHtmlFile );
			RELEASEINTERFACE( oHtmlEvents );
			oHtmlEvents2->UnAdvise( piHtmlFile );
			RELEASEINTERFACE( oHtmlEvents2 );
		}

		oReader.CloseFile();

		return hrRes;		
	}
	STDMETHOD(SaveToFile)( BSTR bstrDstFileName,  BSTR bstrSrcPath,  BSTR bstrXMLOptions)
	{
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)( BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)( BSTR ParamName, VARIANT* ParamValue)
	{
		CString sParamName = ParamName;
		if (_T("Metadata") == sParamName)
		{
			ParamValue->vt		= VT_BSTR;
			ParamValue->bstrVal	= m_strMetadata.AllocSysString();
		}
		return S_OK;
	}

private: 
	BOOL IsMobiFile(CString sFilename)
	{
		CFile oFile;
		oFile.OpenFile(sFilename);
		if (70 > oFile.GetFileSize())
			return FALSE;

		oFile.SetPosition(0);
		oFile.SkipBytes(60);
		
		NSMobi::CStream oStream;
		oStream.CreateFromBuffer(10);
		oFile.ReadFile(oStream.GetBuffer(), 10);
		CString strId = oStream.ReadStringA(8);

		BOOL bIsMobi = FALSE;
		if ((strId == _T("BOOKMOBI")) || (strId == _T("TEXtREAd")))
			bIsMobi = TRUE;

		oFile.CloseFile();
		return bIsMobi;
	}
private: 
	
	CString m_strMetadata;
};