#pragma once

namespace ASCDocFileFormat
{
	union StdfPost2000
	{
	private:
		static const byte SIZE_IN_BYTES = 8;

		struct
		{
			unsigned short istdLink				:	12;
			unsigned short fHasOriginalStyle	:	1;
			unsigned short fSpare				:	3;
			byte rsid[4];
			unsigned short iftcHtml				:	3;
			unsigned short unused				:	1;
			unsigned short iPriority			:	12;
		} StdfPost2000Struct;

		byte StdfPost2000Bytes [ SIZE_IN_BYTES ];

	public:

		StdfPost2000()
		{
			memset( this->StdfPost2000Bytes, 0, SIZE_IN_BYTES );
		}

		StdfPost2000( unsigned short _istdLink, bool _fHasOriginalStyle, unsigned int _rsid, unsigned short _iPriority )
		{
			memset ( StdfPost2000Bytes, 0, SIZE_IN_BYTES );

			if ( _istdLink > 0x0FFF )
			{
				StdfPost2000Struct.istdLink		=	0x0FFF;
			}
			else
			{
				StdfPost2000Struct.istdLink		=	_istdLink;
			}

			( _fHasOriginalStyle ) ? ( StdfPost2000Struct.fHasOriginalStyle = 1 ) : ( StdfPost2000Struct.fHasOriginalStyle = 0 );
			
			StdfPost2000Struct.fSpare			=	0;
			
			FormatUtils::SetBytes (StdfPost2000Struct.rsid, _rsid);
			
			StdfPost2000Struct.iftcHtml			=	0;
			StdfPost2000Struct.unused			=	0;

			if ( _iPriority > 0x0063 )
			{
				StdfPost2000Struct.iPriority	=	0x0063;
			}
			else
			{
				StdfPost2000Struct.iPriority	=	_iPriority;
			}
		}

		StdfPost2000( const StdfPost2000& _stdfPost2000 )
		{
			memset( StdfPost2000Bytes, 0, SIZE_IN_BYTES );
			memcpy( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES );
		}

		bool operator == ( const StdfPost2000& _stdfPost2000 )
		{
			return ( memcmp ( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES ) == 0 );
		}

		bool operator != ( const StdfPost2000& _stdfPost2000 )
		{
			return !( this->operator == ( _stdfPost2000 ) );
		}

		StdfPost2000& operator = ( const StdfPost2000& _stdfPost2000 )
		{
			if ( *this != _stdfPost2000 )
			{
				memset( StdfPost2000Bytes, 0, SIZE_IN_BYTES );
				memcpy( StdfPost2000Bytes, _stdfPost2000.StdfPost2000Bytes, SIZE_IN_BYTES );
			}

			return *this;
		}

		operator byte*() const
		{
			return (byte*)(StdfPost2000Bytes);
		}
	};
}