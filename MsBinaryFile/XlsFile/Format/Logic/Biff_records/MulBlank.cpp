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

#include "MulBlank.h"

namespace XLS
{

std::wstring getColAddress(int col)
{
    static const size_t r = (L'Z' - L'A' + 1);
    std::wstring res;
    size_t r0 = col / r;

    if (r0 > 0)
    {
        const std::wstring rest = getColAddress(col - r*r0);
        const std::wstring res = getColAddress(r0-1) + rest;
        return res;
    }
    else
        return std::wstring(1, (wchar_t)(L'A' + col));
}



std::wstring getRowAddress(int row)
{
    return std::to_wstring(row + 1);
}

std::wstring getColRowRef(int col, int row)
{
	//(0,0) = A1

	return getColAddress(col) + getRowAddress(row);
}

MulBlank::MulBlank()
{
}

MulBlank::~MulBlank()
{
}

BaseObjectPtr MulBlank::clone()
{
	return BaseObjectPtr(new MulBlank(*this));
}

void MulBlank::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	// A little hack to extract colLast before it is used
	record.skipNunBytes(record.getDataSize() - sizeof(unsigned short));
	record >> colLast;
	record.resetPointerToBegin();
	//------------------
	record >> rw >> colFirst;

	int r = rw;
	int colL = colLast;
	int colF = colFirst;

	rgixfe.load(record, colLast - colFirst + 1);

	record.skipNunBytes(sizeof(unsigned short));
}
void MulBlank::writeFields(CFRecord& record)
{
    record << rw << colFirst;
    for(auto i:rgixfe.rgixfe)
        record << i;
    record << colLast;
}
const int MulBlank::GetRow() const
{
	return static_cast<unsigned short>(rw);
}

const int MulBlank::GetColumn() const
{
	return static_cast<unsigned short>(colFirst);
}

int MulBlank::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = GetRow();

		for (long i = colFirst; i <= colLast; i++)
		{
			std::wstring ref =  getColRowRef(i, row);
			CP_XML_NODE(L"c")
			{
				CP_XML_ATTR(L"r", ref);
				
				if ( ((i-colFirst) < rgixfe.rgixfe.size()) && (rgixfe.rgixfe[i - colFirst] < global_info_->cellStyleXfs_count +
																							 global_info_->cellXfs_count))
				{
					if (rgixfe.rgixfe[i - colFirst] >= global_info_->cellStyleXfs_count)
					{
						CP_XML_ATTR(L"s", rgixfe.rgixfe[i - colFirst] - global_info_->cellStyleXfs_count);
					}
					else if (rgixfe.rgixfe[i - colFirst] < global_info_->cellStyleXfs_count)
					{
						//CP_XML_ATTR(L"s", rgixfe.rgixfe[i - colFirst]);
					}		
				}
				else if  ((rgixfe.common_ixfe > 0) && (rgixfe.common_ixfe >= global_info_->cellStyleXfs_count))
				{
					int st = (int)rgixfe.common_ixfe - global_info_->cellStyleXfs_count;
					CP_XML_ATTR(L"s", rgixfe.common_ixfe - global_info_->cellStyleXfs_count);
				}
			}		
		}
	}
	return 0;
}



BiffStructurePtr IXFCellMulBlankSpecial::clone()
{
	return BiffStructurePtr(new IXFCellMulBlankSpecial(*this));
}




void IXFCellMulBlankSpecial::load(CFRecord& record, const size_t num_cells)
{
	size_t sz = (record.getDataSize() - record.getRdPtr()-2)/2;

	unsigned short ixfe;
	for(size_t i = 0; i < (std::min)(sz, num_cells); ++i) //Lighting Load Calculation.xls - third sheet 
	{
		record >> ixfe;
		rgixfe.push_back(ixfe);
	}
	if(rgixfe.size() != 0)
	{
		common_ixfe = rgixfe[0];
		size_t j = 0;
		for(; j < rgixfe.size(); ++j)
		{
			if(rgixfe[j] != common_ixfe) break;
		}
		if(j == rgixfe.size())
		{
			rgixfe.clear();
		}
	}
	else
	{
		common_ixfe = 0; // just to avoid warnings
	}
}



} // namespace XLS

