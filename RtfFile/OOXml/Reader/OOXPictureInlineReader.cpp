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

#include "OOXPictureInlineReader.h"

OOXDrawingInlineReader::OOXDrawingInlineReader(OOX::Drawing::CInline *ooxInline)
{
	m_ooxInline = ooxInline;
}
int OOXDrawingInlineReader::Parse( ReaderParameter oParam, RtfShapePtr & pOutput)
{
	if (m_ooxInline == NULL) return 0;

	pOutput->m_eAnchorTypeShape		= RtfShape::st_inline;

	pOutput->m_nLeft				= 0;
	pOutput->m_nTop					= 0;
	pOutput->m_nPositionHRelative	= 3;
	pOutput->m_nPositionVRelative	= 3;

	int nDistLeft	= m_ooxInline->m_oDistL.IsInit() ? (int)m_ooxInline->m_oDistL->ToTwips() : PROP_DEF;
	int nDistTop	= m_ooxInline->m_oDistT.IsInit() ? (int)m_ooxInline->m_oDistT->ToTwips() : PROP_DEF;
	int nDistRight	= m_ooxInline->m_oDistR.IsInit() ? (int)m_ooxInline->m_oDistR->ToTwips() : PROP_DEF;
	int nDistBottom = m_ooxInline->m_oDistB.IsInit() ? (int)m_ooxInline->m_oDistB->ToTwips() : PROP_DEF;

	int nWidth	= PROP_DEF;
	int nHeight = PROP_DEF;

	if( m_ooxInline->m_oExtent.IsInit() )
	{
		nWidth	= (int)m_ooxInline->m_oExtent->m_oCx.GetValue();
		nHeight	= (int)m_ooxInline->m_oExtent->m_oCy.GetValue();

		if( PROP_DEF != nWidth && PROP_DEF != nHeight )
		{
			nWidth	= RtfUtility::Emu2Twips( nWidth );
			nHeight = RtfUtility::Emu2Twips( nHeight );

			if( PROP_DEF != pOutput->m_nLeft && PROP_DEF != pOutput->m_nTop )
			{
				pOutput->m_nRight	= pOutput->m_nLeft	+ nWidth;
				pOutput->m_nBottom	= pOutput->m_nTop	+ nHeight;
			}
		}
	}

	if ( m_ooxInline->m_oDocPr.IsInit() )
	{
		pOutput->m_nID			= m_ooxInline->m_oDocPr->id + 1025;
		pOutput->m_sName		= m_ooxInline->m_oDocPr->name;

		if (m_ooxInline->m_oDocPr->descr.IsInit())
			pOutput->m_sDescription = m_ooxInline->m_oDocPr->descr.get();
	}

	OOXGraphicReader oGraphicReader(&m_ooxInline->m_oGraphic);

	int result = oGraphicReader.Parse( oParam, pOutput);
	return result;
}
