#pragma once

#include "DocumentMapping.h"
#include "TableInfo.h"

namespace DocFileFormat
{
  class FooterMapping: public DocumentMapping
  {
    private: 
	  CharacterRange _ftr;

    public: 
	  FooterMapping( ConversionContext* ctx, CharacterRange ftr );
      virtual void Apply( IVisitable* visited );
  };
}