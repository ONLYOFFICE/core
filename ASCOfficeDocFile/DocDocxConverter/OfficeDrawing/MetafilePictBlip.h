#pragma once

#include "Record.h"

namespace DocFileFormat
{
	typedef enum _BlipCompression
	{
		BlipCompressionDeflate,
		BlipCompressionNone = 254,
		BlipCompressionTest = 255
	} BlipCompression;

	class MetafilePictBlip: public Record
	{
	private:
		/// The secondary, or data, UID - should always be set.
		byte* m_rgbUid;
		/// The primary UID - this defaults to 0, in which case the primary ID is that of the internal data.
		/// NOTE!: The primary UID is only saved to disk if (blip_instance ^ blip_signature == 1).
		/// Blip_instance is MSOFBH.finst and blip_signature is one of the values defined in MSOBI
		byte* m_rgbUidPrimary;
		/// Cache of the metafile size
		int m_cb;
		RECT m_rcBounds;
		/// Boundary of metafile drawing commands
		POINT m_ptSize;
		/// Cache of saved size (size of m_pvBits)
		int m_cbSave;
		/// Compression
		BlipCompression m_fCompression;
		/// always msofilterNone
		bool m_fFilter;

		/// Compressed bits of metafile.
		byte* m_pvBits;

	public:
		static const unsigned short TYPE_CODE_0xF01A = 0xF01A;
		static const unsigned short TYPE_CODE_0xF01B = 0xF01B;
		static const unsigned short TYPE_CODE_0xF01C = 0xF01C;

		MetafilePictBlip():
		Record(), m_rgbUid(NULL), m_rgbUidPrimary(NULL), m_cb(0), m_cbSave(0), m_fCompression(BlipCompressionNone), m_fFilter(false), 
			m_pvBits(NULL)
		{
		}

		MetafilePictBlip( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
		Record( _reader, size, typeCode, version, instance ), m_rgbUid(NULL), m_rgbUidPrimary(NULL), m_cb(0), m_cbSave(0), 
			m_fCompression(BlipCompressionNone), m_fFilter(false), m_pvBits(NULL)
		{
			this->m_rgbUid = this->Reader->ReadBytes( 16, true );

			if ( ( instance == 0x3D5 ) || ( instance == 0x217 ) || ( instance == 0x543 ) )
			{
				this->m_rgbUidPrimary = this->Reader->ReadBytes( 16, true );
			}

			this->m_cb = this->Reader->ReadInt32();

			this->m_rcBounds.left = this->Reader->ReadInt32();
			this->m_rcBounds.top = this->Reader->ReadInt32();
			this->m_rcBounds.right = this->Reader->ReadInt32() + this->m_rcBounds.left;
			this->m_rcBounds.bottom = this->Reader->ReadInt32() + this->m_rcBounds.top;

			this->m_ptSize.x = this->Reader->ReadInt32();
			this->m_ptSize.y = this->Reader->ReadInt32();

			this->m_cbSave = this->Reader->ReadInt32();
			this->m_fCompression = (BlipCompression)this->Reader->ReadByte();
			this->m_fFilter = ( this->Reader->ReadByte() == 1 ) ? (true) : (false);

			this->m_pvBits = this->Reader->ReadBytes( this->m_cbSave, true );
		}

		virtual ~MetafilePictBlip()
		{
			RELEASEARRAYOBJECTS( this->m_rgbUid );
			RELEASEARRAYOBJECTS( this->m_rgbUidPrimary );
			RELEASEARRAYOBJECTS( this->m_pvBits );
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new MetafilePictBlip( _reader, bodySize, typeCode, version, instance );
		}

		/// Decompresses the bits of the picture if the picture is decompressed.
		/// If the picture is not compressed, it returns original byte array.
		unsigned long Decompress( byte **buffer )
		{
			unsigned long uncomprLen = 0;

#ifdef CREATE_ZIPPED_DOCX

			if ( this->m_fCompression == BlipCompressionDeflate )
			{
				uncomprLen = this->m_cb;
				*buffer = new byte[uncomprLen];

				int err = uncompress( *buffer, &uncomprLen, this->m_pvBits, this->m_cbSave );
			}
			else

#else

			if ( this->m_fCompression == BlipCompressionDeflate )
			{
				uncomprLen = this->m_cb;
				*buffer = new byte[uncomprLen];

				HRESULT res = S_OK;
				IOfficeUtils* pOfficeUtils = NULL; 

				//COM already should be initialized, if this is not a separate thread.
				res = CoInitialize( NULL );

				res = CoCreateInstance( __uuidof(COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(IOfficeUtils), (void **)&pOfficeUtils );

				if ( res == S_OK )
				{
					pOfficeUtils->Uncompress( *buffer, &uncomprLen, this->m_pvBits, this->m_cbSave );

					pOfficeUtils->Release();
					pOfficeUtils = NULL;
				}

				CoUninitialize();
			}
			else if ( this->m_fCompression == BlipCompressionNone )

#endif // CREATE_ZIPPED_DOCX
			{
				uncomprLen = this->m_cbSave;
				*buffer = new byte[uncomprLen];

				memcpy_s( *buffer, uncomprLen, this->m_pvBits, this->m_cbSave ); 
			}

			return uncomprLen;
		}
	};
}
