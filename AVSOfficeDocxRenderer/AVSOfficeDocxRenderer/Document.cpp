#include "stdafx.h"

#include "Document.h"

namespace DocxRenderer
{
  Document::Document( TextAssociationType _textAssociationType ):
  width(0), height(0), pageCount(0), textAssociationType(_textAssociationType)
  {
	this->currentPage = Page( &this->ooxFolder, &this->fontManager, this->textAssociationType );
  }

  /*========================================================================================================*/

  Document::Document( const std::wstring& _ooxDir, TextAssociationType _textAssociationType ):
  width(0), height(0), pageCount(0), textAssociationType(_textAssociationType)
  {
    this->ooxFolder.createFromTemplate( _ooxDir );
		
	this->currentPage = Page( &this->ooxFolder, &this->fontManager, this->textAssociationType );
  }

  /*========================================================================================================*/

  void Document::CreateFromTemplate( const std::wstring& _ooxDir )
  {
    this->ooxFolder.createFromTemplate( _ooxDir );
  }

  /*========================================================================================================*/

  void Document::SetDestinationPath( const std::wstring& _destinationPath )
  {
    this->destinationPath = _destinationPath;
  }

  /*========================================================================================================*/

  std::wstring Document::GetDestinationPath() const
  {
    return this->destinationPath;
  }

  /*========================================================================================================*/

  void Document::AddPage()
  {
    if ( this->pageCount > 1 )
	{
	  if ( !this->ooxFolder.find<OOX::Document>().Items->empty() )
	  {
	    if ( this->ooxFolder.find<OOX::Document>().Items->front().is<OOX::Logic::Paragraph>() )
	    {
	      OOX::Logic::Paragraph& paragraph = this->ooxFolder.find<OOX::Document>().Items->front().as<OOX::Logic::Paragraph>();
		  
	      paragraph.Property.init();
	      paragraph.Property->Spacing.init();
	      paragraph.Property->Spacing->Line = 1;
          paragraph.Property->Spacing->LineRule = "exact";
	    }
	  }
		
	  this->previousPage.Build();

	  std::vector<OOX::Logic::Paragraph> pageParagraphs = this->previousPage.GetParagraphs();

	  for ( std::vector<OOX::Logic::Paragraph>::const_iterator pageParagraphsIter = pageParagraphs.begin(); pageParagraphsIter != pageParagraphs.end(); ++pageParagraphsIter )
	  {
	    this->ooxFolder.find<OOX::Document>().Items->push_back( *pageParagraphsIter );
	  }

	  this->ooxFolder.Commit( this->destinationPath );
	}

	if ( this->pageCount > 0 )
	{
	  if ( ( this->currentPage.GetWidth() == 0 ) && ( this->currentPage.GetHeight() == 0 ) )
	  {
	    this->currentPage.SetWidth( this->width );
		this->currentPage.SetHeight( this->height );
	  }

	  this->previousPage = this->currentPage;
	  this->currentPage = Page( &this->ooxFolder, &this->fontManager, this->textAssociationType );
	}

	this->pageCount++;
  }

  /*========================================================================================================*/

  void Document::SetWidth( double _dWidth )
  {
    if ( this->pageCount > 1 )
	{
      if ( this->previousPage.GetWidth() == 0 )
	  {
	    this->previousPage.SetWidth( this->width );
	  }
	}
	else
	{
	  this->currentPage.SetWidth( 0 );
	}

	this->width = _dWidth;

	this->currentPage.SetGraphicsWidth( this->width );
  }

  /*========================================================================================================*/

  double Document::GetWidth() const
  {
    return this->width;
  }

  /*========================================================================================================*/

  void Document::SetHeight( double _dHeight )
  {
	if ( this->pageCount > 1 )
	{
      if ( this->previousPage.GetHeight() == 0 )
	  {
	    this->previousPage.SetHeight( this->height );
	  }
	}
	else
	{
	  this->currentPage.SetHeight( 0 );
	}

	this->height = _dHeight;

	this->currentPage.SetGraphicsHeight( this->height );
  }

  /*========================================================================================================*/

  double Document::GetHeight() const
  {
    return this->height;
  }

  /*========================================================================================================*/

  void Document::SetTextAssociationType( TextAssociationType _textAssociationType )
  {
    this->textAssociationType = _textAssociationType;

	this->previousPage.SetTextAssociationType( this->textAssociationType );
	this->currentPage.SetTextAssociationType( this->textAssociationType );
  }

  /*========================================================================================================*/

  TextAssociationType Document::GetTextAssociationType() const
  {
    return this->textAssociationType;
  }

  /*========================================================================================================*/

  void Document::BeginCommand( unsigned long _lType )
  {
	this->currentPage.BeginCommand( _lType );
  }

  /*========================================================================================================*/

  void Document::EndCommand( unsigned long _lType )
  {
    this->currentPage.EndCommand( _lType );
  }

  /*========================================================================================================*/

  void Document::AddText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset, const std::wstring& _gidText )
  {
    this->currentPage.AddText( _text, _dX, _dY, _dWidth, _dHeight, _dBaseLineOffset, this->width, this->height, _gidText );
  }

  /*========================================================================================================*/

  void Document::AddImage( const std::wstring& _fileName, double _dX, double _dY, double _dWidth, double _dHeight )
  {
  	this->currentPage.AddImage( _fileName, _dX, _dY, _dWidth, _dHeight );
  }

  /*========================================================================================================*/

  void Document::PathMoveTo( double _dX, double _dY )
  {
    this->currentPage.PathMoveTo( _dX, _dY );
  }

  /*========================================================================================================*/

  void Document::PathLineTo( double _dX, double _dY )
  {
    this->currentPage.PathLineTo( _dX, _dY );
  }

  /*========================================================================================================*/

  void Document::PathLinesTo( const std::vector<double>& _points )
  {
	this->currentPage.PathLinesTo( _points );
  }

  /*========================================================================================================*/

  void Document::PathCurveTo( double _x1, double _y1, double _x2, double _y2, double _x3, double _y3 )
  {
	this->currentPage.PathCurveTo( _x1, _y1, _x2, _y2, _x3, _y3 );
  }

  /*========================================================================================================*/

  void Document::PathCurvesTo( const std::vector<double>& _points )
  {
    this->currentPage.PathCurvesTo( _points );
  }

  /*========================================================================================================*/

  void Document::PathArcTo( double _x, double _y, double _width, double _height, double _startAngle, double _sweepAngle )
  {
	this->currentPage.PathArcTo( _x, _y, _width, _height, _startAngle, _sweepAngle );
  }

  /*========================================================================================================*/

  void Document::PathClose()
  {
    this->currentPage.PathClose();
  }

  /*========================================================================================================*/

  void Document::PathEnd()
  {
	this->currentPage.PathEnd();
  }

  /*========================================================================================================*/

  void Document::DrawPath( long _nType, unsigned int _resize )
  {
	this->currentPage.DrawPath( _nType, _resize );
  }

  /*========================================================================================================*/

  void Document::PathStart()
  {
    //!!!TODO!!!
	this->currentPage.PathStart();
  }

  /*========================================================================================================*/

  void Document::PathGetCurrentPoint( double* _dX, double* _dY )
  {
	this->currentPage.PathGetCurrentPoint( _dX, _dY );
  }

  /*========================================================================================================*/

  void Document::GetCommandParams( double* _dAngle, double* _dLeft, double* _dTop, double* _dWidth, double* _dHeight, unsigned long* _lFlags )
  {
	this->currentPage.GetCommandParams( _dAngle, _dLeft, _dTop, _dWidth, _dHeight, _lFlags );
  }

  /*========================================================================================================*/

  void Document::SetCommandParams( double _dAngle, double _dLeft, double _dTop, double _dWidth, double _dHeight, unsigned long _lFlags )
  {
    this->currentPage.SetCommandParams( _dAngle, _dLeft, _dTop, _dWidth, _dHeight, _lFlags );
  }

  /*========================================================================================================*/

  void Document::SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF )
  {
	this->currentPage.SetTransform( _dA, _dB, _dC, _dD, _dE, _dF );
  }

  /*========================================================================================================*/

  void Document::SetTransform( const TransformationMatrix& _matrix )
  {
	this->currentPage.SetTransform( _matrix );
  }

  /*========================================================================================================*/

  void Document::GetTransform( double* _pdA, double* _pdB, double* _pdC, double* _pdD, double* _pdE, double* _pdF )
  {
    this->currentPage.GetTransform( _pdA, _pdB, _pdC, _pdD, _pdE, _pdF );
  }

  /*========================================================================================================*/

  void Document::ResetTransform()
  {
    this->currentPage.ResetTransform();
  }

  /*========================================================================================================*/

  long Document::GetClipMode()
  {
    //!!!TODO!!!
    return this->currentPage.GetClipMode();
  }

  /*========================================================================================================*/

  void Document::SetClipMode( long _lMode )
  {
    //!!!TODO!!!
    this->currentPage.SetClipMode( _lMode );
  }

  /*========================================================================================================*/

  void Document::PathText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset )
  {
    //!!!TODO!!!
    this->currentPage.PathText( _text, _dX, _dY, _dWidth, _dHeight, _dBaseLineOffset );
  }

  /*========================================================================================================*/

  void Document::SetPenColor( const std::string& _penColor )
  {
	this->currentPage.SetPenColor( _penColor );
  }

  /*========================================================================================================*/

  void Document::SetPenSize( double _penSize )
  {
	this->currentPage.SetPenSize( _penSize );
  }

  /*========================================================================================================*/

  void Document::SetFontColor( const std::string& _fontColor )
  {
	this->currentPage.SetFontColor( _fontColor );
  }

  /*========================================================================================================*/

  void Document::SetBrushColor( const std::string& _brushColor )
  {
    this->currentPage.SetBrushColor( _brushColor );
  }

  /*========================================================================================================*/

  void Document::SetFontName( const std::string& _fontName )
  { 
	if ( this->currentFont.name.empty() )
	{
	  this->currentFont.name = _fontName;
	}
	  
	if ( !this->IsFontPresent( this->currentFont.name ) )
	{
	  if ( this->currentFont.characteristics.IsDefault() )
	  {
	    OOX::FontTable& fontTable = this->ooxFolder.find<OOX::Document>().find<OOX::FontTable>();

	    OOX::FontTable::Font font;

	    font.Name = this->currentFont.name;

	    fontTable.Fonts->push_back( font );
	  }
	  else
	  {
	    this->SetFont( this->currentFont );
	  }
	}
	
	this->currentPage.SetFontName( this->currentFont.name );
  }

  /*========================================================================================================*/

  void Document::SetFont( const Font& _font )
  {
	if ( !this->IsFontPresent( _font.name ) )
	{
	  OOX::FontTable& fontTable = this->ooxFolder.find<OOX::Document>().find<OOX::FontTable>();

      OOX::FontTable::Font font;

	  font.Name = _font.name;
	  font.Panose1 = ToString( _font.characteristics.panose );
	  font.Pitch = ( ( _font.characteristics.isFixedWidth ) ? ( "fixed" ) : ( "variable" ) );
	  font.Usb0 = ToString( _font.characteristics.signature[0] );
      font.Usb1 = ToString( _font.characteristics.signature[1] );
      font.Usb2 = ToString( _font.characteristics.signature[2] );
      font.Usb3 = ToString( _font.characteristics.signature[3] );
      font.Csb0 = ToString( _font.characteristics.signature[4] );
      font.Csb1 = ToString( _font.characteristics.signature[5] );

	  fontTable.Fonts->push_back( font );
	}

	this->currentPage.SetFontName( _font.name );
  }  

  /*========================================================================================================*/

  void Document::SetFontSize( double _fontSize )
  {
	this->currentPage.SetFontSize( _fontSize );
  }

  /*========================================================================================================*/

  void Document::LoadFontFromFile( const std::wstring& _fileName, double _fontSize, double _xDPI, double _yDPI, long _faceIndex )
  {
	std::wstring styleName;

	this->fontManager.LoadFontFromFile( _fileName, _fontSize, _xDPI, _yDPI, _faceIndex );

	FontCharacteristics fontCharacteristics;
	
	fontCharacteristics.panose = this->fontManager.GetPANOSE();
	fontCharacteristics.isFixedWidth = this->fontManager.IsFixedWidth();
	fontCharacteristics.signature = this->fontManager.GetSignature();

	this->currentFont.characteristics = fontCharacteristics;

	if ( fontCharacteristics.IsDefault() )
	{
	  this->currentFont.name = ToString( this->fontManager.GetFamilyName() );
	}
	else
	{
	  this->currentFont.name = ToString( this->FindFont( fontCharacteristics ) );

	  if ( !this->currentFont.name.empty() )
	  {
	    this->currentPage.SetFontName( this->currentFont.name );
	  }
	  else
	  {
	    this->currentFont.name = ToString( this->fontManager.GetFamilyName() );
	  }
	}

	styleName = this->fontManager.GetStyleName();

	if ( styleName.empty() )
	{
	  styleName = _T( "Regular" );
	}

	if ( ( styleName == _T( "Regular" ) ) || ( styleName == _T( "Medium" ) ) )
	{
	  this->SetBold( false );
      this->SetItalic( false );  
	}
	else if ( styleName == _T( "Bold" ) )
	{
	  this->SetBold( true );
	  this->SetItalic( false );
	}
	else if ( styleName == _T( "Italic" ) )
	{
	  this->SetBold( false );
	  this->SetItalic( true );
	}
	else if ( styleName == _T( "Bold Italic" ) )
	{
	  this->SetBold( true );
      this->SetItalic( true ); 
	}
  }  

  /*========================================================================================================*/

  void Document::SetBold( bool _value )
  {
	this->currentPage.SetBold( _value );
  }

  /*========================================================================================================*/

  void Document::SetItalic( bool _value )
  {
	this->currentPage.SetItalic( _value );
  }

  /*========================================================================================================*/

  void Document::SetUnderline( bool _value )
  {
	this->currentPage.SetUnderline( _value );
  }

  /*========================================================================================================*/

  void Document::SetStrikeout( bool _value )
  {
	this->currentPage.SetStrikeout( _value );
  }

  /*========================================================================================================*/
  
  void Document::Save() const
  {
    if ( !this->ooxFolder.find<OOX::Document>().Items->empty() )
	{
	  if ( this->ooxFolder.find<OOX::Document>().Items->front().is<OOX::Logic::Paragraph>() )
	  {
	    OOX::Logic::Paragraph& paragraph = this->ooxFolder.find<OOX::Document>().Items->front().as<OOX::Logic::Paragraph>();
		  
	    paragraph.Property.init();
	    paragraph.Property->Spacing.init();
	    paragraph.Property->Spacing->Line = 1;
        paragraph.Property->Spacing->LineRule = "exact";
	  }
	}

	this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Top = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Right = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Bottom = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Left = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Header = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Footer = 0;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageMargin->Gutter = 0;

    UniversalUnit mmValue( UniversalUnit::Mm );
    UniversalUnit dxValue( UniversalUnit::Dx );

	mmValue = this->width;
    dxValue = mmValue;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageSize->Width = dxValue;
			
    mmValue = this->height;
    dxValue = mmValue;
    this->ooxFolder.find<OOX::Document>().SectorProperty->PageSize->Height = dxValue;

	if(this->width <= this->height )
		this->ooxFolder.find<OOX::Document>().SectorProperty->PageSize->Orient = "portrait";
	else
		this->ooxFolder.find<OOX::Document>().SectorProperty->PageSize->Orient = "landscape";

	this->previousPage.Build();

	std::vector<OOX::Logic::Paragraph> pageParagraphs = this->previousPage.GetParagraphs();

	for ( std::vector<OOX::Logic::Paragraph>::const_iterator pageParagraphsIter = pageParagraphs.begin(); pageParagraphsIter != pageParagraphs.end(); ++pageParagraphsIter )
    {
	  this->ooxFolder.find<OOX::Document>().Items->push_back( *pageParagraphsIter );
	}

	this->currentPage.Build();

	pageParagraphs = this->currentPage.GetParagraphs();

	for ( std::vector<OOX::Logic::Paragraph>::const_iterator pageParagraphsIter = pageParagraphs.begin(); pageParagraphsIter != pageParagraphs.end(); ++pageParagraphsIter )
    {
	  this->ooxFolder.find<OOX::Document>().Items->push_back( *pageParagraphsIter );
	}
	  
	this->ooxFolder.Finalize( this->destinationPath );
  }

  /*========================================================================================================*/
  
  bool Document::IsFontPresent( const std::string& _fontName ) const
  {
    bool result = false;
	  
	OOX::FontTable& fontTable = this->ooxFolder.find<OOX::Document>().find<OOX::FontTable>();
	  
	BOOST_FOREACH( const OOX::FontTable::Font& font, *fontTable.Fonts )
	{
	  if ( font.Name == _fontName )
	  {
	    result = true;

		break;
	  }
	}

	return result;
  }

  /*========================================================================================================*/
  
  std::wstring Document::FindFont( const FontCharacteristics& _fontCharacteristics ) const
  {
	std::wstring fontName;

	OOX::FontTable& fontTable = this->ooxFolder.find<OOX::Document>().find<OOX::FontTable>();

	BOOST_FOREACH( const OOX::FontTable::Font& font, *fontTable.Fonts )
	{
	  FontCharacteristics fontCharacteristics;

	  if ( font.Panose1.is_init() )
	  {
	    fontCharacteristics.panose = ToWString( *font.Panose1 );
	  }

	  if ( font.Pitch == "fixed" )
	  {
	    fontCharacteristics.isFixedWidth = true;
	  }
	  else if ( font.Pitch == "variable" )
	  {
	    fontCharacteristics.isFixedWidth = false;
	  }

      if ( font.Usb0.is_init() )
	  {
	    fontCharacteristics.signature[0] = ToWString( *font.Usb0 );
	  }

	  if ( font.Usb1.is_init() )
	  {
	    fontCharacteristics.signature[1] = ToWString( *font.Usb1 );
	  }

	  if ( font.Usb2.is_init() )
	  {
	    fontCharacteristics.signature[2] = ToWString( *font.Usb2 );
	  }

	  if ( font.Usb3.is_init() )
	  {
	    fontCharacteristics.signature[3] = ToWString( *font.Usb3 );
	  }

	  if ( font.Csb0.is_init() )
	  {
	    fontCharacteristics.signature[4] = ToWString( *font.Csb0 );
	  }

	  if ( font.Csb1.is_init() )
	  {
	    fontCharacteristics.signature[5] = ToWString( *font.Csb1 );
	  }

	  if ( fontCharacteristics == _fontCharacteristics )
	  {
	    fontName = ToWString( *font.Name );

		break;
	  }
	}

	return fontName;
  }
}