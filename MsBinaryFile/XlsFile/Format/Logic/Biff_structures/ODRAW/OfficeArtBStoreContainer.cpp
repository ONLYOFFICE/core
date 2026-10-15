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

void OfficeArtBStoreContainer::save(XLS::CFRecord& record)
{
	rh_own.recVer = 0xF;
	rh_own.recInstance = rgfb.size();
	rh_own.recType =  0xF001;
	for(auto i : rgfb)
	{
		rh_own.recLen += 44; //OfficeArtFBSE
		if(!i->nameData.empty())
			rh_own.recLen += i->nameData.size()+1;
		rh_own.recLen += 25; //blipHeader
		if(i->pict_type == L".emf" || i->pict_type == L".wmf")
			rh_own.recLen += 33;
		rh_own.recLen += i->pict_size;
	}
	record << rh_own;

}

const unsigned short OfficeArtBStoreContainer::GetInstanceToStore()
{
	return rgfb.size();
}

} // namespace XLS
