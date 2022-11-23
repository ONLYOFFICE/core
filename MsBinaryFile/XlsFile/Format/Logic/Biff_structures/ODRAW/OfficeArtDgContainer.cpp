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

#include "OfficeArtDgContainer.h"

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
		switch(child_records[i]->rh_own.recType)
		{
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

} // namespace ODRAW
