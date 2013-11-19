#pragma once

#include "TBD.h"
#include "XAS.h"

namespace AVSDocFileFormat
{
	class PChgTabsAdd : public IOperand
	{
	public:

		PChgTabsAdd() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(byte);

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
			}
		}

		explicit PChgTabsAdd( const vector<XAS>& _rgdxaAdd, const vector<TBD>& _rgtbdAdd ) : bytes(NULL), sizeInBytes(0)
		{
			unsigned int cTabs = min( _rgdxaAdd.size(), _rgtbdAdd.size() );

			if ( cTabs > MAX_RECORDS_NUMBER )
			{
				cTabs = MAX_RECORDS_NUMBER; 
			}

			this->sizeInBytes = ( ( sizeof(XAS) * cTabs ) + ( sizeof(TBD) * cTabs ) + sizeof(byte) );

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				unsigned int offset = 0;

				this->bytes[offset] = (byte)cTabs;
				offset += sizeof(byte);

				for ( unsigned int i = 0; i < cTabs; i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), (short)_rgdxaAdd[i] );
					offset += sizeof(XAS);
				}

				for ( unsigned int i = 0; i < cTabs; i++ )
				{
					this->bytes[offset] = (byte)_rgtbdAdd[i];
					offset += sizeof(TBD);
				}
			}
		}

		PChgTabsAdd( const PChgTabsAdd& _pChgTabsAdd ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _pChgTabsAdd.bytes != NULL ) && ( _pChgTabsAdd.sizeInBytes != 0 ) )
			{
				this->sizeInBytes = _pChgTabsAdd.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );
					memcpy( this->bytes, _pChgTabsAdd.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~PChgTabsAdd()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:

		byte* bytes;
		unsigned int sizeInBytes;

		static const byte MAX_RECORDS_NUMBER = 64;
	};

	class PChgTabsDel : public IOperand
	{
	public:

		PChgTabsDel() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(byte);

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
			}
		}

		explicit PChgTabsDel( const vector<XAS>& _rgdxaDel ) : bytes(NULL), sizeInBytes(0)
		{
			unsigned int cTabs = _rgdxaDel.size();

			if ( cTabs > MAX_RECORDS_NUMBER )
			{
				cTabs = MAX_RECORDS_NUMBER; 
			}

			this->sizeInBytes = ( ( sizeof(XAS) * cTabs ) + sizeof(byte) );

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				unsigned int offset = 0;

				this->bytes[offset] = (byte)cTabs;
				offset += sizeof(byte);

				for ( unsigned int i = 0; i < cTabs; i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), (short)_rgdxaDel[i] );
					offset += sizeof(XAS);
				}
			}
		}

		PChgTabsDel( const PChgTabsDel& _pChgTabsDel ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _pChgTabsDel.bytes != NULL ) && ( _pChgTabsDel.sizeInBytes != 0 ) )
			{
				this->sizeInBytes = _pChgTabsDel.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );
					memcpy( this->bytes, _pChgTabsDel.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~PChgTabsDel()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:
		byte* bytes;
		unsigned int sizeInBytes;

		static const byte MAX_RECORDS_NUMBER = 64;
	};

	class PChgTabsDelClose : public IOperand
	{
	public:

		PChgTabsDelClose() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(byte);

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
			}
		}

		explicit PChgTabsDelClose( const vector<short>& _rgdxaDel, const vector<XAS_plusOne>& _rgdxaClose ) : bytes(NULL), sizeInBytes(0)
		{
			unsigned int cTabs = min( _rgdxaDel.size(), _rgdxaClose.size() );

			if ( cTabs > MAX_RECORDS_NUMBER )
			{
				cTabs = MAX_RECORDS_NUMBER; 
			}

			this->sizeInBytes = ( ( sizeof(short) * cTabs ) + ( sizeof(XAS_plusOne) * cTabs ) + sizeof(byte) );

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				unsigned int offset = 0;

				this->bytes[offset] = (byte)cTabs;
				offset += sizeof(byte);

				for ( unsigned int i = 0; i < cTabs; i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), _rgdxaDel[i] );
					offset += sizeof(short);
				}

				for ( unsigned int i = 0; i < cTabs; i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), (short)_rgdxaClose[i] );
					offset += sizeof(XAS_plusOne);
				}
			}
		}

		PChgTabsDelClose( const PChgTabsDelClose& _pChgTabsDelClose ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _pChgTabsDelClose.bytes != NULL ) && ( _pChgTabsDelClose.sizeInBytes != 0 ) )
			{
				this->sizeInBytes = _pChgTabsDelClose.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _pChgTabsDelClose.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~PChgTabsDelClose()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:
		byte* bytes;
		unsigned int sizeInBytes;

		static const byte MAX_RECORDS_NUMBER = 64;
	};

	class PChgTabsOperand : public IOperand
	{
	public:

		PChgTabsOperand() : bytes(NULL), sizeInBytes(0)
		{
			this->DefaultInit();
		}

		explicit PChgTabsOperand( const PChgTabsDelClose& _pChgTabsDelClose, const PChgTabsAdd& _pChgTabsAdd ) : bytes(NULL), sizeInBytes(0)
		{
			unsigned int cb = ( _pChgTabsDelClose.Size() + _pChgTabsAdd.Size() );

			if ( ( cb >= MIN_SIZE_IN_BYTES ) && ( cb <= MAX_SIZE_IN_BYTES ) )
			{
				this->sizeInBytes = ( cb + sizeof(byte) );

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					this->bytes[0] = (byte)cb;

					memcpy( ( this->bytes + sizeof(byte) ), (byte*)_pChgTabsDelClose, _pChgTabsDelClose.Size() );
					memcpy( ( this->bytes + sizeof(byte) + _pChgTabsDelClose.Size() ), (byte*)_pChgTabsAdd, _pChgTabsAdd.Size() );
				}
			}
			else
			{
				this->DefaultInit();
			}
		}

		PChgTabsOperand( const PChgTabsOperand& _pChgTabsOperand ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _pChgTabsOperand.bytes != NULL ) && ( _pChgTabsOperand.sizeInBytes != 0 ) )
			{
				this->sizeInBytes = _pChgTabsOperand.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _pChgTabsOperand.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~PChgTabsOperand()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:
		void DefaultInit()
		{
			this->sizeInBytes = ( MIN_SIZE_IN_BYTES + sizeof(byte) );

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				this->bytes[0] = MIN_SIZE_IN_BYTES;
			} 
		}

	private:

		byte* bytes;
		unsigned int sizeInBytes;

		static const byte MIN_SIZE_IN_BYTES = 2;
		static const byte MAX_SIZE_IN_BYTES = 255;
	};

	class PChgTabsPapxOperand : public IOperand
	{
	public:

		PChgTabsPapxOperand() :  bytes(NULL), sizeInBytes(0)
		{
			this->DefaultInit();
		}

		explicit PChgTabsPapxOperand( const PChgTabsDel& _pChgTabsDel, const PChgTabsAdd& _pChgTabsAdd ) : bytes(NULL), sizeInBytes(0)
		{
			unsigned int cb = ( _pChgTabsDel.Size() + _pChgTabsAdd.Size() );

			if ( ( cb >= MIN_SIZE_IN_BYTES ) && ( cb <= MAX_SIZE_IN_BYTES ) )
			{
				this->sizeInBytes = ( cb + sizeof(byte) );

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					this->bytes[0] = (byte)cb;

					memcpy( ( this->bytes + sizeof(byte) ), (byte*)_pChgTabsDel, _pChgTabsDel.Size() );
					memcpy( ( this->bytes + sizeof(byte) + _pChgTabsDel.Size() ), (byte*)_pChgTabsAdd, _pChgTabsAdd.Size() );
				}
			}
			else
			{
				this->DefaultInit();
			}
		}

		PChgTabsPapxOperand( const PChgTabsPapxOperand& _pChgTabsPapxOperand ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _pChgTabsPapxOperand.bytes != NULL ) && ( _pChgTabsPapxOperand.sizeInBytes != 0 ) )
			{
				this->sizeInBytes = _pChgTabsPapxOperand.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _pChgTabsPapxOperand.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~PChgTabsPapxOperand()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(this->bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:

		void DefaultInit()
		{
			this->sizeInBytes = ( MIN_SIZE_IN_BYTES + sizeof(byte) );

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				this->bytes[0] = MIN_SIZE_IN_BYTES;
			} 
		}

	private:
		byte* bytes;
		unsigned int sizeInBytes;

		static const byte MIN_SIZE_IN_BYTES = 2;
		static const byte MAX_SIZE_IN_BYTES = 255;

	};
}