#pragma once

#include "PropertiesMapping.h"
#include "TablePropertyExceptions.h"
#include "CharacterPropertyExceptions.h"
#include "RevisionData.h"
#include "SprmTDefTable.h"
#include "BorderCode.h"
#include "IMapping.h"

namespace DocFileFormat
{
  class TableRowPropertiesMapping: public PropertiesMapping, public IMapping
  {
    private: 
      XMLTools::XMLElement<WCHAR>* _trPr;
      XMLTools::XMLElement<WCHAR>* _tblPrEx;
      //XMLTools::XMLElement<WCHAR>* _tblBorders;
      //BorderCode brcLeft, brcTop, brcBottom, brcRight, brcHorz, brcVert;
      CharacterPropertyExceptions* _rowEndChpx;

    public: 
	  TableRowPropertiesMapping( XmlUtils::CXmlWriter* writer, CharacterPropertyExceptions* rowEndChpx );
	  virtual ~TableRowPropertiesMapping();
	  virtual void Apply( IVisitable* visited );
  };
}