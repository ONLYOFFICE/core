#pragma once
#include "stdafx.h"
#include "TemporaryCS.h"
#include "../Common/OfficeFileTemplate.h"
#include "UniversalConverterEvents.h"
[
	object,
	uuid("e707b760-b7c0-11de-8a39-0800200c9a66"),
	dual,	helpstring("IAVSOfficeFileCache Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFilesCache : IDispatch
{
	[id(10), propget]	HRESULT MaxFilesCount([out,retval] long *pVal);
	[id(10), propput]	HRESULT MaxFilesCount([in] long newVal);

	[id(12), propget]	HRESULT MaxFilesStarted([out,retval] long *pVal);
	[id(12), propput]	HRESULT MaxFilesStarted([in] long newVal);

	//[id(15)]			HRESULT AddFile([in] BSTR bstrFilename, [out, retval] LONG* pnID);
	[id(15)]			HRESULT AddFile([in] LONG nID, [in] IUnknown* punkVal);
	//[id(17)]			HRESULT StartFile([in] LONG nID);
	[id(20)]			HRESULT GetFile([in] LONG nID, [out, retval] IUnknown** ppunkVal);
	[id(30)]			HRESULT GetFileByIndex([in] LONG nIndex, [out,retval] IUnknown** ppunkVal);
	//[id(35)]			HRESULT GetFilenameByID([in] LONG nID, [out,retval] BSTR* pbstrVal);
	//[id(37)]			HRESULT GetIDByFilename([in] BSTR ppunkVal, [out,retval] LONG* pnID);

	[id(40)]			HRESULT RemoveFile([in] LONG nID);
	[id(50)]			HRESULT RemoveAll();
	[id(60)]			HRESULT GetCount([out,retval] LONG* pnVal);

	[id(70)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(80)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	//[id(100), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
	//[id(100), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
};



[
	coclass,
	default(IAVSOfficeFilesCache),
	threading(apartment),
	vi_progid("CAVSOfficeFilesCache.AVSDocume"),
	progid("CAVSOfficeFilesCache.AVSDocu.1"),
	version(1.0),
	uuid("a18cf990-b7c3-11de-8a39-0800200c9a66"),
	helpstring("AVSOfficeDocumentFileCache Class")
]
class ATL_NO_VTABLE CAVSOfficeFilesCache
	: public IAVSOfficeFilesCache
{
	struct CacheItem{
		CacheItem()
		{
			m_poEventReciver = NULL;
			m_pOfficeFile = NULL;
			m_nID = -1;
		}
		CacheItem( long nID, OfficeFile::IAVSOfficeFile *pOfficeFile, CAVSOfficeFilesCache* piThis )
		{
			pOfficeFile->QueryInterface( __uuidof( OfficeFile::IAVSOfficeFile ), (void**)&m_pOfficeFile );
			m_poEventReciver = new COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents ,CAVSOfficeFilesCache >( piThis );
			m_poEventReciver->AddRef();
			m_poEventReciver->Advise( pOfficeFile );
			m_nID = nID;
		}
		CacheItem(const CacheItem &oSrc)
		{
			*this = oSrc;
		}
		CacheItem &operator =(const CacheItem &oSrc)
		{
			if( NULL != m_poEventReciver )
			{
				m_poEventReciver->UnAdvise( m_pOfficeFile );
				RELEASEINTERFACE( m_poEventReciver );
			}
			RELEASEINTERFACE( m_pOfficeFile );
			oSrc.m_pOfficeFile->QueryInterface( __uuidof( OfficeFile::IAVSOfficeFile ), (void**)&m_pOfficeFile );
			m_poEventReciver = new COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents,CAVSOfficeFilesCache >( oSrc.m_poEventReciver->m_pReciever );
			m_poEventReciver->AddRef();
			m_poEventReciver->Advise( m_pOfficeFile );

			m_nID = oSrc.m_nID;
			return (*this);
		}
		~CacheItem()
		{
			if( NULL != m_poEventReciver )
			{
				m_poEventReciver->UnAdvise( m_pOfficeFile );
				RELEASEINTERFACE( m_poEventReciver );
			}
			RELEASEINTERFACE( m_pOfficeFile );
		}
		OfficeFile::IAVSOfficeFile *m_pOfficeFile;
		long m_nID;
		COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents ,CAVSOfficeFilesCache >* m_poEventReciver;
	};
public:
	void _OnError( LONG lError )
		{
		}
	void _OnCompleteParseFile( )
		{
			UpdateVideoFilesCacheStarted();
		}
	void _OnProgressParseFile( LONG nID, LONG lProgressPage, SHORT* Cancel, COfficeFileEvents< OfficeFile::_IAVSOfficeFileEvents ,CAVSOfficeFilesCache >* poEventReciver )
		{
		}
	void _OnProgressParsePage( LONG nID )
		{
		}
	void _OnCompleteParsePage( )
		{
		}

public:
	CAVSOfficeFilesCache():m_lMaxFilesCount(g_cnOfficeCacheMaxFilesCount),m_lMaxFilesStarted(g_cnOfficeCacheMaxFilesStarted)
	{
		InitializeCriticalSection(&m_csImageFilesCache);
	}
	virtual ~CAVSOfficeFilesCache()
	{
		RemoveAll();
		DeleteCriticalSection( &m_csImageFilesCache );
	}
	STDMETHOD(get_MaxFilesCount)(long* plVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( NULL == plVal )
			return S_FALSE;
		(*plVal) = m_lMaxFilesCount;
		return S_OK;
	}
	STDMETHOD(put_MaxFilesCount)(long lVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( lVal < 0 )
			return S_FALSE;
		m_lMaxFilesCount = lVal;
		UpdateVideoFilesCache();
		return S_OK;
	}
	STDMETHOD(get_MaxFilesStarted)(long* plVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( NULL == plVal )
			return S_FALSE;
		(*plVal) = m_lMaxFilesStarted;
		return S_OK;
	}
	STDMETHOD(put_MaxFilesStarted)(long lVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( lVal < 0 )
			return S_FALSE;
		m_lMaxFilesStarted = lVal;
		UpdateVideoFilesCacheStarted();
		return S_OK;
	}
	//STDMETHOD(AddFile)( BSTR bstrFilename, LONG* pnID )
	//{
	//	//CTemporaryCS cs(&m_csImageFilesCache);
	//	//if( NULL == pnID )
	//	//	return S_FALSE;
	//	//(*pnID) = -1;
	//	//CString sFilePath; sFilePath = bstrFilename;

	//	//POSITION pos = m_mapOfficeFilesCache.GetStartPosition();
	//	//long nID = -1;
	//	//while( NULL != pos )
	//	//{
	//	//	CAtlMap<long, CacheItem>::CPair *pPair = m_mapOfficeFilesCache.GetNext( pos );
	//	//	if( sFilePath == pPair->m_value.m_sFilePath )
	//	//	{
	//	//		nID = pPair->m_value.m_nID;
	//	//		break;
	//	//	}
	//	//}
	//	//if( -1 == nID )
	//	//{
	//	//	OfficeFile::IAVSOfficeFile *pOfficeFile = NULL;
	//	//	CoCreateInstance( __uuidof( OfficeFile::CAVSOfficeFile ) , NULL, CLSCTX_ALL, __uuidof( OfficeFile::IAVSOfficeFile ), (void**)&pOfficeFile);

	//	//	long nNewID = m_mapOfficeFilesCache.GetCount() + 1;
	//	//	(*pnID) = nNewID;
	//	//	m_mapOfficeFilesCache.SetAt( nNewID,CacheItem(nNewID, sFilePath, pOfficeFile));
	//	//	RELEASEINTERFACE( pOfficeFile );

	//	//	UpdateVideoFilesCache();
	//	//}
	//	//else
	//	//	(*pnID) = nID;
	//	return S_OK;
	//}
	STDMETHOD(AddFile)( LONG nID, IUnknown* punkVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		OfficeFile::IAVSOfficeFile* piCurOfficeFile = NULL;
		if( FAILED( punkVal->QueryInterface( __uuidof(OfficeFile::IAVSOfficeFile), (void**)&piCurOfficeFile ) ) )
			return S_FALSE;

		CAtlMap<long, CacheItem>::CPair *pPair =  m_mapOfficeFilesCache.Lookup( nID );

		if( NULL != pPair )
			m_mapOfficeFilesCache.RemoveKey( nID );

		//добавляем новый элемент
		CacheItem oNewCacheItem(nID, piCurOfficeFile, this);
		m_mapOfficeFilesCache.SetAt( nID, oNewCacheItem );

		RELEASEINTERFACE( piCurOfficeFile );
		UpdateVideoFilesCache();
		UpdateVideoFilesCacheStarted();
		return S_OK;
	}
	//STDMETHOD(StartFile)( LONG nID, BSTR bstrOptions)
	//{
	//	CTemporaryCS cs(&m_csImageFilesCache);
	//	HRESULT hRes = S_FALSE;
	//	CAtlMap<long, CacheItem>::CPair *pPair = m_mapOfficeFilesCache.Lookup( nID );
	//	if( NULL != pPair )
	//	{
	//		CacheItem &oCacheRec = pPair->m_value;
	//		long nCurStatus = c_shOfficeFileIdle;
	//		oCacheRec.m_pOfficeFile->get_Status( &nCurStatus );
	//		if( nCurStatus == c_shOfficeFileOpen )
	//			hRes = oCacheRec.m_pOfficeFile->StartParse();
	//		else if( nCurStatus == c_shOfficeFileSuspended )
	//			hRes = oCacheRec.m_pOfficeFile->ResumeParse();
	//	}
	//	return hRes;
	//}
	STDMETHOD(GetFile)( LONG nID, IUnknown** ppunkVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( NULL == ppunkVal )
			return S_FALSE;
		(*ppunkVal) = NULL;
		CAtlMap<long, CacheItem>::CPair *pPair = m_mapOfficeFilesCache.Lookup( nID );
		if( NULL != pPair )
		{
			CacheItem &oCacheRec = pPair->m_value;
			oCacheRec.m_pOfficeFile->QueryInterface(__uuidof(IUnknown), (void**)ppunkVal);
		}
		return S_OK;
	}
	STDMETHOD(GetFileByIndex)( LONG nIndex, IUnknown** ppunkVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( NULL == ppunkVal )
			return S_FALSE;
		(*ppunkVal) = NULL;
		if( nIndex < 0 || nIndex >= m_mapOfficeFilesCache.GetCount() )
			return S_FALSE;
		POSITION pos = m_mapOfficeFilesCache.GetStartPosition();
		while( nIndex > 0 )
		{
			m_mapOfficeFilesCache.GetNext( pos );
			nIndex--;
		}
		IUnknown* punkCurValue = m_mapOfficeFilesCache.GetNextValue( pos ).m_pOfficeFile;
		punkCurValue->QueryInterface( __uuidof(IUnknown), (void**)ppunkVal);
		return S_OK;
	}
	//STDMETHOD(GetFilenameByID)( LONG nID, BSTR* pbstrVal)
	//{
	//	if( NULL == pbstrVal )
	//		return S_FALSE;	
	//	(*pbstrVal) = NULL;
	//	CAtlMap<long, CacheItem>::CPair *pPair = m_mapOfficeFilesCache.Lookup( nID );
	//	if( NULL != pPair )
	//	{
	//		CacheItem &oCacheRec = pPair->m_value;
	//		(*pbstrVal) = oCacheRec.m_sFilePath.AllocSysString();
	//	}
	//	if( (*pbstrVal) != NULL )
	//		return S_OK;
	//	else
	//		return S_FALSE;
	//}
	//STDMETHOD(GetIDByFilename)( BSTR bstrVal, LONG* pnID )
	//{
	//	if( NULL == pnID )
	//		return S_FALSE;
	//	CString sFilePath; sFilePath = bstrVal;
	//	long nID = -1;
	//	POSITION pos = m_mapOfficeFilesCache.GetStartPosition();
	//	while( NULL != pos )
	//	{
	//		CAtlMap<long, CacheItem>::CPair *pPair = m_mapOfficeFilesCache.GetNext( pos );
	//		if( sFilePath == pPair->m_value.m_sFilePath )
	//		{
	//			nID = pPair->m_value.m_nID;
	//			break;
	//		}
	//	}
	//	(*pnID) = nID;
	//	if( -1 != nID )
	//		return S_OK;
	//	else
	//		return S_FALSE;
	//}
	STDMETHOD(RemoveFile)( LONG nID)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		m_mapOfficeFilesCache.RemoveKey( nID );
		return S_OK;
	}
	STDMETHOD(RemoveAll)()
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		m_mapOfficeFilesCache.RemoveAll();
		return S_OK;
	}
	STDMETHOD(GetCount)( LONG* pnVal )
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if( NULL == pnVal )
			return S_FALSE;
		(*pnVal) = m_mapOfficeFilesCache.GetCount();
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	//STDMETHOD(get_TempDirectory)( BSTR* pbstrDir )
	//{
	//	(*pbstrDir) = m_sTempPath.AllocSysString();
	//	return S_OK;
	//}
	//STDMETHOD(put_TempDirectory)( BSTR bstrDir )
	//{
	//	m_sTempPath = CString( bstrDir );
	//	return S_OK;
	//}

private:
	void UpdateVideoFilesCache()
	{
		size_t nCount = m_mapOfficeFilesCache.GetCount();
		if ((long)nCount<m_lMaxFilesCount)
			return;
		size_t nRemoveCount = nCount - m_lMaxFilesCount;
		POSITION pos = m_mapOfficeFilesCache.GetStartPosition();
		while( nRemoveCount > 0 )
		{
			m_mapOfficeFilesCache.GetNext( pos );
			m_mapOfficeFilesCache.RemoveAtPos( pos );
			nRemoveCount--;
		}
	}
	void UpdateVideoFilesCacheStarted()
	{
		size_t nCount = m_mapOfficeFilesCache.GetCount();
		int nCurStarted = 0;

		POSITION pos = m_mapOfficeFilesCache.GetStartPosition();
		int nTempCount = nCount;
		while( nTempCount > 0 )
		{
			CAtlMap< long, CacheItem >::CPair* pPair = m_mapOfficeFilesCache.GetNext( pos );
			long nStatus;
			pPair->m_value.m_pOfficeFile->get_Status( &nStatus );
			if( nStatus == c_shOfficeFileParsing )
				nCurStarted++;
			nTempCount--;
		}

		size_t nToStartCount = m_lMaxFilesStarted - nCurStarted;
		nTempCount = nCount;
		pos = m_mapOfficeFilesCache.GetStartPosition();
		while( nTempCount > 0 && nToStartCount > 0 )
		{
			CAtlMap< long, CacheItem >::CPair* pPair = m_mapOfficeFilesCache.GetNext( pos );
			long nStatus;
			pPair->m_value.m_pOfficeFile->get_Status( &nStatus );
			if( nStatus == c_shOfficeFileOpen )
			{
				pPair->m_value.m_pOfficeFile->StartParse();
				nToStartCount--;
			}
			else if( nStatus == c_shOfficeFileSuspended )
			{
				pPair->m_value.m_pOfficeFile->ResumeParse();
				nToStartCount--;
			}
			nTempCount--;
		}
	}
private: 
	//CString m_sTempPath;
	CRITICAL_SECTION m_csImageFilesCache;
	long m_lMaxFilesCount;
	long m_lMaxFilesStarted;
	CAtlMap< long, CacheItem > m_mapOfficeFilesCache;
};
