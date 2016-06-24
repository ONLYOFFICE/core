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

#include "FileMoniker.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


FileMoniker::FileMoniker()
{
}


FileMoniker::FileMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr FileMoniker::clone()
{
	return XLS::BiffStructurePtr(new FileMoniker(*this));
}


void FileMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### FileMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FileMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void FileMoniker::load(XLS::CFRecord& record)
{
	_UINT32 ansiLength;
	
	record >> cAnti >> ansiLength >> ansiPath >> endServer >> versionNumber;
	record.skipNunBytes(20); // reserved
	record >> cbUnicodePathSize;
	if(cbUnicodePathSize)
	{
		_UINT32 cbUnicodePathBytes;
		record >> cbUnicodePathBytes >> usKeyValue;

#if defined(_WIN32) || defined(_WIN64)
        unicodePath = std::wstring(record.getCurData<wchar_t>(), cbUnicodePathBytes / 2);
#else
        unicodePath = XLS::convertUtf16ToWString(record.getCurData<UTF16>(), cbUnicodePathBytes / 2);
#endif
		record.skipNunBytes(cbUnicodePathBytes);
	}
}


} // namespace OSHARED

