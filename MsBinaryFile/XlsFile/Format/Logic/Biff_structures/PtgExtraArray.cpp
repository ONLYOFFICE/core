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

#include "PtgExtraArray.h"
#include "SyntaxPtg.h"
#include "SerBool.h"
#include "SerErr.h"
#include "SerNil.h"
#include "SerNum.h"
#include "SerStr.h"

namespace XLS
{


PtgExtraArray::PtgExtraArray()
{
}


PtgExtraArray::PtgExtraArray(const std::wstring& word)
{
	fromString(word);
}


BiffStructurePtr PtgExtraArray::clone()
{
	return BiffStructurePtr(new PtgExtraArray(*this));
}

void PtgExtraArray::load(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        DColunByteU cols_xls;
        DRw rows_xls;
        record >> cols_xls >> rows_xls;
        cols = cols_xls;
        rows = rows_xls;
    }
    else
    {
        record >> cols >> rows;
    }
	for(int i = 0; i < (cols + 1) * (rows + 1); ++i)
	{
		if (record.getRdPtr() >= record.getDataSize()) 
			break;
		unsigned char rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		array_.push_back(ser);
	}
}


const std::wstring PtgExtraArray::toString() const
{
	std::wstring ret_val;
	unsigned char col_cnt = cols + 1;

	if (array_.empty()) return L"";

	for(std::vector<SerArPtr>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		ret_val += (*it)->toString();
		if(--col_cnt)
		{
			ret_val += L',';
		}
		else
		{
			ret_val += L';';
			col_cnt = cols + 1;
		}
	}
	ret_val += array_.back()->toString();
	return ret_val;
}


void PtgExtraArray::fromString(const std::wstring& str)
{
	if(str.length() <= 2)
	{
		// EXCEPT::LE::WhatIsTheFuck("Unknown PtgExtraArray format extracted from formula.", __FUNCTION__);

	}
	std::wstring::const_iterator first = str.begin() + 1;
	std::wstring::const_iterator last = str.end() - 1;

	cols = 0;
    rows = 0;
	while(first != last)
	{
		std::wstring operand_str;
		if(SyntaxPtg::extract_PtgNum(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerNum(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgBool(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerBool(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgErr(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerErr(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgStr(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerStr(operand_str)));
		}
		else if(SyntaxPtg::extract_comma(first, last))
		{
			++cols;
		}
		else if(SyntaxPtg::extract_semicolon(first, last)) // extract semicolon
		{
			++rows;
			cols = 0;
		}
		else
		{
			// EXCEPT::LE::WhatIsTheFuck("Unknown operand format in PtgArray.", __FUNCTION__);
		}

	}

}


} // namespace XLS

