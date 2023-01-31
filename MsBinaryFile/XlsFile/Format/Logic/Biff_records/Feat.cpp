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

