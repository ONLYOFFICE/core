#include "precompiled_xls.h"

#include "OfficeArtContainer.h"
#include "OfficeArtDgContainer.h"
#include "OfficeArtFDG.h"
#include "OfficeArtFSPGR.h"
#include "OfficeArtFSP.h"
#include "OfficeArtFOPT.h"
#include "OfficeArtClientAnchorSheet.h"
#include "OfficeArtClientAnchorChart.h"
#include "OfficeArtClientAnchorHF.h"
#include "OfficeArtFRITContainer.h"
#include "OfficeArtFDGGBlock.h"
#include "OfficeArtColorMRUContainer.h"
#include "OfficeArtSplitMenuColorContainer.h"
#include "SimpleOfficeArtContainers.h"
#include <Binary/CFRecord.h>
//#include "Exception/WrongBiffRecord.h"
#include "OfficeArtBStoreContainer.h"
#include "OfficeArtTertiaryFOPT.h"

namespace ODRAW
{;


OfficeArtContainer::OfficeArtContainer(const unsigned char recVer, const unsigned __int16 recType, const OfficeArtClientAnchorType anchor_type)
:	OfficeArtRecord(recVer, recType), anchor_type_(anchor_type)
{
}


//void OfficeArtContainer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(child_records.begin(), child_records.end(), boost::bind(&OfficeArtRecord::toXML, _1, xml_tag));
//}
//
//
//void OfficeArtContainer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//#pragma message("####################### OfficeArtContainer composite has no BiffStructure::getXMLAttributes() implemented")
//	Log::error(" Error!!! OfficeArtContainer composite has no BiffStructure::getXMLAttributes() implemented.");
//}


void OfficeArtContainer::loadFields(XLS::CFRecord& record)
{
	size_t container_beginning_ptr = record.getRdPtr();

	while(record.getRdPtr() < container_beginning_ptr + rh_own.recLen)
	{
		OfficeArtRecordHeader rh_child;
		record >> rh_child;
		size_t child_beginning_ptr = record.getRdPtr();
		record.RollRdPtrBack(rh_child.size());

		OfficeArtRecordPtr art_record;
		switch (rh_child.recType)
		{
			case DgContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtDgContainer(anchor_type_));
				break;

			case FDG:
				art_record = OfficeArtRecordPtr(new OfficeArtFDG);
				break;

			case FRITContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtFRITContainer);
				break;

			case SpgrContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtSpgrContainer(anchor_type_));
				break;

			case SpContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtSpContainer(anchor_type_));
				break;

			case FSPGR:
				art_record = OfficeArtRecordPtr(new OfficeArtFSPGR);
				break;

			case FSP:
				art_record = OfficeArtRecordPtr(new OfficeArtFSP);
				break;

			case FOPT:
				art_record = OfficeArtRecordPtr(new OfficeArtFOPT);
				break;

			case ClientAnchor:
				switch(anchor_type_)
				{
					case CA_Chart:
						art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorChart);
						break;

					case CA_Sheet:
						art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorSheet);
						break;

					case CA_HF:
						art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorHF);
						break;					
				}
				break;

			case ClientData:
				art_record = OfficeArtRecordPtr(new OfficeArtClientData);
				break;

			case ClientTextbox:
				art_record = OfficeArtRecordPtr(new OfficeArtClientTextbox);
				break;

			case FDGGBlock:
				art_record = OfficeArtRecordPtr(new OfficeArtFDGGBlock);
				break;

			case ColorMRUContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtColorMRUContainer);
				break;

			case SplitMenuColorContainer:
				art_record = OfficeArtRecordPtr(new OfficeArtSplitMenuColorContainer);
				break;
			case BStoreContainer:
				Log::event("OfficeArtBStoreContainer record found. OfficeArtBStoreContainerFileBlock records count = " + STR::int2str(rh_child.recInstance) + 
					". RecLen = " + STR::int2str(rh_child.recLen));
				art_record = OfficeArtRecordPtr(new OfficeArtBStoreContainer);
				break;
			case TertiaryFOPT:
				//art_record = OfficeArtRecordPtr(new OfficeArtTertiaryFOPT);
				break;

		}
		if(art_record)
		{
			record >> *art_record;
			child_records.push_back(art_record);

			if(record.getRdPtr() != child_beginning_ptr + rh_child.recLen)
			{
				if(record.getRdPtr() < child_beginning_ptr + rh_child.recLen)
				{
					Log::warning(STR::int2wstr(child_beginning_ptr + rh_child.recLen - record.getRdPtr(), 10) + 
						L" unsigned chars were not processed while reading from OfficeArt record of type 0x" + 
						STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)));
				}
				else
				{
					throw;// EXCEPT::RT::WrongBiffRecord("Wrong data parsed in OfficeArt record of type 0x" + 
						//STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)), record.getTypeString());
				}
			}
		}
		else // If the found record is not implemented or unknown
		{
			size_t sz = rh_child.size();

			record.skipNunBytes(rh_child.size()); // skip header
			record.skipNunBytes(rh_child.recLen); // skip record data
			Log::warning(std::wstring(L"Unsupported OfficeArtRecord skipped (recType=0x") + 
				STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)) + std::wstring(L")"));
		}
	}


}

void OfficeArtContainer::storeFields(XLS::CFRecord& record)
{
	for(std::vector<OfficeArtRecordPtr>::const_iterator it = child_records.begin(), itEnd = child_records.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


} // namespace ODRAW
