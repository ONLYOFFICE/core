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

//path-string = 1 * path-character 
//path-character = %x0020-%x0021 / %x0023-%x0029 / %x002B-%x002E / %x0030-%x0039 / %x003B / %x003D / %x0040-%x005B / %x005D-%x007B / %x007D-%xFFFF


	bool bFilePathType = false;
	if (!origin.empty())
	{
		std::wstring sTmp = origin;
		std::wstring sResult;
		size_t pos = 0;

		while(pos < sTmp.length())
		{
			switch(sTmp[pos])
			{
			case 1:
			{
				if (bSimple)
				{
					bFilePathType = true;
					bPath = true;	//xls_result.xls 
				}
				else bSimple = true; //file name or file path

				if (false == sResult.empty())
					virtPath.push_back(sResult);
				sResult.clear();
			}break;
			case 2:
			{
				if (bSimple)
					bPath = true;
				
				if (false == sResult.empty())
					virtPath.push_back(sResult);
				sResult.clear();
			}break;
			case 0x03:
			{
				if (false == sResult.empty())
				{
					if (bPath)
					{
						if (virtPath.empty())
							virtPath.push_back(L"");
						
						if (bFilePathType)
						{
							if (sResult.substr(0, 1) == L"@")
								virtPath.back() += L"\\\\" + sResult.substr(1, sResult.length() - 1);
							else
								virtPath.back() += L"file:///" + sResult.substr(0, 1) + L":\\" + sResult.substr(1, sResult.length() - 1);
							bFilePathType = false;
						}
						else
							virtPath.back() += L"/" + sResult;
					}
					else
					{
						bOleLink = true;
						virtPath.push_back(sResult);
					}
					sResult.clear();
				}
			}break;
			case 5:
			{
				if (false == sResult.empty())
					virtPath.push_back(sResult);
				sResult.clear();

				pos++;//skip http size
			}break;
			case 4:
			case 6:
			case 7:
			case 8:
			{
				if (false == sResult.empty())
					virtPath.push_back(sResult);
				sResult.clear();
			}break;
			default:
				sResult += sTmp[pos];
			}
			pos++;
		}
		if (false == sResult.empty())
		{
			if (bPath)
			{
				if (virtPath.empty())
					virtPath.push_back(L"");

				virtPath.back() += L"/" + sResult;
			}
			else
			{
				virtPath.push_back(sResult);
			}
		}
	}
}


} // namespace XLS

