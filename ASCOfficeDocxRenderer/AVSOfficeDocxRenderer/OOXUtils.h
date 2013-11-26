#pragma once

#include "Docx.h"

namespace DocxRenderer
{
  namespace OOXUtils
  {
    inline OOX::Logic::TextFrameProperties GetTextFrameProperties( double _x, double _y, double _width = 0 /*auto*/, double _height = 0 /*auto*/ )
	{
	  OOX::Logic::TextFrameProperties textFrameProperty;
  
      textFrameProperty.HAnchor = "page";
      textFrameProperty.VAnchor = "page";
  
      UniversalUnit mmValue( UniversalUnit::Mm );
      UniversalUnit dxValue( UniversalUnit::Dx );

	  mmValue = _x;
      dxValue = mmValue;

      textFrameProperty.X = ( ( dxValue > 0 ) ? ( dxValue ) : ( 1 ) );
  
      mmValue = _y;
      dxValue = mmValue;

      textFrameProperty.Y = ( ( dxValue > 0 ) ? ( dxValue ) : ( 1 ) );

	  if ( _width > 0 )
	  {
	    mmValue = _width;
        dxValue = mmValue;

        textFrameProperty.W = dxValue;  
	  }

	  if ( _height > 0 )
	  {
	    mmValue = _height;
        dxValue = mmValue;

        textFrameProperty.H = dxValue;  
	  }

	  return textFrameProperty;
	}
  }
}