/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

void HyperlinkObject::save(XLS::CFRecord& record)
{
    record << streamVersion;
    {
        _UINT32 flags = 0;
        SETBIT(flags, 0, hlstmfHasMoniker);
        SETBIT(flags, 1, hlstmfIsAbsolute);
        SETBIT(flags, 2, hlstmfIsAbsolute);
        SETBIT(flags, 3, hlstmfHasLocationStr);
        SETBIT(flags, 4, hlstmfHasDisplayName);
        SETBIT(flags, 5, hlstmfHasGUID);
        SETBIT(flags, 6, hlstmfHasCreationTime);
        SETBIT(flags, 7, hlstmfHasFrameName);
        SETBIT(flags, 8, hlstmfMonikerSavedAsStr);
        SETBIT(flags, 9, hlstmfAbsFromGetdataRel);
        record << flags;
    }
    if(hlstmfHasDisplayName)
    {
       saveHyperlinkString(record,  displayName);
    }
    if(hlstmfHasFrameName)
    {
        saveHyperlinkString(record, targetFrameName);
    }
    if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
    {
        saveHyperlinkString(record, moniker);
    }
    if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
    {
        record << oleMoniker;
    }
    if(hlstmfHasLocationStr)
    {
        saveHyperlinkString(record, location);
    }
    if(hlstmfHasGUID)
    {
        _GUID_ guid_num;
        STR::bstr2guid(guid, guid_num);
        record << guid_num;
    }
    if(hlstmfHasCreationTime)
    {
        record.storeAnyData(fileTime);
    }
}

std::wstring HyperlinkObject::loadHyperlinkString(XLS::CFRecord& record)
{
	std::wstring result;

	_INT32 size = 0;
	record >> size;

	if (size < 1 || record.getRdPtr() + size > record.getDataSize()) return L"";

	_INT32 size_record = record.getDataSize() - record.getRdPtr();
	if (size > size_record)
		size = size_record;

#if defined(_WIN32) || defined(_WIN64)
		result = std::wstring(record.getCurData<wchar_t>(), size);
#else
		result= convertUtf16ToWString(record.getCurData<UTF16>(), size);
#endif
	record.skipNunBytes(size * 2);

	return result;
}
void HyperlinkObject::saveHyperlinkString(XLS::CFRecord& record, std::wstring hlinkString)
{
	hlinkString += L'\0';
	_INT32 size = hlinkString.size();
    record << size;
    for(auto i : hlinkString)
    {
        record << i;
    }
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

