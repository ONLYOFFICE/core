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

#include "OfficeArtBlip.h"

#include "../../../../../../OfficeUtils/src/OfficeUtils.h"

namespace ODRAW
{

OfficeArtBlip::OfficeArtBlip(const unsigned short recType) :	OfficeArtRecord(0, recType)
{
	pict_size = 0;
	pict_data = NULL;
	pict_type = L".jpeg";

	rgbUid1 = L"";
	rgbUid2 = L"";
	result = false;
}

OfficeArtBlip::~OfficeArtBlip()
{
		if (pict_data)
		{
			delete[] pict_data;
			pict_data = NULL;
		}
}

void OfficeArtBlip::readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader & metafileHeader)
{
	if (metafileHeader.cbSave > record.getDataSize() - record.getRdPtr())
		return;
	else
		result = true;

	unsigned char* inBuff = new unsigned char[metafileHeader.cbSave];
	memcpy(inBuff, record.getCurData<unsigned char>(), metafileHeader.cbSave);		

	pict_size = metafileHeader.cbSize;
	pict_data = new char[pict_size];

	COfficeUtils decompressor(NULL);

	HRESULT hr = decompressor.Uncompress((unsigned char*)pict_data, ((unsigned long*)&pict_size), inBuff, metafileHeader.cbSave);
	delete [] inBuff;

	record.skipNunBytes(metafileHeader.cbSave);					

}
void OfficeArtBlip::loadFields(IBinaryReader* reader)
{
	OfficeArtRecordHeader rc_header;
	rc_header.load(reader);
	
	BYTE tag	= 0xff;	
	recType		= rc_header.recType;	
	bool isCompressed = false;
}

void OfficeArtBlip::loadFields(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rc_header;
	record >> rc_header;
	
	BYTE tag	= 0xff;	
	recType		= rc_header.recType;	
	bool isCompressed = false;

	switch (rc_header.recType)
	{
		case OfficeArtRecord::BlipEMF:
			{
				pict_type = L".emf";
				if (rc_header.recInstance == 0x3D4)
					rgbUid1 = ReadMD4Digest(record);
				else
				{
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
				}

				OfficeArtMetafileHeader metafileHeader;	
				record >> metafileHeader;

				if (metafileHeader.compression == 0)
				{
					isCompressed = true;
					readCompressedData(record, metafileHeader);
				}
			}
			break;
		case OfficeArtRecord::BlipWMF:
			{
				pict_type = L".wmf";
				if (rc_header.recInstance == 0x216)
					rgbUid1 = ReadMD4Digest(record);
				else
				{
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
				}

				OfficeArtMetafileHeader metafileHeader;	
				record >> metafileHeader;

				if (metafileHeader.compression == 0)
				{
					isCompressed = true;
					readCompressedData(record, metafileHeader);
				}
			}
			break;				
		case OfficeArtRecord::BlipPICT:
			{
				if (rc_header.recInstance == 0x542)
					rgbUid1 = ReadMD4Digest(record);
				else
				{
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
				}

				OfficeArtMetafileHeader metafileHeader;	
				record >> metafileHeader;

				if (metafileHeader.compression == 0)
				{
					isCompressed = true;
					readCompressedData(record, metafileHeader);
				}
				pict_type = L".pic";///???? todooo
			}
			break;				
		case OfficeArtRecord::BlipJPEG:
			pict_type = L".jpeg";
			if ((rc_header.recInstance == 0x46A) || (rc_header.recInstance == 0x6E2))
			{
				rgbUid1 = ReadMD4Digest(record);
			}
			else
			{
				rgbUid1 = ReadMD4Digest(record);
				rgbUid2 = ReadMD4Digest(record);
			}
			record >> tag;
			break;
		case OfficeArtRecord::BlipPNG:
			pict_type = L".png";
			if (rc_header.recInstance == 0x6E0) {
				rgbUid1 = ReadMD4Digest(record);
			}
			else
			{
				rgbUid1 = ReadMD4Digest(record);
				rgbUid2 = ReadMD4Digest(record);
			}
			record >> tag; 
			break;
		case OfficeArtRecord::BlipDIB:
			pict_type = L"dib_data"; // need write bmp header (for file) - or convert
			if (rc_header.recInstance == 0x7A8)
			{
				rgbUid1 = ReadMD4Digest(record);
			}
			else
			{
				rgbUid1 = ReadMD4Digest(record);
				rgbUid2 = ReadMD4Digest(record);
			}
			record >> tag;
			break;
		case OfficeArtRecord::BlipTIFF:
			pict_type = L".tiff";
			if (rc_header.recInstance == 0x6E4)
			{
				rgbUid1 = ReadMD4Digest(record);
			}
			else
			{
				rgbUid1 = ReadMD4Digest(record);
				rgbUid2 = ReadMD4Digest(record);
			}
			record >> tag;
			break;
		default:
			record.RollRdPtrBack(rc_header.size());
			return;
	}

	
	if (!isCompressed)
	{
		pict_size = record.getDataSize() - record.getRdPtr();

		if (pict_size > record.getDataSize() - record.getRdPtr())
			return;
		else
			result = true;

		{
			pict_data = new char[pict_size];
			memcpy(pict_data, record.getCurData<char>(), pict_size);
		}
		record.skipNunBytes(pict_size);
	}

	//std::ofstream fileOut("d:\\test.jpg", std::ios_base::binary);
	//fileOut.write(pict_data, pict_size);
	//fileOut.close();
}
} // namespace ODRAW
