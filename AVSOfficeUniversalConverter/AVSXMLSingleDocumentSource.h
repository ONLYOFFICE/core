#pragma once
#include "xmlutils.h"
#include "AVSOfficeFileCache.h"
#include "UniversalConverterEvents.h"
#include "TemporaryCS.h"
//#include "OfficeFileTemplate.h"

#define TO_FLOAT 10000
#define ONE_INCH 2.54

#define MM_TO_PIX( Value, DPI )		long( Value * DPI / (10 * ONE_INCH ) )
#define PIX_TO_MM( Value, DPI )		float( 1.0 * Value * (10 * ONE_INCH ) / DPI )


[ object, uuid("074b59d0-a9f3-11df-94e2-0800200c9a66"), dual, pointer_default(unique)]
__interface IAVSXMLOfficeSource
{
	[id(10)] HRESULT Open([in] BSTR XMLString );
	[id(20)] HRESULT Close(void);

	[id(80)] HRESULT GetPageCount([out, retval] long* pVal);
	[id(90)] HRESULT GetPageSize([in]long lPage, [out] double* pdWidth, [out] double* pdHeight);

	[id(100), propget] HRESULT OfficeFilesCache([out, retval] IUnknown** ppunkVal);
	[id(100), propput] HRESULT OfficeFilesCache([in] IUnknown* punkVal);

	[id(110)] HRESULT ReadMetafile([in] long nPage, [in] IUnknown** punkMetafile );
	[id(112)] HRESULT ReadPage([in] long nPage, [in] IUnknown** punkUnkompressedFrame );

	[id(115)] HRESULT ReadThumbnail([in] long nPage, [in] IUnknown** punkUnkompressedFrame );
	[id(120)] HRESULT DrawPage([in] long nPage, [in] IUnknown* punkUnkompressedFrame );
	[id(130)] HRESULT PrintPage( [in] LONG nPageNumber, [in] LONG* pHDC, [in] BSTR bstrXMLMode );

	[id(140)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(150)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(160), propget] HRESULT Watermark([out, retval] BSTR* pbstrWatermark);
	[id(160), propput] HRESULT Watermark([in]BSTR bstrWatermark);
};

[
	dispinterface,
	uuid("325e7a00-aa8c-11df-94e2-0800200c9a66"),
	helpstring("_IAVSOfficeFileTemplateEvents Interface")
]
__interface _IAVSXMLOfficeSourceEvents
{
	[id(0)] HRESULT OnError(LONG lError);
	[id(1)] HRESULT OnComplete();

	[id(5)] HRESULT OnProgress([in] LONG nID, LONG lProgressPage, [in, out, ref] SHORT* Cancel);
	[id(7)] HRESULT OnCompletePage();
	[id(9)] HRESULT OnUpdateWM();
};

[coclass, uuid("39257f30-a9f3-11df-94e2-0800200c9a66"), threading(apartment), event_source(com), vi_progid("AVSUniversalConverter.AVSXMLSingle"), progid("AVSUniversalConverter.AVSXMLSingle.1"), version(1.0)]
class ATL_NO_VTABLE CAVSXMLSingleOfficeSource 
	: public IAVSXMLOfficeSource
{
public:
	__event __interface _IAVSXMLOfficeSourceEvents;
public:
	CAVSXMLSingleOfficeSource()
	{
		InitializeCriticalSection(&m_csCriticalSection);
		CoCreateInstance( __uuidof( CAVSOfficeFilesCache ), NULL , CLSCTX_INPROC_SERVER, __uuidof(IAVSOfficeFilesCache), (void **)(&m_piOfficetFilesCache)  );
		Clear();
	}
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE( m_piOfficetFilesCache );
		DeleteCriticalSection( &m_csCriticalSection );
	}
public:
	void _OnError( LONG lError )
		{
			OnError( lError );
		}
	void _OnCompleteParseFile( )
		{
			m_nCompleteCount++;
			if( m_aSources.GetCount() == m_nCompleteCount )
			{
				m_nStatus = c_shOfficeFileParsingComplete;
				OnUpdateWM();
				OnComplete();
			}
		}
	void _OnProgressParseFile( LONG nID, LONG lProgressPage, SHORT* Cancel, COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents ,CAVSXMLSingleOfficeSource >* poEventReciver )
		{
			long nCurProgress = 0;
			for( int i = 0; i < m_aSources.GetCount(); i++ )
			{
				SourceItem& piSource = m_aSources[i];
				if( poEventReciver = piSource.m_poEventReciver )
					piSource.OnProgress( lProgressPage );
				if( piSource.m_nCurProgress > 0 )
					nCurProgress += piSource.m_nCurProgress;
			}
			if( m_nSourcesCount > 0 )
				nCurProgress /= m_nSourcesCount;
			if( m_nLastProgress < nCurProgress )
			{
				m_nLastProgress = nCurProgress;
				SHORT shCancel = c_shProgressContinue;
				OnProgress( nID, m_nLastProgress, &shCancel );
			}
		}
	void _OnProgressParsePage( LONG nID )
		{
		}
	void _OnCompleteParsePage( )
		{
			long nLastPageCount = m_nPageCount;
			UpdatePageCount();
			if( m_nPageCount > nLastPageCount )
				OnCompletePage();
		}
private:
	struct SourceItem
	{
		IAVSOfficeFile* m_piFile;
		long m_nID;
		long m_lStartPage;
		long m_lEndPage;
		long m_nCurProgress;
		COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents ,CAVSXMLSingleOfficeSource >* m_poEventReciver;
		SourceItem():
			m_piFile(NULL),
			m_nID(-1),
			m_lStartPage(-1),
			m_lEndPage(-1),
			m_poEventReciver(NULL),
			m_nCurProgress(-1)
		{
		}
		SourceItem( const SourceItem& pp )
		{
			(*this) = pp;
		}
		const SourceItem& operator=( const SourceItem& pp )
		{
			if( NULL != m_poEventReciver )
			{
				m_poEventReciver->UnAdvise( m_piFile );
				RELEASEINTERFACE( m_poEventReciver );
			}
			RELEASEINTERFACE( m_piFile );
			pp.m_piFile->QueryInterface( __uuidof( OfficeFile::IAVSOfficeFile ), (void**)&m_piFile );
			m_poEventReciver = new COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents,CAVSXMLSingleOfficeSource >( pp.m_poEventReciver->m_pReciever );
			m_poEventReciver->AddRef();
			m_poEventReciver->Advise( m_piFile );

			m_nID = pp.m_nID;
			m_lStartPage = pp.m_lStartPage;
			m_lEndPage = pp.m_lEndPage;
			m_nCurProgress = pp.m_nCurProgress;
			return (*this);
		}
		~SourceItem()
		{
			if( NULL != m_poEventReciver )
			{
				m_poEventReciver->UnAdvise( m_piFile );
				RELEASEINTERFACE( m_poEventReciver );
			}
			RELEASEINTERFACE( m_piFile );
		}
		SourceItem( IUnknown* piFile, long nID, long lStartPage, long lEndPage, CAVSXMLSingleOfficeSource* piThis ):
			m_piFile(NULL),
			m_nID(nID),
			m_lStartPage(lStartPage),
			m_lEndPage(lEndPage),
			m_poEventReciver(NULL),
			m_nCurProgress(-1)
		{
			if( NULL != piFile )
			{
				piFile->QueryInterface( __uuidof( OfficeFile::IAVSOfficeFile), (void**)&m_piFile );
				m_poEventReciver = new COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents,CAVSXMLSingleOfficeSource >( piThis );
				m_poEventReciver->AddRef();
				m_poEventReciver->Advise( m_piFile );
			}
		}
		long GetPageCount()
		{
			long nRes = 0;
			if( NULL != m_piFile )
			{
				long nFilePageCount = 0;
				m_piFile->GetPageCount( &nFilePageCount );
				if( -1 == m_lStartPage || nFilePageCount >= m_lStartPage )
				{
					nRes = nFilePageCount;
					if( -1 != m_lEndPage && nFilePageCount > m_lEndPage )
						nRes = m_lEndPage;
				}
			}
			return nRes;
		}
		bool IsPageExist( long nPage )
		{
			bool bRes = true;
			if( NULL != m_piFile )
			{
				long nFilePageCount = 0;
				m_piFile->GetPageCount( &nFilePageCount );
				if( nPage < 0 || nPage >= nFilePageCount )
					bRes = false;
				if( -1 != m_lStartPage && nPage < m_lStartPage )
					bRes = false;
				if( -1 != m_lEndPage && nPage > m_lEndPage )
					bRes = false;
			}
			else
				bRes = false;
			return bRes;
		}
		void OnProgress( long nProg )
		{
			if( nProg > m_nCurProgress )
				m_nCurProgress = nProg;
		}
	};
public:
	STDMETHOD(Open)( BSTR XMLString)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == m_piOfficetFilesCache )
			return S_FALSE;
		Close();
		CString strXml = m_oDecoder.DecryptXML( XMLString );
		XmlUtils::CXmlReader oReader;
		if (!oReader.OpenFromXmlString(strXml))
			return S_FALSE;
		oReader.ReadNodeList(_T("Source"));
		HRESULT hRes = S_OK;
		for( int i = 0; i < oReader.GetLengthList(); i++ )
		{		
			long nId = Strings::ToInteger( oReader.ReadNodeAttribute( i, _T("id") ) );
			long nStartPage = Strings::ToInteger( oReader.ReadNodeAttribute( i, _T("startpage"), _T("-1") ) );
			long nEndPage = Strings::ToInteger( oReader.ReadNodeAttribute( i, _T("endpage"), _T("-1") ) );

			hRes = AddNewItem( nId, nStartPage, nEndPage );
			if( FAILED(hRes) )
			{
				m_nLastErrorId = nId;
				break;
			}
		}
		m_nSourcesCount = m_aSources.GetCount();
		return hRes;
	}
	STDMETHOD(Close)(void)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		HRESULT hRes = S_OK;
		m_aSources.RemoveAll();
		Clear();
		return hRes;
	}
	STDMETHOD(get_Status)( long* pVal )
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == pVal )
			return S_FALSE;
		(*pVal) = m_nStatus;
		return S_OK;
	}
	STDMETHOD(GetPageCount)( long* pVal)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == pVal )
			return S_FALSE;
		(*pVal) = m_nPageCount;
		return S_OK;
	}
	STDMETHOD(GetPageSize)(long lPage, double* pfWidth, double* pfHeight)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == pfWidth || NULL == pfHeight )
			return S_FALSE;
		float fMaxWidth = 0;
		float fMaxHeight = 0;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
		{
			if( true == m_aSources[i].IsPageExist(lPage) )
			{
				AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer;
				if( SUCCEEDED( m_aSources[i].m_piFile->get_CommandRenderer( (IUnknown**)&piCommandRenderer ) ) )
				{
					double lCurWidth = 0;
					double lCurHeight = 0;
					piCommandRenderer->GetPageSize( lPage, &lCurWidth, &lCurHeight );
					if( lCurWidth > fMaxWidth )
						fMaxWidth = lCurWidth;
					if( lCurHeight > fMaxHeight )
						fMaxHeight = lCurHeight;
				}
			}
		}
		(*pfWidth) = fMaxWidth;
		(*pfHeight) = fMaxHeight;
		return S_OK;
	}
	STDMETHOD(get_OfficeFilesCache)( IUnknown** ppunkVal)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == ppunkVal )
			return S_FALSE;
		(*ppunkVal) = NULL;
		if( NULL != m_piOfficetFilesCache )
			m_piOfficetFilesCache->QueryInterface( __uuidof(IAVSOfficeFilesCache), (void**)ppunkVal );
		return S_OK;
	}
	STDMETHOD(put_OfficeFilesCache)( IUnknown* punkVal)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		RELEASEINTERFACE( m_piOfficetFilesCache );
		if( NULL != punkVal )
			punkVal->QueryInterface( __uuidof(IAVSOfficeFilesCache), (void**)&m_piOfficetFilesCache );
		return S_OK;
	}

	STDMETHOD(ReadMetafile)( long nPage, IUnknown** punkMetafile )
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == punkMetafile )
			return S_FALSE;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
		{
			if( true == m_aSources[i].IsPageExist(nPage) )
			{
				AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer;
				m_aSources[i].m_piFile->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
				if( NULL != piCommandRenderer )
					piCommandRenderer->LockPage( nPage, punkMetafile  );
				break;
			}
		}
		return S_OK;
	}
	STDMETHOD(ReadPage)( long nPage, IUnknown** punkUnkompressedFrame )
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == punkUnkompressedFrame )
			return S_FALSE;
		//Делаем фон белым
		HRESULT hRes = S_OK;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
		{
			if( true == m_aSources[i].IsPageExist(nPage) )
			{
				AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer;
				m_aSources[i].m_piFile->ReadPage( nPage, punkUnkompressedFrame );
				if( FAILED(hRes) )
					m_nLastErrorId = m_aSources[i].m_nID;
				break;
			}
		}
		return hRes;
	}
	STDMETHOD(ReadThumbnail)( long nPage, IUnknown** punkUnkompressedFrame )
	{
		return S_OK;
	}
	STDMETHOD(DrawPage)( long nPage, IUnknown* punkUnkompressedFrame )
	{
		CTemporaryCS cs(&m_csCriticalSection);
		if( NULL == punkUnkompressedFrame )
			return S_FALSE;
		//Делаем фон белым
		MediaCore::IAVSUncompressedVideoFrame * piFrame = NULL;
		if( SUCCEEDED( punkUnkompressedFrame->QueryInterface( __uuidof( MediaCore::IAVSUncompressedVideoFrame ), (void**)&piFrame ) ) )
			memset( piFrame->Buffer, 255, piFrame->BufferSize);
		HRESULT hRes = S_OK;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
		{
			if( true == m_aSources[i].IsPageExist(nPage) )
			{
				AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer;
				m_aSources[i].m_piFile->DrawPage( nPage, punkUnkompressedFrame );
				if( FAILED(hRes) )
					m_nLastErrorId = m_aSources[i].m_nID;
				break;
			}
		}
		return hRes;
	}
	STDMETHOD(PrintPage)( LONG nPageNumber, LONG* pHDC, BSTR bstrXMLMode )
	{
		CTemporaryCS cs(&m_csCriticalSection);
		HRESULT hRes = S_OK;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
			if( true == m_aSources[i].IsPageExist(nPageNumber) )
			{
				hRes = m_aSources[i].m_piFile->PrintPage( nPageNumber, pHDC, bstrXMLMode );
				if( FAILED(hRes) )
					m_nLastErrorId = m_aSources[i].m_nID;
				break;
			}
		return hRes;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
			if (!m_oDecoder.SetBlowfishKey(ParamValue.punkVal))
				return S_FALSE;
		}
		return S_OK;
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		CComBSTR ParamStr = ParamName;
		if (ParamStr == L"LastErrorId")
		{
			(*ParamValue).vt = VT_I4;
			(*ParamValue).lVal = m_nLastErrorId;
		}
		return S_OK;
	}

	STDMETHOD(get_Watermark)(BSTR* pbstrWatermark)
	{
		if( NULL == pbstrWatermark )
			return S_FALSE;
		(*pbstrWatermark) = m_sWatermark.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Watermark)(BSTR bstrWatermark)
	{
		CTemporaryCS cs(&m_csCriticalSection);
		m_sWatermark = bstrWatermark;
		if( c_shOfficeFileParsingComplete == m_nStatus )
			OnUpdateWM();
		return S_OK;
	}
private:
	void UpdatePageCount()
	{
		long nMax = 0;
		for( int i = 0; i < m_aSources.GetCount(); i++ )
		{
			long nCurPageCount = m_aSources[i].GetPageCount();
			if( nCurPageCount > nMax )
				nMax = nCurPageCount;
		}
		m_nPageCount = nMax;
	}
	void Clear()
	{
		m_aSources.RemoveAll();
		m_nPageCount = 0;
		m_nSourcesCount = 0;
		m_nStatus = c_shOfficeFileIdle;
		m_nLastProgress = -1;
		m_nCompleteCount = 0;
		m_nLastErrorId = -1;
	}

	HRESULT AddNewItem( long nID, long& lStartPage, long& lEndPage )
	{
		HRESULT hRes = S_OK;
		IUnknown* piUnk = NULL;
		if( SUCCEEDED( m_piOfficetFilesCache->GetFile( nID, &piUnk ) ))
		{
			IAVSOfficeFile* piOfficeFile = NULL;
			if( SUCCEEDED(piUnk->QueryInterface( __uuidof( IAVSOfficeFile ), (void**)&piOfficeFile )) )
			{
				SourceItem oSourceItem( piOfficeFile, nID, lStartPage, lEndPage, this );
				piOfficeFile->GetPageCount( &m_nPageCount );
				VARIANT vtLastError;
				vtLastError.lVal = S_OK;
				piOfficeFile->GetAdditionalParam( L"LastError", &vtLastError );
				if( S_OK == hRes && FAILED(vtLastError.lVal) )
					hRes = vtLastError.lVal;
				piOfficeFile->get_Status( &m_nStatus );
				if( SUCCEEDED( hRes ) )
					m_aSources.Add( oSourceItem );
				RELEASEINTERFACE( piOfficeFile );
			}
			RELEASEINTERFACE( piUnk );
		}
		return hRes;
	}
private:
	CRITICAL_SECTION m_csCriticalSection;
	CString m_sTempPath;
	CAtlArray<SourceItem> m_aSources;
	long m_nPageCount;
	long m_nSourcesCount;
	long m_nStatus;
	long m_nLastProgress;
	long m_nCompleteCount;
	long m_nLastErrorId;
	CString m_sWatermark;
	IAVSOfficeFilesCache* m_piOfficetFilesCache;
	CXMLDecoder m_oDecoder;
};
