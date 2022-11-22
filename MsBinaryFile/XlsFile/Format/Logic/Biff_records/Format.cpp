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
#include "Format.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

	Format::Format()
	{}
	Format::~Format()
	{}
	BaseObjectPtr Format::clone()
	{
		return BaseObjectPtr(new Format(*this));
	}
	Format_BIFF23::Format_BIFF23()
	{}
	Format_BIFF23::~Format_BIFF23()
	{}
	BaseObjectPtr Format_BIFF23::clone()
	{
		return BaseObjectPtr(new Format_BIFF23(*this));
	}
//-----------------------------------------------------------------------------------
void Format::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	ifmt = 0xffff;
	if (global_info->Version > 0x0300)
	{
		if (global_info->Version == 0x0400)
		{
			_UINT16 notUsed;
			record >> notUsed;
		}
		else
			record >> ifmt;
	}

	if (global_info->Version < 0x0600)
	{
		ShortXLAnsiString format;
		record >> format;
		
		stFormat = XmlUtils::EncodeXmlString(format.value(), true);
	}
	else
	{
		XLUnicodeString format;
		record >> format;

		stFormat = XmlUtils::EncodeXmlString(format.value(), true);
	}
}
int Format::serialize(std::wostream & stream)
{
	if (ifmt < 5 || (ifmt > 8 && ifmt < 23) || (ifmt > 36 && ifmt < 41) || (ifmt > 44 && ifmt < 50)) return 0;

	stream << L"<numFmt";
	{
		stream << L" numFmtId=\"" << ifmt << L"\"";
		stream << L" formatCode=\"" << stFormat << L"\"";
	}
	stream << L"/>";

	return 1;
}

} // namespace XLS

