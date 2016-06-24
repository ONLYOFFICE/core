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

#include "RadarArea.h"

namespace XLS
{

RadarArea::RadarArea()
{
}


RadarArea::~RadarArea()
{
}


BaseObjectPtr RadarArea::clone()
{
	return BaseObjectPtr(new RadarArea(*this));
}


void RadarArea::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNunBytes(2); // unused
}


void RadarArea::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fRdrAxLab	= GETBIT(flags, 0); //in tick
	fHasShadow	= GETBIT(flags, 1);
	
	record.skipNunBytes(2); // unused
}

int	RadarArea::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:radarStyle")
		{
			CP_XML_ATTR (L"val" , L"filled");
		}
	}

	return 0;
}

} // namespace XLS

