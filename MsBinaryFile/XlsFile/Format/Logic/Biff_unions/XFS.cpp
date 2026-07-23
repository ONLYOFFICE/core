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
    int count = proc.repeated(xf , 0, 0); // "Stock symbols comparison1.xls" (second FORMATING)

	int ind = 0;
	while (count > 0 && elements_.size() > 0)
	{
		//split style & complex
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

void calcChecksum(CFRecord record, _UINT32 &checksum)
{
	auto beginData = record.getCurStaticData<BYTE>() - record.getRdPtr();
	size_t len = record.getRdPtr();
	for (size_t j = 0; j < len; j++)
	{
		checksum ^= ((_UINT32)beginData[j] << 24);

		for (int i = 0; i < 8; i++)
		{
			if (checksum & 0x80000000)
				checksum = ((checksum << 1) ^ 0x000000AFU) & 0xFFFFFFFFU;
			else
				checksum = (checksum << 1) & 0xFFFFFFFFU;
		}
	}
}

const bool XFS::saveContent(BinProcessor& proc)
{
	auto globInfo = proc.getGlobalWorkbookInfo();
	_UINT32 crc = 0;
	for(auto i = 0; i < 15; i++)
	{
		size_t index = 0;
		XF cellStyleMandatory(index, index);
		cellStyleMandatory.fStyle = true;
		cellStyleMandatory.ixfParent = 0xFFF;
		if(!m_arXFext.empty())
		{	CFRecord tempRec(224, proc.getGlobalWorkbookInfo());
			cellStyleMandatory.writeFields(tempRec);
			calcChecksum(tempRec, crc);
		}
		proc.mandatory(cellStyleMandatory);
	}
	{
		size_t index = 0;
		XF cellStyleMandatory(index, index);
		cellStyleMandatory.fStyle = false;
		cellStyleMandatory.ixfParent = 0;
		if(!m_arXFext.empty())
		{	CFRecord tempRec(224, proc.getGlobalWorkbookInfo());
			cellStyleMandatory.writeFields(tempRec);
			calcChecksum(tempRec, crc);
		}
		proc.mandatory(cellStyleMandatory);
	}

	for (auto i: m_arCellStyles)
		if(i!= nullptr)
		{
			proc.mandatory(*i);
			if(!m_arXFext.empty())
			{	CFRecord tempRec(224, proc.getGlobalWorkbookInfo());
				auto xf = static_cast<XF*>(i.get());
				xf->writeFields(tempRec);
				calcChecksum(tempRec, crc);
			}
		}
	globInfo->cellStyleXfs_count = m_arCellStyles.size() + 16; // styles + 16 mandatory styles
	for (auto i: m_arCellXFs)
        if(i!= nullptr)
		{
            proc.mandatory(*i);
			if(!m_arXFext.empty())
			{	CFRecord tempRec(224, proc.getGlobalWorkbookInfo());
				auto xf = static_cast<XF*>(i.get());
				xf->writeFields(tempRec);
				calcChecksum(tempRec, crc);
			}
		}
	globInfo->cellXfs_count = m_arCellXFs.size();
	if(!m_arXFext.empty())
    {
		XLS::XFCRC XFcrc;
		XFcrc.cxfs = globInfo->cellStyleXfs_count + globInfo->cellXfs_count;
		XFcrc.crc = crc;
		proc.mandatory(XFcrc);
        for(auto i : m_arXFext)
            proc.mandatory(*i);
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

