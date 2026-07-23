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
    auto tempcols = 0;
    auto temprows = 0;
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        DColunByteU cols_xls;
        DRw rows_xls;
        record >> cols_xls >> rows_xls;
        cols = cols_xls;
        rows = rows_xls;
        tempcols = cols_xls+1;
        temprows = rows_xls+1;
    }
    else
    {
        record >> cols >> rows;
        tempcols = cols;
        temprows = rows;
		--cols;
		--rows;
    }
    for(int i = 0; i < (tempcols) * (temprows); ++i)
	{
		if (record.getRdPtr() >= record.getDataSize())
		{
			unsigned char rec_type = SerAr::SerType::typeSerNil;
			SerArPtr ser(SerAr::createSerAr(rec_type));
			array_.push_back(ser);
			continue;
		}
		unsigned char rec_type;
		record >> rec_type;
        if (record.getGlobalWorkbookInfo()->Version >= 0x0800)
        {
            switch(rec_type)
            {
                case 0:
                    rec_type = SerAr::SerType::typeSerNum;
                    break;
                case 1:
                    rec_type = SerAr::SerType::typeSerStr;
                    break;
                case 2:
                    rec_type = SerAr::SerType::typeSerBool;
                case 4:
                    rec_type = SerAr::SerType::typeSerErr;
                    break;
                default:
                    rec_type = SerAr::SerType::typeSerNil;
                    break;
            }
        }
		SerArPtr ser(SerAr::createSerAr(rec_type));
        if(ser.get())
        {
            record >> *ser;
            array_.push_back(ser);
        }
	}
}

void PtgExtraArray::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		DColunByteU cols_xls;
		DRw rows_xls;
		cols_xls = cols;
		rows_xls = rows;
		record << cols_xls << rows_xls;
	}
	else
	{
		cols++;
		rows++;
        record << rows << cols;
	}
	for (auto& item : array_)
	{
		record << *item;
	}
}

const std::wstring PtgExtraArray::toString() const
{
	std::wstring ret_val;
	unsigned char col_cnt = cols;

	if (array_.empty()) return L"";

	for(std::vector<SerArPtr>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		auto tempVal = (*it)->toString();
		if(tempVal.empty())
			tempVal = L"#N/A";
		ret_val += tempVal;
		if (col_cnt)
		{
			ret_val += L',';
			col_cnt--;
		}
		else
		{
			ret_val += L';';
			col_cnt = cols;
		}
	}
	auto tempVal = array_.back()->toString();
	if(tempVal.empty())
			tempVal = L"#N/A";
	ret_val += tempVal;
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

