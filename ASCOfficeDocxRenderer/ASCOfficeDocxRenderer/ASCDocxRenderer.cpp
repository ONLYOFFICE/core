// AVSDocxRenderer.cpp : Implementation of CAVSDocxRenderer

#include "stdafx.h"
#include "AVSDocxRenderer.h"


// CAVSDocxRenderer

STDMETHODIMP CAVSDocxRenderer::get_Type(LONG* lType)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::NewPage()
{
  this->ooxDocument.AddPage();

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_Height(double* dHeight)
{
  if ( dHeight != NULL )
  {
    *dHeight = this->ooxDocument.GetHeight();

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_Height(double dHeight)
{
  this->ooxDocument.SetHeight( dHeight );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_Width(double* dWidth)
{
  if ( dWidth != NULL )
  {
    *dWidth = this->ooxDocument.GetWidth();

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_Width(double dWidth)
{
  this->ooxDocument.SetWidth( dWidth );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_DpiX(double* dDpiX)
{
  //!!!TODO!!!
  if ( dDpiX != NULL )
  {
    *dDpiX = 300;  
  }

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_DpiY(double* dDpiY)
{
  //!!!TODO!!!
  if ( dDpiY != NULL )
  {
    *dDpiY = 300;  
  }

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetPen(BSTR bsXML)
{
  XmlUtils::CXmlReader oXmlReader;

  if( TRUE == oXmlReader.OpenFromXmlString( bsXML ) )
  {
    if( TRUE == oXmlReader.ReadRootNode( _T( "pen" ) ) )
	{
	  CString penColor = oXmlReader.ReadNodeAttributeOrValue( _T( "color" ) );
	  CString penSize = oXmlReader.ReadNodeAttributeOrValue( _T( "size" ) );

	  if ( !penColor.IsEmpty() )
	  {
		this->ooxDocument.SetPenColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( Parse<int>( penColor.GetString() ) ) ) ) );
	  }

	  if ( !penSize.IsEmpty() )
	  {
	    this->ooxDocument.SetPenSize( Parse<double>( penSize.GetString() ) );
	  }
	}
  }
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenColor(LONG* lColor)
{
  if ( lColor != NULL )
  {
	*lColor = this->penColor;

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenColor(LONG lColor)
{
  this->penColor = lColor;

  this->ooxDocument.SetPenColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( this->penColor ) ) ) );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenAlpha(LONG* lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenAlpha(LONG lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenSize(double* dSize)
{
  if ( dSize != NULL )
  {
	*dSize = this->penSize;

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenSize(double dSize)
{
  this->penSize = dSize;

  this->ooxDocument.SetPenSize( this->penSize );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenDashStyle(BYTE* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenDashStyle(BYTE val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineStartCap(BYTE* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineStartCap(BYTE val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineEndCap(BYTE* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineEndCap(BYTE val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineJoin(BYTE* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineJoin(BYTE val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenDashOffset(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenDashOffset(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenAlign(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenAlign(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenMiterLimit(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenMiterLimit(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PenDashPattern(SAFEARRAY* pPattern)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetBrush(BSTR bsXML)
{
  XmlUtils::CXmlReader oXmlReader;

  if( TRUE == oXmlReader.OpenFromXmlString( bsXML ) )
  {
    if( TRUE == oXmlReader.ReadRootNode( _T( "brush" ) ) )
	{
	  CString brushColor = oXmlReader.ReadNodeAttributeOrValue( _T( "color1" ) );

	  if ( !brushColor.IsEmpty() )
	  {
	    int brushColorInt = Parse<int>( brushColor.GetString() );

		this->ooxDocument.SetBrushColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( brushColorInt ) ) ) );
		
		this->ooxDocument.SetFontColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( brushColorInt ) ) ) );
	  }
	}
  }
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushType(LONG* lType)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushType(LONG lType)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushColor1(LONG* lColor)
{
  if ( lColor != NULL )
  {
	*lColor = this->brushColor;

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushColor1(LONG lColor)
{
  this->brushColor = lColor;

  this->ooxDocument.SetBrushColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( this->brushColor ) ) ) );
		
  this->ooxDocument.SetFontColor( ToString( DocxRenderer::Utils::ToHexString( DocxRenderer::Utils::little_endian_2_big_endian( this->brushColor ) ) ) );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushAlpha1(LONG* lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushAlpha1(LONG lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushColor2(LONG* lColor)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushColor2(LONG lColor)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushAlpha2(LONG* lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushAlpha2(LONG lAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTexturePath(BSTR* bsPath)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTexturePath(BSTR bsPath)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTextureMode(LONG* lMode)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTextureMode(LONG lMode)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTextureAlpha(LONG lTxAlpha)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushLinearAngle(double* dAngle)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushLinearAngle(double dAngle)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::BrushRect(BOOL val, double left, double top, double width, double height)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetFont(BSTR bsXML)
{
  XmlUtils::CXmlReader oXmlReader;

  if( TRUE == oXmlReader.OpenFromXmlString( bsXML ) )
  {
    if( TRUE == oXmlReader.ReadRootNode( _T( "font" ) ) )
	{
	  CString fontName = oXmlReader.ReadNodeAttributeOrValue( _T( "name" ) );
	  CString fontSize = oXmlReader.ReadNodeAttributeOrValue( _T( "size" ) );
      CString bold = oXmlReader.ReadNodeAttributeOrValue( _T( "bold" ) );
      CString italic = oXmlReader.ReadNodeAttributeOrValue( _T( "italic" ) );
      CString underline = oXmlReader.ReadNodeAttributeOrValue( _T( "underline" ) );
	  CString strikeout = oXmlReader.ReadNodeAttributeOrValue( _T( "strikeout" ) );

	  if ( !fontName.IsEmpty() )
	  {
		this->ooxDocument.SetFontName( ToString( fontName.GetString() ) );
	  }

	  if ( !fontSize.IsEmpty() )
	  {
	    this->ooxDocument.SetFontSize( Parse<double>( ToString( fontSize.GetString() ) ) );
	  }

	  if ( !bold.IsEmpty() )
	  {
		this->ooxDocument.SetBold( ( Parse<int>( ToString( bold.GetString() ) ) != 0 ) ? ( true ) : ( false ) );
	  }

	  if ( !italic.IsEmpty() )
	  {
		this->ooxDocument.SetItalic( ( Parse<int>( ToString( italic.GetString() ) ) != 0 ) ? ( true ) : ( false ) );
	  }

	  if ( !strikeout.IsEmpty() )
	  {
		this->ooxDocument.SetStrikeout( ( Parse<int>( ToString( strikeout.GetString() ) ) != 0 ) ? ( true ) : ( false ) );
	  }

	  if ( !underline.IsEmpty() )
	  {
		this->ooxDocument.SetUnderline( ( Parse<int>( ToString( underline.GetString() ) ) != 0 ) ? ( true ) : ( false ) );
	  }
	}
  }
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontName(BSTR* bsName)
{
  if ( bsName != NULL )
  {
    *bsName = SysAllocString( this->fontName.c_str() );

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontName(BSTR bsName)
{
  this->fontName = std::wstring( bsName );

  this->ooxDocument.SetFontName( ToString( this->fontName ) );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontPath(BSTR* bsName)
{
  if ( bsName != NULL )
  {
    *bsName = SysAllocString( this->fontPath.c_str() );

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontPath(BSTR bsName)
{
  this->fontPath = std::wstring( bsName );

  if ( this->fontSize != 0 )
  {
	this->ooxDocument.LoadFontFromFile( this->fontPath, this->fontSize );
  }

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontSize(double* dSize)
{
  if ( dSize != NULL )
  {
    *dSize = this->fontSize;

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontSize(double dSize)
{
  this->fontSize = dSize;

  this->ooxDocument.SetFontSize( dSize );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontStyle(LONG* lStyle)
{
  if ( lStyle != NULL )
  {
    *lStyle = this->fontStyle;

	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontStyle(LONG lStyle)
{
  this->fontStyle = lStyle;

  this->ooxDocument.SetBold( false );
  this->ooxDocument.SetItalic( false );
  this->ooxDocument.SetUnderline( false );
  this->ooxDocument.SetStrikeout( false );

  if ( this->fontStyle & 1 == 1 )
  {
    this->ooxDocument.SetBold( true );
  }

  if ( this->fontStyle >> 1 == 1 )
  {
    this->ooxDocument.SetItalic( true );
  }

  if ( this->fontStyle >> 2 == 1 )
  {
    this->ooxDocument.SetUnderline( true );
  }

  if ( this->fontStyle >> 3 == 1 )
  {
    this->ooxDocument.SetStrikeout( true );
  }

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontStringGID(BOOL* bGID)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontStringGID(BOOL bGID)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontCharSpace(double* dSpace)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontCharSpace(double dSpace)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetShadow(BSTR bsXML)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowDistanceX(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowDistanceX(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowDistanceY(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowDistanceY(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowBlurSize(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowBlurSize(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowColor(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowColor(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowAlpha(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowAlpha(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowVisible(BOOL* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowVisible(BOOL val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetEdgeText(BSTR bsXML)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeVisible(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeVisible(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeColor(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeColor(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeAlpha(LONG* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeAlpha(LONG val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeDist(double* val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeDist(double val)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
  if ( bsText != NULL )
  {
    std::wstring text;

	for ( unsigned int i = 0; i < SysStringLen( bsText ); i++ )
	{
	  if ( DocxRenderer::Utils::IsUnicodeSymbol( bsText[i] ) )
	  {
	    text += bsText[i];
	  }
	}

	if ( !text.empty() )
	{
	  this->ooxDocument.AddText( text, fX, fY, fWidth, fHeight, fBaseLineOffset );
	}
  }

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
  if ( ( bsUnicodeText != NULL ) && ( bsGidText != NULL ) )
  {
    std::wstring text;

	for ( unsigned int i = 0; i < SysStringLen( bsUnicodeText ); i++ )
	{
	  if ( DocxRenderer::Utils::IsUnicodeSymbol( bsUnicodeText[i] ) )
	  {
	    text += bsUnicodeText[i];
	  }
	}

	if ( !text.empty() )
	{
	  this->ooxDocument.AddText( text, fX, fY, fWidth, fHeight, fBaseLineOffset );
	}
  }
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::BeginCommand(DWORD lType)
{ 
  this->ooxDocument.BeginCommand( lType );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::EndCommand(DWORD lType)
{
  this->ooxDocument.EndCommand( lType );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandMoveTo(double fX, double fY)
{
  this->ooxDocument.PathMoveTo( fX, fY );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandLineTo(double fX, double fY)
{
  this->ooxDocument.PathLineTo( fX, fY );
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandLinesTo(SAFEARRAY* pPoints)
{
  if ( pPoints != NULL )
  {
    LONG lCount = pPoints->rgsabound[0].cElements;
    double* pData = (double*)(pPoints->pvData);

	std::vector<double> points;

    for ( LONG i = 0; i < (LONG)( lCount ); i++ )
	{
	  points.push_back( pData[i] );
	}

	this->ooxDocument.PathLinesTo( points );
	
	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
  this->ooxDocument.PathCurveTo( fX1, fY1, fX2, fY2, fX3, fY3 );
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
  if ( pPoints != NULL )
  {
    LONG lCount = pPoints->rgsabound[0].cElements;
    double* pData = (double*)(pPoints->pvData);

	std::vector<double> points;

    for ( LONG i = 0; i < (LONG)( lCount ); i++ )
	{
	  points.push_back( pData[i] );
	}

	this->ooxDocument.PathCurvesTo( points );
	
	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
  this->ooxDocument.PathArcTo( fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle );
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandClose()
{
  this->ooxDocument.PathClose();
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandEnd()
{
  this->ooxDocument.PathEnd();
  	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawPath(long nType)
{
  this->ooxDocument.DrawPath( nType );
  	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandStart()
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
{
  this->ooxDocument.PathGetCurrentPoint( fX, fY );
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
  this->ooxDocument.GetCommandParams( dAngle, dLeft, dTop, dWidth, dHeight, lFlags );
		
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
  this->ooxDocument.SetCommandParams( dAngle, dLeft, dTop, dWidth, dHeight, lFlags );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
  if ( pInterface != NULL )
  {
    AVSImageStudio3::IImageTransformsPtr piImageStudio = NULL;
    piImageStudio.CreateInstance(__uuidof(AVSImageStudio3::ImageTransforms));
	
    VARIANT_BOOL vbSuccess = VARIANT_FALSE;
    VARIANT vtFrame;
    vtFrame.vt = VT_UNKNOWN;
    vtFrame.punkVal = pInterface;

    piImageStudio->SetSource( 0, vtFrame );

    CString sSaveXml;
	this->imageFiles.push_back( DocxRenderer::Utils::GetTempFile( tempPath, _T( "IMG" ), true ) );
	std::wstring sPngPath = this->imageFiles.back() + _T( ".png" );
	//Удаляем существующий файл, а иначе boost генерирует исключение, а не переписывает файл
	if( boost::filesystem::exists(sPngPath ))
		boost::filesystem::remove(sPngPath);
	boost::filesystem::rename( this->imageFiles.back(), sPngPath );
	this->imageFiles.back() += _T( ".png" );
	sSaveXml.AppendFormat( _T("<ImageFile-SaveAsPng destinationpath=\"%s\" format=\"888\"  quality=\"100\" />"), this->imageFiles.back().c_str() );
    BSTR bstrSaveXml = sSaveXml.AllocSysString();
    
	piImageStudio->SetXml( bstrSaveXml, &vbSuccess );

	if( VARIANT_TRUE == vbSuccess )
	{
      piImageStudio->Transform( &vbSuccess );
		
	  if( VARIANT_TRUE == vbSuccess )
	  {
        if( SUCCEEDED( piImageStudio->GetResult( 0, &vtFrame ) ) )
        {
          RELEASEINTERFACE( vtFrame.punkVal );
          vbSuccess = VARIANT_TRUE;

		  this->DrawImageFromFile( _bstr_t( this->imageFiles.back().c_str() ), fX, fY, fWidth, fHeight );
        }

        SysFreeString( bstrSaveXml );
	  }
	}

	piImageStudio.Release();
	
	return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
  this->ooxDocument.AddImage( std::wstring( bstrVal ), fX, fY, fWidth, fHeight );
  
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
  this->ooxDocument.SetTransform( dA, dB, dC, dD, dE, dF );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
  this->ooxDocument.GetTransform( pdA, pdB, pdC, pdD, pdE, pdF );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::ResetTransform(void)
{
  this->ooxDocument.ResetTransform();
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ClipMode(LONG* plMode)
{
  if ( plMode != NULL )
  {
    *plMode = this->ooxDocument.GetClipMode();
  }
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ClipMode(LONG lMode)
{
  this->ooxDocument.SetClipMode( lMode );
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
  this->ooxDocument.PathText( std::wstring( bsText ), fX, fY, fWidth, fHeight, fBaseLineOffset );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
  this->PathCommandText( bsUnicodeText, fX, fY, fWidth, fHeight, fBaseLineOffset );

  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::Initialize(BSTR bsXMLOptions)
{
  XmlUtils::CXmlReader oXmlReader;

  if( TRUE == oXmlReader.OpenFromXmlString( bsXMLOptions ) )
  {
    if( TRUE == oXmlReader.ReadRootNode( _T( "DocxRenderer" ) ) )
	{
	  CString destinationPath = oXmlReader.ReadNodeAttributeOrValue( _T( "destinationpath" ) );
	  
	  if ( !destinationPath.IsEmpty() )
	  {
	    this->ooxDocument.SetDestinationPath( ::ToWString( destinationPath.GetString() ) );
		//ищем родительскую папку чтобы в ней разместить наши внутренние темповые файлы
		int nIndex = -1;
		int nIndex1 = destinationPath.ReverseFind( '\\' );
		int nIndex2 = destinationPath.ReverseFind( '/' );
		if( -1 != nIndex1 || -1 != nIndex2 )
		{
			if( nIndex1 > nIndex2 )
				nIndex = nIndex1;
			else
				nIndex = nIndex2;
		}
		if( -1 != nIndex )
			tempPath = destinationPath.Mid( 0, nIndex ).GetBuffer();
	  }

	  CString textFormatting = oXmlReader.ReadNodeAttributeOrValue( _T( "textformatting" ) );

	  if ( !textFormatting.IsEmpty() )
	  {
	    this->ooxDocument.SetTextAssociationType( (DocxRenderer::TextAssociationType)Parse<int>( textFormatting.GetString() ) );
	  }
	}
  }
  this->pOfficeUtils.CreateInstance( __uuidof(AVSOfficeUtils::COfficeUtils) );

  criticalSection.Enter();

  if( L"" == tempPath )
	  tempPath = DocxRenderer::Utils::GetTempDir();
  tempFile = DocxRenderer::Utils::GetTempFile( tempPath, _T( "DOCX" ), true );
  tempDir = DocxRenderer::Utils::GetTempFile( tempPath, _T( "DOCX" ), false );

  if ( !tempPath.empty() && !tempFile.empty() && !tempDir.empty() )
  {
	  this->CreateOriginDocx( tempFile, tempDir );
  }

  this->ooxDocument.CreateFromTemplate( tempDir );

  criticalSection.Leave(); 
	
  return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::Save()
{
  this->ooxDocument.Save();

  BOOST_FOREACH( const std::wstring& fileName, this->imageFiles )
  {
    boost::filesystem::remove( fileName );
  }
	
  return S_OK;
}

/*========================================================================================================*/

const unsigned long CAVSDocxRenderer::LoadFromResource( LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName ) const
{
  HMODULE hMod = GetModuleHandle( _T( "AVSOfficeDocxRenderer.dll" ) );
  
  if ( hMod )
  {
    HRSRC hRes = FindResource( hMod, lpResName, lpResType );
	
	if ( hRes )
	{
	  HGLOBAL hGlob = LoadResource( hMod, hRes );
	
	  if (hGlob)
	  {
	    BYTE *lpbArray = (BYTE*)LockResource( hGlob );
		
		if ( lpbArray )
		{
		  const DWORD dwFileSize = SizeofResource( hMod, hRes );
		
		  if ( dwFileSize != 0 )
		  {
		    int hFile = 0;
			
			errno_t err = _wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE);

			if ( !err )
			{
			  _write( hFile, lpbArray, dwFileSize );
			  _close( hFile );
			}
		  }
	    }
	  }
	}
  }

  return GetLastError();
}

/*========================================================================================================*/

void CAVSDocxRenderer::CreateOriginDocx( const std::wstring& tempFileName, const std::wstring& tempDirName ) const
{
  boost::filesystem::create_directories( tempDirName );
  
  unsigned long error = this->LoadFromResource( MAKEINTRESOURCE( IDR_DOCUMENT1 ), _T( "Document" ), tempFileName.c_str() );
  
  this->pOfficeUtils->ExtractToDirectory( _bstr_t( tempFileName.c_str() ), _bstr_t( tempDirName.c_str() ), NULL, 0 );
  
  boost::filesystem::remove( tempFileName );
}
