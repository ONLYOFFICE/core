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
//
#include "Area.h"

namespace XLS
{

Area::Area()
{
}


Area::~Area()
{
}


BaseObjectPtr Area::clone()
{
	return BaseObjectPtr(new Area(*this));
}


void Area::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fStacked);
	SETBIT(flags, 1, f100);
	SETBIT(flags, 2, fHasShadow);
	record << flags;
}


void Area::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fStacked = GETBIT(flags, 0);
	f100 = GETBIT(flags, 1);
	fHasShadow = GETBIT(flags, 2);
}

int	Area::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:grouping")
		{
			if (f100)			CP_XML_ATTR (L"val" , L"percentStacked");			
			else if (fStacked)	CP_XML_ATTR (L"val" , L"stacked");
			else				CP_XML_ATTR (L"val" , L"standard");
		}
	}
	return 0;
}

} // namespace XLS

