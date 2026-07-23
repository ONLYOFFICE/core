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

#include "xlsx_cell_format.h"

namespace cpdoccore {
namespace oox {

xlsx_cell_format::xlsx_cell_format() : cell_type_(XlsxCellType::null)
{}

XlsxCellType::type xlsx_cell_format::get_cell_type() const
{
    return cell_type_;
}

void xlsx_cell_format::set_cell_type(XlsxCellType::type type)
{
    cell_type_ = type;
}

int xlsx_cell_format::get_num_format() const
{
    return num_format_;
}

void xlsx_cell_format::set_num_format(int numFmt)
{
    num_format_ = numFmt;
}

namespace {

	int odf2buildin_map[] = {0,0,0,0,0,0,0,0};
//{
//    {L"", 0},
//    {L"float", 0},
//    {L"currency", 0},
//    {L"percentage", 10},
//    {L"date", 15},
//    {L"time", 21},
//    {L"boolean", 0},
//    {L"string", 49}
//};
//    
}

int odf_string_to_build_in(const int odf_type_value)
{
    return odf2buildin_map[odf_type_value];    
}

}
}
