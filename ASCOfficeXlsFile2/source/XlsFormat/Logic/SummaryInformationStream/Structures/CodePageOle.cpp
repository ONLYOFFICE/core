/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "CodePageOle.h"
#include <Binary/CFStream.h>
#include <Auxiliary/HelpFunc.h>

namespace OLEPS
{
unsigned short code_page_ = 0;

PropertyCodePage::PropertyCodePage(unsigned int prop_type, const unsigned short value_type, XLS::CFStreamPtr stream)
	: Property(prop_type)
{
	code_page = 0;
	if (value_type == Property::VT_I2)
	{
		*stream >> code_page;
	}
	code_page_ = code_page;
}
PropertyStr::PropertyStr(unsigned int prop_type, const unsigned short value_type, XLS::CFStreamPtr stream)
	: Property(prop_type)
{
	if (value_type == Property::VT_LPSTR)
	{
		_UINT32 size;
		*stream >> size;

		if (size > 0)
		{
			char *s = new char[size];
			stream->read(s,size);
			value  = STR::toStdWString(s, size, code_page_);
			delete []s;
		}
	}
}
PropertyDTM::PropertyDTM(unsigned int prop_type, const unsigned short value_type, XLS::CFStreamPtr stream)
	: Property(prop_type)
{
	_UINT32 dwLowDateTime = 0, dwHighDateTime = 0;
	if (value_type == Property::VT_FILETIME)
	{
		*stream >> dwLowDateTime >> dwHighDateTime;
	}
	//todoooo
}
PropertyInt::PropertyInt(unsigned int prop_type, const unsigned short value_type, XLS::CFStreamPtr stream)
	: Property(prop_type)
{
	value = 0;
	if (value_type == Property::VT_I4)
	{
		*stream >> value;
	}
}
PropertyBool::PropertyBool(unsigned int prop_type, const unsigned short value_type, XLS::CFStreamPtr stream)
	: Property(prop_type)
{
	value = false;
	if (value_type == Property::VT_BOOL)
	{
		_UINT32 v;
		*stream >> v;

		if (v != 0)
			value = true;
	}
}
} // namespace OLEPS
