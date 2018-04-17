﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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


#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePageOle.h"


namespace OLEPS
{

PropertyFactory::PropertyFactory()
{
}

PropertyPtr PropertyFactory::ReadProperty(const unsigned int prop_type, XLS::CFStreamPtr stream, const unsigned int property_offset)
{
	stream->seekFromBegin(property_offset);
	unsigned short value_type;

	if (stream->getStreamPointer() + 2 > stream->getStreamSize()) return PropertyPtr();
	*stream >> value_type;
	stream->seekFromCurForward(2); // Skip 2 reserved unsigned chars

	value_type = value_type & 0x00ff;
	switch(prop_type)
	{
        case 0x01:
            return PropertyPtr(new PropertyCodePage(prop_type, value_type, stream));
 		default:
		{
/*			if (value_type == 0x001E)
			{
				return PropertyPtr(new PropertyStr(prop_type, value_type, stream));
			}
			else */if (value_type == 0x0003)
			{
				return PropertyPtr(new PropertyInt(prop_type, value_type, stream));
			}
			else if (value_type == 0x000b)
			{
				return PropertyPtr(new PropertyBool(prop_type, value_type, stream));
			}
			else if (value_type == 0x0040)
			{
				return PropertyPtr(new PropertyDTM(prop_type, value_type, stream));
			}
			else
				return PropertyPtr();
		}break;
	}

}



} // namespace OLEPS
