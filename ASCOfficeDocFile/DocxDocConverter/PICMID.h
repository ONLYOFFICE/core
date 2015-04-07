#pragma once

#include "Brc80MayBeNil.h"

namespace ASCDocFileFormat
{
	class PICMID: public IOperand
	{
	public:
		PICMID()
		{
			memset(bytes, 0, SIZE_IN_BYTES);
		}

		explicit PICMID(short _dxaGoal, short _dyaGoal, unsigned short _mx, unsigned short _my, Brc80 _brcTop80, Brc80 _brcLeft80, Brc80 _brcBottom80, Brc80 _brcRight80 )
		{
			memset(bytes, 0, SIZE_IN_BYTES);

			unsigned int offset = 0;

			FormatUtils::SetBytes( ( this->bytes + offset ), _dxaGoal );
			offset += sizeof(_dxaGoal);

			FormatUtils::SetBytes( ( this->bytes + offset ), _dyaGoal );
			offset += sizeof(_dyaGoal);

			FormatUtils::SetBytes( ( this->bytes + offset ), _mx );
			offset += sizeof(_mx);

			FormatUtils::SetBytes( ( this->bytes + offset ), _my );
			offset += ( sizeof(_my) + 10 );

			unsigned int brc80Value = _brcTop80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcLeft80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcBottom80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);

			brc80Value = _brcRight80;

			FormatUtils::SetBytes( ( this->bytes + offset ), brc80Value );
			offset += sizeof(brc80Value);
		}

		PICMID (const PICMID& oICMID)
		{
			memset(bytes, 0, SIZE_IN_BYTES );
			memcpy(bytes, oICMID.bytes, SIZE_IN_BYTES );
		}

		virtual ~PICMID()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

	private:
		static const unsigned char SIZE_IN_BYTES = 38;
		unsigned char bytes[SIZE_IN_BYTES];
	};
}