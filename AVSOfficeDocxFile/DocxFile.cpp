#include "stdafx.h"
#include "DocxFile.h"
#include "SimpleDocxFormat.h"

COfficeDocxFile::COfficeDocxFile()
{
}


HRESULT COfficeDocxFile::FinalConstruct()
{
  m_pOfficeUtils = NULL; 
  return CoCreateInstance(__uuidof(COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(IOfficeUtils), (void **)&(this->m_pOfficeUtils));
}


void COfficeDocxFile::FinalRelease()
{
  if (m_pOfficeUtils != NULL )
  {
		m_pOfficeUtils->Release();
		m_pOfficeUtils = NULL;
  }
}


STDMETHODIMP COfficeDocxFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	OfficeUtilsEvents< _IAVSOfficeUtilsEvents, COfficeDocxFile >* oEventUtils = new OfficeUtilsEvents< _IAVSOfficeUtilsEvents, COfficeDocxFile >(this);
	oEventUtils->AddRef();
	oEventUtils->Advise( m_pOfficeUtils );
	
	const HRESULT extractResult =  m_pOfficeUtils->ExtractToDirectory(sSrcFileName, sDstPath, NULL, 0);

	oEventUtils->UnAdvise( m_pOfficeUtils );
	oEventUtils->Release();

	if ( extractResult != S_OK )
	{
	  return extractResult;
	}

	XmlUtils::CXmlReader oXmlReader;
	if( TRUE == oXmlReader.OpenFromXmlString( sXMLOptions ) )
		if( TRUE == oXmlReader.ReadRootNode( _T("Options") ) )
			if( TRUE == oXmlReader.ReadNode( _T("ExtractOption") ) )
				if( TRUE == oXmlReader.ReadNode( _T("Images") ) )
				{
					CString sResultPath = oXmlReader.ReadNodeAttribute( _T("path") );
					CString sSourcePath = sDstPath;

					SimpleDocxFormat oSimpleDocxFormat;
					oSimpleDocxFormat.ExtractImages( sSourcePath, sResultPath );
				}
	return S_OK;
}


STDMETHODIMP COfficeDocxFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    OfficeUtilsEvents< _IAVSOfficeUtilsEvents, COfficeDocxFile >* oEventUtils = new OfficeUtilsEvents< _IAVSOfficeUtilsEvents, COfficeDocxFile >(this);
	oEventUtils->AddRef();
	oEventUtils->Advise( m_pOfficeUtils );
	
	const HRESULT compressResult = m_pOfficeUtils->CompressFileOrDirectory(sSrcPath, sDstFileName, ZIP_DEFAULT_COMPRESSION);

    oEventUtils->UnAdvise( m_pOfficeUtils );
	oEventUtils->Release();

	return compressResult;
}


STDMETHODIMP COfficeDocxFile::AddObjects(BSTR sSrcPath, BSTR sXMLOptions)
{
	SimpleDocxFormat oSimpleDocxFormat;
	return oSimpleDocxFormat.AddObject( sSrcPath, sXMLOptions );
}