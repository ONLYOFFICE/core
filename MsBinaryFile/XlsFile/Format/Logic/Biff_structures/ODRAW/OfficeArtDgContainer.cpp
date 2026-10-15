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

#include "OfficeArtDgContainer.h"
#include "SimpleOfficeArtContainers.h"

namespace ODRAW
{

OfficeArtDgContainer::OfficeArtDgContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, DgContainer, anchor_type)
{
}


XLS::BiffStructurePtr OfficeArtDgContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtDgContainer(*this));
}


const bool OfficeArtDgContainer::CheckIfContainerStartFound(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	return 0xF002 == rh.recType;
}


const bool OfficeArtDgContainer::CheckIfContainerSizeOK(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	bool res = record.getDataSize() == rh.recLen + rh.size();

	return 0xF002 == rh.recType && res;
}

void OfficeArtDgContainer::loadFields(XLS::CFRecord& record)
{
	try
	{
		OfficeArtRecordHeader rh_test;
		record >> rh_test;
		record.RollRdPtrBack(8);//sizeof(OfficeArtRecordHeader)
		
		if ((rh_test.recType & 0xF000) != 0xF000)
		{
			return;
		}
		OfficeArtContainer::loadFields(record);
	}catch(...)
	{
	}
	
    for (size_t i = 0 ; i < child_records.size(); i++)
	{
		//Log::warning("ArtRecord : " + STR::int2str(child_records[i]->rh_own.recType, 16));
		
		switch(child_records[i]->rh_own.recType)
		{
			case ODRAW::OfficeArtRecord::DggContainer:
			{
			}break;
			case ODRAW::OfficeArtRecord::DgContainer:
			{
				OfficeArtDgContainer * dg = dynamic_cast<OfficeArtDgContainer *>(child_records[i].get());
				if (dg)
				{
					for (size_t i = 0 ; i < dg->child_records.size(); i++)
					{
						child_records.push_back(dg->child_records[i]);
					}
					dg->child_records.clear();

					if (dg->m_OfficeArtSpgrContainer)	m_OfficeArtSpgrContainer	= dg->m_OfficeArtSpgrContainer;
					if (dg->m_OfficeArtFDG)				m_OfficeArtFDG				= dg->m_OfficeArtFDG;
					if (dg->m_OfficeArtSpContainer.size() > 0)	
						m_OfficeArtSpContainer.insert(m_OfficeArtSpContainer.end(), dg->m_OfficeArtSpContainer.begin(), dg->m_OfficeArtSpContainer.end());

				}
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
			case ODRAW::OfficeArtRecord::FDG:
			{
				m_OfficeArtFDG = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
			case ODRAW::OfficeArtRecord::FRITContainer:
			{
				m_OfficeArtFRITContainer = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
			case ODRAW::OfficeArtRecord::SpgrContainer:
			{
				m_OfficeArtSpgrContainer = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
			case ODRAW::OfficeArtRecord::SpContainer:
			{
				m_OfficeArtSpContainer.push_back(child_records[i]);
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;			
		//case ODRAW::OfficeArtRecord::SpgrContainerFileBlock:
		//	{
		//		m_OfficeArtSpgrContainerFileBlock = OfficeArtContainerPtr(art_container);
		//		child_records.erase(child_records.begin() + i,child_records.begin() + i + 1);
		//	}break;		
			default:
				break;
		}
	}


}

void OfficeArtDgContainer::save(XLS::CFRecord& record)
{
	auto sizePos = 0;
	if(first)
	{
		rh_own.recVer = 0xF;
		rh_own.recInstance = 0;
		rh_own.recType =  0xF002;
		record << rh_own;
		sizePos = record.getRdPtr();
		if(m_OfficeArtFDG != nullptr)
			m_OfficeArtFDG->save(record);
		if(m_OfficeArtFRITContainer != nullptr)
			m_OfficeArtFRITContainer->save(record);
		if(m_OfficeArtSpgrContainer != nullptr)
		{
			auto castedContainer = static_cast<OfficeArtSpgrContainer*>(m_OfficeArtSpgrContainer.get());
			castedContainer->rh_own.recLen += totalSize;
			m_OfficeArtSpgrContainer->save(record);
		}
	}
	for(auto i : m_OfficeArtSpContainer)
		if(i != nullptr)
			i->save(record);
	if(m_OfficeArtSpgrContainerFileBlock != nullptr)
		m_OfficeArtSpgrContainerFileBlock->save(record);
	if(first)
	{
		//calculating size
		rh_own.recLen = record.getRdPtr() - sizePos;
		record.RollRdPtrBack(rh_own.recLen + 4);
		auto recLen = rh_own.recLen + totalSize;
		record << recLen;
		record.skipNunBytes(rh_own.recLen);
	}
}

} // namespace ODRAW
