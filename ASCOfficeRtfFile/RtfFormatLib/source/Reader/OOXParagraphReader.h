#pragma once
#include "OOXRunReader.h"
#include "OOXpPrReader.h"
#include "OOXMathReader.h"

#include "../RtfDocument.h"
#include "../RtfField.h"

class OOXParagraphReader
{
private:
	//OOXRunReader					 m_oRunReader; //???? 
	OOX::Logic::CParagraph								*m_ooxParagraph;
	OOX::WritingElementWithChilds<OOX::WritingElement>	*m_ooxElement;
public: 
	
	OOXParagraphReader (OOX::Logic::CParagraph *ooxParagraph)
	{
		m_ooxElement	= NULL; 
		m_ooxParagraph	= ooxParagraph;
	}
	OOXParagraphReader (OOX::WritingElementWithChilds<OOX::WritingElement> *ooxElement)
	{
		m_ooxParagraph	= NULL;
		m_ooxElement	= ooxElement;
	}
	bool Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, bool& bStartNewSection );
	bool Parse2( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, bool& bStartNewSection, RtfStylePtr poStyle);
};
