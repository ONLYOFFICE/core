#pragma once

#include "OOXReaderBasic.h"

#include "../RtfMath.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/WritingElement.h"

class OOXMathReader
{
private:
	OOX::WritingElementWithChilds<OOX::WritingElement>	*m_ooxElem;

public: 
	OOXMathReader(OOX::WritingElementWithChilds<OOX::WritingElement>* ooxElem)
	{
		m_ooxElem = ooxElem;
	}

	bool ParseElement(ReaderParameter oParam , OOX::WritingElement * ooxMath, RtfMathPtr & rtfMath);

	bool Parse( ReaderParameter oParam , RtfMath& oOutput)
	{
		if (m_ooxElem == NULL) return false;

		for(int i = 0; i < m_ooxElem->m_arrItems.size(); i++)
		{
			RtfMathPtr oNewMath;
			if (ParseElement(oParam, m_ooxElem->m_arrItems[i], oNewMath))
			{
				oOutput.AddItem( oNewMath );
			}
		}
		return true;
	}
};
