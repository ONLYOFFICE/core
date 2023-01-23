/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "OOXPictureAnchorReader.h"

OOXDrawingAnchorReader::OOXDrawingAnchorReader(OOX::Drawing::CAnchor *ooxAnchor)
{
	m_ooxAnchor = ooxAnchor;
}
int OOXDrawingAnchorReader::Parse( ReaderParameter oParam , RtfShapePtr & pOutput)
{
	if (m_ooxAnchor == NULL) return 0;

	pOutput->m_eAnchorTypeShape	= RtfShape::st_anchor;

	if ( m_ooxAnchor->m_oDocPr.IsInit() )
	{
		pOutput->m_nID			= m_ooxAnchor->m_oDocPr->id + 1025;
		pOutput->m_sName		= m_ooxAnchor->m_oDocPr->name;

		if (m_ooxAnchor->m_oDocPr->descr.IsInit())
			pOutput->m_sDescription = m_ooxAnchor->m_oDocPr->descr.get();
	}

	pOutput->m_bAllowOverlap	= m_ooxAnchor->m_oAllowOverlap.IsInit() ? m_ooxAnchor->m_oAllowOverlap->ToBool()	: false;
	pOutput->m_nZOrderRelative	= m_ooxAnchor->m_oBehindDoc.IsInit()	? m_ooxAnchor->m_oBehindDoc->ToBool()		: false;
	pOutput->m_bHidden			= m_ooxAnchor->m_oHidden.IsInit()		? m_ooxAnchor->m_oHidden->ToBool()			: false;
	pOutput->m_bLayoutInCell	= m_ooxAnchor->m_oLayoutInCell.IsInit() ? m_ooxAnchor->m_oLayoutInCell->ToBool()	: false;
	pOutput->m_bLockAnchor		= m_ooxAnchor->m_oLocked.IsInit()		? m_ooxAnchor->m_oLocked->ToBool()			: false;
	pOutput->m_nZOrder			= m_ooxAnchor->m_oRelativeHeight.IsInit() ? true : false;

	int nDistLeft	= m_ooxAnchor->m_oDistL.IsInit() ? (int)(m_ooxAnchor->m_oDistL->ToTwips()) : PROP_DEF;
	int nDistTop	= m_ooxAnchor->m_oDistT.IsInit() ? (int)(m_ooxAnchor->m_oDistT->ToTwips()) : PROP_DEF;
	int nDistRight	= m_ooxAnchor->m_oDistR.IsInit() ? (int)(m_ooxAnchor->m_oDistR->ToTwips()) : PROP_DEF;
	int nDistBottom = m_ooxAnchor->m_oDistB.IsInit() ? (int)(m_ooxAnchor->m_oDistB->ToTwips()) : PROP_DEF;

	bool bSimplePos = m_ooxAnchor->m_oSimplePos.IsInit() ? true : false;

	int nWidth	= PROP_DEF;
	int nHeight = PROP_DEF;

	int nLeft	= PROP_DEF;
	int nTop	= PROP_DEF;

	if( m_ooxAnchor->m_oSimplePos.IsInit())
	{
		nLeft	= (int)m_ooxAnchor->m_oSimplePos->m_oX.ToTwips() ;
		nTop	= (int)m_ooxAnchor->m_oSimplePos->m_oY.ToTwips();
	}

	if( m_ooxAnchor->m_oExtent.IsInit() )
	{
		nWidth	= (int)m_ooxAnchor->m_oExtent->m_oCx.ToTwips();
		nHeight = (int)m_ooxAnchor->m_oExtent->m_oCy.ToTwips();

		if( PROP_DEF != pOutput->m_nLeft && PROP_DEF != pOutput->m_nTop )//всегда !!
		{
			pOutput->m_nRight	= pOutput->m_nLeft	+ nWidth;
			pOutput->m_nBottom	= pOutput->m_nTop	+ nHeight;
		}
	}
	if( m_ooxAnchor->m_oPositionH.IsInit() )
	{
		if (m_ooxAnchor->m_oPositionH->m_oRelativeFrom.IsInit())
		{
			switch(m_ooxAnchor->m_oPositionH->m_oRelativeFrom->GetValue())
			{
			case SimpleTypes::relfromhCharacter:
			{
				pOutput->m_nPositionHRelative	= 3;
				pOutput->m_eXAnchor				= RtfShape::ax_margin;
			}break;
			case SimpleTypes::relfromhColumn:
			{
				pOutput->m_nPositionHRelative = 2;
				pOutput->m_eXAnchor = RtfShape::ax_column;
			}break;
			case SimpleTypes::relfromhInsideMargin:
			{
				pOutput->m_nPositionHRelative = 6;
				pOutput->m_eXAnchor = RtfShape::ax_margin;
			}break;
			case SimpleTypes::relfromhLeftMargin:
			{
				pOutput->m_nPositionHRelative = 4;
				pOutput->m_eXAnchor = RtfShape::ax_margin;
			}break;
			case SimpleTypes::relfromhMargin:
			{
				pOutput->m_nPositionHRelative = 0;
				pOutput->m_eXAnchor = RtfShape::ax_margin;
			}break;
			case SimpleTypes::relfromhOutsideMargin:
			{
				pOutput->m_nPositionHRelative = 7;
				pOutput->m_eXAnchor = RtfShape::ax_margin;
			}break;
			case SimpleTypes::relfromhPage          :
			{
				pOutput->m_nPositionHRelative = 1;
				pOutput->m_eXAnchor = RtfShape::ax_page;
			}break;
			case SimpleTypes::relfromhRightMargin   :
			{
				pOutput->m_nPositionHRelative = 5;
				pOutput->m_eXAnchor = RtfShape::ax_margin;
			}break;
			}
		}
		if (m_ooxAnchor->m_oPositionH->m_oAlign.IsInit())
		{
			switch(m_ooxAnchor->m_oPositionH->m_oAlign->GetValue())
			{
			case SimpleTypes::alignhCenter  : pOutput->m_nPositionH = 2; break;
			case SimpleTypes::alignhInside  : pOutput->m_nPositionH = 4; break;
			case SimpleTypes::alignhLeft    : pOutput->m_nPositionH = 1; break;
			case SimpleTypes::alignhOutside : pOutput->m_nPositionH = 5; break;
			case SimpleTypes::alignhRight   : pOutput->m_nPositionH = 3; break;
			}
		}
		if (m_ooxAnchor->m_oPositionH->m_oPosOffset.IsInit())
			pOutput->m_nLeft = (int)m_ooxAnchor->m_oPositionH->m_oPosOffset->ToTwips();
	}
	if(m_ooxAnchor->m_oPositionV.IsInit())
	{
		if (m_ooxAnchor->m_oPositionV->m_oRelativeFrom.IsInit())
		{
			switch(m_ooxAnchor->m_oPositionV->m_oRelativeFrom->GetValue())
			{
			case SimpleTypes::relfromvBottomMargin  :
			{
				pOutput->m_nPositionVRelative = 5;
				pOutput->m_eYAnchor = RtfShape::ay_margin;
			}break;
			case SimpleTypes::relfromvInsideMargin  :
			{
				pOutput->m_nPositionVRelative = 6;
				pOutput->m_eYAnchor = RtfShape::ay_margin;
			}break;
			case SimpleTypes::relfromvLine          :
			{
				pOutput->m_nPositionVRelative = 3;
				pOutput->m_eYAnchor = RtfShape::ay_Para;
			}break;
			case SimpleTypes::relfromvMargin        :
			{
				pOutput->m_nPositionVRelative = 0;
				pOutput->m_eYAnchor = RtfShape::ay_margin;
			}break;
			case SimpleTypes::relfromvOutsideMargin :
			{
				pOutput->m_nPositionVRelative = 7;
				pOutput->m_eYAnchor = RtfShape::ay_margin;
			}break;
			case SimpleTypes::relfromvPage          :
			{
				pOutput->m_nPositionVRelative = 1;
				pOutput->m_eYAnchor = RtfShape::ay_page;
			}break;
			case SimpleTypes::relfromvParagraph     :
			{
				pOutput->m_nPositionVRelative = 2;
				pOutput->m_eYAnchor = RtfShape::ay_Para;
			}break;
			case SimpleTypes::relfromvTopMargin     :
			{
				pOutput->m_nPositionVRelative = 4;
				pOutput->m_eYAnchor = RtfShape::ay_margin;
			}break;
			}
		}
		if(m_ooxAnchor->m_oPositionV->m_oAlign.IsInit())
		{
			switch(m_ooxAnchor->m_oPositionV->m_oAlign->GetValue())
			{
			case SimpleTypes::alignvBottom  : pOutput->m_nPositionV = 3; break;
			case SimpleTypes::alignvCenter  : pOutput->m_nPositionV = 2; break;
			case SimpleTypes::alignvInside  : pOutput->m_nPositionV = 4; break;
			case SimpleTypes::alignvOutside : pOutput->m_nPositionV = 5; break;
			case SimpleTypes::alignvTop     : pOutput->m_nPositionV = 1; break;
			}
		}
		if(m_ooxAnchor->m_oPositionV->m_oPosOffset.IsInit())
			pOutput->m_nTop = (int)m_ooxAnchor->m_oPositionV->m_oPosOffset->ToTwips();
	}

	if(m_ooxAnchor->m_oWrapNone.IsInit())
		pOutput->m_nWrapType = 3;
	if(m_ooxAnchor->m_oWrapSquare.IsInit())
	{
		pOutput->m_nWrapType = 2;
		if (m_ooxAnchor->m_oWrapSquare->m_oWrapText.IsInit())
		{
			switch(m_ooxAnchor->m_oWrapSquare->m_oWrapText->GetValue())
			{
			case SimpleTypes::wraptextBothSides : pOutput->m_nWrapSideType = 0; break;
			case SimpleTypes::wraptextLargest   : pOutput->m_nWrapSideType = 3; break;
			case SimpleTypes::wraptextLeft      : pOutput->m_nWrapSideType = 1; break;
			case SimpleTypes::wraptextRight     : pOutput->m_nWrapSideType = 2; break;
			}
		}
	}
	if(m_ooxAnchor->m_oWrapThrough.IsInit())
	{
		pOutput->m_nWrapType = 5;
		if (m_ooxAnchor->m_oWrapThrough->m_oWrapText.IsInit())
		{
			switch(m_ooxAnchor->m_oWrapThrough->m_oWrapText->GetValue())
			{
			case SimpleTypes::wraptextBothSides : pOutput->m_nWrapSideType = 0; break;
			case SimpleTypes::wraptextLargest   : pOutput->m_nWrapSideType = 3; break;
			case SimpleTypes::wraptextLeft      : pOutput->m_nWrapSideType = 1; break;
			case SimpleTypes::wraptextRight     : pOutput->m_nWrapSideType = 2; break;
			}
		}
		if (m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon.IsInit())///??? todooo twips ? pt?
		{
			int nValueX, nValueY;
			if (m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart.IsInit())
			{
				nValueX = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart->m_oX.ToTwips();
				nValueY = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart->m_oY.ToTwips();

				pOutput->m_aWrapPoints.push_back( std::pair<int,int>(nValueX, nValueY));
			}
			for (size_t i = 0; i < m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo.size(); i++ )
			{
				nValueX = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo[i]->m_oX.ToTwips();
				nValueY = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo[i]->m_oY.ToTwips();

				pOutput->m_aWrapPoints.push_back( std::pair<int,int>(nValueX, nValueY));
			}
		}
	}
	if(m_ooxAnchor->m_oWrapTight.IsInit())
	{
		pOutput->m_nWrapType = 4;
		if (m_ooxAnchor->m_oWrapTight->m_oWrapText.IsInit())
		{
			switch(m_ooxAnchor->m_oWrapTight->m_oWrapText->GetValue())
			{
			case SimpleTypes::wraptextBothSides : pOutput->m_nWrapSideType = 0; break;
			case SimpleTypes::wraptextLargest   : pOutput->m_nWrapSideType = 3; break;
			case SimpleTypes::wraptextLeft      : pOutput->m_nWrapSideType = 1; break;
			case SimpleTypes::wraptextRight     : pOutput->m_nWrapSideType = 2; break;
			}
		}
		if (m_ooxAnchor->m_oWrapTight->m_oWrapPolygon.IsInit())
		{
			__int64 nValueX, nValueY;

			if (m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart.IsInit())
			{
				nValueX = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart->m_oX.ToEmu();
				nValueY = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart->m_oY.ToEmu();

				pOutput->m_aWrapPoints.push_back( std::pair<__int64,__int64>(nValueX, nValueY) );
			}

			for (size_t i = 0; i < m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo.size(); i++ )
			{
				nValueX = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo[i]->m_oX.ToEmu();
				nValueY = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo[i]->m_oY.ToEmu();

				pOutput->m_aWrapPoints.push_back( std::pair<__int64,__int64>(nValueX, nValueY) );
			}
		}
	}
	if(m_ooxAnchor->m_oWrapTopAndBottom.IsInit())
		pOutput->m_nWrapType = 1;

	if( PROP_DEF == pOutput->m_nBottom && pOutput->m_nTop !=PROP_DEF )
	{
		pOutput->m_nBottom	= pOutput->m_nTop	+ nHeight;
	}
	if( PROP_DEF == pOutput->m_nRight && pOutput->m_nLeft !=PROP_DEF )
	{
		pOutput->m_nRight	= pOutput->m_nLeft	+ nWidth;
	}
	OOXGraphicReader oGraphicReader(&m_ooxAnchor->m_oGraphic);
	int result = oGraphicReader.Parse( oParam, pOutput);
	return result;
}
