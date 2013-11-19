#pragma once

#include "OfficeArtBlip.h"

namespace OfficeArt
{
	namespace Enumerations
	{
		enum BlipJPEGInstance
		{
			blipJPEGInstanceRGB1UID = 0x46A, //JPEG in RGB color space, number of unique identifiers == 1
			blipJPEGInstanceRGB2UID = 0x46B, //JPEG in RGB color space, number of unique identifiers == 2
			blipJPEGInstanceCMYK1UID = 0x6E2, //JPEG in CMYK color space, number of unique identifiers == 1
			blipJPEGInstanceCMYK2UID = 0x6E3, //JPEG in CMYK color space, number of unique identifiers == 2
		};
	}

	class OfficeArtBlipJPEG : public OfficeArtBlip
	{
	public:
		OfficeArtBlipJPEG():  OfficeArtBlip(OfficeArtRecordHeader( 0x0, (unsigned short)Enumerations::blipJPEGInstanceRGB1UID, 0xF01D, 17 )), tag(0), BLIPFileData(NULL), blipFileDataSize(0), bytes(NULL), size(0)
		{
			memset(rgbUid1, 0, rgbUidsSize);
			memset(rgbUid2, 0, rgbUidsSize);

			this->Initialize();
		}

		explicit OfficeArtBlipJPEG( byte* _blipFileData, unsigned int _blipFileDataSize,
			const vector<byte>& _rgbUid1 = vector<byte>(),
			const vector<byte>& _rgbUid2 = vector<byte>(), byte _tag = 0xFF,
			Enumerations::BlipJPEGInstance _recInstance = Enumerations::blipJPEGInstanceRGB1UID ):
		OfficeArtBlip(OfficeArtRecordHeader( 0x0, (unsigned short)_recInstance, 0xF01D, 17 )), tag(_tag), BLIPFileData(NULL),
			blipFileDataSize(_blipFileDataSize), bytes(NULL), size(0)
		{
			memset( this->rgbUid1, 0, rgbUidsSize );

			memset( this->rgbUid2, 0, rgbUidsSize );

			if ( ( _recInstance == Enumerations::blipJPEGInstanceRGB1UID ) ||
				( _recInstance == Enumerations::blipJPEGInstanceCMYK1UID ) )
			{
				this->size = 17;
			}
			else if ( ( _recInstance == Enumerations::blipJPEGInstanceRGB2UID ) ||
				( _recInstance == Enumerations::blipJPEGInstanceCMYK2UID ) )
			{
				this->size = 33;
			}

			this->size += _blipFileDataSize;

			this->rh = OfficeArtRecordHeader( 0x0, (unsigned short)_recInstance, 0xF01D, this->size );

			if ( _rgbUid1.size() == rgbUidsSize )
			{
				for ( unsigned int i = 0; i < _rgbUid1.size(); i++ )
				{
					this->rgbUid1[i] = _rgbUid1[i];
				}
			}

			if ( _rgbUid2.size() == rgbUidsSize )
			{
				for ( unsigned int i = 0; i < _rgbUid2.size(); i++ )
				{
					this->rgbUid2[i] = _rgbUid2[i];
				}
			}

			if ( ( _blipFileData != NULL ) && ( this->blipFileDataSize != 0 ) )
			{
				this->BLIPFileData = new byte[this->blipFileDataSize];

				if ( this->BLIPFileData != NULL )
				{
					memset( this->BLIPFileData, 0, this->blipFileDataSize );

					memcpy( this->BLIPFileData, _blipFileData, this->blipFileDataSize );
				}
			}

			this->Initialize();
		}

		OfficeArtBlipJPEG (const OfficeArtBlipJPEG& _officeArtBlipJPEG) : OfficeArtBlip(OfficeArtRecordHeader( _officeArtBlipJPEG.rh )), tag(_officeArtBlipJPEG.tag), BLIPFileData(NULL), blipFileDataSize(_officeArtBlipJPEG.blipFileDataSize), bytes(NULL), size(_officeArtBlipJPEG.size)
		{
			memset(rgbUid1, 0, rgbUidsSize);
			memcpy(rgbUid1, _officeArtBlipJPEG.rgbUid1, rgbUidsSize);

			memset(rgbUid2, 0, rgbUidsSize );
			memcpy(rgbUid2, _officeArtBlipJPEG.rgbUid2, rgbUidsSize);

			if (blipFileDataSize != 0)
			{
				BLIPFileData = new byte[blipFileDataSize];

				if (BLIPFileData)
				{
					memset(BLIPFileData, 0, blipFileDataSize);
					memcpy(BLIPFileData, _officeArtBlipJPEG.BLIPFileData, blipFileDataSize);
				}
			}

			if (size != 0)
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _officeArtBlipJPEG.bytes, size);
				}
			}
		}

		virtual operator const byte* () const
		{
			return (const byte*)(bytes);
		}

		virtual operator byte* () const
		{
			return (byte*)(bytes);
		}

		virtual unsigned int Size() const
		{
			return this->size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtBlipJPEG;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtBlipJPEG( *this );
		}

		virtual ~OfficeArtBlipJPEG() 
		{
			RELEASEARRAYOBJECTS (BLIPFileData);
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		void Initialize()
		{
			this->size = ( sizeof(this->rh) + this->rh.GetLength() );

			if ( this->size != 0 )
			{
				this->bytes = new byte[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					unsigned int offset = 0;

					memcpy( ( this->bytes + offset ), (byte*)(this->rh), sizeof(this->rh) );
					offset += sizeof(this->rh);

					memcpy( ( this->bytes + offset ), (byte*)(this->rgbUid1), sizeof(this->rgbUid1) );
					offset += sizeof(this->rgbUid1);

					if ( ( this->rh.GetInstance() == Enumerations::blipJPEGInstanceRGB2UID ) ||
						( this->rh.GetInstance() == Enumerations::blipJPEGInstanceCMYK2UID ) )
					{
						memcpy( ( this->bytes + offset ), (byte*)(this->rgbUid2), sizeof(this->rgbUid2) );
						offset += sizeof(this->rgbUid2);  
					}

					memcpy( ( this->bytes + offset ), &(this->tag), sizeof(this->tag) );
					offset += sizeof(this->tag);

					if ( ( this->BLIPFileData != NULL ) && ( this->blipFileDataSize != 0 ) )
					{
						memcpy( ( this->bytes + offset ), this->BLIPFileData, this->blipFileDataSize );
						offset += this->blipFileDataSize;
					}
				}
			}	      
		}

	private:

		static const byte rgbUidsSize = 16;

		byte						rgbUid1[rgbUidsSize];
		byte						rgbUid2[rgbUidsSize];
		byte						tag;
		byte*						BLIPFileData;
		unsigned int				blipFileDataSize;

		byte*						bytes;
		unsigned int				size;
	};
}