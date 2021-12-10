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
#include <boost/algorithm/string.hpp>

#include "stylecellprotect.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 

	std::wostream & operator << (std::wostream & _Wostream, const style_cell_protect & _Val)
	{
		switch (_Val.get_type())
		{
		case style_cell_protect::none:
			_Wostream << L"none";
			break;
		case style_cell_protect::formula_hidden:
			_Wostream << L"formula-hidden";
			break;
		case style_cell_protect::hidden_and_protected:
			_Wostream << L"hidden-and-protected";
			break;
		case style_cell_protect::protected_:
			_Wostream << L"protected";
			break;
		case style_cell_protect::protected_formula_hidden:
			_Wostream << L"protected formula-hidden";
			break;
		default:
			break;
		}
		return _Wostream;
	}

	style_cell_protect style_cell_protect::parse(const std::wstring & Str)
	{
		std::wstring tmp = Str;
		boost::algorithm::to_lower(tmp);

		if (tmp == L"formula_hidden")
			return style_cell_protect(formula_hidden);
		else if (tmp == L"hidden-and-protected")
			return style_cell_protect(hidden_and_protected);
		else if (tmp == L"protected")
			return style_cell_protect(protected_);
		else if (tmp == L"protected formula-hidden" ||
				 tmp == L"formula-hidden protected")
			return style_cell_protect(protected_formula_hidden);
		else
		{
			return style_cell_protect(none);
		}
	}


} }
