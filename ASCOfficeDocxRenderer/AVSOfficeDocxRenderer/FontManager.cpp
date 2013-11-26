#include "stdafx.h"

#include "FontManager.h"

namespace DocxRenderer
{
  FontManager::FontManager( const std::wstring& _xmlOptions, double _xDPI, double _yDPI ):
  xDPI(_xDPI), yDPI(_yDPI)
  {
	this->pFontManager.CreateInstance( __uuidof(AVSGraphics::CAVSFontManager) );

	this->pFontManager->Initialize( _bstr_t( _xmlOptions.c_str() ) );
	    
	this->defaultFontName = _T( "Arial" );
	
	this->pFontManager->SetDefaultFont( _bstr_t( this->defaultFontName.c_str() ) );
  }

  /*========================================================================================================*/

  FontManager::~FontManager()
  {
	this->pFontManager.Release();
  }

  /*========================================================================================================*/

  void FontManager::SetDefaultFont( const std::wstring& _familyName )
  {
    this->defaultFontName = _familyName;
	  
	this->pFontManager->SetDefaultFont( _bstr_t( this->defaultFontName.c_str() ) );
  }

  /*========================================================================================================*/

  std::wstring FontManager::GetDefaultFont() const
  {
    return this->defaultFontName;
  }

  /*========================================================================================================*/

  void FontManager::SetXDPI( double _xDPI )
  {
    this->xDPI = _xDPI;
  }

  /*========================================================================================================*/

  double FontManager::GetXDPI() const
  {
    return this->xDPI;
  }

  /*========================================================================================================*/

  void FontManager::SetYDPI( double _yDPI )
  {
    this->yDPI = _yDPI;
  }

  /*========================================================================================================*/

  double FontManager::GetYDPI() const
  {
    return this->yDPI;
  }

  /*========================================================================================================*/

  double FontManager::GetTextWidth( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType ) const
  {
    UniversalUnit ptValue( UniversalUnit::Pt );
	UniversalUnit mmValue( UniversalUnit::Mm );
    
	HRESULT res = S_FALSE;

    float fBoxX = 0;
	float fBoxY = 0;
	float fBoxWidth = 0;
	float fBoxHeight = 0;

	res = this->pFontManager->LoadFontByName( _bstr_t( _fontName.c_str() ), _fontSize, _fontStyle, this->xDPI, this->yDPI );
	res = this->pFontManager->LoadStringW( _bstr_t( _text.c_str() ), 0, 0 );
	
	if ( _measureType == MeasureTypeGlyph )
	{
	  res = this->pFontManager->MeasureString( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}
	else if ( _measureType == MeasureTypePosition )
	{
	  res = this->pFontManager->MeasureString2( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}

	ptValue = fBoxWidth;
    mmValue = ptValue;
	
    return mmValue;
  }

  /*========================================================================================================*/

  double FontManager::GetTextWidth( const std::wstring& _text, MeasureType _measureType, bool _isGidText ) const
  {
    UniversalUnit ptValue( UniversalUnit::Pt );
	UniversalUnit mmValue( UniversalUnit::Mm );
    
	HRESULT res = S_FALSE;

    float fBoxX = 0;
	float fBoxY = 0;
	float fBoxWidth = 0;
	float fBoxHeight = 0;

	if ( _isGidText )
	{
	  res = this->pFontManager->SetStringGID( 1 );
	}

	res = this->pFontManager->LoadStringW( _bstr_t( _text.c_str() ), 0, 0 );
	
	if ( _measureType == MeasureTypeGlyph )
	{
	  res = this->pFontManager->MeasureString( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}
	else if ( _measureType == MeasureTypePosition )
	{
	  res = this->pFontManager->MeasureString2( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}

	if ( _isGidText )
	{
	  res = this->pFontManager->SetStringGID( 0 );
	}

	ptValue = fBoxWidth;
    mmValue = ptValue;
	
    return mmValue;
  }

  /*========================================================================================================*/

  double FontManager::GetTextHeight( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType ) const
  {
    UniversalUnit ptValue( UniversalUnit::Pt );
	UniversalUnit mmValue( UniversalUnit::Mm );
    
	HRESULT res = S_FALSE;

    float fBoxX = 0;
	float fBoxY = 0;
	float fBoxWidth = 0;
	float fBoxHeight = 0;
	
	res = this->pFontManager->LoadFontByName( _bstr_t( _fontName.c_str() ), _fontSize, _fontStyle, this->xDPI, this->yDPI );
	res = this->pFontManager->LoadStringW( _bstr_t( _text.c_str() ), 0, 0 );
	
	if ( _measureType == MeasureTypeGlyph )
	{
	  res = this->pFontManager->MeasureString( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}
	else if ( _measureType == MeasureTypePosition )
	{
	  res = this->pFontManager->MeasureString2( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	}

	ptValue = fBoxHeight;
    mmValue = ptValue;
	
    return mmValue;
  }

  /*========================================================================================================*/

  double FontManager::GetFontHeight( const std::wstring& _fontName, double _fontSize, long _fontStyle ) const
  {
    HRESULT res = S_FALSE;

	double fontHeight = 0;

	UniversalUnit ptValue( UniversalUnit::Pt );
	UniversalUnit mmValue( UniversalUnit::Mm );

	res = this->pFontManager->LoadFontByName( _bstr_t( _fontName.c_str() ), _fontSize, _fontStyle, this->xDPI, this->yDPI );

	if ( SUCCEEDED( res ) )
	{
	  unsigned short lineSpacing = 0;
	  unsigned short eMHeight = 0;
	
	  res = this->pFontManager->GetLineSpacing( &lineSpacing );
	  res = this->pFontManager->GetEmHeight( &eMHeight );

	  if ( SUCCEEDED( res ) )
	  {
	    ptValue = ( ( lineSpacing * _fontSize ) / eMHeight );
	    mmValue = ptValue;

	    fontHeight = mmValue;
	  }
	}

	return fontHeight;
  }

  /*========================================================================================================*/

  double FontManager::GetFontHeight( double _fontSize ) const
  {
    HRESULT res = S_FALSE;

	double fontHeight = 0;

	UniversalUnit ptValue( UniversalUnit::Pt );
	UniversalUnit mmValue( UniversalUnit::Mm );

	unsigned short lineSpacing = 0;
	unsigned short eMHeight = 0;
	
	res = this->pFontManager->GetLineSpacing( &lineSpacing );
	res = this->pFontManager->GetEmHeight( &eMHeight );

	if ( SUCCEEDED( res ) )
	{
	  ptValue = ( ( lineSpacing * _fontSize ) / eMHeight );
	  mmValue = ptValue;

	  fontHeight = mmValue;
	}

	return fontHeight;
  }

  /*========================================================================================================*/

  void FontManager::MeasureString( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, double _x, double _y, double* _boxX, double* _boxY, double* _boxWidth, double* _boxHeight, MeasureType _measureType ) const
  {
    if ( ( _boxX != NULL ) && ( _boxY != NULL ) && ( _boxWidth != NULL ) && ( _boxHeight != NULL ) )
    {
      UniversalUnit ptValue( UniversalUnit::Pt );
      UniversalUnit mmValue( UniversalUnit::Mm );
    
      HRESULT res = S_FALSE;

      float fBoxX = 0;
      float fBoxY = 0;
      float fBoxWidth = 0;
      float fBoxHeight = 0;

	  mmValue = _x;
	  ptValue = mmValue;
	  _x = ptValue;

	  mmValue = _y;
	  ptValue = mmValue;
	  _y = ptValue;

	  res = this->pFontManager->LoadFontByName( _bstr_t( _fontName.c_str() ), _fontSize, _fontStyle, this->xDPI, this->yDPI );
      res = this->pFontManager->LoadStringW( _bstr_t( _text.c_str() ), _x, _y );
	
	  if ( _measureType == MeasureTypeGlyph )
	  {
	    res = this->pFontManager->MeasureString( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	  }
	  else if ( _measureType == MeasureTypePosition )
	  {
	    res = this->pFontManager->MeasureString2( &fBoxX, &fBoxY, &fBoxWidth, &fBoxHeight );
	  }

      ptValue = fBoxX;
      mmValue = ptValue;

	  *_boxX = mmValue;

	  ptValue = fBoxY;
      mmValue = ptValue;

	  *_boxY = mmValue;

	  ptValue = fBoxWidth;
      mmValue = ptValue;

	  *_boxWidth = mmValue;

	  ptValue = fBoxHeight;
      mmValue = ptValue;

	  *_boxHeight = mmValue;
	}
  }

  /*========================================================================================================*/

  double FontManager::GetSpaceWidthBetweenStrings( const std::wstring& _firstString, const std::wstring& _secondString, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType ) const
  {
    const std::wstring stringWithSpace = ( _firstString + _T( " " ) + _secondString );
	
	double stringWithSpaceWidth = this->GetTextWidth( stringWithSpace, _fontName, _fontSize, _fontStyle, _measureType );
	double firstStringWidth = this->GetTextWidth( _firstString, _fontName, _fontSize, _fontStyle, _measureType );
	double secondStringWidth = this->GetTextWidth( _secondString, _fontName, _fontSize, _fontStyle, _measureType );

	return ( stringWithSpaceWidth - firstStringWidth - secondStringWidth );
  }

  /*========================================================================================================*/

  void FontManager::LoadFontFromFile( const std::wstring& _fileName, double _fontSize, double _xDPI, double _yDPI, long _faceIndex )
  {
    HRESULT res = S_FALSE;
	
	res = this->pFontManager->LoadFontFromFile( _bstr_t( _fileName.c_str() ), _fontSize, _xDPI, _yDPI, _faceIndex );
  }

  /*========================================================================================================*/

  std::wstring FontManager::GetFamilyName() const
  {
    HRESULT res = S_FALSE;
	_bstr_t familyNameBSTR;
	std::wstring familyName;

	res = this->pFontManager->GetFamilyNameEx( _bstr_t( "<DeletePDFPrefix/>" ), familyNameBSTR.GetAddress() );

	if ( familyNameBSTR.length() != 0 )
	{
	  familyName = std::wstring( familyNameBSTR );
	}

	return familyName;
  }

  /*========================================================================================================*/

  std::wstring FontManager::GetStyleName() const
  {
    HRESULT res = S_FALSE;
	_bstr_t styleNameBSTR;
	std::wstring styleName;

	res = this->pFontManager->GetStyleName( styleNameBSTR.GetAddress() );

	if ( styleNameBSTR.length() != 0 )
	{
	  styleName = std::wstring( styleNameBSTR );
	}

	return styleName;
  }

  /*========================================================================================================*/

  std::wstring FontManager::GetPANOSE() const
  {
    HRESULT res = S_FALSE;
	SAFEARRAY* panoseSafeArray = NULL;
	std::wstring panose;

	res = this->pFontManager->GetPanose( &panoseSafeArray );

	if ( panoseSafeArray != NULL )
	{
	  LONG lCount = panoseSafeArray->rgsabound[0].cElements;
      byte* pData = (byte*)(panoseSafeArray->pvData);

	  for ( LONG i = 0; i < (LONG)( lCount ); i++ )
	  {
	    panose += ToWString( Utils::ToHexString( pData[i] ) );
	  }

	  SafeArrayDestroy( panoseSafeArray );
	}
	
	return panose;
  }

  /*========================================================================================================*/

  bool FontManager::IsFixedWidth() const
  {
    HRESULT res = S_FALSE;
	long isFixedWidthLong = 0;

	res = this->pFontManager->IsFixedWidth( &isFixedWidthLong );

	return ( ( isFixedWidthLong != 0 ) ? ( true ) : ( false ) );
  }

  /*========================================================================================================*/

  std::vector<std::wstring> FontManager::GetSignature() const
  {
    HRESULT res = S_FALSE;
	std::vector<std::wstring> signature;
	
	VARIANT_BOOL vbSuccess = VARIANT_FALSE;

	for ( unsigned int i = 0; i < 6; i++ )
	{
	  unsigned int value = 0;
		
	  for ( unsigned long bit = 0; bit < 32; bit++ )
	  {
	    res = this->pFontManager->IsUnicodeRangeAvailable( bit, i, &vbSuccess );
 
		if( VARIANT_TRUE == vbSuccess )
		{
  		  value |= ( 1 << bit );
		}
	  }

	  signature.push_back( ToWString( Utils::ToHexString( value ) ) );
	}

	return signature;
  }
}