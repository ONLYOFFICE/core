#pragma once

#include "Record.h"
#include "../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

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
		unsigned char* m_rgbUid;
		/// The primary UID - this defaults to 0, in which case the primary ID is that of the internal data.
		/// NOTE!: The primary UID is only saved to disk if (blip_instance ^ blip_signature == 1).
		/// Blip_instance is MSOFBH.finst and blip_signature is one of the values defined in MSOBI
		unsigned char* m_rgbUidPrimary;
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
		unsigned char* m_pvBits;

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

            this->m_rcBounds.left   = this->Reader->ReadInt32();
            this->m_rcBounds.top    = this->Reader->ReadInt32();
            this->m_rcBounds.right  = this->Reader->ReadInt32() + this->m_rcBounds.left;
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
		/// If the picture is not compressed, it returns original unsigned char array.
		unsigned long Decompress( unsigned char **buffer )
		{
			unsigned long uncomprLen = 0;


			if ( this->m_fCompression == BlipCompressionDeflate )
			{
				uncomprLen = this->m_cb;
				*buffer = new unsigned char[uncomprLen];

				HRESULT res = S_OK;
				COfficeUtils* pOfficeUtils = new COfficeUtils(NULL); 

				if (pOfficeUtils)
				{
					pOfficeUtils->Uncompress( *buffer, &uncomprLen, this->m_pvBits, this->m_cbSave );

					delete pOfficeUtils;
					pOfficeUtils = NULL;
				}
			}
			else if ( this->m_fCompression == BlipCompressionNone )

			{
				uncomprLen = this->m_cbSave;
				*buffer = new unsigned char[uncomprLen];

                memcpy( *buffer, this->m_pvBits , this->m_cbSave );
			}

			return uncomprLen;
		}
	};
}
