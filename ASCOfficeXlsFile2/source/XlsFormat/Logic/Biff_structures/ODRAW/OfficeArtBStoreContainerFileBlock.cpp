#include "precompiled_xls.h"
#include "OfficeArtBStoreContainerFileBlock.h"
#include <Document/Document.h>

#import "..\..\..\Redist\ASCOfficeUtils.dll" named_guids raw_interfaces_only rename_namespace("AVSOfficeUtilsLib")

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtBStoreContainerFileBlock::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtBStoreContainerFileBlock(*this));
}


//void OfficeArtBStoreContainerFileBlock::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{		
//	bin_data_id = Document::appendBinaryData(xml_tag, pict_data, data_size);
//	xml_tag->setAttribute("bin_data_id", bin_data_id);
//	xml_tag->setAttribute("type", pict_type);	
//	xml_tag->setAttribute("rgbUid1", rgbUid1.c_str());
//	xml_tag->setAttribute("rgbUid2", rgbUid2.c_str());
//}
//
//void OfficeArtBStoreContainerFileBlock::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}

void OfficeArtBStoreContainerFileBlock::store(XLS::CFRecord& record)
{	
}

void OfficeArtBStoreContainerFileBlock::readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader metafileHeader)
{
	if (metafileHeader.cbSave > record.getDataSize() - record.getRdPtr())
		return;
	else
		result = true;

	unsigned char* inBuff = new unsigned char[metafileHeader.cbSave];
	memcpy(inBuff, record.getCurData<unsigned char>(), metafileHeader.cbSave);		

	data_size = metafileHeader.cbSize;
	pict_data = new char[data_size];

	CoInitialize(NULL);
	AVSOfficeUtilsLib::IOfficeUtilsPtr decompressor;
	decompressor.CreateInstance(AVSOfficeUtilsLib::CLSID_COfficeUtils);

	HRESULT hr = decompressor->Uncompress((unsigned char*)pict_data, ((unsigned long*)&data_size), inBuff, metafileHeader.cbSave);
	decompressor.Release();
	delete [] inBuff;

	record.skipNunBytes(metafileHeader.cbSave);					

	CoUninitialize();
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
		unsigned __int16 tag;
		record >> tag;
		unsigned int size;
		record >> size;
		unsigned int cRef;
		record >> cRef;
		unsigned int foDelay;
		record >> foDelay;
		record.skipNunBytes(1);
		char cbName;
		record >> cbName;
		record.skipNunBytes(2);
		record.skipNunBytes(cbName);		

		// read OfficeArtBlipPNG, OfficeArtBlipJPG ...			
		record >> rc_header;
		size_t skipLen = 0;
		bool isCompressed = false;
		bool dumpToFile = true;
		recType = rc_header.recType;

		switch (rc_header.recType)
		{
			case OfficeArtRecord::BlipEMF:
				{
					pict_type = L"emf";
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
					pict_type = L"wmf";
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
				}
				break;				
			case OfficeArtRecord::BlipJPEG:
				pict_type = L"jpeg";
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
				pict_type = L"png";
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

				dumpToFile = false;
				break;
			case OfficeArtRecord::BlipTIFF:
				pict_type = L"tiff";
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
			data_size = rc_header.recLen - skipLen;

			if (data_size > record.getDataSize() - record.getRdPtr())
				return;
			else
				result = true;

			if (dumpToFile)
			{
				pict_data = new char[data_size];
				memcpy(pict_data, record.getCurData<char>(), data_size);
			}
			record.skipNunBytes(data_size);
		}

		/*std::ofstream fileOut("d:\\test.jpg", std::ios_base::binary);
		fileOut.write(record.getCurData<char>(), dataSize);
		fileOut.close();*/
	}
}


} // namespace XLS
