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

#include "EnhancedProtection.h"
#include <Binary/CFRecord.h>

namespace XLS
{


EnhancedProtection::EnhancedProtection()
{
}


EnhancedProtection::EnhancedProtection(CFRecord& record)
{
	load(record);
}


BiffStructurePtr EnhancedProtection::clone()
{
	return BiffStructurePtr(new EnhancedProtection(*this));
}


void EnhancedProtection::store(CFRecord& record)
{
#pragma message("####################### EnhancedProtection record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EnhancedProtection record has no BiffStructure::store() implemented.");
	//record << something;
}


void EnhancedProtection::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	iprotObjects = GETBIT(flags, 0);
	iprotScenarios = GETBIT(flags, 1);
	iprotFormatCells = GETBIT(flags, 2);
	iprotFormatColumns = GETBIT(flags, 3);
	iprotFormatRows = GETBIT(flags, 4);
	iprotInsertColumns = GETBIT(flags, 5);
	iprotInsertRows = GETBIT(flags, 6);
	iprotInsertHyperlinks = GETBIT(flags, 7);
	iprotDeleteColumns = GETBIT(flags, 8);
	iprotDeleteRows = GETBIT(flags, 9);
	iprotSelLockedCells = GETBIT(flags, 10);
	iprotSort = GETBIT(flags, 11);
	iprotAutoFilter = GETBIT(flags, 12);
	iprotPivotTables = GETBIT(flags, 13);
	iprotSelUnlockedCells = GETBIT(flags, 14);
	record.skipNunBytes(2); // reserved
}


} // namespace XLS

