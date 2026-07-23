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

#include "CommonSubstream.h"

#include "Biff_records/HFPicture.h"
#include "Biff_records/SheetExt.h"
#include "Biff_records/CodeName.h"
#include "Biff_records/WsBool.h"

#include "Biff_unions/GLOBALS.h"

#include "Biff_structures/ODRAW/OfficeArtDgContainer.h"

namespace XLS
{;

void CommonSubstream::LoadHFPicture()
{
	if (m_arHFPicture.empty()) return;

	size_t current_size_hf = 0, j = 0;
	for ( size_t i = 0; i < m_arHFPicture.size(); i++)
	{
		HFPicture* hf = dynamic_cast<HFPicture*>(m_arHFPicture[i].get());
		if ((hf) && (hf->recordDrawingGroup))
		{
			if (!hf->fContinue && current_size_hf > 0)
			{
				XLS::CFRecord record(CFRecordType::ANY_TYPE, global_info_);
				for (; j < i; j++)
				{
					hf = dynamic_cast<HFPicture*>(m_arHFPicture[j].get());
					record.appendRawData(hf->recordDrawingGroup);
				}
				ODRAW::OfficeArtDgContainerPtr rgDrawing = ODRAW::OfficeArtDgContainerPtr(new ODRAW::OfficeArtDgContainer(ODRAW::OfficeArtRecord::CA_HF));
				rgDrawing->loadFields(record);
				m_arHFPictureDrawing.push_back(rgDrawing);
				current_size_hf = 0;

			}
			current_size_hf += hf->recordDrawingGroup->getDataSize();
		}
	}
	if (current_size_hf > 0)
	{
		XLS::CFRecord record(ODRAW::OfficeArtRecord::DggContainer, global_info_);
		for (; j < m_arHFPicture.size(); j++)
		{
			HFPicture* hf = dynamic_cast<HFPicture*>(m_arHFPicture[j].get());
			record.appendRawData(hf->recordDrawingGroup);
		}
		ODRAW::OfficeArtDgContainerPtr rgDrawing = ODRAW::OfficeArtDgContainerPtr(new ODRAW::OfficeArtDgContainer(ODRAW::OfficeArtRecord::CA_HF));
		rgDrawing->loadFields(record);
		m_arHFPictureDrawing.push_back(rgDrawing);
	}
}

int CommonSubstream::serialize_format(std::wostream & strm)
{
	GLOBALS *globals  = dynamic_cast<GLOBALS*>(m_GLOBALS.get());
	SheetExt *sheet_ext = dynamic_cast<SheetExt*>(m_SheetExt.get());
	CodeName *code_name = dynamic_cast<CodeName*>(m_CodeName.get());
	WsBool *wsBool = globals ? dynamic_cast<WsBool*>(globals->m_WsBool.get()) : NULL;

	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"sheetPr")
		{	
			if (code_name)
			{
				CP_XML_ATTR(L"codeName", code_name->value);
			}
			if ((sheet_ext) && (sheet_ext->sheetExtOptional.bEnabled))
			{
				if (!sheet_ext->sheetExtOptional.fCondFmtCalc)	
					CP_XML_ATTR(L"enableFormatConditionsCalculation", false);
				if (!sheet_ext->sheetExtOptional.fNotPublished)	
					CP_XML_ATTR(L"published", false);
				
				if (sheet_ext->sheetExtOptional.color.xclrType.type == XColorType::XCLRRGB ||
					sheet_ext->sheetExtOptional.color.xclrType.type == XColorType::XCLRINDEXED)
				{
					CP_XML_NODE(L"tabColor")
					{
						if (sheet_ext->sheetExtOptional.color.xclrType.type == XColorType::XCLRRGB)
						{			
							CP_XML_ATTR(L"rgb", sheet_ext->sheetExtOptional.color.rgb.strARGB);
						}
						else if (sheet_ext->sheetExtOptional.color.xclrType.type == XColorType::XCLRINDEXED)
						{
							CP_XML_ATTR(L"indexed", sheet_ext->sheetExtOptional.color.icv);
						}
					}
				}
			}
			if (wsBool)
			{
				if (wsBool->fApplyStyles || !wsBool->fDspGuts || !wsBool->fRowSumsBelow || !wsBool->fColSumsRight)
				{
					CP_XML_NODE(L"outlinePr")
					{
						if (wsBool->fApplyStyles)	CP_XML_ATTR(L"applyStyles", wsBool->fApplyStyles);
						if (!wsBool->fDspGuts)		CP_XML_ATTR(L"showOutlineSymbols", wsBool->fDspGuts);
						if (!wsBool->fRowSumsBelow)	CP_XML_ATTR(L"summaryBelow", wsBool->fRowSumsBelow);
						if (!wsBool->fColSumsRight)	CP_XML_ATTR(L"summaryRight", wsBool->fColSumsRight);
					}
				}
				if (!wsBool->fShowAutoBreaks || wsBool->fFitToPage)
				{
					CP_XML_NODE(L"pageSetUpPr")
					{
						if (!wsBool->fShowAutoBreaks)	CP_XML_ATTR(L"autoPageBreaks", wsBool->fShowAutoBreaks);
						if (wsBool->fFitToPage)			CP_XML_ATTR(L"fitToPage", wsBool->fFitToPage);
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

