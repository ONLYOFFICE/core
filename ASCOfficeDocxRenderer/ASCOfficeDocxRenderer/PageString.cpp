#include "stdafx.h"

#include "PageString.h"

namespace DocxRenderer
{
  PageString::PageString():
  xStart(0), y(0), height(0), fontManager(NULL)
  {
  }

  /*========================================================================================================*/

  PageString::PageString( double _y, FontManager* _fontManager ):
  xStart(0), y(0), height(0), fontManager(_fontManager)
  {
    if ( _y > 0 )
	{
	  this->y = _y;
	}
  }

  /*========================================================================================================*/

  PageString::PageString( const std::wstring& _text, const std::wstring& _gidText, const OOX::Logic::RunProperty& _properties, double _x, double _y, FontManager* _fontManager ):
  xStart(0), y(0), height(0), fontManager(_fontManager)
  {
    if ( _y > 0 )
	{
	  this->y = _y;
	}

    this->AddText( _text, _properties, _x, _gidText );
  }

  /*========================================================================================================*/

  long PageString::GetFontStyle( const OOX::Logic::RunProperty& _runProperty ) const
  {
    long fontStyle = 0;

	if ( _runProperty.Bold.get_value_or_default() )
	{
	  fontStyle |= 1;
	}
	
    if ( _runProperty.Italic.get_value_or_default() )
	{
	  fontStyle |= 2;
	}

	if ( _runProperty.Under.get_value_or_default() )
	{
	  fontStyle |= 4;
	}

	if ( _runProperty.Strike.get_value_or_default() )
	{
	  fontStyle |= 8;
	}

	return fontStyle;
  }

  /*========================================================================================================*/

  bool operator == ( const OOX::Logic::RunProperty& _runProperty1, const OOX::Logic::RunProperty& _runProperty2 )
  {
    bool result = true;
	  
	if ( ( _runProperty1.FontColor.is_init() && !_runProperty2.FontColor.is_init() ) ||
		 ( !_runProperty1.FontColor.is_init() && _runProperty2.FontColor.is_init() ) ||
	     ( _runProperty1.FontColor.is_init() && _runProperty2.FontColor.is_init() &&
		 ( ( _runProperty1.FontColor->Blue != _runProperty2.FontColor->Blue ) ||
	     ( _runProperty1.FontColor->Green != _runProperty2.FontColor->Green ) ||
		 ( _runProperty1.FontColor->Red != _runProperty2.FontColor->Red ) ) ) )
	{
	  result = false;    
	}

	if ( ( _runProperty1.RFonts.is_init() && !_runProperty2.RFonts.is_init() ) ||
		 ( !_runProperty1.RFonts.is_init() && _runProperty2.RFonts.is_init() ) ||
	     ( _runProperty1.RFonts.is_init() && _runProperty2.RFonts.is_init() &&
		 ( _runProperty1.RFonts->Ascii != _runProperty2.RFonts->Ascii ) ) )
	{
	  result = false;
	}

	if ( ( _runProperty1.FontSize.is_init() && !_runProperty2.FontSize.is_init() ) ||
		 ( !_runProperty1.FontSize.is_init() && _runProperty2.FontSize.is_init() ) ||
	     ( _runProperty1.FontSize.is_init() && _runProperty2.FontSize.is_init() &&
		 ( _runProperty1.FontSize != _runProperty2.FontSize ) ) )
	{
	  result = false;
	}

	if ( ( _runProperty1.Bold.is_init() && !_runProperty2.Bold.is_init() ) ||
		 ( !_runProperty1.Bold.is_init() && _runProperty2.Bold.is_init() ) ||
	     ( _runProperty1.Bold.is_init() && _runProperty2.Bold.is_init() &&
		 ( _runProperty1.Bold != _runProperty2.Bold ) ) )
	{
	  result = false;
	}

	if ( ( _runProperty1.Italic.is_init() && !_runProperty2.Italic.is_init() ) ||
		 ( !_runProperty1.Italic.is_init() && _runProperty2.Italic.is_init() ) ||
	     ( _runProperty1.Italic.is_init() && _runProperty2.Italic.is_init() &&
		 ( _runProperty1.Italic != _runProperty2.Italic ) ) )
	{
	  result = false;
	}

	if ( ( _runProperty1.Strike.is_init() && !_runProperty2.Strike.is_init() ) ||
		 ( !_runProperty1.Strike.is_init() && _runProperty2.Strike.is_init() ) ||
	     ( _runProperty1.Strike.is_init() && _runProperty2.Strike.is_init() &&
		 ( _runProperty1.Strike != _runProperty2.Strike ) ) )
	{
	  result = false;
	}

	if ( ( _runProperty1.Under.is_init() && !_runProperty2.Under.is_init() ) ||
		 ( !_runProperty1.Under.is_init() && _runProperty2.Under.is_init() ) ||
	     ( _runProperty1.Under.is_init() && _runProperty2.Under.is_init() &&
		 ( _runProperty1.Under != _runProperty2.Under ) ) )
	{
	  result = false;
	}

	return result;
  }

  /*========================================================================================================*/

  double PageString::GetMinSymbolWidth() const
  {
	double minSymbolWidth = DBL_MAX;
	  
	for ( std::map<double, Element>::const_iterator iter = this->elements.begin(); iter != this->elements.end(); ++iter )
	{
	  if ( iter->second.width < minSymbolWidth )
	  {
	    minSymbolWidth = iter->second.width;
	  }
	}

	return minSymbolWidth;
  }

  /*========================================================================================================*/

  double PageString::GetMaxSymbolWidth() const
  {
    double maxSymbolWidth = 0;

	for ( std::map<double, Element>::const_iterator iter = this->elements.begin(); iter != this->elements.end(); ++iter )
	{
	  if ( iter->second.width > maxSymbolWidth )
	  {
	    maxSymbolWidth = iter->second.width;
	  }
	}

	return maxSymbolWidth;
  }

  /*========================================================================================================*/

  double PageString::GetXStart() const
  {
    return this->xStart;
  }

  /*========================================================================================================*/

  double PageString::GetY() const
  {
    return this->y;
  }

  /*========================================================================================================*/

  double PageString::GetWidth() const
  {
    double width = 0;
	
	for ( std::map<double, Element>::const_iterator iter = this->elements.begin(); iter != this->elements.end(); ++iter )
	{
	  double xOffset = ( iter->first - this->xStart - width );

	  width += iter->second.width;
	  width += xOffset;
	}

    return width;
  }

  /*========================================================================================================*/

  double PageString::GetHeight() const
  {
    return this->height;
  }

  /*========================================================================================================*/

  bool PageString::IsEmpty() const
  {
    return this->elements.empty();
  }

  /*========================================================================================================*/

  OOX::Logic::Paragraph PageString::GetParagraph() const
  {
    OOX::Logic::Paragraph paragraph;

	double width = 0;

	UniversalUnit mmValue( UniversalUnit::Mm );
    UniversalUnit dxValue( UniversalUnit::Dx );

    std::map<double, Element>::const_iterator iter = this->elements.begin();

	if ( iter != this->elements.end() )
	{
	  OOX::Logic::Run run;
	  std::string runText;
		
	  runText += iter->second.text;
	  run.Property = iter->second.properties;

	  double xOffset = ( iter->first - this->xStart - width );
	  double gidXOffset = 0;
	  
	  width += iter->second.width;
	  width += xOffset;

	  double maxSymbolWidth = 0;
	  double gidXOffsetPrecision = 0;
	  
	  if ( !iter->second.gidText.empty() )
	  {
	    maxSymbolWidth = this->GetMaxSymbolWidth();
	    gidXOffsetPrecision = this->GetMinSymbolWidth();
	  }

	  std::map<double, Element>::const_iterator prevIter = iter++;
	  
	  for ( ; iter != this->elements.end(); ++iter )
	  {
        double spaceWidth = 0;
		double totalWidth = 0;
		double pairWidth = 0;

		if ( iter->second.gidText.empty() )
		{
          spaceWidth = this->fontManager->GetTextWidth( _T( " " ), ::ToWString( *iter->second.properties.RFonts->HAnsi ), ( *iter->second.properties.FontSize / 2 ), this->GetFontStyle( iter->second.properties ), FontManager::MeasureTypePosition );
		}
		else
		{
		  totalWidth = ( iter->first - prevIter->first + iter->second.width );
		  pairWidth = ( iter->second.width + prevIter->second.width );
		}

		xOffset = ( iter->first - this->xStart - width );

	    width += iter->second.width;
	    width += xOffset;

		gidXOffset = ( totalWidth - pairWidth );
        		
		if ( ( ( xOffset > spaceWidth ) && ( xOffset < ( 5 * spaceWidth ) ) && iter->second.gidText.empty() ) ||
		     ( ( gidXOffset > gidXOffsetPrecision ) && ( gidXOffset < maxSymbolWidth ) && !iter->second.gidText.empty() ) && ( prevIter->second.text != " " ) )
	    {
          OOX::Logic::Run spaceRun;
		  
		  run.add( new OOX::Logic::Text( runText ) );
		  paragraph.Add( run );

		  runText.clear();
		  run.clear();
		  run.Property.reset();
		  
		  spaceRun.add( new OOX::Logic::Text( ToString( " " ) ) );

		  spaceRun.Property = iter->second.properties;

		  paragraph.Add( spaceRun );

		  runText += iter->second.text;
		  run.Property =  iter->second.properties;
		}
	    else if ( ( ( xOffset >= ( 5 * spaceWidth ) ) && iter->second.gidText.empty() ) ||
			      ( ( gidXOffset >= maxSymbolWidth ) && !iter->second.gidText.empty() ) )
	    {
	      OOX::Logic::Run spaceRun;

		  run.add( new OOX::Logic::Text( runText ) );
		  paragraph.Add( run );

		  runText.clear();
		  run.clear();
		  run.Property.reset();

		  spaceRun.add( new OOX::Logic::Text( ToString( " " ) ) );
	  	  
	 	  spaceRun.Property = iter->second.properties;

		  if ( iter->second.gidText.empty() )
		  {
		    mmValue = ( xOffset - spaceWidth );
		  }
		  else
		  {
		    mmValue = ( iter->first - prevIter->first - prevIter->second.width );
		  }

	      dxValue = mmValue;
		  
		  spaceRun.Property->RFonts = iter->second.properties.RFonts;
		  spaceRun.Property->FontSize = iter->second.properties.FontSize;
		  spaceRun.Property->Spacing = dxValue;

		  paragraph.Add( spaceRun );

		  runText += iter->second.text;
		  run.Property =  iter->second.properties;
	    }
		else
		{
		  if ( *run.Property == iter->second.properties )
	      {
		    runText += iter->second.text;
	      }
	      else
	      {
	        run.add( new OOX::Logic::Text( runText ) );
			paragraph.Add( run );

		    runText.clear();
			run.clear();
		    run.Property.reset();

		    runText += iter->second.text;
		    run.Property =  iter->second.properties;
	      }
		}

		prevIter = iter;
	  }

	  if ( !runText.empty() )
	  {
	    run.add( new OOX::Logic::Text( runText ) );
		
		paragraph.Add( run );
	  }
	}

	paragraph.Property = this->paragraphProperties;

	return paragraph;
  }

  /*========================================================================================================*/

  void PageString::SetParagraphProperties( const OOX::Logic::ParagraphProperty& _paragraphProperties )
  {
    this->paragraphProperties = _paragraphProperties;
  }

  /*========================================================================================================*/

  void PageString::SetFontManager( FontManager* _fontManager )
  {
    this->fontManager = _fontManager;
  }

  /*========================================================================================================*/

  void PageString::AddText( const std::wstring& _text, const OOX::Logic::RunProperty& _properties, double _x, const std::wstring& _gidText )
  {
	if ( ( _x > 0 ) && ( ( this->xStart == 0 ) || ( _x < this->xStart ) ) )
	{
	  this->xStart = _x;
	}

	long fontStyle = this->GetFontStyle( _properties );

	double textWidth = 0;
    double textHeight = 0;

	if ( _gidText.empty() )
	{
	  textWidth = this->fontManager->GetTextWidth( _text, ::ToWString( *_properties.RFonts->HAnsi ), ( *_properties.FontSize / 2 ), fontStyle, FontManager::MeasureTypePosition );
      textHeight = this->fontManager->GetFontHeight( ::ToWString( *_properties.RFonts->HAnsi ), ( *_properties.FontSize / 2 ), fontStyle );
	}
	else
	{
	  textWidth = this->fontManager->GetTextWidth( _gidText, FontManager::MeasureTypePosition, true );
      textHeight = this->fontManager->GetFontHeight( *_properties.FontSize / 2 );
	}

	if ( textHeight > this->height )
	{
	  this->height = textHeight;
	}

	this->elements.insert( std::make_pair( _x, Element( ToString( _text ), _properties, textWidth, _gidText ) ) );
  }

  /*========================================================================================================*/

  void PageString::Merge( const PageString& _pageString )
  {
    if ( !_pageString.IsEmpty() )
	{
	  UniversalUnit subPageStringPositionMm( UniversalUnit::Mm );
      UniversalUnit subPageStringPositionPt( UniversalUnit::Pt );

      subPageStringPositionMm = ( this->y - _pageString.y );
      subPageStringPositionPt = subPageStringPositionMm;
	  
	  if ( _pageString.xStart < this->xStart )
	  {
        this->xStart = _pageString.xStart;
	  }

	  if ( _pageString.y <= this->y )
	  {
	    this->height = ( this->y - _pageString.y + _pageString.height );
	  }
	  else
	  {
	    this->height = ( _pageString.y - this->y + this->height );
	  }

	  for ( std::map<double, Element>::const_iterator iter = _pageString.elements.begin(); iter != _pageString.elements.end(); ++iter )
	  {
	    OOX::Logic::RunProperty properties = iter->second.properties;
		
		double position = subPageStringPositionPt;
		properties.Position = position;

		Element newElement( iter->second.text, properties, iter->second.width, iter->second.gidText ); 
		
		this->elements.insert( std::make_pair( iter->first, newElement ) );
	  }
	}
  }

  /*========================================================================================================*/

  bool PageString::LessYPosition( const PageString& _pageString1, const PageString& _pageStrig2 )
  {
    return ( ( _pageString1.GetY() < _pageStrig2.GetY() ) ? true : false );
  }

  /*========================================================================================================*/

  void PageString::Merge( std::list<PageString>& _pageStrings, double _affinity )
  {
    if ( !_pageStrings.empty() )
	{
	  std::list<PageString>::iterator currentIter = _pageStrings.begin();
      std::list<PageString>::iterator nextIter = ++_pageStrings.begin();

	  while ( nextIter != _pageStrings.end() )
	  {
	    if ( fabs( currentIter->GetY() - nextIter->GetY() ) < _affinity )
	    {
	      currentIter->Merge( *nextIter );

		  nextIter = _pageStrings.erase( nextIter );

		  continue;
	    }

	    currentIter = nextIter;
	    nextIter++;
	  }
	}
  }
}