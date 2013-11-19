#pragma once

#include "..\Common\LanguageCodes.h"

namespace AVSDocFileFormat
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

	  virtual operator byte*() const
	  {
	    return (byte*)(&(this->lid));
	  }

	  virtual operator const byte*() const
	  {
	    return (const byte*)(&(this->lid));
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