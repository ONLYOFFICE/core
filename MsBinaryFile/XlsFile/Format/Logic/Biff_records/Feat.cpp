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

#include "Feat.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

Feat::Feat()
{
}


Feat::~Feat()
{
}

BaseObjectPtr Feat::clone()
{
	return BaseObjectPtr(new Feat(*this));
}
//		ISFPROTECTION	= 0x0002, // Specifies the enhanced protection type. 
//		ISFFEC2			= 0x0003, // Specifies the ignored formula errors type.
//		ISFFACTOID		= 0x0004, // Specifies the smart tag type.
//		ISFLIST			= 0x0005, // Specifies the list type.

void Feat::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Feat);
	
	record >> frtHeader >> isf;
	record.skipNunBytes(5); // reserved

	record >> cref >> cbFeatData;
	record.skipNunBytes(2); // reserved
	
	for (int i = 0; i < cref ; ++i)
	{
		Ref8U reff;
		record >> reff;
		refs.push_back(BiffStructurePtr(new Ref8U(reff)));
		
		sqref += reff.toString() + ((i == cref - 1) ? L"" : L" ");
	}

	switch(isf)
	{
		case 0x0002://ISFPROTECTION:
			is_object = BiffStructurePtr(new FeatProtection);
			break;
		case 0x0003://ISFFEC2:
			is_object = BiffStructurePtr(new FeatFormulaErr2);
			break;
		case 0x0004://ISFFACTOID:
			is_object = BiffStructurePtr(new FeatSmartTag);
			break;
	}
	if (is_object)
		is_object->load(record);
}

} // namespace XLS

