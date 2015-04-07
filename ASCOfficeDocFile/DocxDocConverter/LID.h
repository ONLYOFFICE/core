#pragma once

#include "../Common/LanguageCodes.h"

namespace ASCDocFileFormat
{
  class LID: public IOperand
  {
    private:
	  DocFileFormat::LanguageCode lid;

    public:
	  LID():
	  lid(DocFileFormat::Nothing)
	  {
	  }

	  explicit LID( DocFileFormat::LanguageCode _lid ):
	  lid(_lid)
	  {
	  }

	  virtual ~LID()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)(&(this->lid));
	  }

	  virtual operator const unsigned char*() const
	  {
	    return (const unsigned char*)(&(this->lid));
	  }

	  virtual unsigned int Size() const
	  {
	    return sizeof(unsigned short);
	  }

	  operator unsigned int() const
	  {
	    return (unsigned int)this->lid;
	  }
  };
}
