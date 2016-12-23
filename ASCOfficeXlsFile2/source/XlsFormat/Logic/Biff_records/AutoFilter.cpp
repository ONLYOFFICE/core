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
#include "AutoFilter.h"

//#include <algorithm>
//#include <functional>

namespace XLS
{
//static inline std::wstring &ltrim(std::wstring &s) 
//{
//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
//	return s;
//}
//
//// trim from end
//static inline std::wstring &rtrim(std::wstring &s) 
//{
//	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
//	return s;
//}

// trim from both ends
static inline void/*std::wstring &*/trim(std::wstring &s) 
{
	int new_size = s.length();
	for (int i = new_size - 1; i >=0; i--)
	{
		if (s[i] != 0) break;
		else new_size--;
	}
	if (new_size < s.length())
		s.erase(new_size);
	//return ltrim(rtrim(s));
}

AutoFilter::AutoFilter()
{
}


AutoFilter::~AutoFilter()
{
}


BaseObjectPtr AutoFilter::clone()
{
	return BaseObjectPtr(new AutoFilter(*this));
}

void AutoFilter::readFields(CFRecord& record)
{
	m_bAutoFilter12 = false;

	unsigned short flags;
	record >> iEntry >> flags;
	
	wJoin		= static_cast<unsigned char>(GETBITS(flags, 0, 1));
	
	fSimple1	= GETBIT(flags, 2);
	fSimple2	= GETBIT(flags, 3);
	fTopN		= GETBIT(flags, 4);
	fTop		= GETBIT(flags, 5); //top(1) or bottom(0)
	fPercent	= GETBIT(flags, 6);
	
	wTopN		= static_cast<unsigned short>(GETBITS(flags, 7, 15));

	unsigned short	_iEntry		= iEntry;
	unsigned char	_wJoin		= wJoin;			
	unsigned short	_wTopN		= wTopN;			
	
	if (fTopN != 1)
		record >> doper1;			
	if (fTopN != 1)
		record >> doper2;

	if ((doper1.vt == BIFF_BYTE(0)) && (doper1.vt == BIFF_BYTE(0)))
	{
		m_bAutoFilter12 = true;	
		return;
	}

	if (doper1.vt == BIFF_BYTE(0x06))
	{
		XLUnicodeStringNoCch s;
		s.setSize(doper1.vtValueStr.cch);
		record >> s;	

		str1 = s.value();
		trim(str1);
	}

	if (doper2.vt == BIFF_BYTE(0x06))
	{
		XLUnicodeStringNoCch s;
		s.setSize(doper2.vtValueStr.cch);
		record >> s;
		
		str2 = s.value();
		trim(str2);
	}	

	if (record.getRdPtr()  < record.getDataSize())
	{
		int sz = record.getDataSize() - record.getRdPtr();
		char *dd = new char [sz];
		
		memcpy(dd, record.getCurData<char>(), sz);

		record.skipNunBytes(sz);
		delete []dd;
	}
}

} // namespace XLS

