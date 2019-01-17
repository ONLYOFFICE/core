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
#pragma once

#include "../../../DesktopEditor/common/Types.h"
namespace OfficeArt
{
	enum COMPRESSION_METHOD
	{
		COMPRESSION_METHOD_DEFLATE = 0x00,
		COMPRESSION_METHOD_NO_COMPRESSION = 0xFE
	};

	struct POINT
	{
		int x;
		int y;

		POINT():
		x(0), y(0)
		{
		}

		explicit POINT( int _x, int _y ):
		x(_x), y(_y)
		{
		}
	};

	struct RECT
	{
		int left;
		int top;
		int right;
		int bottom;

		RECT():
		left(0), top(0), right(0), bottom(0)
		{
		}

		explicit RECT( int _left, int _top, int _right, int _bottom ):
		left(_left), top(_top), right(_right), bottom(_bottom)
		{
		}
	};

	class EnglishMetricUnit
	{
	private:
		long value;

	public:
		explicit EnglishMetricUnit( long _value = 0 ):
		value(_value)
		{
		}

		void FromTwip( long _value )
		{
			this->value = ( _value * 65 );
		}

		void FromHalfPoint( long _value )
		{
			this->value = ( _value * 6350 );
		}

		void FromPoint( long _value )
		{
			this->value = ( _value * 12700 );
		}

		void FromInch( long _value )
		{
			this->value = ( _value * 914400 );
		}

		long GetValue() const
		{
			return this->value;
		}

		long ToTwip() const
		{
			return ( this->value / 65 );
		}

		long ToHalfPoint() const
		{
			return ( this->value / 6350 );
		}

		long ToPoint() const
		{
			return ( this->value / 12700 );
		}

		long ToInch() const
		{
			return ( this->value / 914400 );
		}
	};

	typedef unsigned int MSOSPID;
}

namespace OfficeArt
{
	struct ADJH
	{
		int handle[9];
	};

	struct SG
	{
		int sg[2];
	};

}
