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
		//сгенерим ole (olePic)
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


