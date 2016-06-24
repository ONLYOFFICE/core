/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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

        for (long i=0; i < m_ooxNumbering->m_arrNumPicBullet.size(); i++)
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
