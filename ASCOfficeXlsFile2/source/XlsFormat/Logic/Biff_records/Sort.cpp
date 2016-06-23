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

#include "Sort.h"

namespace XLS
{

Sort::Sort()
{
}


Sort::~Sort()
{
}


BaseObjectPtr Sort::clone()
{
	return BaseObjectPtr(new Sort(*this));
}


void Sort::writeFields(CFRecord& record)
{
#pragma message("####################### Sort record is not implemented")
	Log::error("Sort record is not implemented.");
	//record << some_value;
}


void Sort::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fCol = GETBIT(flags, 0);
	fKey1Dsc = GETBIT(flags, 1);
	fKey2Dsc = GETBIT(flags, 2);
	fKey3Dsc = GETBIT(flags, 3);
	fCaseSensitive = GETBIT(flags, 4);
	iOrder = GETBITS(flags, 5, 9);
	fAltMethod = GETBIT(flags, 10);

	unsigned char cchKey1;
	unsigned char cchKey2;
	unsigned char cchKey3;
	record >> cchKey1 >> cchKey2 >> cchKey3;
	if(cchKey1)
	{
		stKey1.setSize(cchKey1);
		record >> stKey1;
	}
	if(cchKey2)
	{
		stKey2.setSize(cchKey2);
		record >> stKey2;
	}
	if(cchKey3)
	{
		stKey3.setSize(cchKey3);
		record >> stKey3;
	}
	record.skipNunBytes(1); // reserved
}

} // namespace XLS

