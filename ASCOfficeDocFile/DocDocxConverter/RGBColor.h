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

#include "../../DesktopEditor/common/Types.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../Common/DocxFormat/Source/XML/stringcommon.h"

namespace DocFileFormat
{
	typedef enum _ByteOrder 
	{
		RedFirst,
		RedLast
	} ByteOrder;

	class RGBColor
	{
	private:
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
		unsigned char Alpha;

	public:
		wstring SixDigitHexCode;
		wstring EightDigitHexCode;

		RGBColor( int cv, ByteOrder order )
		{
			unsigned char* bytes = FormatUtils::GetBytes( cv );

//			wchar_t rgbColor6[7];
//			wchar_t rgbColor8[9];
            CString rgbColor6;
            CString rgbColor8;

			if( order == RedFirst )
			{
				//R
				this->Red = bytes[0];
				//G
				this->Green = bytes[1];
				//B
				this->Blue = bytes[2];
				//Alpha
				this->Alpha = bytes[3];

                rgbColor6.Format( _T( "%02x%02x%02x" ),     /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
                rgbColor8.Format( _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

                SixDigitHexCode     = string2std_string( rgbColor6 );
                EightDigitHexCode   = string2std_string( rgbColor8 );
			}
			else if ( order == RedLast )
			{
				//R
				this->Red = bytes[2];
				//G
				this->Green = bytes[1];
				//B
				this->Blue = bytes[0];
				//Alpha
				this->Alpha = bytes[3];

                rgbColor6.Format( _T( "%02x%02x%02x" ),     /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
                rgbColor8.Format( _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

                SixDigitHexCode     = string2std_string( rgbColor6 );
                EightDigitHexCode   = string2std_string( rgbColor8 );
            }

			RELEASEARRAYOBJECTS( bytes );
		}
	};
}
