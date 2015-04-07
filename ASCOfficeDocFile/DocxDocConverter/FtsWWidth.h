#pragma once

#include "Constants.h"

namespace ASCDocFileFormat
{
	class FtsWWidth_Table: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		unsigned short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_Table( Constants::Fts _ftsWidth, unsigned short _wWidth = 0 )
		{
			if ( _ftsWidth == Constants::ftsDxaSys )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsPercent )
			{
				if ( _wWidth > 30000 )
				{
					this->wWidth = 30000;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else
				{
					this->wWidth = _wWidth;
				}  
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_Table( const FtsWWidth_Table& ftsWWidth_Table ):
		ftsWidth(ftsWWidth_Table.ftsWidth), wWidth(ftsWWidth_Table.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_Table()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
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

	class FtsWWidth_TablePart: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		unsigned short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_TablePart( Constants::Fts _ftsWidth, unsigned short _wWidth = 0 )
		{
			if ( _ftsWidth == Constants::ftsDxaSys )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else if(_ftsWidth == Constants::ftsDxa && 0 == _wWidth)
			{
				this->ftsWidth = Constants::ftsAuto;
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsPercent )
			{
				if ( _wWidth > 5000 )
				{
					this->wWidth = 5000;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else
				{
					this->wWidth = _wWidth;
				}  
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_TablePart( const FtsWWidth_TablePart& ftsWWidth_TablePart ):
		ftsWidth(ftsWWidth_TablePart.ftsWidth), wWidth(ftsWWidth_TablePart.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_TablePart()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
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

	class FtsWWidth_Indent: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_Indent( Constants::Fts _ftsWidth, short _wWidth = 0 )
		{
			if ( ( _ftsWidth == Constants::ftsPercent ) || ( _ftsWidth == Constants::ftsDxaSys ) )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else if ( _wWidth < -31560 )
				{
					this->wWidth = -31560;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_Indent( const FtsWWidth_Indent& ftsWWidth_Indent ):
		ftsWidth(ftsWWidth_Indent.ftsWidth), wWidth(ftsWWidth_Indent.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_Indent()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
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