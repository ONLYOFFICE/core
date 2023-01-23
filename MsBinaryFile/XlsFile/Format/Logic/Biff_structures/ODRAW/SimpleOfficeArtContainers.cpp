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

#include "SimpleOfficeArtContainers.h"

namespace ODRAW
{

void OfficeArtFConnectorRule::loadFields(XLS::CFRecord& record)
{
	record >> ruid >> spidA >> spidB >> spidC >> cptiA >> cptiB;
}
	
void OfficeArtFArcRule::loadFields(XLS::CFRecord& record)
{
	record >> ruid >> spid;
}

void OfficeArtFCalloutRule::loadFields(XLS::CFRecord& record)
{
	record >> ruid >> spid;
}

void OfficeArtClientTextbox::loadFields(XLS::CFRecord& record)
{
	if (rh_own.recLen > 0)
	{
	}
}

void OfficeArtClientData::loadFields(XLS::CFRecord& record)
{
	if (rh_own.recLen > 0)
	{
	}
}
void OfficeArtSolverContainer::loadFields(XLS::CFRecord& record)
{
	//array of OfficeArtSolverContainerFileBlock
}

void OfficeArtDggContainer::loadFields(XLS::CFRecord& record)
{
	OfficeArtContainer::loadFields(record);
	
    for (size_t i = 0 ; i < child_records.size(); i++)
	{
		switch(child_records[i]->rh_own.recType)
		{
		case ODRAW::OfficeArtRecord::BStoreContainer:
			{
				m_OfficeArtBStoreContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::ColorMRUContainer:
			{
				m_OfficeArtColorMRUContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::SplitMenuColorContainer:
			{
				m_OfficeArtSplitMenuColorContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::FDGGBlock:
			{
				m_OfficeArtFDGGBlock = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;			
		//case ODRAW::OfficeArtRecord::SpgrContainerFileBlock:
		//	{
		//		m_OfficeArtSpgrContainerFileBlock = OfficeArtContainerPtr(art_container);
		//		child_records.erase(child_records.begin() + i,child_records.begin() + i + 1);
		//	}break;		

		}
	}
}

void OfficeArtSpgrContainer::loadFields(XLS::CFRecord& record)
{
	OfficeArtContainer::loadFields(record);

    //for (size_t i = 0 ; i < child_records.size(); i++)
	//{
	//	switch(child_records[i]->rh_own.recType)
	//	{
	//	case ODRAW::OfficeArtRecord::SpgrContainerFileBlock:
	//		{
	//			m_OfficeArtSpgrContainerFileBlock.push_back(child_records[i]);
	//			child_records.erase(child_records.begin() + i,child_records.begin() + i + 1);
	//		}break;
	//	}
	//}

}
void OfficeArtSpContainer::loadFields(XLS::CFRecord& record)
{
	OfficeArtContainer::loadFields(record);

    for (size_t i = 0 ; i < child_records.size(); i++)
	{
		switch(child_records[i]->rh_own.recType)
		{
			case ODRAW::OfficeArtRecord::FSP:
			{
				m_OfficeArtFSP = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--;
			}break;

			case ODRAW::OfficeArtRecord::ChildAnchor:
			case ODRAW::OfficeArtRecord::ClientAnchor:
			{
				m_OfficeArtAnchor = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--;
			}break;
		}
	}

}

} // namespace ODRAW
