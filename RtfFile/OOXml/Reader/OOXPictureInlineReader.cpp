/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
