#pragma once

#include "Page.h"

namespace DocxRenderer
{
  class Document
  {
    private:
	  struct FontCharacteristics
	  {
	    std::wstring panose;
		bool isFixedWidth;
		std::vector<std::wstring> signature;

		FontCharacteristics():
        panose(_T( "00000000000000000000" )), isFixedWidth(false)
		{
		  this->signature.clear();
			
		  this->signature.push_back( _T( "00000000" ) );
		  this->signature.push_back( _T( "00000000" ) );
          this->signature.push_back( _T( "00000000" ) );
          this->signature.push_back( _T( "00000000" ) );
          this->signature.push_back( _T( "00000000" ) );
          this->signature.push_back( _T( "00000000" ) );
		}

        bool operator == ( const FontCharacteristics& _fontCharacteristics ) const
		{
		  if ( this->panose != _fontCharacteristics.panose )
		  {
		    return false;
		  }

		  if ( this->isFixedWidth != _fontCharacteristics.isFixedWidth )
		  {
		    return false;
		  }

		  for ( unsigned int i = 0; i < this->signature.size(); i++ )
		  {
		    if ( this->signature[i] != _fontCharacteristics.signature[i] )
			{
			  return false;
			}
		  }

		  return true;
		}

		bool IsDefault() const
		{
		  if ( this->panose != _T( "00000000000000000000" ) )
		  {
		    return false;
		  }

		  for ( unsigned int i = 0; i < this->signature.size(); i++ )
		  {
		    if ( this->signature[i] != _T( "00000000" ) )
			{
			  return false;
			}
		  }

		  if ( this->isFixedWidth )
		  {
		    return false;
		  }

		  return true;
		}
	  };

	  struct Font
	  {
        std::string name;
		FontCharacteristics characteristics;
	  };

    private:
	  Page previousPage;
	  Page currentPage;
	  mutable Docx::Folder ooxFolder;
	  FontManager fontManager;
	  TextAssociationType textAssociationType;
	  double width;
	  double height;
	  unsigned int pageCount;
	  std::wstring destinationPath;
	  Font currentFont;

    private:
	  bool IsFontPresent( const std::string& _fontName ) const;
	  std::wstring FindFont( const FontCharacteristics& _fontCharacteristics ) const;

    public:
      explicit Document( TextAssociationType _textAssociationType = TextAssociationTypeDefault );
	  Document( const std::wstring& _ooxDir, TextAssociationType _textAssociationType = TextAssociationTypeDefault );
	  void CreateFromTemplate( const std::wstring& _ooxDir );
	  void SetDestinationPath( const std::wstring& _destinationPath );
	  std::wstring GetDestinationPath() const;
	  void AddPage();
	  void SetWidth( double _dWidth );
	  double GetWidth() const;
	  void SetHeight( double _dHeight );
	  double GetHeight() const;
	  void SetTextAssociationType( TextAssociationType _textAssociationType );
      TextAssociationType GetTextAssociationType() const;
	  void BeginCommand( unsigned long _lType );
	  void EndCommand( unsigned long _lType );
	  void AddText( const std::wstring& _text, double _dX, double _dY, double _dWidth, double _dHeight, double _dBaseLineOffset, const std::wstring& _gidText = _T( "" ) );
	  void AddImage( const std::wstring& _fileName, double _dX, double _dY, double _dWidth, double _dHeight );
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
	  void SetFont( const Font& _font );
	  void SetFontSize( double _fontSize );
	  void LoadFontFromFile( const std::wstring& _fileName, double _fontSize, double _xDPI = 72, double _yDPI = 72, long _faceIndex = 0 );
	  void SetBold( bool _value );
	  void SetItalic( bool _value );
	  void SetUnderline( bool _value );
	  void SetStrikeout( bool _value );
	  void Save() const;
  };
}