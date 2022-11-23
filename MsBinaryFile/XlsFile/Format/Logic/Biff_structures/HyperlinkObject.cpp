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

#include "HyperlinkObject.h"
#include "../../Binary/CFRecord.h"

namespace OSHARED
{

XLS::BiffStructurePtr HyperlinkObject::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkObject(*this));
}
void HyperlinkObject::load(XLS::CFRecord& record)
{
	_UINT32 flags;
	
	record >> streamVersion >> flags;
	
	hlstmfHasMoniker = GETBIT(flags, 0);
	hlstmfIsAbsolute = GETBIT(flags, 1);
	hlstmfSiteGaveDisplayName = GETBIT(flags, 2);
	hlstmfHasLocationStr = GETBIT(flags, 3);
	hlstmfHasDisplayName = GETBIT(flags, 4);
	hlstmfHasGUID = GETBIT(flags, 5);
	hlstmfHasCreationTime = GETBIT(flags, 6);
	hlstmfHasFrameName = GETBIT(flags, 7);
	hlstmfMonikerSavedAsStr = GETBIT(flags, 8);
	hlstmfAbsFromGetdataRel = GETBIT(flags, 9);

	if(hlstmfHasDisplayName)
	{
		displayName = loadHyperlinkString(record);
	}
	if(hlstmfHasFrameName)
	{
		targetFrameName = loadHyperlinkString(record);
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		moniker = loadHyperlinkString(record);
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		record >> oleMoniker;
	}
	if(hlstmfHasLocationStr)
	{
		location = loadHyperlinkString(record);
	}
	if(hlstmfHasGUID)
	{
		_GUID_ guid_num;
		record >> guid_num;
		guid = STR::guid2bstr(guid_num);
	}
	if(hlstmfHasCreationTime)
	{
		record.loadAnyData(fileTime);
	}
}

std::wstring HyperlinkObject::loadHyperlinkString(XLS::CFRecord& record)
{
	std::wstring result;

	_INT32 size = 0;
	record >> size;

	if (size < 1) return L"";

#if defined(_WIN32) || defined(_WIN64)
		result = std::wstring(record.getCurData<wchar_t>(), size);
#else
		result= convertUtf16ToWString(record.getCurData<UTF16>(), size);
#endif
	record.skipNunBytes(size * 2);

	return result;
}
std::wstring HyperlinkObject::loadHyperlinkString(IBinaryReader* reader)
{
	std::wstring result;
	
	_INT32 size = reader->ReadUInt32();

	if (size < 1) return L"";

	unsigned char* pData = reader->ReadBytes(size * 2, true);
	
#if defined(_WIN32) || defined(_WIN64)
		result = std::wstring((wchar_t*)pData, size);
#else
		result = convertUtf16ToWString((UTF16*)pData, size);
#endif

	delete []pData;
	return result;
}

void HyperlinkObject::load(IBinaryReader* reader)
{
	streamVersion = reader->ReadUInt32();
	_UINT32 flags = reader->ReadUInt32();
		
	hlstmfHasMoniker = GETBIT(flags, 0);
	hlstmfIsAbsolute = GETBIT(flags, 1);
	hlstmfSiteGaveDisplayName = GETBIT(flags, 2);
	hlstmfHasLocationStr = GETBIT(flags, 3);
	hlstmfHasDisplayName = GETBIT(flags, 4);
	hlstmfHasGUID = GETBIT(flags, 5);
	hlstmfHasCreationTime = GETBIT(flags, 6);
	hlstmfHasFrameName = GETBIT(flags, 7);
	hlstmfMonikerSavedAsStr = GETBIT(flags, 8);
	hlstmfAbsFromGetdataRel = GETBIT(flags, 9);

	if(hlstmfHasDisplayName)
	{
		displayName= loadHyperlinkString(reader);
	}
	if(hlstmfHasFrameName)
	{
		targetFrameName= loadHyperlinkString(reader);
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		moniker= loadHyperlinkString(reader);
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		oleMoniker.load(reader);
	}
	if(hlstmfHasLocationStr)
	{
		location= loadHyperlinkString(reader);
	}
	if(hlstmfHasGUID)
	{
		_GUID_ guid_num;
		
		guid_num.Data1 = reader->ReadUInt32();
		guid_num.Data2 = reader->ReadUInt16();
		guid_num.Data3 = reader->ReadUInt16();
	    
		unsigned char* pData = reader->ReadBytes(8, true);
		memcpy(guid_num.getData4(), pData, 8) ;
		delete pData;

		guid = STR::guid2bstr(guid_num);
	}
	if(hlstmfHasCreationTime)
	{
		fileTime.dwLowDateTime = reader->ReadUInt32();
		fileTime.dwHighDateTime = reader->ReadUInt32();
	}
}


} // namespace OSHARED

