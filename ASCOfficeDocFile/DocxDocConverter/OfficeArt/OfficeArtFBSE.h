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

#include "OfficeArtBStoreContainerFileBlock.h"
#include "OfficeArtBlip.h"

#include <string>
#include <vector>

namespace OfficeArt
{
	namespace Enumerations
	{
		enum MSOBLIPTYPE
		{
			msoblipERROR	= 0x00, //	There was an error reading the file.
			msoblipUNKNOWN	= 0x01, //	Unknown BLIP type.
			msoblipEMF		= 0x02, //	EMF format.
			msoblipWMF		= 0x03, //	WMF format.
			msoblipPICT		= 0x04, //	Macintosh PICT format.
			msoblipJPEG		= 0x05, //	JPEG format.
			msoblipPNG		= 0x06, //	PNG format.
			msoblipDIB		= 0x07, //	DIB format.
			msoblipTIFF		= 0x11, //	TIFF format.
			msoblipCMYKJPEG = 0x12	//	JPEG format in YCCK or CMYK color space.
		};
	}

	typedef unsigned int MSOFO;

	class OfficeArtFBSE : public OfficeArtBStoreContainerFileBlock
	{
	public:

		OfficeArtFBSE () : OfficeArtBStoreContainerFileBlock(OfficeArtRecordHeader( 0x2, (unsigned short)Enumerations::msoblipUNKNOWN, 0xF007, 36 )), btWin32(Enumerations::msoblipUNKNOWN), btMacOS(Enumerations::msoblipUNKNOWN), tag(0), size(0), cRef(0), foDelay(0xFFFFFFFF), cbName(0), bytes(NULL), totalSize(0)
		{
			m_bHaveEmbeddedBlip	=	FALSE;
			m_pBlip				=	NULL;

			memset (rgbUid, 0, sizeof(rgbUid));

			Initialize();
		}

		OfficeArtFBSE (bool bEmBlip, Enumerations::MSOBLIPTYPE _btWin32,
			Enumerations::MSOBLIPTYPE _btMacOS,
			unsigned int _size,
			const std::vector<unsigned char>& _rgbUid = std::vector<unsigned char>( 0, 16 ),
			unsigned int _cRef = 1, 
			unsigned short _tag = 0xFF,
			OfficeArt::MSOFO _foDelay = 0,
			const wchar_t* _nameData = NULL ):
		OfficeArtBStoreContainerFileBlock(OfficeArtRecordHeader( 0x2, (unsigned short)_btWin32, 0xF007, 36 )),
			btWin32(_btWin32), 
			btMacOS(_btMacOS), 
			tag(_tag),
			size(_size), 
			cRef(_cRef), 
			foDelay(_foDelay),
			cbName(0),
			bytes(NULL), 
			totalSize(0)
		{
			m_bHaveEmbeddedBlip		=	bEmBlip;

			//m_bHaveEmbeddedBlip	=	FALSE;
			//m_pBlip				=	NULL;

			memset( this->rgbUid, 0, sizeof(this->rgbUid) );

			if (_nameData)
			{
				nameData = std::wstring( _nameData );

				cbName = ( sizeof(wchar_t) * ( this->nameData.length() + 1 ) );

				if (cbName > 0xFE)
				{
					cbName	=	0;
					nameData	=	std::wstring();
				}
			}

			if ( _rgbUid.size() == rgbUidSize )
			{
				for ( unsigned int i = 0; i < _rgbUid.size(); i++ )
				{
					rgbUid[i] = _rgbUid[i];
				}
			}

			if (m_bHaveEmbeddedBlip)
			{
				rh = OfficeArtRecordHeader (0x2, (unsigned short)_btWin32, 0xF007, (cbName + 36 + size));
			}
			else
			{
				rh = OfficeArtRecordHeader (0x2, (unsigned short)_btWin32, 0xF007, (cbName + 36));
			}

			Initialize();
		}


		OfficeArtFBSE(const OfficeArtFBSE& _officeArtFBSE ):
		OfficeArtBStoreContainerFileBlock(OfficeArtRecordHeader( _officeArtFBSE.rh )),
			btWin32(_officeArtFBSE.btWin32), btMacOS(_officeArtFBSE.btMacOS), tag(_officeArtFBSE.tag),
			size(_officeArtFBSE.size), cRef(_officeArtFBSE.cRef), foDelay(_officeArtFBSE.foDelay),
			cbName(_officeArtFBSE.cbName), bytes(NULL), totalSize(_officeArtFBSE.totalSize), nameData(_officeArtFBSE.nameData), m_bHaveEmbeddedBlip (_officeArtFBSE.m_bHaveEmbeddedBlip), m_pBlip (NULL)
		{

			//if (_officeArtFBSE.m_pBlip)
			//{
			//	m_bHaveEmbeddedBlip	=	TRUE;
			//	m_pBlip				=	(OfficeArtBlip*)_officeArtFBSE.m_pBlip->Clone();
			//}

			memset (rgbUid, 0, sizeof(rgbUid) );
			memcpy (rgbUid, _officeArtFBSE.rgbUid, sizeof(rgbUid) );

			if (totalSize != 0 )
			{
				bytes = new unsigned char[totalSize];

				if (bytes)
				{
					memset (bytes, 0, totalSize);
					memcpy (bytes, _officeArtFBSE.bytes, totalSize);
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
			return this->totalSize;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFBSE;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFBSE( *this );
		}

		virtual ~OfficeArtFBSE() 
		{
			RELEASEARRAYOBJECTS(bytes);
			//RELEASEOBJECT (m_pBlip);
		}

		// 
		inline void SetFoDelay (int nFoDelay)
		{
			foDelay	=	nFoDelay;

			Initialize();
		}


		//inline void SetEmbeddedBlip (OfficeArtBlip* pBlip)
		//{
		//	m_bHaveEmbeddedBlip	=	TRUE;
		//	m_pBlip				=	(OfficeArtBlip*)pBlip->Clone();

		//	Initialize();		
		//}

	private:

		void Initialize()
		{
			this->totalSize = ( sizeof(this->rh) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(this->rgbUid) +
				sizeof(this->tag) + sizeof(this->size) + sizeof(this->cRef) +
				sizeof(this->foDelay) + sizeof(unsigned char) + sizeof(this->cbName) +
				sizeof(unsigned char) + sizeof(unsigned char) + this->cbName );

			//if (m_pBlip)
			//{
			//	totalSize += m_pBlip->Size();
			//}

			bytes = new unsigned char[totalSize];

			if (bytes)
			{
				memset( this->bytes, 0, this->totalSize );

				unsigned int offset = 0;

				memcpy( ( this->bytes + offset ), (unsigned char*)(this->rh), sizeof(this->rh) );
				offset += sizeof(this->rh);

				this->bytes[offset] = (unsigned char)(this->btWin32);
				offset += sizeof(unsigned char);

				this->bytes[offset] = (unsigned char)(this->btMacOS);
				offset += sizeof(unsigned char);

				memcpy( ( this->bytes + offset ), this->rgbUid, sizeof(this->rgbUid) );
				offset += sizeof(this->rgbUid);

				memcpy( ( this->bytes + offset ), &(this->tag), sizeof(this->tag) );
				offset += sizeof(this->tag);

				memcpy( ( this->bytes + offset ), &(this->size), sizeof(this->size) );
				offset += sizeof(this->size);

				memcpy( ( this->bytes + offset ), &(this->cRef), sizeof(this->cRef) );
				offset += sizeof(this->cRef);

				memcpy( ( this->bytes + offset ), &(this->foDelay), sizeof(this->foDelay) );
				offset += ( sizeof(this->foDelay) + sizeof(unsigned char) );

				this->bytes[offset] = this->cbName;
				offset += ( sizeof(this->cbName) + sizeof(unsigned char) + sizeof(unsigned char) );

				if ( (cbName != 0) && (nameData.length() != 0) )
				{
					memcpy ((bytes + offset), nameData.c_str(), ( sizeof(wchar_t) * (nameData.length() + 1) ));
					offset		+=	( sizeof(wchar_t) * ( nameData.length() + 1));
				}

				//if (m_pBlip)
				//{
				//	totalSize	+=	m_pBlip->Size();
				//	memcpy ((bytes + offset), (*m_pBlip), m_pBlip->Size());
				//}
			}
		}

	private:

		Enumerations::MSOBLIPTYPE	btWin32;
		Enumerations::MSOBLIPTYPE	btMacOS;
		static const unsigned char rgbUidSize = 16;
		unsigned char rgbUid[rgbUidSize];

		unsigned short tag;
		unsigned int size;
		unsigned int cRef;
		OfficeArt::MSOFO foDelay;
		unsigned char cbName;
		std::wstring nameData;

		unsigned char* bytes;
		unsigned int totalSize;

		bool			m_bHaveEmbeddedBlip;
		OfficeArtBlip*	m_pBlip;
	};
}