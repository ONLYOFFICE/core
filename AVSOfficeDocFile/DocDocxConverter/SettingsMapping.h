#pragma once

#include "PropertiesMapping.h"
#include "DocumentProperties.h"
#include "ConversionContext.h"
#include "ContentTypes.h"
#include "IMapping.h"

namespace DocFileFormat
{
  static const WCHAR FootnotePositionMap[4][12] =
  {
    _T( "beneathText" ),
    _T( "docEnd" ),
    _T( "pageBottom" ),
    _T( "sectEnd" )
  };

  static const WCHAR ZoomTypeMap[3][9] =
  {
    _T( "none" ),
    _T( "fullPage" ),
    _T( "bestFit" )
  };
	
  class SettingsMapping: public PropertiesMapping, public IMapping
  {
    private:
	  ConversionContext* _ctx;
	  XmlUtils::CXmlWriter m_oXmlWriter;

    public: 
	  SettingsMapping( ConversionContext* ctx/*, OpenXmlPart targetPart*/ );
      virtual void Apply( IVisitable* visited );
      
	private:
	  void writeRsidList();
      void writeCompatibilitySettings( const WordDocumentProperties* dop );
  };
}