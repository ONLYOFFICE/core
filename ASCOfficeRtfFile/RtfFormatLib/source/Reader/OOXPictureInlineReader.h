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
#include "OOXPictureGraphicReader.h"

class OOXPictureInlineReader
{
private:
	OOX::Drawing::CInline *m_ooxInline;
public: 
	OOXPictureInlineReader(OOX::Drawing::CInline *ooxInline)
	{
		m_ooxInline = ooxInline;
	}
	bool Parse( ReaderParameter oParam, RtfShape& oOutput)
	{
		if (m_ooxInline == NULL) return false;

		oOutput.m_oPicture			= RtfPicturePtr( new RtfPicture() );
		oOutput.m_eAnchorTypeShape	= RtfShape::st_inline;
		oOutput.m_nShapeType		= 75;//NSOfficeDrawing::sptPictureFrame;
		oOutput.m_nLeft				= 0;
		oOutput.m_nTop				= 0;
		oOutput.m_nPositionHRelative = 3;
		oOutput.m_nPositionVRelative = 3;
		
		int nDistLeft	= m_ooxInline->m_oDistL.IsInit() ? m_ooxInline->m_oDistL->ToTwips() : PROP_DEF;
		int nDistTop	= m_ooxInline->m_oDistT.IsInit() ? m_ooxInline->m_oDistT->ToTwips() : PROP_DEF;
		int nDistRight	= m_ooxInline->m_oDistR.IsInit() ? m_ooxInline->m_oDistR->ToTwips() : PROP_DEF;
		int nDistBottom = m_ooxInline->m_oDistB.IsInit() ? m_ooxInline->m_oDistB->ToTwips() : PROP_DEF;
		
		int nWidth = PROP_DEF;
		int nHeight = PROP_DEF;

		if( m_ooxInline->m_oExtent.IsInit() )
		{
			nWidth = m_ooxInline->m_oExtent->m_oCx.GetValue();
			nHeight = m_ooxInline->m_oExtent->m_oCy.GetValue();

			if( PROP_DEF != nWidth && PROP_DEF != nHeight )
			{
				nWidth = RtfUtility::Emu2Twips( nWidth );
				nHeight = RtfUtility::Emu2Twips( nHeight );
				if( PROP_DEF != oOutput.m_nLeft && PROP_DEF != oOutput.m_nTop )
				{
					oOutput.m_nRight = oOutput.m_nLeft + nWidth;
					oOutput.m_nBottom = oOutput.m_nTop + nHeight;
				}
				oOutput.m_oPicture->m_nWidthGoal = nWidth;
				oOutput.m_oPicture->m_nHeightGoal = nHeight;
			}
		}
		
		if(m_ooxInline->m_oGraphic.IsInit())
		{
			OOXPictureGraphicReader oGraphicReader(m_ooxInline->m_oGraphic.GetPointer());
			oGraphicReader.Parse( oParam, oOutput);
		}
		//изменяем scale в соответсявии с выходным размером
		if( PROP_DEF != nWidth && PROP_DEF != oOutput.m_oPicture->m_nWidthGoal )
		{
			double dNewScale = 100 * ( 1.0 * nWidth / oOutput.m_oPicture->m_nWidthGoal ) ;
			oOutput.m_oPicture->m_dScaleX = dNewScale;
		}
		if( PROP_DEF != nHeight && PROP_DEF != oOutput.m_oPicture->m_nHeightGoal )
		{
			double dNewScale = 100 * ( 1.0 * nHeight / oOutput.m_oPicture->m_nHeightGoal );
			oOutput.m_oPicture->m_dScaleY = dNewScale;
		}
		return true;
	}
};
