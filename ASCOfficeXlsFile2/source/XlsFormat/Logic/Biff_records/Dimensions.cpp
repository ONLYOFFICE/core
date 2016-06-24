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

#include "Dimensions.h"
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{

Dimensions::Dimensions()
{
}


Dimensions::~Dimensions()
{
}


BaseObjectPtr Dimensions::clone()
{
	return BaseObjectPtr(new Dimensions(*this));
}


void Dimensions::writeFields(CFRecord& record)
{
	if(!(ref_ == std::wstring (L"") || ref_ == std::wstring (L"A1")))
	{
		CellRangeRef ref_ref(ref_);
		rwMic = ref_ref.getRowFirst();
		rwMac = ref_ref.getRowLast() + 1;// zero-based index of the row after the last row in the sheet that contains a used cell
		colMic = static_cast<unsigned short>(ref_ref.getColumnFirst());
		colMac = static_cast<unsigned short>(ref_ref.getColumnLast()) + 1; // zero-based index of the column after the last column in the sheet that contains a used cell
	}
	else
	{
		rwMic = 0;
		rwMac = 0;
		colMic = 0;
		colMac = 0;
	}
	record << rwMic << rwMac << colMic << colMac;
	record.reserveNunBytes(2); // reserved
}


void Dimensions::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		_UINT16	Mic, Mac;
		record >> Mic >> Mac;
		
		rwMic = Mic;
		rwMac = Mac;
	}
	else
	{
		record >> rwMic >> rwMac;
	}
	record >> colMic >> colMac;
	
	record.skipNunBytes(2); // reserved
	
	if(rwMac >= 0 && colMac >= 0)
	{
		ref_ = static_cast<std::wstring >(CellRangeRef(CellRef(rwMic, colMic, true, true), CellRef(rwMac - 1, colMac - 1, true, true)).toString(false).c_str());
	}
	else
	{
		ref_ = std::wstring (L"");
	}
}

int Dimensions::serialize(std::wostream & stream)
{
	if (ref_.empty()) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dimension")
		{
			CP_XML_ATTR(L"ref", ref_);
		}
	}
	return 0;
}
} // namespace XLS

