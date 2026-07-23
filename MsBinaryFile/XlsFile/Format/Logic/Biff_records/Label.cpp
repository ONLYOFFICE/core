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
		unsigned short test;
		record >> test;
		record.RollRdPtrBack(2);
		
		if (test > record.getDataSize())
		{
			//wrong version !! 
			record.RollRdPtrBack(record.getRdPtr());

			int store = global_info_->Version;
			global_info_->Version = 0x0200;

			ShortXLAnsiString name;

			record >> cell >> name;
			st = name;

			global_info_->Version = store;
		}
		else
		{
			LPAnsiString name;
			record >> name;

			st = name;
		}
	}
	else
	{
		record >> st;
	}

    isst_ = global_info_->startAddedSharedStrings + global_info_->arAddedSharedStrings.size() ;
	global_info_->arAddedSharedStrings.push_back(st.value());
}

void Label::writeFields(CFRecord& record)
{
    record << cell << st;
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

