#pragma once

#include "IVisitable.h"
#include "../Common/LanguageCodes.h"
	
namespace DocFileFormat
{	
  class LanguageId: public IVisitable
  {
    public:
	  int Id;
      LanguageCode Code;

      LanguageId( int id )
      {
        this->Id = id;
        this->Code = (LanguageCode)id;
      }
  };
}