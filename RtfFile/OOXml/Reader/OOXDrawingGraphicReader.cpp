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

#include "OOXDrawingGraphicReader.h"
#include "OOXShapeReader.h"
#include "OOXReader.h"

#include "../../../OOXML/PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"
#include "../../../OOXML/PPTXFormat/Theme.h"

int OOXGraphicReader::Parse( ReaderParameter oParam , RtfShapePtr & pOutput)
{
	if (m_ooxGraphic == NULL) return 0;
	
	if (m_ooxGraphic->element.is_init())
	{
		if (m_ooxGraphic->element.getType() == OOX::et_p_ShapeTree ||
			m_ooxGraphic->element.getType() == OOX::et_lc_LockedCanvas)
		{
			OOXShapeGroupReader groupReader(dynamic_cast<PPTX::Logic::SpTree*>(m_ooxGraphic->element.GetElem().GetPointer()));
			return (groupReader.Parse(oParam, pOutput) ? 1 : 0);
		}
		else
		{
			OOXShapeReader shapeReader(m_ooxGraphic->element.GetElem().GetPointer());
			return (shapeReader.Parse(oParam, pOutput) ? 1 : 0);
		}
	}
	if (m_ooxGraphic->chartRec.IsInit())
	{
		//generate ole (olePic)
		m_ooxGraphic->ChartToOlePackageInStorage(oParam.oReader->m_currentContainer, oParam.oReader->m_sTempFolder, ++oParam.oReader->m_nCurOleChartId);
	}
	if (m_ooxGraphic->olePic.IsInit())
	{
		OOXShapeReader shapeReader(m_ooxGraphic->olePic.GetPointer());
		return (shapeReader.Parse(oParam, pOutput) ? 1 : 0);
	}
	if (m_ooxGraphic->smartArt.IsInit())
	{
		m_ooxGraphic->smartArt->LoadDrawing();
		if (m_ooxGraphic->smartArt->m_oDrawing.IsInit())
		{
			OOXShapeGroupReader groupReader(dynamic_cast<PPTX::Logic::SpTree*>(m_ooxGraphic->smartArt->m_oDrawing.GetPointer()));
			return (groupReader.Parse(oParam, pOutput) ? 1 : 0);
		}
	}

	
//nullable_string			spid;
//nullable<Table>			table;
	return 0;
}


