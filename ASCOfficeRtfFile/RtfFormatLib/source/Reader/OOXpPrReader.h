#pragma once

#include "OOXShadingReader.h"
#include "OOXBorderReader.h"
#include "OOXReaderBasic.h"
#include "OOXcnfStyleReader.h"
#include "OOXrPrReader.h"
#include "OOXSectionPropertyReader.h"

#include "OOXcnfStyleReader.h"


class OOXpPrReader
{
private:
	OOX::Logic::CParagraphProperty *m_ooxParaProps;
public: 
	bool m_bDefStyle;
	
	OOXpPrReader(OOX::Logic::CParagraphProperty *ooxParaProps)
	{
		m_bDefStyle = true;
		m_ooxParaProps = ooxParaProps;
	}
	bool Parse( ReaderParameter oParam ,RtfParagraphProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle, bool& bStartNewSection );
};
