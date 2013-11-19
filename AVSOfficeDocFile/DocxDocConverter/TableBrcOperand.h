#pragma once

#include "Brc.h"
#include "ItcFirstLim.h"
#include "Constants.h"

namespace AVSDocFileFormat
{
	class TableBrcOperand: public IOperand
	{
	private:
		static const byte SIZE_IN_BYTES = 12;
		byte bytes[SIZE_IN_BYTES];

	public:
		TableBrcOperand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );
		}

		explicit TableBrcOperand( const ItcFirstLim& _itc, byte _bordersToApply, const Brc& _brc )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = ( SIZE_IN_BYTES - 1 );

			FormatUtils::SetBytes( ( this->bytes + sizeof(byte) ), (short)_itc );
			this->bytes[sizeof(byte) + sizeof(short)] = _bordersToApply;
			memcpy( ( this->bytes + sizeof(byte) + sizeof(short) + sizeof(byte) ), (byte*)_brc, _brc.Size() );
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