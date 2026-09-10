/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "OfficeArtBStoreContainerFileBlock.h"

#include "../../../../../../OfficeUtils/src/OfficeUtils.h"

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtBStoreContainerFileBlock::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtBStoreContainerFileBlock(*this));
}

void OfficeArtBStoreContainerFileBlock::readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader & metafileHeader)
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

void OfficeArtBStoreContainerFileBlock::load(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh_child;
	record >> rh_child;	
	record.RollRdPtrBack(rh_child.size());

	OfficeArtRecordPtr art_record;		
	if (rh_child.recType == OfficeArtRecord::FBSE)
	{			
		OfficeArtRecordHeader rc_header;
		record >> rc_header;

		record.skipNunBytes(18);
		unsigned short tag;
		record >> tag;
		
		_UINT32 size;
		record >> size;
		
		_UINT32 cRef;
		record >> cRef;
		
		_UINT32 foDelay;
		record >> foDelay;
		record.skipNunBytes(1);
		
		char cbName;
		record >> cbName;
		record.skipNunBytes(2);
		record.skipNunBytes(cbName);		

		record >> rc_header;
		size_t skipLen = 0;

		recType = rc_header.recType;
		
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
				pict_type = L".pcz";
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
			}
			break;
			case OfficeArtRecord::BlipJPEG:
				pict_type = L".jpeg";
				if ((rc_header.recInstance == 0x46A) || (rc_header.recInstance == 0x6E2))
				{
					skipLen = 17;
					rgbUid1 = ReadMD4Digest(record);
					record.RollRdPtrBack(16);
				}
				else
				{
					skipLen = 33;
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
					record.RollRdPtrBack(32);
				}
				break;
			case OfficeArtRecord::BlipPNG:
				pict_type = L".png";
				if (rc_header.recInstance == 0x6E0) {
					skipLen = 17;
					rgbUid1 = ReadMD4Digest(record);
					record.RollRdPtrBack(16);
				}
				else
				{
					skipLen = 33;
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
					record.RollRdPtrBack(32);
				}
				break;
			case OfficeArtRecord::BlipDIB:
				pict_type = L"dib_data";
				if (rc_header.recInstance == 0x7A8)
				{
					skipLen = 17;
					rgbUid1 = ReadMD4Digest(record);
					record.RollRdPtrBack(16);
				}
				else
				{
					skipLen = 33;
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
					record.RollRdPtrBack(32);
				}

				break;
			case OfficeArtRecord::BlipTIFF:
				pict_type = L".tiff";
				if (rc_header.recInstance == 0x6E4)
				{
					skipLen = 17;
					rgbUid1 = ReadMD4Digest(record);
					record.RollRdPtrBack(16);
				}
				else
				{
					skipLen = 33;
					rgbUid1 = ReadMD4Digest(record);
					rgbUid2 = ReadMD4Digest(record);
					record.RollRdPtrBack(32);
				}
				break;
			case 0xf018:
			{
				record.skipNunBytes(rc_header.recLen);
				return;
			}break;
			default: //0xf007 
				record.RollRdPtrBack(rc_header.size());
				return;
		}

		
		if (!isCompressed)
		{
			record.skipNunBytes(skipLen);
			pict_size = rc_header.recLen - skipLen;

			if (pict_size > record.getDataSize() - record.getRdPtr())
			{
				return;
			}
			else
				result = true;

			{
				pict_data = new char[pict_size];
				memcpy(pict_data, record.getCurData<char>(), pict_size);
			}
			record.skipNunBytes(pict_size);
		}

		/*std::ofstream fileOut("d:\\test.jpg", std::ios_base::binary);
		fileOut.write(record.getCurData<char>(), dataSize);
		fileOut.close();*/
	}
}


const void WriteMD4Digest(std::wstring UidStr, XLS::CFRecord& record)
{
	if(UidStr.size() < 16 )
		UidStr = L"0000000000000000";
	for(int i = 0; i < 16; i++)
	{
		unsigned char hex_data = UidStr.at(i);
		record << hex_data;
	}
}

void OfficeArtBStoreContainerFileBlock::save(XLS::CFRecord& record)
{
	//fbse
	{
		OfficeArtRecordHeader FbseHeader;
		FbseHeader.recVer = 2;
		if(pict_type == L".emf")
			FbseHeader.recInstance =  0x2;
		else if(pict_type == L".wmf")
			FbseHeader.recInstance =  0x3;
		else if(pict_type == L".png")
			FbseHeader.recInstance =  0x6;
		else
			FbseHeader.recInstance = 0x5;
		FbseHeader.recType =  0xF007;
		FbseHeader.recLen = pict_size + 36;
		if(pict_type == L".emf" || pict_type == L".wmf")
		  FbseHeader.recLen += 58;
		else
			FbseHeader.recLen += 25;
		if(!nameData.empty())
			FbseHeader.recLen += nameData.size()+1;
		record << FbseHeader;
		BYTE btOs = FbseHeader.recInstance;
		record << btOs << btOs;
		WriteMD4Digest(rgbUid1, record);
		unsigned short tag = 0xFF;
		record << tag;
		unsigned int Size = pict_size + 17 + 8;
		if(pict_type == L".emf" || pict_type == L".wmf")
			Size += 33; //metadata block
		record << Size;
		unsigned int Cref = 1;
		record << Cref;
		unsigned int foDelay = 0;
		record << foDelay;
		record.reserveNunBytes(1);
		BYTE cbName = 0;
		if(!nameData.empty())
			cbName = nameData.size()+1;
		record << cbName;

		record.reserveNunBytes(2);
		if(cbName)
		{
			for(auto i : nameData)
			{
				record << i;
			}
			BYTE terminal = L'\0';
			record  << terminal;
		}
	}

	if(pict_type == L".emf" || pict_type == L".wmf")
	{
		OfficeArtRecordHeader rc_header;
		rc_header.recVer = 0;
		if(pict_type == L".emf")
		{
			rc_header.recInstance = 0x3D4;
			rc_header.recType =  0xF01A;
		}
		else if(pict_type == L".wmf")
		{
			rc_header.recInstance = 0x216;
			rc_header.recType =  0xF01B;
		}
		rc_header.recLen = pict_size + 50;
		record << rc_header;
		record.reserveNunBytes(16);

		_UINT32 cbSize = pict_size;
		record << cbSize;
		record.reserveNunBytes(16);
		_UINT64 PtSize = 0;
		record << PtSize;
		_UINT32 cbSave = pict_size;
		record << cbSave;
		BYTE compression = 0xFE;
		record << compression;
		BYTE filter = 0xFE;
		record << filter;

	}
	else
	{
		OfficeArtRecordHeader rc_header;
		rc_header.recVer = 0;
		if(pict_type == L".png")
		{
			rc_header.recInstance = 0x6E0;
			rc_header.recType =  0xF01E;
		}
		else
		{
			rc_header.recInstance = 0x46A;
			rc_header.recType =  0xF01D;
		}
		rc_header.recLen = pict_size + 17;
		record << rc_header;
		record.reserveNunBytes(16);
		BYTE tag = 0xFF;
		record << tag;
	}

	//record.appendRawDataToStatic((BYTE*)pict_data, pict_size);
}


} // namespace XLS
