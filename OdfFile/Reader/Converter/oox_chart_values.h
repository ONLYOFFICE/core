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
#pragma once 

#include <iosfwd>
#include <CPOptional.h>

                 
namespace cpdoccore {
namespace oox {

struct _oox_numLit
{
	bool present;

};
//extLst (Chart Extensibility) §21.2.2.64
//formatCode (Format Code) §21.2.2.71
//pt (Numeric Point) §21.2.2.150
//ptCount (Point Count)

//extLst (Chart Extensibility) §21.2.2.64
//formatCode (Format Code) §21.2.2.71
//pt (Numeric Point) §21.2.2.150
//ptCount (Point Count)

struct _oox_strRef
{
	bool						present = false;

	std::wstring				formula;
	int							str_cache_count = 0;
	std::vector<std::wstring>	str_cache;

};
struct _oox_numRef
{
	bool						present = false;

	std::wstring				formula;
	std::wstring				formatCode;
	bool						link_to_source = false;
	int							num_cache_count = 0;
	std::vector<std::wstring>	num_cache;

};
//extLst (Chart Extensibility) §21.2.2.64
//f (Formula) §21.2.2.65
//numCache (Number Cache)

struct _oox_chart_values
{
public:
	_oox_chart_values()
	{
		numRef_.present			= false;
		numRef_.formatCode		= L"General";
		numRef_.num_cache_count	= 0;
		numRef_.link_to_source	= true;

		strRef_.present			= false;
		strRef_.str_cache_count	= 0;

		present					= false;
	}

	bool			present;
	std::wstring	type;
	
	_oox_numRef		numRef_;
	_oox_strRef		strRef_;

};
//numLit (Number Literal) §21.2.2.122
//numRef (Number Reference) §21.2.2.123
}
}
