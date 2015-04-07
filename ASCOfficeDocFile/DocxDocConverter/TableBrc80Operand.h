#pragma once

#include "Brc80MayBeNil.h"
#include "ItcFirstLim.h"
#include "Constants.h"
#include "../Common/IOperand.h"

namespace ASCDocFileFormat
{
	class TableBrc80Operand: public IOperand
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 8;
		unsigned char bytes[SIZE_IN_BYTES];

	public:
		TableBrc80Operand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );
		}

		explicit TableBrc80Operand( const ItcFirstLim& _itc, unsigned char _bordersToApply, const Brc80MayBeNil& _brc )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );

			FormatUtils::SetBytes( ( this->bytes + sizeof(unsigned char) ), (short)_itc );
			this->bytes[sizeof(unsigned char) + sizeof(short)] = _bordersToApply;
			unsigned int brc = (unsigned int)_brc;
			memcpy( ( this->bytes + sizeof(unsigned char) + sizeof(short) + sizeof(unsigned char) ), &brc, sizeof(brc) );
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