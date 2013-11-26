#pragma once

#include "Docx.h"
#include "Singleton.h"
#include "Utils.h"

#import ".\..\..\..\..\Redist\AVSGraphics.dll" raw_interfaces_only

namespace DocxRenderer
{
  class FontManager
  {
    public:
	  enum MeasureType
	  {
	    MeasureTypeGlyph = 0,
		MeasureTypePosition
	  };

    private:
	  AVSGraphics::IAVSFontManagerPtr pFontManager;
	  double xDPI;
	  double yDPI;
	  std::wstring defaultFontName;

    public:
	  FontManager( const std::wstring& _xmlOptions = _T( "" ), double _xDPI = 72, double _yDPI = 72 );
	  ~FontManager();
	  void SetDefaultFont( const std::wstring& _familyName );
	  std::wstring GetDefaultFont() const;
	  void SetXDPI( double _xDPI );
	  double GetXDPI() const;
      void SetYDPI( double _yDPI );
	  double GetYDPI() const;
	  double GetTextWidth( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType = MeasureTypeGlyph ) const;
	  double GetTextWidth( const std::wstring& _text, MeasureType _measureType = MeasureTypeGlyph, bool _isGidText = false ) const;
	  double GetTextHeight( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType = MeasureTypeGlyph ) const;
	  double GetFontHeight( const std::wstring& _fontName, double _fontSize, long _fontStyle ) const;
	  double GetFontHeight( double _fontSize ) const;
	  void MeasureString( const std::wstring& _text, const std::wstring& _fontName, double _fontSize, long _fontStyle, double _x, double _y, double* _boxX, double* _boxY, double* _boxWidth, double* _boxHeight, MeasureType _measureType = MeasureTypeGlyph ) const;
	  double GetSpaceWidthBetweenStrings( const std::wstring& _firstString, const std::wstring& _secondString, const std::wstring& _fontName, double _fontSize, long _fontStyle, MeasureType _measureType = MeasureTypeGlyph ) const;
      void LoadFontFromFile( const std::wstring& _fileName, double _fontSize, double _xDPI = 0, double _yDPI = 0, long _faceIndex = 0 );
	  std::wstring GetFamilyName() const;
	  std::wstring GetStyleName() const;
	  std::wstring GetPANOSE() const;
	  bool IsFixedWidth() const;
	  std::vector<std::wstring> GetSignature() const;
  };

  class FontManagerSingleton: public Singleton<FontManagerSingleton>, public FontManager
  {
    protected:
	  FontManagerSingleton(): FontManager() {};
	  virtual ~FontManagerSingleton() {};
	  friend class Singleton<FontManagerSingleton>;
  };
}