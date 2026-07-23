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

#include "presetclass.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types {

	preset_class preset_class::parse(const std::wstring& Str)
	{
		std::wstring tmp = Str;
		boost::algorithm::to_lower(tmp);

			 if (tmp == L"custom")			return preset_class(custom);
		else if (tmp == L"entrance")		return preset_class(entrance);
		else if (tmp == L"exit")			return preset_class(exit);
		else if (tmp == L"emphasis")		return preset_class(emphasis);
		else if (tmp == L"motion-path")		return preset_class(motion_path);
		else if (tmp == L"ole-action")		return preset_class(ole_action);
		else if (tmp == L"media-call")		return preset_class(media_call);
		else
		{
			return preset_class(custom);
		}
	}

	std::wostream& operator << (std::wostream& _Wostream, const preset_class& _Val)
	{
		switch (_Val.get_type())
		{
		case preset_class::custom:			_Wostream << L"custom"; break;
		case preset_class::entrance:		_Wostream << L"entrance"; break;
		case preset_class::exit:			_Wostream << L"exit"; break;
		case preset_class::emphasis:		_Wostream << L"emphasis"; break;
		case preset_class::motion_path:		_Wostream << L"motion-path"; break;
		case preset_class::ole_action:		_Wostream << L"ole-action"; break;
		case preset_class::media_call:		_Wostream << L"media-call"; break;
		}

		return _Wostream;
	}

}
}