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

#include "styleprint.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_print & _Val)
{
	std::wstring result;
	if (_Val.bAnnotations)	result += L" annotations";
	if (_Val.bCharts)		result += L" charts";
	if (_Val.bDrawings)		result += L" drawings";
	if (_Val.bFormulas)		result += L" formulas";
	if (_Val.bHeaders)		result += L" headers";
	if (_Val.bGrid)			result += L" grid";
	if (_Val.bObjects)		result += L" objects";
	if (_Val.bZeroValues)	result += L" zero-values";

	if (!result.empty())
		_Wostream << result.substr(1);

    return _Wostream;    
}

style_print style_print::parse(const std::wstring & Str)
{
	if (Str.empty()) return style_print();

	style_print result(false);
	
	std::vector<std::wstring> values;   
	boost::algorithm::split(values, Str, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
 
	for (size_t i = 0; i < values.size(); ++i)
	{
		boost::algorithm::to_lower(values[i]);
		
		if (values[i] == L"annotations")		result.bAnnotations = true;
		else if (values[i] == L"charts")		result.bCharts = true;
		else if (values[i] == L"drawings")		result.bDrawings = true;
		else if (values[i] == L"formulas")		result.bFormulas = true;
		else if (values[i] == L"headers")		result.bHeaders = true;
		else if (values[i] == L"grid")			result.bGrid = true;
		else if (values[i] == L"objects")		result.bObjects = true;
		else if (values[i] == L"zero-values")	result.bZeroValues = true;
	}

	return result;
}

} }
