#pragma once

#include "OOXFontReader.h"
#include "OOXReaderBasic.h"
#include "OOXColorReader.h"
#include "OOXShadingReader.h"
#include "OOXBorderReader.h"

#include "../RtfDocument.h"
#include "../RtfProperty.h"	

class OOXrPrReader
{
private:
	OOX::Logic::CRunProperty *m_ooxRunProps;
public: 
	bool m_bDefStyle;
	
	OOXrPrReader(OOX::Logic::CRunProperty *ooxRunProps)
	{
		m_bDefStyle = true;
		m_ooxRunProps = ooxRunProps;
	}
	
	bool Parse( ReaderParameter oParam ,RtfCharProperty& oOutputProperty);
};
