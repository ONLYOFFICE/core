#pragma once
#include "OOXAbstractNumReader.h"
#include "OOXNumberingMapReader.h"
#include "OOXShapeReader.h"

class OOXNumberingReader
{
private:
	OOX::CNumbering* m_ooxNumbering;
public: 
	OOXNumberingReader( OOX::CNumbering* ooxNumbering)
	{
		m_ooxNumbering = ooxNumbering;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxNumbering == NULL) return false;

		for (long i=0; m_ooxNumbering->m_arrNumPicBullet.size(); i++)
		{
			if (m_ooxNumbering->m_arrNumPicBullet[i] == NULL) continue;
			
			int nID = m_ooxNumbering->m_arrNumPicBullet[i]->m_oNumPicBulletId.GetValue();
					
			if (m_ooxNumbering->m_arrNumPicBullet[i]->m_oVmlDrawing.IsInit())
			{
				OOXShapeReader oShapeReader(m_ooxNumbering->m_arrNumPicBullet[i]->m_oVmlDrawing.GetPointer());

				RtfShapePtr oNewShape( new RtfShape() );	
				if( true == oShapeReader.Parse( oParam, oNewShape ) )
				{
					oNewShape->m_eShapeType = RtfShape::st_inline;
					oNewShape->m_nPositionHRelative = 3;
					oNewShape->m_nPositionVRelative = 3;
					
					int nIndex = oParam.oRtf->m_oListTabel.m_aPictureList.GetCount();
					oParam.oReader->m_mapPictureBullet[nID] = nIndex;
					oParam.oRtf->m_oListTabel.m_aPictureList.AddItem( oNewShape );
				}
			}
			if (m_ooxNumbering->m_arrNumPicBullet[i]->m_oDrawing.IsInit())
			{
				//???? todooo
			}
		}

		for (long i=0; i < m_ooxNumbering->m_arrAbstractNum.size(); i++)
		{
			OOXAbstractNumReader oParagraph(m_ooxNumbering->m_arrAbstractNum[i]);
			oParagraph.Parse( oParam );
		}
		for (long i=0; i < m_ooxNumbering->m_arrNum.size(); i++)
		{
			OOXNumberingMapReader oParagraph(m_ooxNumbering->m_arrNum[i]);
			oParagraph.Parse( oParam );
		}

		
		return true;
	}
};
