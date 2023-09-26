/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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