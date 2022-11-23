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

