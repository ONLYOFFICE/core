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

#include "DConRef.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

DConRef::DConRef()
{
	index_external = -1;
	bFilePath = false;
}


DConRef::~DConRef()
{
}


BaseObjectPtr DConRef::clone()
{
	return BaseObjectPtr(new DConRef(*this));
}

void DConRef::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	record >> ref >> cchFile;
	
	if (cchFile > 1)
	{
		XLUnicodeStringNoCch	stFile_;
		stFile_.setSize(cchFile);
		record >> stFile_;
//dcon-file				= external-virt-path / self-reference
//external-virt-path	= volume / unc-volume / rel-volume / transfer-protocol / startup / alt-startup / library / simple-file-path-dcon
//simple-file-path-dcon = %x0001 file-path
//self-reference		= %x0002 sheet-name

		stFile = stFile_.value();

		std::wstring sTmp = stFile;

		while(true)
		{
			int pos = sTmp.find(L"\x0001");
			if (pos >= 0)
			{
				bFilePath = true;

				path.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0002");
			if (pos >= 0)
			{
				path.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0003");
			if (pos >= 0)
			{
				bFilePath = true;

				path.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			break;
		}
		int pos = sTmp.find(L"]");
		if (pos >= 0)
		{
			file_name	= sTmp.substr(1, pos - 1);
			sheet_name	= sTmp.substr(pos + 1);
		}
		else
		{
			sheet_name = sTmp;
		}
	}

	int unused = record.getDataSize() - record.getRdPtr();
	record.skipNunBytes(unused);
}

void DConRef::check_external()
{
	bool bFound = false;

	for (size_t i = 0; !bFilePath && i < global_info_->sheets_info.size(); i++)
	{
		if (global_info_->sheets_info[i].name == sheet_name)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound && (!path.empty() || !file_name.empty()) && bFilePath)
	{//external sheet
		std::wstring full_path;
		if (!path.empty())
		{
			full_path = get_external_path();
		}		
		std::unordered_map<std::wstring, std::wstring>::iterator pFind = global_info_->mapPivotCacheExternal.find(file_name);

		if (pFind == global_info_->mapPivotCacheExternal.end())
		{
			index_external = global_info_->mapPivotCacheExternal.size() ;

			global_info_->mapPivotCacheExternal.insert(std::make_pair(file_name, full_path));
		}
		else
		{
			if (pFind->second.empty() && !full_path.empty())
			{
				pFind->second = full_path;
			}
			index_external = std::distance( global_info_->mapPivotCacheExternal.begin(), pFind) ;			
		}
	}
}

std::wstring DConRef::get_external_path()
{
	if (path.empty() && file_name.empty()) return L"";

	std::wstring result = L"file:///";

	for (size_t i = 0; i < path.size(); i++)
	{
		result += L"\\" + path[i];
	}

	if (!file_name.empty())
		result += L"\\" + file_name;

	return result;
}

} // namespace XLS

