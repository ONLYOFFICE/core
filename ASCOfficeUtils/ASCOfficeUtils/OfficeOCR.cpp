#include "stdafx.h"
#include "../../Common/ASCUtils.h"
#include "../../Common/MediaFormatDefine.h"
#include "OfficeOCR.h"
#include "ImageToPNM.h"
#include "gocr.h"

COfficeOCR::COfficeOCR ()
: CAVSATLError ( __uuidof ( COfficeOCR ), __uuidof ( IOfficeOCR ) )
{
	m_lOutputFormatType = OUTPUT_FORMAT_TYPE_XML;
	m_lGrayLevel = 0;
	m_lDustSize = -1;
	m_lSpaceWidthDots = 0;
	m_lCertainty = 95;
}

HRESULT COfficeOCR::FinalConstruct()
{
	return S_OK;
}

void COfficeOCR::FinalRelease()
{
}
// ѕолучает изображение, возвращает текст
STDMETHODIMP COfficeOCR::Recognize ( IUnknown *Image, BSTR* Text )
{
#ifdef BUILD_CONFIG_FULL_VERSION
	if ( NULL == Text )
		return CAVSATLError::Error ( MEMORY );

	*Text = NULL;

	if ( NULL == Image )
		return CAVSATLError::Error ( FILEFORMAT );

	MediaCore::IAVSUncompressedVideoFrame *pUncompressedVideoFrame = NULL; Image->QueryInterface ( &pUncompressedVideoFrame );
	if ( NULL == pUncompressedVideoFrame )
		return CAVSATLError::Error ( FILEFORMAT );

	LPBYTE pBuffer = NULL; pUncompressedVideoFrame->get_Buffer ( &pBuffer );

	LONG lWidth = 0; pUncompressedVideoFrame->get_Width ( &lWidth );
	LONG lHeight = 0; pUncompressedVideoFrame->get_Height ( &lHeight );
	LONG lColorSpace = 0; pUncompressedVideoFrame->get_ColorSpace ( &lColorSpace );

	LONG lBitCount = 0;

	if ( ( CSP_BGRA & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_BGRA | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace ||
		 ( CSP_ABGR & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_ABGR | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace ||
		 ( CSP_RGBA & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_RGBA | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace ||
		 ( CSP_ARGB & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_ARGB | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace )
		lBitCount = 32;
	else if ( ( CSP_BGR & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_BGR | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace )
		lBitCount = 24;
	else if ( ( CSP_RGB555 & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_RGB555 | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace ||
			  ( CSP_RGB565 & CSP_COLOR_MASK ) == lColorSpace || ( ( CSP_RGB565 | CSP_VFLIP ) & CSP_COLOR_MASK ) == lColorSpace )
		lBitCount = 16;
	else
		lBitCount = 32;

	long lCount = 0;

	CHAR *pPBMImage = convertToPNM ( pBuffer, lWidth, lHeight, lBitCount, true, true, lCount );

	CStringA sOutputFormat = "";
	switch ( m_lOutputFormatType )
	{
	case OUTPUT_FORMAT_TYPE_XML :
		sOutputFormat = "XML";
		break;
	case OUTPUT_FORMAT_TYPE_TEXT :
		sOutputFormat = "UTF8";
		break;
	}

	CHAR *pText = PNMToText (pPBMImage, lCount, sOutputFormat.GetBuffer(), m_lGrayLevel, m_lDustSize, m_lSpaceWidthDots, m_lCertainty);
	free ( pPBMImage );
	RELEASEINTERFACE ( pUncompressedVideoFrame );

	CString sText ( pText );
	free ( pText );

	CString sResult = _T("");
	if ( OUTPUT_FORMAT_TYPE_XML == m_lOutputFormatType )
	{
		sResult += _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
		sResult += _T ("<Text>");
	}
	sResult += sText;
	if ( OUTPUT_FORMAT_TYPE_XML == m_lOutputFormatType )
		sResult += _T ("</Text>");

	*Text = sResult.AllocSysString();
#endif
	return S_OK;
}
// ¬ыставл€ет формат выходного текста ( по умолчанию XML )
STDMETHODIMP COfficeOCR::put_OutputFormat ( LONG Type )
{
	if ( 0 <= Type && 2 >Type )
		m_lOutputFormatType = Type;

	return S_OK;
}
// ¬озвращает формат выходного текста ( по умолчанию XML )
STDMETHODIMP COfficeOCR::get_OutputFormat ( LONG *Type )
{
	*Type = m_lOutputFormatType;
	return S_OK;
}
// ¬ыставл€ет уровень серого ( по умолчанию 0 - автоматически )
STDMETHODIMP COfficeOCR::put_GrayLevel ( LONG Type )
{
	m_lGrayLevel = Type;
	if ( 0 > m_lGrayLevel )
		m_lGrayLevel = 0;
	if ( 255 < m_lGrayLevel )
		m_lGrayLevel = 255;

	return S_OK;
}
// ¬озвращает уровень серого ( по умолчанию 0 - автоматически )
STDMETHODIMP COfficeOCR::get_GrayLevel ( LONG *Type )
{
	*Type = m_lDustSize;
	return S_OK;
}
// ¬ыставл€ет размер шума ( по умолчанию -1 - автоматически )
STDMETHODIMP COfficeOCR::put_DustSize ( LONG Type )
{
	m_lDustSize = Type;
	return S_OK;
}
// ¬озвращает размер шума ( по умолчанию -1 - автоматически )
STDMETHODIMP COfficeOCR::get_DustSize ( LONG *Type )
{
	*Type = m_lGrayLevel;
	return S_OK;
}
// ¬ыставл€ет ширину пробелов/точек ( по умолчанию 0 - автоматически )
STDMETHODIMP COfficeOCR::put_SpaceWidthDots ( LONG Type )
{
	m_lSpaceWidthDots = Type;
	if ( 0 > m_lSpaceWidthDots )
		m_lSpaceWidthDots = 0;
	return S_OK;
}
// ¬озвращает ширину пробелов/точек ( по умолчанию 0 - автоматически )
STDMETHODIMP COfficeOCR::get_SpaceWidthDots ( LONG *Type )
{
	*Type = m_lSpaceWidthDots;
	return S_OK;
}
// ¬ыставл€ет процент достоверности объекта ( по умолчанию 95 )
STDMETHODIMP COfficeOCR::put_Certainty ( LONG Type )
{
	m_lCertainty = Type;
	if ( 0 > m_lCertainty )
		m_lSpaceWidthDots = 0;
	if ( 100 < m_lCertainty )
		m_lCertainty = 100;
	return S_OK;
}
// ¬озвращает процент достоверности объекта ( по умолчанию 95 )
STDMETHODIMP COfficeOCR::get_Certainty ( LONG *Type )
{
	*Type = m_lCertainty;
	return S_OK;
}
// ”становка дополнительных параметров
STDMETHODIMP COfficeOCR::SetAdditionalParam ( BSTR ParamName, VARIANT ParamValue )
{
	return S_OK;
}
// ѕолучение дополнительных параметров
STDMETHODIMP COfficeOCR::GetAdditionalParam ( BSTR ParamName, VARIANT *ParamValue )
{
	return S_OK;
}