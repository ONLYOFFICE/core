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

#include "CalcMode.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{

CalcMode::CalcMode()
{
}


CalcMode::~CalcMode()
{
}


BaseObjectPtr CalcMode::clone()
{
	return BaseObjectPtr(new CalcMode(*this));
}


void CalcMode::writeFields(CFRecord& record)
{
	short fAutoRecalc_num;
	if(fAutoRecalc == std::wstring (L"manual"))
	{
		fAutoRecalc_num = 0;
	}
	else if(fAutoRecalc == std::wstring (L"auto"))
	{
		fAutoRecalc_num = 1;
	}
	else if( fAutoRecalc == std::wstring (L"autoNoTable"))
	{
		fAutoRecalc_num = 2;
	}
	else 
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"fAutoRecalc", record.getTypeString().c_str(), fAutoRecalc);
	}
	record << fAutoRecalc_num;
}


void CalcMode::readFields(CFRecord& record)
{
	short fAutoRecalc_num;
	record >> fAutoRecalc_num;
	switch (fAutoRecalc_num)
	{
		case 0:
			fAutoRecalc = std::wstring (L"manual");
			break;
		case 1:
			fAutoRecalc = std::wstring (L"auto");
			break;
		case 2:
			fAutoRecalc = std::wstring (L"autoNoTable");
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of fAutoRecalc.", record.getTypeString());
	}
}

} // namespace XLS

