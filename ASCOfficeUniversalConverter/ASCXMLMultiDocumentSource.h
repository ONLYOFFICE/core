#pragma once
#include "xmlutils.h"
#include "ASCXMLSingleDocumentSource.h"

[coclass, uuid("64e3dae0-aa89-11df-94e2-0800200c9a66"), threading(apartment), event_source(com), vi_progid("AVSUniversalConverter.AVSXMLSingle"), progid("AVSUniversalConverter.AVSXMLSingle.1"), version(1.0)]
class ATL_NO_VTABLE CAVSXMLMultiOfficeSource 
	: public IAVSXMLOfficeSource
{
public:
	__event __interface _IAVSXMLOfficeSourceEvents;
public:
	CAVSXMLMultiOfficeSource()
	{
		CoCreateInstance( __uuidof( CAVSXMLSingleOfficeSource ), NULL , CLSCTX_INPROC_SERVER, __uuidof(IAVSXMLOfficeSource), (void **)(&m_piSingleOfficeSource)  );
	}
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE( m_piSingleOfficeSource );
	}
public:
	STDMETHOD(Open)( BSTR XMLString )
	{
		if( NULL == m_piSingleOfficeSource )
			return AVS_ERROR_UNEXPECTED;

		HRESULT hRes = S_FALSE;
		XmlUtils::CXmlReader oReader;
		if (FALSE != oReader.SetXmlString(XMLString))
			if (FALSE != oReader.ReadNode( _T("MultiSource") ))
			{
				XML::IXMLDOMNodePtr pXmlNode = NULL;
				if( FALSE != oReader.GetNode( pXmlNode ) )
				{
					//читаем первый source
					XmlUtils::CXmlReader oSourceReader;
					if( FALSE != oSourceReader.OpenFromXmlNode( pXmlNode ) )
						if( FALSE != oSourceReader.ReadNode(_T("SingleSource") ) )
						{
							CComBSTR bstrXML( oSourceReader.ReadNodeXml() );
							hRes = m_piSingleOfficeSource->Open( bstrXML.m_str );
						}
					//читаем watermark
					XmlUtils::CXmlReader oWatermarkReader;
					if( SUCCEEDED( hRes ) && oWatermarkReader.OpenFromXmlNode( pXmlNode ) )
						if( FALSE != oSourceReader.ReadNode(_T("Watermarks") ) )
						{
							m_sWatermark = oSourceReader.ReadNodeXml();
							//CComBSTR bstrWatermark(m_sWatermark);
							//m_piOfficeWatermark->ParseXml( bstrWatermark.m_str );
						}
				}
			}
		return hRes;
	}
	STDMETHOD(Close)(void)
	{
		return m_piSingleOfficeSource->Close();
	}
	//STDMETHOD(Start)()
	//{
	//	return m_piSingleOfficeSource->Start();
	//}
	//STDMETHOD(Stop)()
	//{
	//	return m_piSingleOfficeSource->Stop();
	//}
	//STDMETHOD(Suspend)()
	//{
	//	return m_piSingleOfficeSource->Suspend();
	//}
	//STDMETHOD(Resume)()
	//{
	//	return m_piSingleOfficeSource->Resume();
	//}
	//STDMETHOD(get_Status)( long* pVal )
	//{
	//	return m_piSingleOfficeSource->get_Status( pVal );
	//}
	STDMETHOD(GetPageCount)( long* pVal)
	{
		return m_piSingleOfficeSource->GetPageCount( pVal );
	}
	STDMETHOD(GetPageSize)(long lPage, double* pdWidth, double* pdHeight)
	{
		return m_piSingleOfficeSource->GetPageSize( lPage, pdWidth, pdHeight );
	}
	STDMETHOD(get_OfficeFilesCache)( IUnknown** ppunkVal)
	{
		return m_piSingleOfficeSource->get_OfficeFilesCache( ppunkVal );
	}
	STDMETHOD(put_OfficeFilesCache)( IUnknown* punkVal)
	{
		return m_piSingleOfficeSource->put_OfficeFilesCache( punkVal );
	}
	STDMETHOD(ReadMetafile)( long nPage, IUnknown** punkMetafile )
	{
		HRESULT hRes = m_piSingleOfficeSource->ReadMetafile( nPage, punkMetafile );
		return hRes;
	}
	STDMETHOD(ReadPage)( long nPage, IUnknown** punkMetafile )
	{
		HRESULT hRes = m_piSingleOfficeSource->ReadPage( nPage, punkMetafile );
		return hRes;
	}
	STDMETHOD(ReadThumbnail)( long nPage, IUnknown** punkUnkompressedFrame )
	{
		return m_piSingleOfficeSource->ReadThumbnail( nPage, punkUnkompressedFrame );
	}
	STDMETHOD(DrawPage)( long nPage, IUnknown* punkUnkompressedFrame )
	{
		HRESULT hRes = m_piSingleOfficeSource->DrawPage( nPage, punkUnkompressedFrame );
		//long nWatermarksCount = 0;
		//m_piOfficeWatermark->GetCount( &nWatermarksCount );
		//if( nWatermarksCount > 0 )
		//{
		//	//m_piOfficeWatermark->DrawWatermark(  );
		//}

		return hRes;
	}
	STDMETHOD(PrintPage)( LONG nPageNumber, LONG* pHDC, BSTR bstrXMLMode )
	{
		HRESULT hRes = S_OK;
		hRes = m_piSingleOfficeSource->PrintPage( nPageNumber, pHDC, bstrXMLMode );
		//OfficeFile::IAVSOfficeWatermarkPtr piWaterMark = NULL;
		//piWaterMark.CreateInstance( __uuidof(OfficeFile::CAVSOfficeWatermark) );
		//if( NULL != piWaterMark )
		//{
		//	//piWaterMark->
		//}
		return hRes;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return m_piSingleOfficeSource->SetAdditionalParam( ParamName, ParamValue );
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return m_piSingleOfficeSource->GetAdditionalParam( ParamName, ParamValue );
	}
	STDMETHOD(get_Watermark)(BSTR* pbstrWatermark)
	{
		return m_piSingleOfficeSource->get_Watermark( pbstrWatermark );
	}
	STDMETHOD(put_Watermark)(BSTR bstrWatermark)
	{
		return m_piSingleOfficeSource->put_Watermark( bstrWatermark );
	}
	//STDMETHOD(get_TempDirectory)( BSTR* pbstrDir )
	//{
	//	return m_piSingleOfficeSource->get_TempDirectory( pbstrDir );
	//}
	//STDMETHOD(put_TempDirectory)( BSTR bstrDir )
	//{
	//	return m_piSingleOfficeSource->put_TempDirectory( bstrDir );
	//}
private:
	IAVSXMLOfficeSource* m_piSingleOfficeSource;
	CString m_sWatermark;
};
