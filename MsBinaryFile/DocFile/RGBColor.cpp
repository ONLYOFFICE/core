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
