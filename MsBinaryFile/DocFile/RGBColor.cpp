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

#include <sstream>
#include "RGBColor.h"

namespace DocFileFormat
{
	RGBColor::RGBColor( _UINT32 cv, ByteOrder order )
	{
		unsigned char bytes[4];
		bytes[0] = cv & 0x000000FF;
		bytes[1] = (cv >> 8) & 0x000000FF;
		bytes[2] = (cv >> 16) & 0x000000FF;
		bytes[3] = (cv >> 24) & 0x000000FF;

		std::wstringstream rgbColor6, rgbColor8;

		if( order == RedFirst )
		{
			this->Red = bytes[0];
			this->Green = bytes[1];
			this->Blue = bytes[2];
			this->Alpha = bytes[3];

			rgbColor6 << boost::wformat( L"%02x%02x%02x" ) %  Red % Green % Blue;
			rgbColor8 << boost::wformat( L"%02x%02x%02x%02x" ) % Red % Green % Blue % Alpha;

			SixDigitHexCode     = rgbColor6.str();
			EightDigitHexCode   = rgbColor8.str();
		}
		else if ( order == RedLast )
		{
			this->Red = bytes[2];
			this->Green = bytes[1];
			this->Blue = bytes[0];
			this->Alpha = bytes[3];

			rgbColor6 << boost::wformat( L"%02x%02x%02x" ) % Red % Green % Blue;
			rgbColor8 << boost::wformat( L"%02x%02x%02x%02x" ) % Red % Green % Blue % Alpha;

			SixDigitHexCode     = rgbColor6.str();
			EightDigitHexCode   = rgbColor8.str();
		}
	}
}
