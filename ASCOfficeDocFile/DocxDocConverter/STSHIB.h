#pragma once

#include "LPStshiGrpPrl.h"

//!!!A structure that has no effect and MUST be ignored.!!!

namespace ASCDocFileFormat
{
	class STSHIB: public IOperand
	{
	private:
		LPStshiGrpPrl grpprlChpStandard;
		LPStshiGrpPrl grpprlPapStandard;

		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:
		STSHIB():
		  grpprlChpStandard(), grpprlPapStandard(), bytes(NULL), sizeInBytes(0)
		  {
		  }

		  explicit STSHIB( const LPStshiGrpPrl& _grpprlChpStandard, const LPStshiGrpPrl& _grpprlPapStandard ):
		  grpprlChpStandard(_grpprlChpStandard), grpprlPapStandard(_grpprlPapStandard), bytes(NULL), sizeInBytes(0)
		  {
			  this->sizeInBytes = ( grpprlChpStandard.Size() + grpprlPapStandard.Size() );

			  this->bytes = new unsigned char[this->sizeInBytes];

			  if ( this->bytes != NULL )
			  {
				  memset( this->bytes, 0, this->sizeInBytes );
				  memcpy( this->bytes, (unsigned char*)(this->grpprlChpStandard), this->grpprlChpStandard.Size() );
				  memcpy( ( this->bytes + this->grpprlChpStandard.Size() ), (unsigned char*)(this->grpprlPapStandard), this->grpprlPapStandard.Size() );
			  }
		  }

		  STSHIB( const STSHIB& _stshib ):
		  grpprlChpStandard(_stshib.grpprlChpStandard), grpprlPapStandard(_stshib.grpprlPapStandard), bytes(NULL), sizeInBytes(_stshib.sizeInBytes)
		  {
			  this->bytes = new unsigned char[this->sizeInBytes];

			  if ( this->bytes != NULL )
			  {
				  memset( this->bytes, 0, this->sizeInBytes );
				  memcpy( this->bytes, _stshib.bytes, this->sizeInBytes );
			  }
		  }

		  virtual ~STSHIB()
		  {
			  RELEASEARRAYOBJECTS( bytes );
		  }

		  virtual operator unsigned char*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const unsigned char*() const
		  {
			  return (const unsigned char*)this->bytes;
		  }

		  virtual unsigned int Size() const
		  {
			  return this->sizeInBytes;
		  }
	};
}