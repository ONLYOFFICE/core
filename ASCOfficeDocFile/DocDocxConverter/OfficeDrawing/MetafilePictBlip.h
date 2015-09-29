#pragma once

#include "Record.h"
#include "../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

//#if defined(_WIN32) || defined(_WIN64)
//	#include <atlcoll.h>
//	#include <gdiplus.h>
//#else
//    #include "../../../ASCOfficePPTXFile/PPTXLib/Linux/PPTXFormatLib/linux_gdiplus.h"
//#endif

namespace DocFileFormat
{
typedef struct
{
    INT16           Left;
    INT16           Top;
    INT16           Right;
    INT16           Bottom;
} PWMFRect16;

typedef struct
{
    UINT32          Key;            // GDIP_WMF_PLACEABLEKEY
    INT16           Hmf;            // Metafile HANDLE number (always 0)
    PWMFRect16      BoundingBox;    // Coordinates in metafile units
    INT16           Inch;           // Number of metafile units per inch
    UINT32          Reserved;       // Reserved (always 0)
    INT16           Checksum;       // Checksum value for previous 10 WORDs
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

public:
	CMetaHeader()
	{
	}


	//void ToEMFHeader(Gdiplus::ENHMETAHEADER3* pHeader)
	//{
	//	if (NULL == pHeader)
	//		return;

	//	pHeader->iType				= 0x00000001;
	//	pHeader->nSize				= 88;

	//	pHeader->rclBounds.left		= rcBounds.left;
	//	pHeader->rclBounds.top		= rcBounds.top;
	//	pHeader->rclBounds.right	= rcBounds.right;
	//	pHeader->rclBounds.bottom	= rcBounds.bottom;

	//	// нужно перевести в мм
	//	pHeader->rclFrame.left		= rcBounds.left;
	//	pHeader->rclFrame.top		= rcBounds.top;
	//	pHeader->rclFrame.right		= rcBounds.right;
	//	pHeader->rclFrame.bottom	= rcBounds.bottom;

	//	pHeader->dSignature			= 0x464D4520;
	//	pHeader->nVersion			= 0x00010000;
	//	pHeader->nBytes				= cbSize;

	//	pHeader->nRecords			= 1;
	//	pHeader->nHandles			= 0;

	//	pHeader->sReserved			= 0;

	//	pHeader->nDescription		= 0;
	//	pHeader->offDescription		= 0;

	//	pHeader->nPalEntries		= 0;

	//	pHeader->szlDevice.cx		= 200;
	//	pHeader->szlDevice.cy		= 200;

	//	// нужно перевести в мм
	//	pHeader->szlMillimeters.cx	= 100;
	//	pHeader->szlMillimeters.cy	= 100;
	//}

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
	CString			m_sExtension;
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
			m_pMetaFile		= pCompress;
			m_lMetaFileSize = lUncompressSize;
		}
		else
		{
			ULONG lSize = lUncompressSize;
			m_pMetaFile = new BYTE[lUncompressSize];
            //bool bRes	= NSZLib::Decompress(pCompress, (ULONG)lCompressSize, m_pMetaFile, lSize);

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
		if (NULL != m_pMetaFile)
		{
			pFile->WriteFile((BYTE*)m_pMetaFile, m_lMetaFileSize);
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
			this->m_rgbUid = this->Reader->ReadBytes( 16, true );

			if ( ( instance == 0x3D5 ) || ( instance == 0x217 ) || ( instance == 0x543 ) )
			{
				this->m_rgbUidPrimary = this->Reader->ReadBytes( 16, true );
			}
			
			oMetaFile.m_bIsValid	= TRUE;
			oMetaFile.m_sExtension	= L".emf";
			
			CMetaHeader oMetaHeader;
			
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
			
			int sz = Reader->GetSize() - Reader->GetPosition();
			this->m_pvBits = this->Reader->ReadBytes( sz/*this->m_cbSave*/, true );

			oMetaHeader.rcBounds	= m_rcBounds;
			oMetaHeader.cbSize		= m_cb;
			oMetaHeader.ptSize		= m_ptSize;
			oMetaHeader.cbSave		= m_cbSave ;
			oMetaHeader.compression	= m_fCompression;
			oMetaHeader.filter		= m_fFilter;

			WmfPlaceableFileHeader oWmfHeader = {};
			oMetaHeader.ToWMFHeader(&oWmfHeader);
			
			LONG lLenHeader = 22;
			BYTE* pMetaHeader = new BYTE[lLenHeader];
			memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

			oMetaFile.SetHeader(pMetaHeader, lLenHeader);
			
			oMetaFile.SetData(m_pvBits, oMetaHeader.cbSave, oMetaHeader.cbSize, 0 == oMetaHeader.compression);

			//if (pos < sz)
			//{
			//	NSFile::CFileBinary oFile;

			//	if (oFile.CreateFile(L"d:\\blop.dat"))
			//	{
			//		BYTE * d = Reader->ReadBytes( sz - pos, true );
			//		if (d)
			//		{
			//			oFile.WriteFile (d, sz - pos);
			//			delete []d;
			//			oFile.CloseFile();
			//		}
			//	}
			//}
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
