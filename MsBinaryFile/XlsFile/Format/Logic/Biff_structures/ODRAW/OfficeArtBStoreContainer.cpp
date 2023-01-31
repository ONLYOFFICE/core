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

#include "OfficeArtBStoreContainer.h"

namespace ODRAW
{


OfficeArtBStoreContainer::OfficeArtBStoreContainer()
:	OfficeArtRecord(0, BStoreContainer)
{	
}


XLS::BiffStructurePtr OfficeArtBStoreContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtBStoreContainer(*this));
}

void OfficeArtBStoreContainer::loadFields(XLS::CFRecord& record)
{	
	for (size_t i = 0; i < rh_own.recInstance; i++)
	{
		OfficeArtBStoreContainerFileBlock *fileBlock = new OfficeArtBStoreContainerFileBlock();;		
		record >> *fileBlock;

		rgfb.push_back(fileBlock);
	}	

	//int skip_data = record.getDataSize() - record.getRdPtr();
	//record.skipNunBytes(skip_data);

	// OpenOffice
	while(1)
	{
		OfficeArtRecordHeader rh_child;
		record >> rh_child;	
		record.RollRdPtrBack(rh_child.size());

		if (rh_child.recType == OfficeArtRecord::FBSE)
		{
			OfficeArtBStoreContainerFileBlock *fileBlock = new OfficeArtBStoreContainerFileBlock();;		
			record >> *fileBlock;	

			if (fileBlock->result && (fileBlock->recType != OfficeArtRecord::BlipDIB))
				rgfb.push_back(fileBlock);
		}
		else
			break;
	}
}

const unsigned short OfficeArtBStoreContainer::GetInstanceToStore()
{
	return rgfb.size();
}

} // namespace XLS
