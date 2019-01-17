/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "OfficeArtBlip.h"
#include "OfficeArtMetafileHeader.h"

namespace OfficeArt
{
	class OfficeArtBlipEMF: public OfficeArtBlip
	{
	private:
		static const unsigned char rgbUidsSize = 16;
		unsigned char rgbUid1[rgbUidsSize];
		unsigned char rgbUid2[rgbUidsSize];
		OfficeArtMetafileHeader metafileHeader;
		unsigned char* BLIPFileData;
		unsigned int blipFileDataSize;

		unsigned char* bytes;
		unsigned int size;

	public:
		OfficeArtBlipEMF():
		  OfficeArtBlip(OfficeArtRecordHeader( 0x0, 0x3D4, 0xF01A, ( sizeof(metafileHeader) + sizeof(rgbUid1) ) )),
			  metafileHeader(), BLIPFileData(NULL), blipFileDataSize(0), bytes(NULL), size(0)
		  {
			  memset( this->rgbUid1, 0, rgbUidsSize );

			  memset( this->rgbUid2, 0, rgbUidsSize );

			  this->Initialize();
		  }

		  explicit OfficeArtBlipEMF( const OfficeArtMetafileHeader& _metafileHeader,
			  unsigned char* _blipFileData, 
			  const std::vector<unsigned char>& _rgbUid1 = std::vector<unsigned char>(),
			  const std::vector<unsigned char>& _rgbUid2 = std::vector<unsigned char>() ):
		  OfficeArtBlip(OfficeArtRecordHeader( 0x0, 0x3D4, 0xF01A, ( sizeof(metafileHeader) + sizeof(rgbUid1) ) )),
			  metafileHeader(_metafileHeader), BLIPFileData(NULL), blipFileDataSize(0), bytes(NULL), size(0)
		  {
			  memset( this->rgbUid1, 0, rgbUidsSize );

			  memset( this->rgbUid2, 0, rgbUidsSize );

			  this->blipFileDataSize = this->metafileHeader.GetCompressedSize();
			  this->size = this->blipFileDataSize;

			  if ( _rgbUid2.size() != rgbUidsSize )
			  {
				  this->size += ( sizeof(this->metafileHeader) + sizeof(this->rgbUid1) );

				  this->rh = OfficeArtRecordHeader( 0x0, 0x3D4, 0xF01A, this->size );
			  }
			  else
			  {
				  this->size += ( sizeof(this->metafileHeader) + sizeof(this->rgbUid1) + sizeof(this->rgbUid2) );

				  this->rh = OfficeArtRecordHeader( 0x0, 0x3D5, 0xF01A, this->size );
			  }

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
				  this->BLIPFileData = new unsigned char[this->blipFileDataSize];

				  if ( this->BLIPFileData != NULL )
				  {
					  memset( this->BLIPFileData, 0, this->blipFileDataSize );
					  memcpy( this->BLIPFileData, _blipFileData, this->blipFileDataSize );
				  }
			  }

			  this->Initialize();
		  }

		  OfficeArtBlipEMF( const OfficeArtBlipEMF& _officeArtBlipEMF  ):
		  OfficeArtBlip(OfficeArtRecordHeader( _officeArtBlipEMF.rh )), metafileHeader(_officeArtBlipEMF.metafileHeader),
			  BLIPFileData(NULL), blipFileDataSize(_officeArtBlipEMF.blipFileDataSize), bytes(NULL), size(_officeArtBlipEMF.size)
		  {
			  memset( this->rgbUid1, 0, rgbUidsSize );
			  memcpy( this->rgbUid1, _officeArtBlipEMF.rgbUid1, rgbUidsSize );

			  memset( this->rgbUid2, 0, rgbUidsSize );
			  memcpy( this->rgbUid2, _officeArtBlipEMF.rgbUid2, rgbUidsSize );

			  if ( this->blipFileDataSize != 0 )
			  {
				  this->BLIPFileData = new unsigned char[this->blipFileDataSize];

				  if ( this->BLIPFileData != NULL )
				  {
					  memset( this->BLIPFileData, 0, this->blipFileDataSize );

					  memcpy( this->BLIPFileData, _officeArtBlipEMF.BLIPFileData, this->blipFileDataSize );
				  }
			  }

			  if ( this->size != 0 )
			  {
				  this->bytes = new unsigned char[this->size];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->size );
					  memcpy( this->bytes, _officeArtBlipEMF.bytes, this->size );
				  }
			  }
		  }

		  virtual operator const unsigned char* () const
		  {
			  return (const unsigned char*)(this->bytes);
		  }

		  virtual operator unsigned char* () const
		  {
			  return (unsigned char*)(this->bytes);
		  }

		  virtual unsigned int Size() const
		  {
			  return this->size;
		  }

		  virtual IOfficeArtRecord* New() const
		  {
			  return new OfficeArtBlipEMF;
		  }

		  virtual IOfficeArtRecord* Clone() const
		  {
			  return new OfficeArtBlipEMF( *this );
		  }

		  virtual ~OfficeArtBlipEMF() 
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
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					unsigned int offset = 0;

					memcpy( ( this->bytes + offset ), (unsigned char*)(this->rh), sizeof(this->rh) );
					offset += sizeof(this->rh);

					memcpy( ( this->bytes + offset ), (unsigned char*)(this->rgbUid1), sizeof(this->rgbUid1) );
					offset += sizeof(this->rgbUid1);

					if ( this->rh.GetInstance() == 0x3D5 )
					{
						memcpy( ( this->bytes + offset ), (unsigned char*)(this->rgbUid2), sizeof(this->rgbUid2) );
						offset += sizeof(this->rgbUid2);  
					}

					memcpy( ( this->bytes + offset ), &(this->metafileHeader), sizeof(this->metafileHeader) );
					offset += sizeof(this->metafileHeader);

					if ( ( this->BLIPFileData != NULL ) && ( this->blipFileDataSize != 0 ) )
					{
						memcpy( ( this->bytes + offset ), this->BLIPFileData, this->blipFileDataSize );
						offset += this->blipFileDataSize;
					}
				}
			}	      
		}
	};
}