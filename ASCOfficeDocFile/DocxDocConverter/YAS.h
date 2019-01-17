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

namespace ASCDocFileFormat
{
  class YAS
  {
    private:
      short value;

    public:
      static const short MAX_YAS_VALUE = 31680;
	  static const short MIN_YAS_VALUE = -31680;

	  YAS():
	  value(0)
	  {
	  }

	  explicit YAS( short _value )
	  {
	    if ( _value < MIN_YAS_VALUE )
	    {
	      this->value = MIN_YAS_VALUE;
	    }
	    else if ( _value > MAX_YAS_VALUE )
	    {
	      this->value = MAX_YAS_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  YAS( const YAS& xas ):
	  value(xas.value)
	  {
	  }

	  operator short() const
	  {
	    return this->value;
	  }
  };

  class YAS_nonNeg
  {
    private:
      unsigned short value;

    public:
      static const unsigned short MAX_YAS_NONNEG_VALUE = 31680;
	  
	  YAS_nonNeg():
	  value(0)
	  {
	  }

	  explicit YAS_nonNeg( unsigned short _value )
	  {
        if ( _value > MAX_YAS_NONNEG_VALUE )
	    {
	      this->value = MAX_YAS_NONNEG_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  YAS_nonNeg( const YAS_nonNeg& xas ):
	  value(xas.value)
	  {
	  }

	  operator unsigned short() const
	  {
	    return this->value;
	  }
  };

  class YAS_plusOne
  {
    private:
      short value;

    public:
      static const short MAX_YAS_PLUSONE_VALUE = 31681;
	  static const short MIN_YAS_PLUSONE_VALUE = -31679;
	  
	  YAS_plusOne():
	  value(0)
	  {
	  }

	  explicit YAS_plusOne( short _value )
	  {
	    if ( _value < MIN_YAS_PLUSONE_VALUE )
	    {
	      this->value = MIN_YAS_PLUSONE_VALUE;
	    }
	    else if ( _value > MAX_YAS_PLUSONE_VALUE )
	    {
	      this->value = MAX_YAS_PLUSONE_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  YAS_plusOne( const YAS_plusOne& xas ):
	  value(xas.value)
	  {
	  }

	  operator short() const
	  {
	    return this->value;
	  }
  };
}