#pragma once

#include "StringTable.h"
#include "FontFamilyName.h"
#include "ContentTypes.h"
#include "AbstractOpenXmlMapping.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
  static const wchar_t FontFamily[6][11] =
  {
    _T( "auto" ),
    _T( "roman" ),
    _T( "swiss" ),
    _T( "modern" ),
    _T( "script" ),
    _T( "decorative" )
  };

  static const wchar_t FontPitch[3][9] =
  {
    _T( "default" ),
    _T( "fixed" ),
    _T( "variable" )
  };

  class FontTableMapping: public AbstractOpenXmlMapping, public IMapping
  {
    private:
	  ConversionContext* _ctx;

    public:
      FontTableMapping( ConversionContext* ctx );
	  virtual ~FontTableMapping();
      virtual void Apply( IVisitable* visited );
  };
}
