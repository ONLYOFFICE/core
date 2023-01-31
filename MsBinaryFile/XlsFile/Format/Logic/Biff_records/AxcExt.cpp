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

#include "AxcExt.h"

namespace XLS
{

AxcExt::AxcExt()
{
}


AxcExt::~AxcExt()
{
}


BaseObjectPtr AxcExt::clone()
{
	return BaseObjectPtr(new AxcExt(*this));
}


void AxcExt::readFields(CFRecord& record)
{
	unsigned char flags, reserved;
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags >> reserved;

	fAutoMin	= GETBIT(flags, 0);
	fAutoMax	= GETBIT(flags, 1);
	fAutoMajor	= GETBIT(flags, 2);
	fAutoMinor	= GETBIT(flags, 3);
	
	fDateAxis	= GETBIT(flags, 4);
	fAutoBase	= GETBIT(flags, 5);
	fAutoCross	= GETBIT(flags, 6);
	fAutoDate	= GETBIT(flags, 7);
}

int AxcExt::serialize(std::wostream & _stream)
{
	if (fDateAxis == false) return 0;
	
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:auto")	{  CP_XML_ATTR(L"val", !fAutoCross); }
		
		if (fAutoMax == false)
		{
			CP_XML_NODE(L"c:max") 
			{ 
				CP_XML_ATTR(L"val", catMax);
			}
		}			
		if (fAutoMin == false)
		{
			CP_XML_NODE(L"c:min") 
			{
				CP_XML_ATTR(L"val", catMin); 
			}
		}
		std::wstring DateUnit[] = {L"days", L"months", L"years"};
		
		if ( fAutoMajor == false )
		{
			CP_XML_NODE(L"c:majorUnit")
			{
				CP_XML_ATTR(L"val", catMajor);
			}
		} 
		if ( fAutoMinor == false )
		{
			CP_XML_NODE(L"c:minorUnit")
			{
				CP_XML_ATTR(L"val", catMinor);
			}
		}	
		if ( fAutoBase == false)
		{
			CP_XML_NODE(L"c:baseTimeUnit")
			{
				CP_XML_ATTR(L"val", DateUnit[duBase]);
			}
		}
		CP_XML_NODE(L"c:majorTimeUnit")
		{
			CP_XML_ATTR(L"val", DateUnit[duMajor]);
		}
		CP_XML_NODE(L"c:minorTimeUnit")
		{
			CP_XML_ATTR(L"val", DateUnit[duMinor]);
		}
	}
	return 0;
}

} // namespace XLS

