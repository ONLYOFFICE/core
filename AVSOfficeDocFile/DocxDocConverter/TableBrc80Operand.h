#pragma once

#include "Brc80MayBeNil.h"
#include "ItcFirstLim.h"
#include "Constants.h"

namespace AVSDocFileFormat
{
	class TableBrc80Operand: public IOperand
	{
	private:
		static const byte SIZE_IN_BYTES = 8;
		byte bytes[SIZE_IN_BYTES];

	public:
		TableBrc80Operand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );
		}

		explicit TableBrc80Operand( const ItcFirstLim& _itc, byte _bordersToApply, const Brc80MayBeNil& _brc )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );

			FormatUtils::SetBytes( ( this->bytes + sizeof(byte) ), (short)_itc );
			this->bytes[sizeof(byte) + sizeof(short)] = _bordersToApply;
			unsigned int brc = (unsigned int)_brc;
			memcpy( ( this->bytes + sizeof(byte) + sizeof(short) + sizeof(byte) ), &brc, sizeof(brc) );
		}

		TableBrc80Operand( const TableBrc80Operand& _tableBrc80Operand )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			memcpy( this->bytes, _tableBrc80Operand.bytes, SIZE_IN_BYTES );
		}

		virtual ~TableBrc80Operand()
		{
		}

		virtual unsigned int Size() const
		{
			return (unsigned int)sizeof(this->bytes);  
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}
	};
}