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

#include "XFS.h"

#include "../Biff_records/XF.h"
#include "../Biff_records/XFCRC.h"
#include "../Biff_records/XFExt.h"

#include "../Biff_structures/ExtProp.h"

namespace XLS
{	
	XFS::XFS() : cell_xf_current_id(0), style_xf_current_id(0)
	{
	}
	XFS::~XFS()
	{
	}
	BaseObjectPtr XFS::clone()
	{
		return BaseObjectPtr(new XFS(*this));
	}
//-------------------------------------------------------------------------------------
// XFS = 16*XF [XFCRC 16*4050XFExt]
const bool XFS::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();
	
	int cellStyleXfs_count	= 0;
	int cellXfs_count		= 0;
	
    XF xf(cell_xf_current_id, style_xf_current_id);
    int count = proc.repeated(xf , 0, 0); // "Stock symbols comparison1.xls" (второй FORMATING)

	int ind = 0;
	while (count > 0 && elements_.size() > 0)
	{
		//разделить style & complex
		XF* xfs = dynamic_cast<XF*>(elements_.front().get());

		xfs->ind_xf = ind++;

		if (xfs->fStyle)
		{
			m_arCellStyles.push_back(elements_.front());
			cellStyleXfs_count++;
		}
		else
		{
			m_arCellXFs.push_back(elements_.front());
			cellXfs_count++;
		}
		
		elements_.pop_front();
		count--;
	}

	if (cellXfs_count > 0)
		global_info->cellXfs_count = cellXfs_count;

	if (cellStyleXfs_count > 0)
		global_info->cellStyleXfs_count = cellStyleXfs_count;
	
	if(proc.optional<XFCRC>())
	{
		m_XFCRC = elements_.back(); elements_.pop_back(); 
		XFCRC* crc = dynamic_cast<XFCRC*>(m_XFCRC.get());

		count = proc.repeated<XFExt>(0/*16*/, 4050); // 074_JKH.OPEN.INFO.PRICE.VO_зПТПДУЛЙЕ ПЛТХЗБ юЕМСВЙОУЛПК ПВМ ...
		while (count > 0)
		{
			if (elements_.empty()) break;

			XFExt* ext = dynamic_cast<XFExt*>(elements_.front().get());

			m_arXFext.push_back(elements_.front());
			elements_.pop_front();
			count--;
		}
	}
	return true;
}
void XFS::RegisterFillBorder()
{
	int first_xf_ext = 0;

	for (size_t i = 0 ; i < m_arCellStyles.size(); i++)
	{
		XF *xfs = dynamic_cast<XF*>(m_arCellStyles[i].get());

		for (size_t j = first_xf_ext ; j < m_arXFext.size(); j++)
		{
			XFExt *ext = dynamic_cast<XFExt*>(m_arXFext[j].get());

			if (ext->ixfe > xfs->ind_xf)
				break;

			if (ext->ixfe == xfs->ind_xf)
			{
				xfs->ext_props = ext->rgExt;
				first_xf_ext	= j + 1;
				break;
			}
		}
		xfs->RegisterFillBorderFont();

	}
	first_xf_ext = 0;
	
    for (size_t i = 0 ; i < m_arCellXFs.size(); i++)
	{
		XF *xfs = dynamic_cast<XF*>(m_arCellXFs[i].get());

		if (!m_arXFext.empty() && xfs->fHasXFExt)
		{
			for (size_t j = first_xf_ext ; j < m_arXFext.size(); j++)
			{
				XFExt *ext = dynamic_cast<XFExt*>(m_arXFext[j].get());
				
				if (ext->ixfe > xfs->ind_xf)
					break;

				if (ext->ixfe == xfs->ind_xf)
				{
					xfs->ext_props = ext->rgExt;
					first_xf_ext	= j + 1;
					break;
				}
			}
		}		
		xfs->RegisterFillBorderFont();
	}
}
int XFS::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cellStyleXfs")
		{
			if (false == m_arCellStyles.empty())
			{
				CP_XML_ATTR(L"count", m_arCellStyles.size());
				for (size_t i = 0; i < m_arCellStyles.size(); i++)
				{
					m_arCellStyles[i]->serialize(CP_XML_STREAM());
				}
			}
			else
			{
				CP_XML_ATTR(L"count", 1);
				CP_XML_NODE(L"xf")
				{
					CP_XML_ATTR(L"numFmtId", 0);
					CP_XML_ATTR(L"fontId", 0);
					CP_XML_ATTR(L"fillId", 0);
					CP_XML_ATTR(L"borderId", 0);
				}
			}
		}
		CP_XML_NODE(L"cellXfs")
		{
			if (false == m_arCellXFs.empty())
			{
				CP_XML_ATTR(L"count", m_arCellXFs.size());
				for (size_t i = 0; i < m_arCellXFs.size(); i++)
				{
					m_arCellXFs[i]->serialize(CP_XML_STREAM());
				}
			}
			else
			{
				CP_XML_ATTR(L"count", 1);
				CP_XML_NODE(L"xf")
				{
					CP_XML_ATTR(L"xfId", 0);
					CP_XML_ATTR(L"numFmtId", 0);
					CP_XML_ATTR(L"fontId", 0);
					CP_XML_ATTR(L"fillId", 0);
					CP_XML_ATTR(L"borderId", 0);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

