#pragma once

#include <vector>
#include "md4c.h"
#include "../../Common/ASCUtils.h"

class MD4
{
public:

	MD4(const void* _message = NULL, unsigned int _messageSize = 0) : message(NULL), messageSize(_messageSize)
	{
		SetMessage(_message, _messageSize);

		memset(md4, 0, sizeof(md4));
	}

	MD4(const MD4& _md4) : message(NULL), messageSize(_md4.messageSize)
	{
		memset( this->md4, 0, sizeof(this->md4) );

		memcpy( this->md4, _md4.md4, sizeof(this->md4) );

		if ( _md4.message != NULL )
		{
			this->message = new unsigned char[this->messageSize];

			if ( this->message != NULL )
			{
				memset( this->message, 0, this->messageSize );
				memcpy( this->message, _md4.message, this->messageSize );  
			}
		}
	}

	~MD4 ()
	{
		RELEASEARRAYOBJECTS(message);
	}

	inline void SetMessage (const void* pMessage, unsigned int nMessageSize)
	{
		if ( ( pMessage != NULL ) && ( 0 != nMessageSize ) )
		{
			RELEASEARRAYOBJECTS(message);

			messageSize	=	0;

			messageSize	=	nMessageSize;

			message		=	new unsigned char [ messageSize ];
			if ( message )
			{
				memset ( message, 0, messageSize );
				memcpy ( message, pMessage, messageSize );
			}
		}  
	}

	inline std::vector<unsigned char> GetMD4Bytes() const
	{
		std::vector<unsigned char> md4Bytes;

		MD4_CTX context;

		MD4Init( &context );
		MD4Update( &context, this->message, this->messageSize );
		MD4Final( (unsigned char*)this->md4, &context );

		for ( unsigned int i = 0; i < md4Size; i++ )
		{
			md4Bytes.push_back( this->md4[i] );
		}

		return md4Bytes;
	}

private:

	static const unsigned char	md4Size	=	16;

	unsigned char*				message;
	unsigned int				messageSize;
	unsigned char				md4[md4Size];
};