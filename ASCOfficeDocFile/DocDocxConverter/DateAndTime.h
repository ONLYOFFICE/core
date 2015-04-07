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
      /// Creates a new DateAndTime with default values
      DateAndTime()
      {
        setDefaultValues();
      }

      /// Parses the unsigned char sto retrieve a DateAndTime
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
