#pragma once

#include "FontManager.h"
#include "OOXUtils.h"
#include "TransformationMatrix.h"
#include "VMLGraphicsPath.h"

#include <limits>

namespace DocxRenderer
{
  enum TextAssociationType
  {
    TextAssociationTypeDefault = 0,
	TextAssociationTypeLine,
	TextAssociationTypeNoFrames,
	TextAssociationTypeBlock
  };

  class PageString
  {
    private:
      struct Element
	  {
	    std::string text;
		OOX::Logic::RunProperty properties;
		double width;

		std::wstring gidText; //!!!For PDF!!!
		
		Element( const std::string& _text, const OOX::Logic::RunProperty& _properties, double _width, const std::wstring& _gidText = _T( "" ) ):
		text(_text), properties(_properties), width(_width), gidText(_gidText)
		{
		}
	  };
	  
	  double xStart;
	  double y;
	  double height;

	  std::map<double, Element> elements;

	  OOX::Logic::ParagraphProperty paragraphProperties;

	  FontManager* fontManager;

    private:
	  long GetFontStyle( const OOX::Logic::RunProperty& _runProperty ) const;
	  friend bool operator == ( const OOX::Logic::RunProperty& _runProperty1, const OOX::Logic::RunProperty& _runProperty2 );
	  double GetMinSymbolWidth() const;
	  double GetMaxSymbolWidth() const;
    
    public:
	  PageString();
	  PageString( double _y, FontManager* _fontManager );
	  PageString( const std::wstring& _text, const std::wstring& _gidText, const OOX::Logic::RunProperty& _properties, double _x, double _y, FontManager* _fontManager );
	  double GetXStart() const;
	  double GetY() const;
	  double GetWidth() const;
	  double GetHeight() const;
	  bool IsEmpty() const;
	  OOX::Logic::Paragraph GetParagraph() const;
	  void SetParagraphProperties( const OOX::Logic::ParagraphProperty& _paragraphProperties );
	  void SetFontManager( FontManager* _fontManager );
	  void AddText( const std::wstring& _text, const OOX::Logic::RunProperty& _properties, double _x, const std::wstring& _gidText = _T( "" ) );
	  void Merge( const PageString& _pageString );

	  static bool LessYPosition( const PageString& _pageString1, const PageString& _pageStrig2 );
	  static void Merge( std::list<PageString>& _pageStrings, double _affinity );
  };
}