#pragma once
#include "OOXReaderBasic.h"
#include "OOXHeaderReader.h"

class OOXSectionPropertyReader
{
private:
	OOX::Logic::CSectionProperty *m_ooxSectionProperty;
public: 
	OOXSectionPropertyReader(OOX::Logic::CSectionProperty *ooxSectionProperty)
	{
		m_ooxSectionProperty = ooxSectionProperty;
	}
	bool Parse( ReaderParameter oParam , RtfSectionProperty& oOutput);
};
