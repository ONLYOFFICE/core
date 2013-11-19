#pragma once

#include "ITextItem.h"

namespace AVSDocFileFormat
{
  struct ITableItem: public ITextItem
  {
    virtual vector<PapxInFkp> GetAllMarkersProperties( vector<unsigned int>* allMarkersOffsets ) const = 0;
    virtual ~ITableItem() {};
  };
}