#include "stdafx.h"

#include "Page.h"

namespace DocxRenderer
{
  const double Page::STANDART_STRING_HEIGHT_MM = 4.2333333333333334;

  const double Page::THE_SAME_STRING_Y_PRECISION_MM = 0.01;
	
  OOX::Logic::RunProperty Page::GetRunProperties() const
  {
    OOX::Logic::RunProperty runProperty;

	if ( !this->fontColor.empty() )
	{
	  runProperty.FontColor = this->fontColor;
	}

	OOX::Logic::RFonts fonts;
	
	if ( !this->fontName.empty() )
	{
	  fonts.Ascii = fonts.Cs = fonts.HAnsi = this->fontName;
	}
	else
	{
	  fonts.Ascii = fonts.Cs = fonts.HAnsi = ::ToString( this->fontManager->GetDefaultFont() );
	}

	runProperty.RFonts = fonts;

	if ( this->fontSize != 0 )
	{
	  runProperty.FontSize = ( 2 * this->fontSize );

	  //!!!TODO: Scaling!!!
	  if ( *runProperty.FontSize <= 2 )
	  {
	    runProperty.FontSize = ( 2 * this->matrix.GetD() );
	  }
	}
	if( true == this->bold )
		runProperty.Bold = this->bold;
	if( true == this->italic )
		runProperty.Italic = this->italic;
	if( true == this->strikeout )
		runProperty.Strike = this->strikeout;

	if ( this->underline )
	{
	  runProperty.Under = true;
	  runProperty.UnderType = "single";
	}
	//else
	//{
	//  runProperty.Under = false;
	//  runProperty.UnderType = "none";
	//}

	return runProperty;
  }

  /*========================================================================================================*/

  PageString* Page::GetPageString( double _y ) const
  {
    PageString* pageString = NULL;

	for ( std::vector<PageString>::iterator iter = this->strings.begin(); iter != this->strings.end(); ++iter )
	{
	  if ( fabs( iter->GetY() - _y ) <= THE_SAME_STRING_Y_PRECISION_MM )
	  {
	    pageString = &(*iter);

		break;
	  }
	}

	return pageString;
  }

  /*========================================================================================================*/

  Page::Page():
  hyperlinkMode(false), clipMode(false), width(0), height(0), textAssociationType(TextAssociationTypeDefault), ooxFolder(NULL), fontManager(NULL),
  bold(false), italic(false), underline(false), strikeout(false), dAngle(0), dLeft(0), dTop(0), dWidth(0),
  dHeight(0), lFlags(0), fontSize(0), penSize(0)
  {
    this->drawingParagraph.Property.init();
	this->drawingParagraph.Property->Spacing.init();
	this->drawingParagraph.Property->Spacing->Line = 1;
    this->drawingParagraph.Property->Spacing->LineRule = "exact";
  }

  /*========================================================================================================*/

  Page::Page( Docx::Folder* _ooxFolder, FontManager* _fontManager, TextAssociationType _textAssociationType, double _width, double _height ):
  hyperlinkMode(false), clipMode(false), width(_width), height(_height), textAssociationType(_textAssociationType), ooxFolder(_ooxFolder), fontManager(_fontManager),
  bold(false), italic(false), underline(false), strikeout(false), dAngle(0), dLeft(0), dTop(0), dWidth(0),
  dHeight(0), lFlags(0), fontSize(0), penSize(0)
  {
    this->drawingParagraph.Property.init();
	this->drawingParagraph.Property->Spacing.init();
	this->drawingParagraph.Property->Spacing->Line = 1;
    this->drawingParagraph.Property->Spacing->LineRule = "exact";
  }

  /*========================================================================================================*/

  void Page::SetWidth( double _width )
  {
    this->width = _width;
  }

  /*========================================================================================================*/

  double Page::GetWidth() const
  {
    return this->width;
  }

  /*========================================================================================================*/

  void Page::SetGraphicsWidth( double _width )
  {
    this->vmlGraphicsPath.SetWidth( _width );	      
  }

  /*========================================================================================================*/

  void Page::SetHeight( double _height )
  {
    this->height = _height;

	this->vmlGraphicsPath.SetHeight( this->height );
  }

  /*========================================================================================================*/

  double Page::GetHeight() const
  {
    return this->height;
  }

  /*========================================================================================================*/

  void Page::SetGraphicsHeight( double _height )
  {
    this->vmlGraphicsPath.SetHeight( _height );	      
  }

  /*========================================================================================================*/

  void Page::SetTextAssociationType( TextAssociationType _textAssociationType )
  {
    this->textAssociationType = _textAssociationType;
  }

  /*========================================================================================================*/

  TextAssociationType Page::GetTextAssociationType() const
  {
    return this->textAssociationType;
  }

  /*========================================================================================================*/

  void Page::BeginCommand( unsigned long _lType )
  {
    //!!!TODO!!!
	switch ( _lType )
	{
	  case c_nHyperlinkType:
	  {
	    this->hyperlinkMode = true;
	  }
	  break;

	  case c_nClipType:
	  {
	    this->clipMode = true;
	  }
	  break;

	  case c_nResetClipType:
	  {
	    this->clipMode = false;
	  }
	  break;
	}
  }

  /*========================================================================================================*/

  void Page::EndCommand( unsigned long _lType )
  {
    //!!!TODO!!!
	switch ( _lType )
	{
	  case c_nHyperlinkType:
	  {
	    this->hyperlinkMode = false;
	  }
	  break;

	  case c_nClipType:
	  {
	    this->clipMode = false;

		this->PathEnd();
	  }
	  break;

	  case c_nResetClipType:
	  {
	    this->clipMode = false;
	  }
	  break;
	}
  }

  /*========================================================================================================*/

  void Page::SetOOXFolder( Docx::Folder* _ooxFolder )
  {
    this->ooxFolder = _ooxFolder;
  }

  /*========================================================================================================*/

  void Page::SetFontManager( FontManager* _fontManager )
  {
    this->fontManager = _fontManager;
  }

  /*========================================================================================================*/

  void Page::AddText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset, double _xRestriction, double _yRestriction, const std::wstring& _gidText )
  {
    if ( !this->hyperlinkMode )
	{
	  double x = _dX;
      double y = _dY;

	  this->matrix.AffineTransform( x, y );

	  //!!!TODO: Sometimes x and/or y comes too large from some PDF-files or less than 0.!!!
	  if ( ( x >= 0 ) && ( y >= 0 ) && ( ( ( _xRestriction == 0 ) || ( ( _xRestriction != 0 ) && ( x < _xRestriction ) ) ) ||
		                                 ( ( _yRestriction == 0 ) || ( ( _yRestriction != 0 ) && ( y < _yRestriction ) ) ) ) )
	  {
	    OOX::Logic::RunProperty runProperty = this->GetRunProperties();

	    switch ( this->textAssociationType )
	    {
	      case TextAssociationTypeDefault:
	      {
            this->strings.push_back( PageString( _text, _gidText, runProperty, x, y, this->fontManager ) );
	      }
	      break;

	      case TextAssociationTypeLine:
	      {
	        PageString* pageString = this->GetPageString( y );
		
		    if ( pageString != NULL )
		    {
		      pageString->AddText( _text, runProperty, x, _gidText );
		    }
		    else
		    {
		      this->strings.push_back( PageString( _text, _gidText, runProperty, x, y, this->fontManager ) );
	  	    }
	      }
	      break;

	      case TextAssociationTypeBlock:
	      {
		    PageString* pageString = this->GetPageString( y );
	    
		    if ( pageString != NULL )
		    {
		      pageString->AddText( _text, runProperty, x, _gidText );
		    }
		    else
		    {
		      this->strings.push_back( PageString( _text, _gidText, runProperty, x, y, this->fontManager ) );
		    }
	      }
	      break;

	      case TextAssociationTypeNoFrames:
	      {
		    PageString* pageString = this->GetPageString( y );
	    
		    if ( pageString != NULL )
		    {
		      pageString->AddText( _text, runProperty, x, _gidText );
		    }
		    else
		    {
		      this->strings.push_back( PageString( _text, _gidText, runProperty, x, y, this->fontManager ) );
		    }
	      }
	      break;
	    }
	  }
	}
  }

  /*========================================================================================================*/

  void Page::AddImage( const std::wstring& _fileName, double _dX, double _dY, double _dWidth, double _dHeight )
  {
    UniversalUnit mmX( UniversalUnit::Mm );
    UniversalUnit mmY( UniversalUnit::Mm );
    UniversalUnit mmWidth( UniversalUnit::Mm );
    UniversalUnit mmHeight( UniversalUnit::Mm );

    double x = _dX;
    double y = _dY;
    double width = _dWidth;
    double height = _dHeight;

	this->matrix.AffineTransform( x, y );
	this->matrix.LinearTransform( width, height );

	mmX = x;
	mmY = y;
	mmWidth = std::abs( width );
	mmHeight = std::abs( height );

	OOX::Image* docxImage = new OOX::Image( _fileName );

	if ( docxImage != NULL )
	{
	  const OOX::RId rId = this->ooxFolder->find<OOX::Document>().add( boost::shared_ptr<OOX::File>( docxImage ) );
	  OOX::Logic::Drawing* drawing = new OOX::Logic::Drawing( rId, _fileName, mmX.value( UniversalUnit::Multi ), "page", mmY.value( UniversalUnit::Multi ), "page", mmWidth.value( UniversalUnit::Multi ), mmHeight.value( UniversalUnit::Multi ) );
	
	  if ( drawing != NULL )
	  {
	    drawing->Inline->BehindDoc = true;
		drawing->Inline->Wrap.init();
		drawing->Inline->Wrap->Type = "wrapNone";

	    OOX::Logic::Run run;
	    run.add( drawing );

		this->drawingParagraph.Add( run );
	  }
	}
  }

  /*========================================================================================================*/

  void Page::AddShape( const OOX::Logic::Shape& _shape )
  {
    OOX::Logic::Pict* pict = new OOX::Logic::Pict();
    pict->Name = "pict";
    pict->Shape = _shape;

    OOX::Logic::Run run;
    run.add( pict );

	this->drawingParagraph.Add( run );
  }

  /*========================================================================================================*/

  void Page::PathMoveTo( double _dX, double _dY )
  {
    this->vmlGraphicsPath.MoveTo( _dX, _dY );
  }

  /*========================================================================================================*/

  void Page::PathLineTo( double _dX, double _dY )
  {
    this->vmlGraphicsPath.LineTo( _dX, _dY );
  }

  /*========================================================================================================*/

  void Page::PathLinesTo( const std::vector<double>& _points )
  {
    this->vmlGraphicsPath.LinesTo( _points );
  }

  /*========================================================================================================*/

  void Page::PathCurveTo( double _x1, double _y1, double _x2, double _y2, double _x3, double _y3 )
  {
    this->vmlGraphicsPath.CurveTo( _x1, _y1, _x2, _y2, _x3, _y3 );
  }

  /*========================================================================================================*/

  void Page::PathCurvesTo( const std::vector<double>& _points )
  {
    this->vmlGraphicsPath.CurvesTo( _points );
  }

  /*========================================================================================================*/

  void Page::PathArcTo( double _x, double _y, double _width, double _height, double _startAngle, double _sweepAngle )
  {
    this->vmlGraphicsPath.ArcTo( _x, _y, _width, _height, _startAngle, _sweepAngle );
  }

  /*========================================================================================================*/

  void Page::PathClose()
  {
    this->vmlGraphicsPath.Close();
  }

  /*========================================================================================================*/

  void Page::PathEnd()
  {
    this->vmlGraphicsPath.Reset();
  }

  /*========================================================================================================*/

  void Page::DrawPath( long _nType, unsigned int _resize )
  {
    bool stroked = ( 0x01 == ( _nType & 0x01 ) );
	bool filled = ( ( ( _nType & 0x300 ) != 0 ) ? true : false );
	  
	this->AddShape( this->vmlGraphicsPath.GetShape( stroked, filled, _resize ) );
  }

  /*========================================================================================================*/

  void Page::PathStart()
  {
    //!!!TODO!!!
  }

  /*========================================================================================================*/

  void Page::PathGetCurrentPoint( double* _dX, double* _dY )
  {
    if ( ( _dX != NULL ) && ( _dY != NULL ) )
    {
      std::vector<std::pair<double, double> > pathPoints = this->vmlGraphicsPath.GetPathPoints();

      if ( !pathPoints.empty() )
      {
	    *_dX = pathPoints.back().first;
	    *_dY = pathPoints.back().second;
      }
    }
  }

  /*========================================================================================================*/

  void Page::GetCommandParams( double* _dAngle, double* _dLeft, double* _dTop, double* _dWidth, double* _dHeight, unsigned long* _lFlags )
  {
    if ( NULL != _dAngle ) 
    {
      *_dAngle = this->dAngle;
    }
		
    if ( NULL != _dLeft )
    {
      *_dLeft = this->dLeft;
    }

    if ( NULL != _dTop )
    {
      *_dTop = this->dTop;
    }

    if ( NULL != _dWidth )
    {
      *_dWidth = this->dWidth;
    }

    if ( NULL != _dHeight )
    {
      *_dHeight = this->dHeight;
    }

    if ( NULL != _lFlags )
    {
      *_lFlags = this->lFlags;
    }
  }

  /*========================================================================================================*/

  void Page::SetCommandParams( double _dAngle, double _dLeft, double _dTop, double _dWidth, double _dHeight, unsigned long _lFlags )
  {
    this->dAngle = _dAngle;
    this->dLeft = _dLeft;
    this->dTop = _dTop;
    this->dWidth = _dWidth;
    this->dHeight = _dHeight;

    this->lFlags = _lFlags;

    if ( ( _dWidth <= 1 ) || ( _dHeight <= 1 ) )
    {
      _lFlags = 0;
    }

    bool bFlipX = ( 0 != ( c_nParamFlipX & _lFlags ) );
    bool bFlipY = ( 0 != ( c_nParamFlipY & _lFlags ) );
		
    double m11 = ( bFlipX ? -1.0f : 1.0f );
    double m22 = ( bFlipY ? -1.0f : 1.0f );
		
    DocxRenderer::TransformationMatrix matrix( 1, 0, 0, 1, 0, 0 );

    if ( ( 0 != this->dAngle ) || ( 0 != _lFlags ) )
    {
      double dCentreX = ( _dLeft + ( _dWidth / 2.0 ) );
	  double dCentreY = ( _dTop + ( _dHeight / 2.0 ) );
			
	  matrix.Translate( -dCentreX, -dCentreY, DocxRenderer::TransformationMatrix::MatrixOrderAppend );
		
	  matrix.Rotate( this->dAngle, DocxRenderer::TransformationMatrix::MatrixOrderAppend );
	  matrix.Scale( m11, m22, DocxRenderer::TransformationMatrix::MatrixOrderAppend );
			
	  matrix.Translate( dCentreX, dCentreY, DocxRenderer::TransformationMatrix::MatrixOrderAppend );
    }

    this->SetTransform( matrix );
  }

  /*========================================================================================================*/

  void Page::SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF )
  {
    this->matrix.SetTransform( _dA, _dB, _dC, _dD, _dE, _dF );

	this->vmlGraphicsPath.SetTransform( _dA, _dB, _dC, _dD, _dE, _dF );
  }

  /*========================================================================================================*/

  void Page::SetTransform( const TransformationMatrix& _matrix )
  {
    double dA = 0;
    double dB = 0;
    double dC = 0;
    double dD = 0;
    double dE = 0;
    double dF = 0;

    _matrix.GetTransform( &dA, &dB, &dC, &dD, &dE, &dF );

    this->SetTransform( dA, dB, dC, dD, dE, dF );
  }

  /*========================================================================================================*/

  void Page::GetTransform( double* _pdA, double* _pdB, double* _pdC, double* _pdD, double* _pdE, double* _pdF )
  {
    this->matrix.GetTransform( _pdA, _pdB, _pdC, _pdD, _pdE, _pdF );
  }

  /*========================================================================================================*/

  void Page::ResetTransform()
  {
    this->matrix.ResetTransform();

	this->vmlGraphicsPath.ResetTransform();
  }

  /*========================================================================================================*/

  long Page::GetClipMode()
  {
    //!!!TODO!!!
	return 0;
  }

  /*========================================================================================================*/

  void Page::SetClipMode( long _lMode )
  {
    //!!!TODO!!!
  }

  /*========================================================================================================*/

  void Page::PathText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset )
  {
    //!!!TODO!!!
  }

  /*========================================================================================================*/

  void Page::SetPenColor( const std::string& _penColor )
  {
    this->penColor = _penColor;

	this->vmlGraphicsPath.SetPenColor( this->penColor );
  }

  /*========================================================================================================*/

  void Page::SetPenSize( double _penSize )
  {
    this->penSize = _penSize;

	UniversalUnit penSizeMm( UniversalUnit::Mm );
	UniversalUnit penSizePt( UniversalUnit::Pt );
  
	penSizeMm = this->penSize;
    penSizePt = penSizeMm;

	this->vmlGraphicsPath.SetPenSize( penSizePt.ToString() );
  }

  /*========================================================================================================*/

  void Page::SetFontColor( const std::string& _fontColor )
  {
    this->fontColor = _fontColor;
  }

  /*========================================================================================================*/

  void Page::SetBrushColor( const std::string& _brushColor )
  {
    this->brushColor = _brushColor;

    this->vmlGraphicsPath.SetBrushColor( this->brushColor );
  }

  /*========================================================================================================*/

  void Page::SetFontName( const std::string& _fontName )
  {
    this->fontName = _fontName;
  } 

  /*========================================================================================================*/

  void Page::SetFontSize( double _fontSize )
  {
    this->fontSize = _fontSize;
  }

  /*========================================================================================================*/

  void Page::SetBold( bool _value )
  {
    this->bold = _value;
  }

  /*========================================================================================================*/

  void Page::SetItalic( bool _value )
  {
    this->italic = _value;
  }

  /*========================================================================================================*/

  void Page::SetUnderline( bool _value )
  {
    this->underline = _value;
  }

  /*========================================================================================================*/

  void Page::SetStrikeout( bool _value )
  {
    this->strikeout = _value;
  }

  /*========================================================================================================*/

  void Page::Build() const
  {
    switch ( this->textAssociationType )
	{
	  case TextAssociationTypeDefault:
	  {
	    for ( std::vector<PageString>::iterator iter = this->strings.begin(); iter != this->strings.end(); ++iter )
		{
		  OOX::Logic::ParagraphProperty paragraphProperty;
			  
		  paragraphProperty.TextFrameProperties = OOXUtils::GetTextFrameProperties( iter->GetXStart(), ( iter->GetY() - iter->GetHeight() ) );

		  iter->SetParagraphProperties( paragraphProperty );
		}
	  }
	  break;

	  case TextAssociationTypeLine:
	  {
		for ( std::vector<PageString>::iterator iter = this->strings.begin(); iter != this->strings.end(); ++iter )
	    {
		  OOX::Logic::ParagraphProperty paragraphProperty;
			  
		  paragraphProperty.TextFrameProperties = OOXUtils::GetTextFrameProperties( iter->GetXStart(), ( iter->GetY() - iter->GetHeight() ) );

		  iter->SetParagraphProperties( paragraphProperty );
		}
	  }
	  break;

	  case TextAssociationTypeBlock:
	  {
		//!!!TODO!!!
	    if ( !this->strings.empty() )
		{
		  std::vector<PageString>::iterator currentPageStringIter = this->strings.begin();
		  std::vector<PageString>::iterator nextPageStringIter = ++this->strings.begin();

		  OOX::Logic::ParagraphProperty paragraphProperty;

		  paragraphProperty.TextFrameProperties = OOXUtils::GetTextFrameProperties( currentPageStringIter->GetXStart(), ( currentPageStringIter->GetY() - currentPageStringIter->GetHeight() ) );

		  currentPageStringIter->SetParagraphProperties( paragraphProperty );
		  
		  for ( ; nextPageStringIter != this->strings.end(); ++nextPageStringIter )
		  {
			if ( ( ( fabs( nextPageStringIter->GetY() - nextPageStringIter->GetHeight() - currentPageStringIter->GetY() ) > STANDART_STRING_HEIGHT_MM ) &&
			       ( nextPageStringIter->GetXStart() == currentPageStringIter->GetXStart() ) ) ||
				 ( ( nextPageStringIter->GetXStart() != currentPageStringIter->GetXStart() ) && 
				   ( nextPageStringIter->GetY() != currentPageStringIter->GetY() ) ) )
		    {
				paragraphProperty.TextFrameProperties = OOXUtils::GetTextFrameProperties( nextPageStringIter->GetXStart(), ( nextPageStringIter->GetY() - nextPageStringIter->GetHeight() ) );
		    }
			  
			nextPageStringIter->SetParagraphProperties( paragraphProperty );

			currentPageStringIter = nextPageStringIter;
		  }
		}
	  }
	  break;

	  case TextAssociationTypeNoFrames:
	  {
		std::vector<PageString> sortedStrings( this->strings.begin(), this->strings.end() );
		
		std::sort( sortedStrings.begin(), sortedStrings.end(), PageString::LessYPosition );

		this->sortedStrings.assign( sortedStrings.begin(), sortedStrings.end() );

		sortedStrings.clear();

	    PageString::Merge( this->sortedStrings, ( STANDART_STRING_HEIGHT_MM / 3 ) ); 

		double previousStringOffset = 0;

	    UniversalUnit mmValue( UniversalUnit::Mm );
        UniversalUnit dxValue( UniversalUnit::Dx );

		for ( std::list<PageString>::iterator iter = this->sortedStrings.begin(); iter != this->sortedStrings.end(); ++iter )
		{
		  mmValue = iter->GetXStart();
		  dxValue = mmValue;

		  OOX::Logic::ParagraphProperty paragraphProperty;
		  
		  paragraphProperty.Ind.init();
          paragraphProperty.Ind->Left = dxValue;

		  double beforeSpacing = ( iter->GetY() - previousStringOffset - iter->GetHeight() );

		  if ( beforeSpacing > 0 )
		  {
		    mmValue = fabs( beforeSpacing );
		    dxValue = mmValue;
		  }
		  else
		  {
		    dxValue = 0;
		  }

          paragraphProperty.Spacing.init();
          
		  paragraphProperty.Spacing->Before = dxValue;
          
		  if ( iter->GetHeight() == 0 )
		  {
		    dxValue = 1;
		  }
		  else
		  {
		    if ( beforeSpacing < 0 )
			{
			  mmValue = ( iter->GetHeight() + beforeSpacing );
			}
			else
			{
			  mmValue = iter->GetHeight();
			}            
    
		    dxValue = mmValue;
		  }
		  
		  paragraphProperty.Spacing->Line = dxValue;
          paragraphProperty.Spacing->LineRule = "exact";

		  iter->SetParagraphProperties( paragraphProperty );

		  previousStringOffset = iter->GetY();
		}
	  }
	  break;
	}  
  }

  /*========================================================================================================*/

  std::vector<OOX::Logic::Paragraph> Page::GetParagraphs() const
  {
    std::vector<OOX::Logic::Paragraph> paragraphs;

	if ( !this->drawingParagraph.Items->empty() )
	{
	  paragraphs.push_back( this->drawingParagraph );
	}
	
	if ( this->textAssociationType != TextAssociationTypeNoFrames )
	{
	  for ( std::vector<PageString>::const_iterator iter = this->strings.begin(); iter != this->strings.end(); ++iter )
	  {
	    paragraphs.push_back( iter->GetParagraph() );
	  }
	}
	else
	{
	  std::list<PageString>::iterator iter = this->sortedStrings.begin();

	  while ( !this->sortedStrings.empty() )
	  {
	    paragraphs.push_back( iter->GetParagraph() );

		iter = this->sortedStrings.erase( iter );
	  }
	}

	if ( ( this->width > 0 ) || ( this->height > 0 ) )
	{
      OOX::Logic::SectorProperty sectionProperties;
	  
	  UniversalUnit mmValue( UniversalUnit::Mm );
      UniversalUnit dxValue( UniversalUnit::Dx );

	  mmValue = this->width;
      dxValue = mmValue;

      sectionProperties.PageSize->Width = dxValue;

	  mmValue = this->height;
      dxValue = mmValue;

	  sectionProperties.PageSize->Height = dxValue;

	  if(this->width <= this->height )
		sectionProperties.PageSize->Orient = "portrait";
	  else
		sectionProperties.PageSize->Orient = "landscape";

	  OOX::Logic::Paragraph sectionParagraph;
		
	  sectionParagraph.Property.init();

	  sectionParagraph.Property->Spacing.init();
	  sectionParagraph.Property->Spacing->Line = 1;
      sectionParagraph.Property->Spacing->LineRule = "exact";

	  sectionParagraph.Property->SectorProperty = sectionProperties;

	  paragraphs.push_back( sectionParagraph );
	}

	return paragraphs;
  }

  /*========================================================================================================*/

  unsigned int Page::GetParagraphsCount() const
  {
    unsigned int paragraphsCount = 0;

	if ( !this->drawingParagraph.Items->empty() )
	{
	  paragraphsCount++;  
	}

	paragraphsCount += (unsigned int)this->strings.size();

	return paragraphsCount;
  }
}