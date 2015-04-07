#pragma once

#include "FileInformationBlock.h"
#include "ListLevel.h"
#include "ListData.h"
#include "IVisitable.h"

namespace DocFileFormat
{
  class ListTable: public IVisitable
  {
    public: 
	  list<ListData*> listData;

	  virtual ~ListTable();
	  ListTable( FileInformationBlock* fib, POLE::Stream* tableStream );
  };
}