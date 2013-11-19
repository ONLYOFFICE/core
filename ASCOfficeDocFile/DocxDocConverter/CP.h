#pragma once

namespace AVSDocFileFormat
{
  class CP
  {
    private:
	  unsigned int cp;

    public:
	  CP():
	  cp(0)
	  {
	  }

	  explicit CP( unsigned int _cp )
	  {
	    if ( _cp >= 0x7FFFFFFF )
		{
		  this->cp = 0x7FFFFFFE;
		}
		else
		{
		  this->cp = _cp;
		}
	  }

	  ~CP()
	  {
	  }

	  bool operator == ( const CP& _cp )
	  {
	    return ( this->cp == _cp.cp );
	  }

	  bool operator != ( const CP& _cp )
	  {
	    return ( this->cp != _cp.cp );
	  }

	  bool operator < ( const CP& _cp )
	  {
	    return ( this->cp < _cp.cp );
	  }

	  bool operator <= ( const CP& _cp )
	  {
	    return ( this->cp <= _cp.cp );
	  }
	  
      bool operator > ( const CP& _cp )
	  {
	    return ( this->cp > _cp.cp );
	  }
	  
      bool operator >= ( const CP& _cp )
	  {
	    return ( this->cp >= _cp.cp );
	  }
	  
      CP& operator = ( const CP& _cp )
	  {
	    if ( this != &_cp )
		{
		  this->cp = _cp.cp;  
		}

		return *this;
	  }

	  operator unsigned int() const
	  {
	    return this->cp;
	  }
  };
}