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