#pragma once

namespace ASCDocFileFormat
{
	class Stshif : public IOperand
	{
	public:

		Stshif ()
		{
			memset ( bytes, 0, SIZE_IN_BYTES );
		}

		Stshif (unsigned short cstd, bool StdfPost2000Exists, unsigned short stiMaxWhenSaved, short ftcAsci = 0, short ftcFE = 0, short ftcOther = 0 )
		{
			memset ( bytes, 0, SIZE_IN_BYTES);

			if ( cstd < 0x000F )
			{
				FormatUtils::SetBytes ( bytes, (unsigned short)0x000F );
			}
			else if ( cstd >= 0x0FFE )
			{
				FormatUtils::SetBytes ( bytes, (unsigned short)0x0FFE );
			}
			else
			{
				FormatUtils::SetBytes ( bytes, cstd );
			}

			if ( StdfPost2000Exists )
			{
				FormatUtils::SetBytes( ( bytes + sizeof(cstd) ), (unsigned short)0x0012 );
			}
			else
			{
				FormatUtils::SetBytes( ( bytes + sizeof(cstd) ), (unsigned short)0x000A );
			}

			FormatUtils::SetBytes( ( bytes + 4 ), (unsigned short)0x0001 );
			FormatUtils::SetBytes( ( bytes + 6 ), stiMaxWhenSaved );
			FormatUtils::SetBytes( ( bytes + 8 ), (unsigned short)0x000F );
			FormatUtils::SetBytes( ( bytes + 10 ), (unsigned short)0x0000 );
			FormatUtils::SetBytes( ( bytes + 12 ), ftcAsci );
			FormatUtils::SetBytes( ( bytes + 14 ), ftcFE );
			FormatUtils::SetBytes( ( bytes + 16 ), ftcOther );
		}

		Stshif (const Stshif& stshif)
		{
			memset ( bytes, 0, SIZE_IN_BYTES );
			memcpy ( bytes, stshif.bytes, SIZE_IN_BYTES );
		}

		virtual ~Stshif()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;
		}

	private:

		static const unsigned char	SIZE_IN_BYTES = 18;
		unsigned char				bytes[SIZE_IN_BYTES];
	};
}