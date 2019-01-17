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

#include "Record.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../Common/DocxFormat/Source/Base/Types_32.h"


namespace DocFileFormat
{
typedef struct
{
    short           Left;
    short           Top;
    short           Right;
    short           Bottom;
} PWMFRect16;

typedef struct
{
    _UINT32         Key;            // GDIP_WMF_PLACEABLEKEY
    short           Hmf;            // Metafile HANDLE number (always 0)
    PWMFRect16      BoundingBox;    // Coordinates in metafile units
    short           Inch;           // Number of metafile units per inch
    _UINT32         Reserved;       // Reserved (always 0)
    short           Checksum;       // Checksum value for previous 10 WORDs
} WmfPlaceableFileHeader;

class CMetaHeader
{

public:
	DWORD cbSize;
	RECT rcBounds;
	POINT ptSize;
	DWORD cbSave;
	BYTE compression;
	BYTE filter;

	CMetaHeader()
	{
		cbSize = cbSave = 0;
		filter = compression = 0;
		ptSize.x = ptSize.y = 0;

		rcBounds.bottom = rcBounds.left = rcBounds.right = rcBounds.top = 0;

	}

	void ToWMFHeader(WmfPlaceableFileHeader* pHeader)
	{
		if (NULL == pHeader)
			return;

		pHeader->Key				= 0x9AC6CDD7;
		pHeader->Hmf				= 0;

		pHeader->BoundingBox.Left	= (short)rcBounds.left;
		pHeader->BoundingBox.Top	= (short)rcBounds.top;
		pHeader->BoundingBox.Right	= (short)rcBounds.right;
		pHeader->BoundingBox.Bottom = (short)rcBounds.bottom;

		pHeader->Inch				= 1440; // 1:1
		pHeader->Reserved			= 0;

		pHeader->Checksum			= 0;
		pHeader->Checksum			^= (pHeader->Key & 0x0000FFFFL);
		pHeader->Checksum			^= ((pHeader->Key & 0xFFFF0000L) >> 16);
		
		pHeader->Checksum			^= pHeader->Hmf; 
		
		pHeader->Checksum			^= pHeader->BoundingBox.Left;
		pHeader->Checksum			^= pHeader->BoundingBox.Top; 
		pHeader->Checksum			^= pHeader->BoundingBox.Right;
		pHeader->Checksum			^= pHeader->BoundingBox.Bottom; 
		
		pHeader->Checksum			^= pHeader->Inch;
		pHeader->Checksum			^= (pHeader->Reserved & 0x0000FFFFL);
		pHeader->Checksum			^= ((pHeader->Reserved & 0xFFFF0000L) >> 16);
	}
};

class CMetaFileBuffer
{
public:
    bool			m_bIsValid;
	std::wstring	m_sExtension;
private:
	BYTE* m_pMetaHeader;
	BYTE* m_pMetaFile;

	LONG m_lMetaHeaderSize;
	LONG m_lMetaFileSize;

public:
	CMetaFileBuffer()
	{
        m_bIsValid			= false;

		m_pMetaHeader		= NULL;
		m_pMetaFile			= NULL;

		m_lMetaHeaderSize	= 0;
		m_lMetaFileSize		= 0;
	}
	~CMetaFileBuffer()
	{
		RELEASEARRAYOBJECTS(m_pMetaHeader);
		RELEASEARRAYOBJECTS(m_pMetaFile);
	}

	void SetHeader(BYTE* pHeader, LONG lSize)
	{
		m_pMetaHeader		= pHeader;
		m_lMetaHeaderSize	= lSize;
	}

    void SetData(BYTE* pCompress, LONG lCompressSize, LONG lUncompressSize, bool bIsCompressed)
	{
		if (!bIsCompressed)
		{
			m_pMetaFile		= new BYTE[lCompressSize];
			m_lMetaFileSize = lCompressSize;
			memcpy(m_pMetaFile, pCompress, lCompressSize);
		}
		else
		{
			ULONG lSize = lUncompressSize;
			m_pMetaFile = new BYTE[lUncompressSize];

			HRESULT res = S_OK;
			COfficeUtils* pOfficeUtils = new COfficeUtils(NULL); 

			if (pOfficeUtils)
			{
				pOfficeUtils->Uncompress( m_pMetaFile, &lSize, pCompress, lCompressSize );

				delete pOfficeUtils;
				pOfficeUtils = NULL;
	
				m_lMetaFileSize = (LONG)lSize;
			}
			else
			{
				RELEASEARRAYOBJECTS(m_pMetaFile);
				m_lMetaFileSize = 0;
			}
		}
	}


	int ToBuffer(BYTE *& Data)
	{
		int sz = 0;
	
		if (NULL != m_pMetaHeader)	sz += m_lMetaHeaderSize;
		if (NULL != m_pMetaFile)	sz += m_lMetaFileSize;

		Data = new BYTE[sz];
		int pos = 0;

		if (NULL != m_pMetaHeader)
		{
			memcpy(Data, (BYTE*)m_pMetaHeader, m_lMetaHeaderSize);
			pos += m_lMetaHeaderSize;
		}
		if (NULL != m_pMetaFile)
		{
			memcpy(Data + pos, (BYTE*)m_pMetaFile, m_lMetaFileSize);
		}

		return sz;
	}
	void ToFile(NSFile::CFileBinary* pFile)
	{
		if (NULL != m_pMetaHeader)
		{
			pFile->WriteFile((BYTE*)m_pMetaHeader, m_lMetaHeaderSize);
		}
	}
};


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
		CMetaFileBuffer		oMetaFile;
		
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
			m_rgbUid = Reader->ReadBytes( 16, true );

			if ( ( instance == 0x3D5 ) || ( instance == 0x217 ) || ( instance == 0x543 ) )
			{
				m_rgbUidPrimary = Reader->ReadBytes( 16, true );
			}
			
			oMetaFile.m_bIsValid	= TRUE;
			oMetaFile.m_sExtension	= L".emf";
			
			CMetaHeader oMetaHeader;
			
			m_cb = Reader->ReadInt32();

            m_rcBounds.left   = Reader->ReadInt32();
            m_rcBounds.top    = Reader->ReadInt32();
            m_rcBounds.right  = Reader->ReadInt32() + m_rcBounds.left;
			m_rcBounds.bottom = Reader->ReadInt32() + m_rcBounds.top;

			m_ptSize.x = Reader->ReadInt32();
			m_ptSize.y = Reader->ReadInt32();

			m_cbSave = Reader->ReadInt32();
			m_fCompression = (BlipCompression)Reader->ReadByte();
			m_fFilter = ( Reader->ReadByte() == 1 ) ? (true) : (false);
			
			int sz = Reader->GetSize() - Reader->GetPosition();
			m_pvBits = Reader->ReadBytes( sz/*m_cbSave*/, true );

			oMetaHeader.rcBounds	= m_rcBounds;
			oMetaHeader.cbSize		= m_cb;
			oMetaHeader.ptSize		= m_ptSize;
			oMetaHeader.cbSave		= m_cbSave ;
			oMetaHeader.compression	= m_fCompression;
			oMetaHeader.filter		= m_fFilter;

			if (typeCode == 0xf01b)
			{
				oMetaFile.m_sExtension	= L".wmf";
				
				WmfPlaceableFileHeader oWmfHeader = {};
				oMetaHeader.ToWMFHeader(&oWmfHeader);
				
				LONG lLenHeader = 22;
				BYTE* pMetaHeader = new BYTE[lLenHeader];
				memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

				oMetaFile.SetHeader(pMetaHeader, lLenHeader);	
			}
			if (typeCode == 0xf01c)
			{
				oMetaFile.m_sExtension	= L".pcz";
				//decompress???
			}
			oMetaFile.SetData(m_pvBits, oMetaHeader.cbSave, oMetaHeader.cbSize, 0 == oMetaHeader.compression);
		}

		virtual ~MetafilePictBlip()
		{
			RELEASEARRAYOBJECTS( m_rgbUid );
			RELEASEARRAYOBJECTS( m_rgbUidPrimary );
			RELEASEARRAYOBJECTS( m_pvBits );
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


			if ( m_fCompression == BlipCompressionDeflate )
			{
				uncomprLen = m_cb;
				*buffer = new unsigned char[uncomprLen];

				HRESULT res = S_OK;
				COfficeUtils* pOfficeUtils = new COfficeUtils(NULL); 

				if (pOfficeUtils)
				{
					pOfficeUtils->Uncompress( *buffer, &uncomprLen, m_pvBits, m_cbSave );

					delete pOfficeUtils;
					pOfficeUtils = NULL;
				}
			}
			else if ( m_fCompression == BlipCompressionNone )

			{
				uncomprLen = m_cbSave;
				*buffer = new unsigned char[uncomprLen];

                memcpy( *buffer, m_pvBits , m_cbSave );
			}

			return uncomprLen;
		}
	};
}
