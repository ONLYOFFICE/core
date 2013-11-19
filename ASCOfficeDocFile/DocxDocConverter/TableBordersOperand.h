#pragma once

#include "Brc.h"
#include "Prl.h"

namespace AVSDocFileFormat
{
	class TableBordersOperand: public IOperand
	{
	private:
		byte cb;
		Brc brcTop;
		Brc brcLeft;
		Brc brcBottom;
		Brc brcRight;
		Brc brcHorizontalInside;
		Brc brcVerticalInside;

		byte* bytes;

		void Initialize()
		{
			RELEASEARRAYOBJECTS (bytes);

			this->cb = ( this->brcTop.Size() +  this->brcLeft.Size() + this->brcBottom.Size() + 
				this->brcRight.Size() + this->brcHorizontalInside.Size() + this->brcVerticalInside.Size() );

			this->bytes = new byte[this->cb + 1];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, ( this->cb + 1 ) );

				this->bytes[0] = this->cb;

				memcpy( ( this->bytes + sizeof(this->cb) ), (byte*)this->brcTop, this->brcTop.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() ), (byte*)this->brcLeft, this->brcLeft.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() ), (byte*)this->brcBottom, this->brcBottom.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() ), (byte*)this->brcRight, this->brcRight.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() + this->brcRight.Size() ), (byte*)this->brcHorizontalInside, this->brcHorizontalInside.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() + this->brcRight.Size() + this->brcHorizontalInside.Size() ), (byte*)this->brcVerticalInside, this->brcVerticalInside.Size() );
			}
		}

	public:
		TableBordersOperand():
		  bytes(NULL), cb(0)
		  {
			  this->Initialize();
		  }

		  explicit TableBordersOperand( const Brc& _brcTop, const Brc& _brcLeft, const Brc& _brcBottom,
			  const Brc& _brcRight, const Brc& _brcHorizontalInside, const Brc& _brcVerticalInside ):
		  brcTop(_brcTop), brcLeft(_brcLeft), brcBottom(_brcBottom), brcRight(_brcRight),
			  brcHorizontalInside(_brcHorizontalInside), brcVerticalInside(_brcVerticalInside),
			  bytes(NULL), cb(0)
		  {
			  this->Initialize();  
		  }

		  TableBordersOperand( const TableBordersOperand& _tableBordersOperand ):
		  brcTop(_tableBordersOperand.brcTop), brcLeft(_tableBordersOperand.brcLeft), 
			  brcBottom(_tableBordersOperand.brcBottom), brcRight(_tableBordersOperand.brcRight),
			  brcHorizontalInside(_tableBordersOperand.brcHorizontalInside), 
			  brcVerticalInside(_tableBordersOperand.brcVerticalInside), bytes(NULL), cb(0)
		  {
			  this->bytes = new byte[this->cb + 1];

			  memcpy_s( this->bytes, ( this->cb + 1 ), _tableBordersOperand.bytes, ( _tableBordersOperand.cb + 1 ) );
		  }

		  explicit TableBordersOperand( const Prl& _prl ):
		  bytes(NULL), cb(0)
		  {
			  unsigned long prlSize = 0;

			  byte* bytes = _prl.GetBytes( &prlSize );

			  if ( bytes != NULL )
			  {
				  this->brcTop = Brc( bytes +  0 + sizeof(unsigned short) + sizeof(byte) );
				  this->brcLeft = Brc( bytes + 8 + sizeof(unsigned short) + sizeof(byte) );
				  this->brcBottom = Brc( bytes + 16 + sizeof(unsigned short) + sizeof(byte) );
				  this->brcRight = Brc( bytes + 24 + sizeof(unsigned short) + sizeof(byte) );
				  this->brcHorizontalInside = Brc( bytes + 32 + sizeof(unsigned short) + sizeof(byte) );
				  this->brcVerticalInside = Brc( bytes + 40 + sizeof(unsigned short) + sizeof(byte) );

				  RELEASEARRAYOBJECTS (bytes);
			  }

			  this->Initialize();
		  }

		  TableBordersOperand& operator = ( const TableBordersOperand& _tableBordersOperand )
		  {
			  if ( this != &_tableBordersOperand )
			  {
				  this->brcTop = _tableBordersOperand.brcTop;
				  this->brcLeft = _tableBordersOperand.brcLeft;
				  this->brcBottom = _tableBordersOperand.brcBottom;
				  this->brcRight = _tableBordersOperand.brcRight;
				  this->brcHorizontalInside = _tableBordersOperand.brcHorizontalInside;
				  this->brcVerticalInside = _tableBordersOperand.brcVerticalInside;

				  this->Initialize();
			  }

			  return *this;
		  }

		  void SetTopBorder( const Brc& _brcTop )
		  {
			  this->brcTop = _brcTop;

			  this->Initialize();
		  }

		  void SetLeftBorder( const Brc& _brcLeft )
		  {
			  this->brcLeft = _brcLeft;

			  this->Initialize();
		  }

		  void SetBottomBorder( const Brc& _brcBottom )
		  {
			  this->brcBottom = _brcBottom;

			  this->Initialize();
		  }

		  void SetRightBorder( const Brc& _brcRight )
		  {
			  this->brcRight = _brcRight;

			  this->Initialize();
		  }

		  void SetHorizontalInsideBorder( const Brc& _brcHorizontalInside )
		  {
			  this->brcHorizontalInside = _brcHorizontalInside;

			  this->Initialize();
		  }

		  void SetVerticalInsideBorder( const Brc& _brcVerticalInside )
		  {
			  this->brcVerticalInside = _brcVerticalInside;

			  this->Initialize();
		  }

		  virtual ~TableBordersOperand()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  virtual unsigned int Size() const
		  {
			  return ( this->cb + 1 );  
		  }

		  virtual operator byte*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const byte*() const
		  {
			  return (const byte*)this->bytes;
		  }
	};
}