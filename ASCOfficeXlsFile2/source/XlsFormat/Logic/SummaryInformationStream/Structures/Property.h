/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include <boost/shared_ptr.hpp>

namespace OLEPS
{

// The only necessity of this class is to place all properties into a single array
class Property
{
public:
	Property();
	virtual ~Property() = 0;

	enum ValueTypes
	{
		VT_I2 = 0x0002,
		VT_I4 = 0x0003,
		VT_R4 = 0x0004,
		VT_R8 = 0x0005,
		VT_CY = 0x0006,
		VT_DATE = 0x0007,
		VT_BSTR = 0x0008,
		VT_ERROR = 0x000A,
		VT_BOOL = 0x000B,
		VT_DECIMAL = 0x000E,
		VT_I1 = 0x0010,
		VT_UI1 = 0x0011,
		VT_UI2 = 0x0012,
		VT_UI4 = 0x0013,
		VT_I8 = 0x0014,
		VT_UI8 = 0x0015,
		VT_INT = 0x0016,
		VT_UINT = 0x0017,
		VT_LPSTR = 0x001E,
		VT_LPWSTR = 0x001F,
		VT_FILETIME = 0x0040,
		VT_CF = 0x0047,
		VT_CLSID = 0x0047,
	};

};

typedef boost::shared_ptr<Property> PropertyPtr;

} // namespace OLEPS
