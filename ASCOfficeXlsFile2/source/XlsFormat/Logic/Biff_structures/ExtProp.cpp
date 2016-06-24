/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "ExtProp.h"
#include <Binary/CFRecord.h>
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{


BiffStructurePtr ExtProp::clone()
{
	return BiffStructurePtr(new ExtProp(*this));
}

void ExtProp::store(CFRecord& record)
{
#pragma message("####################### ExtProp record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtProp record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtProp::load(CFRecord& record)
{
	record >> extType >> cb;
	switch(extType)
	{
		case 0x0004:
		case 0x0005:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
		case 0x000D:
			record >> extPropData.color;
			break;
		case 0x0006:
			record >> extPropData.gradient_fill;
			break;
		case 0x000E:
			record >> extPropData.font_scheme;
			break;
		case 0x000F:
			record >> extPropData.indent_level;
			break;
		default:
			//throw EXCEPT::RT::WrongBiffRecord("Unsupported type of the extension.", record.getTypeString());
			break;
	}
}


} // namespace XLS
