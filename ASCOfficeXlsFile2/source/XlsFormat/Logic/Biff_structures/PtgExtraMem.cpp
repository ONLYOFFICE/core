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

#include "PtgExtraMem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgExtraMem::PtgExtraMem()
{
}


PtgExtraMem::PtgExtraMem(CFRecord& record)
{
	load(record);
}


BiffStructurePtr PtgExtraMem::clone()
{
	return BiffStructurePtr(new PtgExtraMem(*this));
}


void PtgExtraMem::store(CFRecord& record)
{
#pragma message("####################### PtgExtraMem record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgExtraMem record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgExtraMem::load(CFRecord& record)
{
	unsigned short count;
	record >> count;
	for(int i = 0; i < count; ++i)
	{
		Ref8U ref8;
		record >> ref8;
		array_.push_back(ref8);
	}
}


const std::wstring PtgExtraMem::toString() const
{
	std::wstring range;
	for(std::vector<Ref8U>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += it->toString() + L';';
	}
	return range + array_.back().toString();
}


} // namespace XLS

