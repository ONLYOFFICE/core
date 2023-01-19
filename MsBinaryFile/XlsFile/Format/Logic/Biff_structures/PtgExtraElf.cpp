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

#include "PtgExtraElf.h"
#include "CellRangeRef.h"

namespace XLS
{


PtgExtraElf::PtgExtraElf()
{
}


PtgExtraElf::PtgExtraElf(CFRecord& record)
{
	load(record);
}


BiffStructurePtr PtgExtraElf::clone()
{
	return BiffStructurePtr(new PtgExtraElf(*this));
}

void PtgExtraElf::load(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;
	
	_UINT32 count = flags & 0x3fffffff;
	
	fRel = GETBIT(flags, 31);
	for(_UINT32 i = 0; i < count; ++i)
	{
		RgceElfLocExtra cell_ref;
		record >> cell_ref;
		array_.push_back(cell_ref);
	}
}


const std::wstring PtgExtraElf::toString() const
{
	std::wstring range;
	for(std::vector<RgceElfLocExtra>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += it->toString() + L';';
	}
	return range + array_.back().toString();
}


const std::wstring PtgExtraElf::toColumnsSet() const
{
	std::wstring range;
	for(std::vector<RgceElfLocExtra>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += CellRangeRef(*it, CellRangeRef::stColumnBelow).toString() + L';';
	}
	return range + CellRangeRef(array_.back(), CellRangeRef::stColumnBelow).toString();
}


} // namespace XLS

