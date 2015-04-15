#pragma once
#include "OOXFontReader.h"

class OOXFontTableReader
{
private: 
	OOX::CFontTable* m_fontTable;

public: 
	OOXFontTableReader(OOX::CFontTable* fontTable)
	{
		m_fontTable = fontTable;
	}
	bool Parse( ReaderParameter oParam )
	{
		for (long i = 0; i < m_fontTable->m_arrFonts.size(); i++)
		{
			OOXFontReader oFontReader(m_fontTable->m_arrFonts[i]);
			RtfFont oNewFont;
			if( true == oFontReader.Parse( oParam, oNewFont ) )
				oParam.oRtf->m_oFontTable.AddItem(oNewFont);
		}	
		return true;
	}
};
