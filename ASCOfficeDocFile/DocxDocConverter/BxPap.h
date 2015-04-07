#pragma once

namespace ASCDocFileFormat
{
  class BxPap: public IStreamWriter
  {
  public:
    static const unsigned char RESERVED_SIZE = 12;  

  private:
	unsigned char bOffset; //An unsigned integer that specifies the offset of a PapxInFkp in a PapxFkp. The offset of the PapxInFkp is bOffset*2.
	unsigned char reserved[RESERVED_SIZE];

  public:
  	BxPap():
    bOffset(0)
	{
	  memset( this->reserved, 0, RESERVED_SIZE );
	}

	BxPap( unsigned char _bOffset, const unsigned char* _reserved = NULL ):
    bOffset(_bOffset)
	{
	  if ( _reserved != NULL )
	  {
	    memcpy( this->reserved, _reserved, RESERVED_SIZE ); 
	  }
	  else
	  {
	    memset( this->reserved, 0, RESERVED_SIZE );
	  }
	}

	BxPap( const BxPap& bxp ):
	bOffset(bxp.bOffset)
    {
	  memcpy( this->reserved, bxp.reserved, RESERVED_SIZE );  
	}

	virtual ~BxPap()
	{
	}

	bool operator == ( const BxPap& bxp )
	{
	  return ( ( this->bOffset == bxp.bOffset ) && ( memcmp( this->reserved, bxp.reserved, RESERVED_SIZE ) == 0 ) );
	}

	bool operator != ( const BxPap& bxp )
	{
	  return !( *this == bxp );
	}

	BxPap& operator = ( const BxPap& bxp )
	{
	  if ( *this != bxp )
	  {
	    this->bOffset = bxp.bOffset;
	    memcpy( this->reserved, bxp.reserved, RESERVED_SIZE );
	  }

	  return *this;
	}

	virtual unsigned char* GetBytes( unsigned long* size ) const
    {
      unsigned char* bytes = NULL;

	  if ( size != NULL )
	  {
		*size = sizeof(this->bOffset) + RESERVED_SIZE;
		bytes = new unsigned char[*size];
		
        if ( bytes != NULL )
		{
	      memset( bytes, 0, *size );
		  bytes[0] = this->bOffset;

          memcpy( ( bytes + sizeof(this->bOffset) ), this->reserved, RESERVED_SIZE );
		}
	  }

	  return bytes;
    }

	unsigned long Size() const
	{
	  return ( sizeof(this->bOffset) + RESERVED_SIZE );
	}

	unsigned char GetOffset() const
	{
	  return this->bOffset;
	}
  };
}