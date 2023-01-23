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

#include "Label.h"

namespace XLS
{
Label::Label() : isst_(-1)
{}
Label::~Label()
{}
BaseObjectPtr Label::clone()
{
	return BaseObjectPtr(new Label(*this));
}
Label_BIFF2::Label_BIFF2() : Label()
{}
Label_BIFF2::~Label_BIFF2()
{}
BaseObjectPtr Label_BIFF2::clone()
{
	return BaseObjectPtr(new Label_BIFF2(*this));
}
//---------------------------------------------------------------------------------
void Label::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	
	record >> cell;
	
	if (global_info_->Version == 0x0200)
	{
		ShortXLAnsiString name;
		record >> name;

		st = name;
	}
	else if (global_info_->Version < 0x0600)
	{
		LPAnsiString name;
		record >> name;
		
		st = name;
	}
	else
		record >> st;

    isst_ = global_info_->startAddedSharedStrings + global_info_->arAddedSharedStrings.size() ;
	global_info_->arAddedSharedStrings.push_back(st.value());
}

int Label::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe >= global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}

			CP_XML_ATTR(L"t", L"s");
			
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << isst_;
			}
		}			
	}
	return 0;
}
//----------------------------------------------------------------

CommentText::CommentText()
{
	isst_ = -1;
}


CommentText::~CommentText()
{
}


BaseObjectPtr CommentText::clone()
{
	return BaseObjectPtr(new CommentText(*this));
}

void CommentText::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	
	//record >> cell;
	Rw				row;
	Col				col;
	XLUnicodeString	stText;
	
	unsigned short flags;
	record >> row >> col;
	
	if (global_info_->Version < 0x0600)
	{
		LPAnsiString stText1;
		record >> stText1;
		
		stText = stText1;
	}
	else
	{
	}


    isst_ = global_info_->startAddedSharedStrings + global_info_->arAddedSharedStrings.size() ;
	global_info_->arAddedSharedStrings.push_back(st.value());
}

int CommentText::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			int st = (int)cell.ixfe - global_info_->cellStyleXfs_count;
			if (cell.ixfe > global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}

			CP_XML_ATTR(L"t", L"s");
			
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << isst_;
			}
		}			
	}
	return 0;
}
} // namespace XLS

