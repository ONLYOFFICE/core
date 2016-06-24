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

#include "DXFN12.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr DXFN12::clone()
{
	return BiffStructurePtr(new DXFN12(*this));
}

void DXFN12::store(CFRecord& record)
{
	if(dxfn)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned int)/*cbDxf*/, rt_STRUCT_DXFN12);
		size_t offset_dxfn = record.getDataSize();
		record << *dxfn;
		if(xfext)
		{
			record << *xfext;
		}
		record.registerDelayedDataSource(record.getDataSize() - offset_dxfn, rt_STRUCT_DXFN12);
	}
	else
	{
		record.reserveNunBytes(4); // cbDxf
		record.reserveNunBytes(2); // reserved
	}
}


void DXFN12::load(CFRecord& record)
{
	record >> cbDxf;
	
	if(cbDxf)
	{
		const size_t start_of_struct_pos	= record.getRdPtr();
		const size_t end_of_struct_pos		= record.getRdPtr() + cbDxf;
		
		dxfn = DXFNPtr(new DXFN);
		record >> *dxfn;

		if(record.getRdPtr() < end_of_struct_pos)
		{
			xfext = XFExtNoFRTPtr(new XFExtNoFRT);
			record >> *xfext;
		}
		record.resetPointerToBegin	(); // file (42).xls (sheet2)
		record.skipNunBytes			(end_of_struct_pos);
	}
	else
	{
		record.skipNunBytes(2); // reserved
	}
}

int DXFN12::serialize(std::wostream & stream)
{
	if (dxfn && !xfext)
	{
		dxfn->serialize(stream);
	}
	else
	{
		dxfn->serialize(stream);
	//todoooo - file (42).xls
		//CP_XML_WRITER(stream)    
		//{	
		//	CP_XML_NODE(L"dxf")
		//	{
		//		if (xfext)
		//		{
		//		}
		//	}
		//}
	}
	return 0;
}

} // namespace XLS

