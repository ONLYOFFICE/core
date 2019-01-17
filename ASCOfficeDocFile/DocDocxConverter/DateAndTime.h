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

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
  class DateAndTime
  {
    private:
	  /// minutes (0-59)
      short mint;
      /// hours (0-23)
      short hr;
      /// day of month (1-31)
      short dom;
      /// month (1-12)
      short mon;
      /// year (1900-2411)-1900
      short yr;
      /// weekday<br/>
      /// 0 Sunday
      /// 1 Monday
      /// 2 Tuesday
      /// 3 Wednesday
      /// 4 Thursday
      /// 5 Friday
      /// 6 Saturday
      short wdy;

    public:
		DateAndTime()
		{
			setDefaultValues();
		}
		DateAndTime( unsigned int val )
		{
			DateAndTime((unsigned char*)&val, 4);
		}
		DateAndTime( unsigned char* bytes, int size )
		{
			if ( size == 4 )
			{
			  this->mint    = (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 0, 6 );
			  this->hr      = (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 6, 5 );
			  this->dom     = (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 11, 5 );
			  this->mon     = (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 16, 4 );
			  this->yr      = (short)( 1900 + FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 20, 9 ) );
			  this->wdy     = (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 29, 3 );
			}
			else
			{
			  //throw new ByteParseException("Cannot parse the struct DTTM, the length of the struct doesn't match");
			}
		}
#if defined(_WIN32) || defined(_WIN64)
	  SYSTEMTIME ToSYSTEMTIME()
      {
	    SYSTEMTIME st = { 0 };
		
		if ( ( this->yr == 1900 ) && ( this->mon == 0 ) && ( this->dom == 0 ) &&
			 ( this->hr == 0 ) && ( this->mint == 0 ) )
        {
		  st.wYear = 1900;
		  st.wMonth = 1;
		  st.wDayOfWeek = 0;
		  st.wDay = 1;
		  st.wHour = 0;
		  st.wMinute = 0;
		  st.wSecond = 0;
		  st.wMilliseconds = 0;

		  return st;
        }
        else
        {
          st.wYear = this->yr;
		  st.wMonth = this->mon;
		  st.wDayOfWeek = 0;
		  st.wDay = this->dom;
		  st.wHour = this->hr;
		  st.wMinute = this->mint;
		  st.wSecond = 0;
		  st.wMilliseconds = 0;

		  return st;
        } 
      }
#endif
      private:
	    void setDefaultValues()
        {
          this->dom = 0;
          this->hr = 0;
          this->mint = 0;
          this->mon = 0;
          this->wdy = 0;
          this->yr = 0;
        }
  };
}
