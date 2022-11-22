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

#include "Bes.h"

#include <boost/algorithm/string.hpp>

namespace XLS
{


BiffStructurePtr Bes::clone()
{
	return BiffStructurePtr(new Bes(*this));
}


void Bes::load(CFRecord& record)
{
	record >> bBoolErr >> fError;
}


const std::wstring Bes::toString()
{
	if(fError || bBoolErr > 0x01)
	{
		switch(bBoolErr)
		{
		case 0x00:
			return L"#NULL!";
		case 0x07:
			return L"#DIV/0!";
		case 0x0F:
			return L"#VALUE!";
		case 0x17:
			return L"#REF!";
		case 0x1D:
			return L"#NAME?";
		case 0x24:
			return L"#NUM!";
		case 0x2A:
			return L"#N/A";
		case 0x2B:
			return L"#GETTING_DATA";
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
			break;
		}
	}
	else
	{
		return Boolean<unsigned char>(bBoolErr);
	}
    
    return L""; // TODO: need fixed
}


void Bes::fromString(const std::wstring str)
{
	const std::wstring upper_str = boost::algorithm::to_upper_copy(str);
	if(L"#NULL!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x00;
	}
	else if(L"#DIV/0!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x07;
	}
	else if(L"#VALUE!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x0F;
	}
	else if(L"#REF!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x17;
	}
	else if(L"#NAME?" == upper_str)
	{
		fError = true;
		bBoolErr = 0x1D;
	}
	else if(L"#NUM!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x24;
	}
	else if(L"#N/A" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2A;
	}
	else if(L"#GETTING_DATA" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2B;
	}
	else if(L"TRUE" == upper_str || L"1" == str)
	{
		fError = false;
		bBoolErr = 0x01;
	}
	else if(L"FALSE" == upper_str || L"0" == str)
	{
		fError = false;
		bBoolErr = 0x00;
	}
	else
	{
		// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
	}
}


} // namespace XLS

