#pragma once

namespace AVSDocFileFormat
{
  class XAS
  {
    private:
      short value;

    public:
      static const short MAX_XAS_VALUE = 31680;
	  static const short MIN_XAS_VALUE = -31680;

	  XAS():
	  value(0)
	  {
	  }

	  explicit XAS( short _value )
	  {
	    if ( _value < MIN_XAS_VALUE )
	    {
	      this->value = MIN_XAS_VALUE;
	    }
	    else if ( _value > MAX_XAS_VALUE )
	    {
	      this->value = MAX_XAS_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  XAS( const XAS& xas ):
	  value(xas.value)
	  {
	  }

	  operator short() const
	  {
	    return this->value;
	  }
  };

  class XAS_nonNeg
  {
    private:
      unsigned short value;

    public:
      static const unsigned short MAX_XAS_NONNEG_VALUE = 31680;
	  
	  XAS_nonNeg():
	  value(0)
	  {
	  }

	  explicit XAS_nonNeg( unsigned short _value )
	  {
        if ( _value > MAX_XAS_NONNEG_VALUE )
	    {
	      this->value = MAX_XAS_NONNEG_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  XAS_nonNeg( const XAS_nonNeg& xas ):
	  value(xas.value)
	  {
	  }

	  operator unsigned short() const
	  {
	    return this->value;
	  }
  };

  class XAS_plusOne
  {
    private:
      short value;

    public:
      static const short MAX_XAS_PLUSONE_VALUE = 31681;
	  static const short MIN_XAS_PLUSONE_VALUE = -31679;
	  
	  XAS_plusOne():
	  value(0)
	  {
	  }

	  explicit XAS_plusOne( short _value )
	  {
	    if ( _value < MIN_XAS_PLUSONE_VALUE )
	    {
	      this->value = MIN_XAS_PLUSONE_VALUE;
	    }
	    else if ( _value > MAX_XAS_PLUSONE_VALUE )
	    {
	      this->value = MAX_XAS_PLUSONE_VALUE;
	    }
	    else
	    {
	      this->value = _value;
	    }
	  }

	  XAS_plusOne( const XAS_plusOne& xas ):
	  value(xas.value)
	  {
	  }

	  operator short() const
	  {
	    return this->value;
	  }
  };
}