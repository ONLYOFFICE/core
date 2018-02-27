/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "SupBook.h"

namespace XLS
{

SupBook::SupBook() : bOleLink(false), bSimple(false), bPath(false)
{
}


SupBook::~SupBook()
{
}


BaseObjectPtr SupBook::clone()
{
	return BaseObjectPtr(new SupBook(*this));
}

void SupBook::readFields(CFRecord& record)
{
	record >> ctab >> cch;
	if(0x0001 <= cch && 0x00ff >= cch)
	{
		XLUnicodeStringNoCch temp;		
		temp.setSize(cch);
		record >> temp;

		origin = temp.value();

		while(!record.isEOF())
		{
			XLUnicodeString temp2;
			record >> temp2;

			rgst.push_back(temp2.value());
		}
	}
//virt-path = volume / unc-volume / rel-volume / transfer-protocol / startup / alt-startup / library / simple-file-path / ole-link 

//ole-link			= path-string %x0003 path-string 
//simple-file-path	= [%x0001] 
//file-path startup	= %x0001 %x0006 file-path
//alt-startup		= %x0001 %x0007 file-path
//library			= %x0001 %x0008 file-path
//transfer-protocol = %x0001 %x0005 count transfer-path
//transfer-path		= transfer-base-path / "[" transfer-base-path "]" sheet-name 
//transfer-base-path = transfer-type "://" file-path 
//transfer-type		= "ftp" / "http" / "https" 
//rel-volume = %x0001 %x0002 file-path
//...
	bool bFilePathType = false;
	if (!origin.empty())
	{
		std::wstring sTmp = origin;

		while(true)
		{
			int pos = sTmp.find(L"\x0001");
			if (pos >= 0)
			{
				if (bSimple)
				{
					bFilePathType = true;
					bPath = true;	//xls_result.xls 
				}
				else			bSimple = true; //file name or file path

				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0002");
			if (pos >= 0)
			{
				if (bSimple)
					bPath = true;
				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0003");
			if (pos >= 0)
			{
				if (bPath)
				{
					if (bFilePathType)
					{
						virtPath.back() += L"file:///" + sTmp.substr(0, 1) + L":\\" + sTmp.substr(1, pos - 1);
						bFilePathType = false;
					}
					else
						virtPath.back() += L"/" + sTmp.substr(0, pos);
				}
				else
				{
					bOleLink = true;
					virtPath.push_back(sTmp.substr(0, pos));
				}
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0004");
			if (pos >= 0)
			{
				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0005");
			if (pos >= 0)
			{
				virtPath.push_back(sTmp.substr(0, pos));
				//skip http size
				sTmp = sTmp.substr(pos + 2);
				continue;
			}
			pos = sTmp.find(L"\x0006");
			if (pos >= 0)
			{
				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0007");
			if (pos >= 0)
			{
				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			pos = sTmp.find(L"\x0008");
			if (pos >= 0)
			{
				virtPath.push_back(sTmp.substr(0, pos));
				sTmp = sTmp.substr(pos + 1);
				continue;
			}
			break;
		}
		if (bPath)
		{
			virtPath.back() += L"/" + sTmp;
		}
		else
		{
			virtPath.push_back(sTmp);
		}
	}
}


} // namespace XLS

