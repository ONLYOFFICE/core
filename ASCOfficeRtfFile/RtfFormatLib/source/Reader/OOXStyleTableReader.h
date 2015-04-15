#pragma once
#include "OOXStyleReader.h"
#include "OOXDocDefaultsReader.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Styles.h"

class OOXStyleTableReader
{
private: 
	OOX::CStyles* m_ooxStyles;

public: 
	OOXStyleTableReader(OOX::CStyles* ooxStyles)
	{
		m_ooxStyles = ooxStyles;
	}
	
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxStyles == NULL) return false;

		if (m_ooxStyles->m_oDocDefaults.IsInit())
		{
			OOXDocDefaultsReader oDocDefaultsReader(m_ooxStyles->m_oDocDefaults.GetPointer());
			oDocDefaultsReader.Parse( oParam );
		}
		
		for (long i=0; i< m_ooxStyles->m_arrStyle.size(); i++)
		{
			RtfStylePtr oNewStyle;
			OOXStyleReader oStyleReader(m_ooxStyles->m_arrStyle[i]);
			
			oStyleReader.ParseStyle( oParam, oNewStyle);
			
			if( NULL != oNewStyle )
				oParam.oRtf->m_oStyleTable.AddItem( oNewStyle );
		}
		//только чтобы добавить связи между стилями
		
		for (long i=0; i< m_ooxStyles->m_arrStyle.size(); i++)
		{
			OOXStyleReader oStyleReader(m_ooxStyles->m_arrStyle[i]);
			oStyleReader.ParseRelations( oParam );
		}	
		return true;
	}
};
