#pragma once

#include "Brc.h"
#include "ItcFirstLim.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	class TableBrcOperand: public IOperand
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 12;
		unsigned char bytes[SIZE_IN_BYTES];

	public:
		TableBrcOperand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );
		}

		explicit TableBrcOperand( const ItcFirstLim& _itc, unsigned char _bordersToApply, const Brc& _brc )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );

			FormatUtils::SetBytes( ( this->bytes + sizeof(unsigned char) ), (short)_itc );
			this->bytes[sizeof(unsigned char) + sizeof(short)] = _bordersToApply;
			memcpy( ( this->bytes + sizeof(unsigned char) + sizeof(short) + sizeof(unsigned char) ), (unsigned char*)_brc, _brc.Size() );
		}

		TableBrcOperand( const TableBrcOperand& _tableBrcOperand )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			memcpy( this->bytes, _tableBrcOperand.bytes, SIZE_IN_BYTES );
		}

		virtual ~TableBrcOperand()
		{
		}

		virtual unsigned int Size() const
		{
			return (unsigned int)sizeof(this->bytes);  
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}
	};
}