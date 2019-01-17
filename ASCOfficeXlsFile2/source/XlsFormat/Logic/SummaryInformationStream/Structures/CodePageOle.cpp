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

#include "CodePageOle.h"
#include <Binary/CFStream.h>
#include <Auxiliary/HelpFunc.h>

#include <../../../UnicodeConverter/UnicodeConverter.h>

namespace OLEPS
{

bool PropertyCodePage::Read (XLS::CFStreamPtr stream)
{
	if (value_type == Property::VT_I2)
	{
		*stream >> code_page;
	}
	return true;
}
bool PropertyStr::Read (XLS::CFStreamPtr stream)
{
	if (value_type == Property::VT_LPSTR)
	{
		_INT32 size;
		*stream >> size;

		if (size > 0)
		{
			char *s = new char[size];
			stream->read(s, size);
			
			for (_INT32 i = size - 1; i >= 0; i--)
			{
				if ( s[i] == 0 ) size--;
			}

			try
			{
				value  = size > 0 ? STR::toStdWString(std::string(s, size), code_page) : L"";
		
			}
			catch(...)
			{
				value  = size > 0 ? STR::toStdWStringSystem(std::string(s, size), code_page) : L"";
			}
			delete []s;
		}
	}
	return true;
}
bool PropertyDTM::Read (XLS::CFStreamPtr stream)
{
	_UINT32 dwLowDateTime = 0, dwHighDateTime = 0;
	if (value_type == Property::VT_FILETIME)
	{
		*stream >> dwLowDateTime >> dwHighDateTime;
	}
	//todoooo
	return true;
}
bool PropertyInt::Read (XLS::CFStreamPtr stream)
{
	value = 0;
	if (value_type == Property::VT_I4)
	{
		*stream >> value;
	}
	return true;
}
bool PropertyBool::Read (XLS::CFStreamPtr stream)
{
	value = false;
	if (value_type == Property::VT_BOOL)
	{
		_UINT32 v;
		*stream >> v;

		if (v != 0)
			value = true;
	}
	return true;
}
} // namespace OLEPS
