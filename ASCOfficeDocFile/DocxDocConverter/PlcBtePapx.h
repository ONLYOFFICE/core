#pragma once

namespace ASCDocFileFormat
{
  class PlcBtePapx: public IStreamWriter
  {
  private:
    vector<unsigned int> aFC;
	vector<unsigned int> aPnBtePapx;
 
  public:
	PlcBtePapx( const vector<unsigned int>& _aFC, const vector<unsigned int>& _aPnBtePapx ):
    aFC(_aFC), aPnBtePapx(_aPnBtePapx)
	{
	}

	virtual ~PlcBtePapx()
	{
	}

	virtual byte* GetBytes( unsigned long* size ) const
    {
      byte* bytes = NULL;

	  if ( size != NULL )
	  {
		*size = ( ( sizeof(unsigned int) * this->aFC.size() ) + ( sizeof(unsigned int) * this->aPnBtePapx.size() ) );
		bytes = new byte[*size];
		
        if ( bytes != NULL )
		{
	      memset( bytes, 0, *size );

		  unsigned int i = 0;

		  for ( i = 0; i < this->aFC.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( i * sizeof(unsigned int) ) ), (int)this->aFC[i] );
		  }

		  for ( i = 0; i < this->aPnBtePapx.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( sizeof(unsigned int) * this->aFC.size() ) + ( i * sizeof(unsigned int) ) ), (int)this->aPnBtePapx[i] );
		  }
		}
	  }

	  return bytes;
    }
  };
}