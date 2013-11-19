#pragma once

namespace AVSDocFileFormat
{
  class PlcBteChpx: public IStreamWriter
  {
  private:
    vector<unsigned int> aFC;
	vector<unsigned int> aPnBteChpx;
 
  public:
	PlcBteChpx( const vector<unsigned int>& _aFC, const vector<unsigned int>& _aPnBteChpx ):
    aFC(_aFC), aPnBteChpx(_aPnBteChpx)
	{
	}

	virtual ~PlcBteChpx()
	{
	}

	virtual byte* GetBytes( unsigned long* size ) const
    {
      byte* bytes = NULL;

	  if ( size != NULL )
	  {
		*size = ( ( sizeof(unsigned int) * this->aFC.size() ) + ( sizeof(unsigned int) * this->aPnBteChpx.size() ) );
		bytes = new byte[*size];
		
        if ( bytes != NULL )
		{
	      memset( bytes, 0, *size );

		  unsigned int i = 0;

		  for ( i = 0; i < this->aFC.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( i * sizeof(unsigned int) ) ), (int)this->aFC[i] );
		  }

		  for ( i = 0; i < this->aPnBteChpx.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( sizeof(unsigned int) * this->aFC.size() ) + ( i * sizeof(unsigned int) ) ), (int)this->aPnBteChpx[i] );
		  }
		}
	  }

	  return bytes;
    }
  };
}