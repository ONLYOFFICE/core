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
			default:
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


} // namespace XLS
