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

#include "HyperlinkObject.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{


XLS::BiffStructurePtr HyperlinkObject::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkObject(*this));
}


void HyperlinkObject::store(XLS::CFRecord& record)
{
	_UINT32 flags = 0;
	
	SETBIT(flags, 0, hlstmfHasMoniker);
	SETBIT(flags, 1, hlstmfIsAbsolute);
	SETBIT(flags, 2, hlstmfSiteGaveDisplayName);
	SETBIT(flags, 3, hlstmfHasLocationStr);
	SETBIT(flags, 4, hlstmfHasDisplayName);
	SETBIT(flags, 5, hlstmfHasGUID);
	SETBIT(flags, 6, hlstmfHasCreationTime);
	SETBIT(flags, 7, hlstmfHasFrameName);
	SETBIT(flags, 8, hlstmfMonikerSavedAsStr);
	SETBIT(flags, 9, hlstmfAbsFromGetdataRel);

	record << streamVersion << flags;

	if(hlstmfHasDisplayName)
	{
		record << displayName;
	}
	if(hlstmfHasFrameName)
	{
		record << targetFrameName;
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		record << moniker;
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		record << oleMoniker;
	}
	if(hlstmfHasLocationStr)
	{
		record << location;
	}
	if(hlstmfHasGUID)
	{
		_GUID_ guid_num;
		if(!STR::bstr2guid(guid, guid_num))
		{
			throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"HyperlinkObject", guid);
		}
		record << guid_num;
	}
	if(hlstmfHasCreationTime)
	{
		record.storeAnyData(fileTime);
	}
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
		record >> displayName;
	}
	if(hlstmfHasFrameName)
	{
		record >> targetFrameName;
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		record >> moniker;
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		record >> oleMoniker;
	}
	if(hlstmfHasLocationStr)
	{
		record >> location;
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


} // namespace OSHARED

