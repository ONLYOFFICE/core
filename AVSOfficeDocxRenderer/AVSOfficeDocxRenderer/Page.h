#pragma once

#include "PageString.h"

namespace DocxRenderer
{
  class Page
  {
    private:
	  bool hyperlinkMode;
	  bool clipMode;
	  double width;
	  double height;
	  mutable std::vector<PageString> strings;
	  mutable std::list<PageString> sortedStrings;
	  DocxRenderer::VMLGraphicsPath vmlGraphicsPath;
	  OOX::Logic::Paragraph drawingParagraph;
      TransformationMatrix matrix;
	  TextAssociationType textAssociationType;
	  Docx::Folder* ooxFolder;
	  FontManager* fontManager;
  	  std::string penColor;
	  double penSize;
	  std::string fontColor;
	  std::string brushColor;
	  std::string fontName;
	  double fontSize;
	  bool bold;
	  bool italic;
	  bool underline;
	  bool strikeout;
	  double dAngle;
	  double dLeft;
	  double dTop;
	  double dWidth;
	  double dHeight;
	  unsigned long lFlags;

	  static const double STANDART_STRING_HEIGHT_MM;
	  static const double THE_SAME_STRING_Y_PRECISION_MM;

    private:
	  OOX::Logic::RunProperty GetRunProperties() const;
	  PageString* GetPageString( double _y ) const;

    public:
	  Page();
	  Page( Docx::Folder* _ooxFolder, FontManager* _fontManager, TextAssociationType _textAssociationType = TextAssociationTypeDefault, double _width = 0, double _height = 0 );
	  void SetWidth( double _width );
	  double GetWidth() const;
	  void SetGraphicsWidth( double _width );
	  void SetHeight( double _height );
	  double GetHeight() const;
	  void SetGraphicsHeight( double _height );
	  void SetTextAssociationType( TextAssociationType _textAssociationType );
      TextAssociationType GetTextAssociationType() const;
	  void BeginCommand( unsigned long _lType );
	  void EndCommand( unsigned long _lType );
	  void SetOOXFolder( Docx::Folder* _ooxFolder );
	  void SetFontManager( FontManager* _fontManager );
	  void AddText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset, double _xRestriction = 0, double _yRestriction = 0, const std::wstring& _gidText = _T( "" ) );
      void AddImage( const std::wstring& _fileName, double _dX, double _dY, double _dWidth, double _dHeight );
	  void AddShape( const OOX::Logic::Shape& _shape );
      void PathMoveTo( double _dX, double _dY );
	  void PathLineTo( double _dX, double _dY );
	  void PathLinesTo( const std::vector<double>& _points );
	  void PathCurveTo( double _x1, double _y1, double _x2, double _y2, double _x3, double _y3 );
	  void PathCurvesTo( const std::vector<double>& _points );
	  void PathArcTo( double _x, double _y, double _width, double _height, double _startAngle, double _sweepAngle );
	  void PathClose();
	  void PathEnd();
	  void DrawPath( long _nType, unsigned int _resize = 1000000 );
	  void PathStart();
	  void PathGetCurrentPoint( double* _dX, double* _dY );
	  void GetCommandParams( double* _dAngle, double* _dLeft, double* _dTop, double* _dWidth, double* _dHeight, unsigned long* _lFlags );
      void SetCommandParams( double _dAngle, double _dLeft, double _dTop, double _dWidth, double _dHeight, unsigned long _lFlags );
	  void SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF );
	  void SetTransform( const TransformationMatrix& _matrix );
	  void GetTransform( double* _pdA, double* _pdB, double* _pdC, double* _pdD, double* _pdE, double* _pdF );
	  void ResetTransform();
	  long GetClipMode();
      void SetClipMode( long _lMode );
	  void PathText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset );
	  void SetPenColor( const std::string& _penColor );
      void SetPenSize( double _penSize );
      void SetFontColor( const std::string& _fontColor );
      void SetBrushColor( const std::string& _brushColor );
      void SetFontName( const std::string& _fontName );
      void SetFontSize( double _fontSize );
      void SetBold( bool _value );
      void SetItalic( bool _value );
      void SetUnderline( bool _value );
      void SetStrikeout( bool _value );
	  void Build() const;
	  std::vector<OOX::Logic::Paragraph> GetParagraphs() const;
	  unsigned int GetParagraphsCount() const;
  };
}