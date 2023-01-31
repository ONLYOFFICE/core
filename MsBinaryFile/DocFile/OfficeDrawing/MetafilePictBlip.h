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
#include "../../../OOXML/Base/Base.h"
#include "../../../DesktopEditor/common/File.h"

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

	CMetaHeader();

	void ToWMFHeader(WmfPlaceableFileHeader* pHeader);
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
	CMetaFileBuffer();
	~CMetaFileBuffer();

	void SetHeader(BYTE* pHeader, LONG lSize);

	void SetData(BYTE* pCompress, LONG lCompressSize, LONG lUncompressSize, bool bIsCompressed);

	int ToBuffer(BYTE *& Data);

	void ToFile(NSFile::CFileBinary* pFile);
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

		MetafilePictBlip();
		MetafilePictBlip( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance );
		virtual ~MetafilePictBlip();

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance );

		/// Decompresses the bits of the picture if the picture is decompressed.
		/// If the picture is not compressed, it returns original unsigned char array.
		unsigned long Decompress( unsigned char **buffer );
	};
}
