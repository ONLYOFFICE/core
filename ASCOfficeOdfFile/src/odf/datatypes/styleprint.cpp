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
