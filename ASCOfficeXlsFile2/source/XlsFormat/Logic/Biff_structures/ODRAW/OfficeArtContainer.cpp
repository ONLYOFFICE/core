

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
{


OfficeArtContainer::OfficeArtContainer(const unsigned char recVer, const unsigned short recType, const OfficeArtClientAnchorType anchor_type)
:	OfficeArtRecord(recVer, recType), anchor_type_(anchor_type)
{
}

void OfficeArtContainer::loadFields(XLS::CFRecord& record)
{
	size_t container_beginning_ptr = record.getRdPtr();

	while(record.getRdPtr() < container_beginning_ptr + rh_own.recLen - 8)
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

			case FConnectorRule:
				art_record = OfficeArtRecordPtr(new OfficeArtFConnectorRule);
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
				art_record = OfficeArtRecordPtr(new OfficeArtBStoreContainer);
				break;
			case TertiaryFOPT:
				art_record = OfficeArtRecordPtr(new OfficeArtTertiaryFOPT);
				break;

		}
		if(art_record)
		{
			record >> *art_record;
			child_records.push_back(art_record);

			if(record.getRdPtr() != child_beginning_ptr + rh_child.recLen)
			{
				if(record.getRdPtr() < child_beginning_ptr + rh_child.recLen - 8)
				{
					Log::warning(STR::int2wstr(child_beginning_ptr + rh_child.recLen - record.getRdPtr(), 10) + 
						L" unsigned chars were not processed while reading from OfficeArt record of type 0x" + 
						STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)));
				}
				else if(record.getRdPtr() > child_beginning_ptr + rh_child.recLen)
				{
					 //throw EXCEPT::RT::WrongBiffRecord("Wrong data parsed in OfficeArt record of type 0x" + 
						//STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)), record.getTypeString());
					Log::warning(std::wstring(L"Wrong data parsed in OfficeArt record of type 0x") + 
						STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)));

					record.RollRdPtrBack( record.getRdPtr() - child_beginning_ptr + rh_child.recLen);
				}
			}
		}
		else // If the found record is not implemented or unknown
		{
			try
			{
				size_t sz = rh_child.size();

				record.skipNunBytes(rh_child.size()); // skip header
				record.skipNunBytes(rh_child.recLen); // skip record data
				Log::warning(std::wstring(L"Unsupported OfficeArtRecord skipped (recType=0x") + 
					STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)) + std::wstring(L")"));
			}
			catch(...)
			{
			}
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
