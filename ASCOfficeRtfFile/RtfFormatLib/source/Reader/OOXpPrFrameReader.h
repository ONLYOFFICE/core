#pragma once
#include "OOXReaderBasic.h"
#include "../RtfProperty.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/ParagraphProperty.h"

class OOXpPrFrameReader
{
private:
	ComplexTypes::Word::CFramePr *m_ooxFramePr;
public: 
	OOXpPrFrameReader(ComplexTypes::Word::CFramePr *ooxFramePr)
	{
		m_ooxFramePr = ooxFramePr;
	}
	bool Parse( ReaderParameter oParam ,RtfFrame& oOutputProperty);
};
