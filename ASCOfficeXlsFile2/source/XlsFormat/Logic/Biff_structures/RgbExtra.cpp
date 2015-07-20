
#include "RgbExtra.h"
#include <Binary/CFRecord.h>
#include "PtgExtraArray.h"
#include "PtgExtraMem.h"
#include "PtgExtraElf.h"
#include "RevNameTabid.h"
#include "RevName.h"
#include "RevExtern.h"
#include "Ptg.h"


namespace XLS
{;


BiffStructurePtr RgbExtra::clone()
{
	return BiffStructurePtr(new RgbExtra(*this));
}


//void RgbExtra::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(!ptg_records.empty())
//	{
//		PtgQueue copy_queue(ptg_records);
//		while(!copy_queue.empty())
//		{
//			copy_queue.front()->toXML(xml_tag);
//			copy_queue.pop();
//		}
//	}
//}
//
//
//void RgbExtra::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//// No need special processing here
//}
//

void RgbExtra::store(CFRecord& record)
{
	while(ptg_records.size())
	{
		record << *ptg_records.front();
		ptg_records.pop();
	}
}


// This is just a stub for the virtual 'load' realization
void RgbExtra::load(CFRecord& record)
{
}


void RgbExtra::load(CFRecord& record, const PtgVector& records_from_rgce, bool is_part_of_a_revision)
{
	for(PtgVectorIterator it = records_from_rgce.begin(), 
		itEnd = records_from_rgce.end(); it != itEnd; ++it)
	{
		load(record, (*it)->getPtgId(), is_part_of_a_revision);
	}
}


void RgbExtra::load(CFRecord& record, unsigned short rgce_record_type, bool is_part_of_a_revision)
{
	PtgPtr extra_ptg;
	switch(rgce_record_type)
	{
		case 0x0020: // PtgArray type = REFERENCE
		case 0x0040: // PtgArray type = VALUE
		case 0x0060: // PtgArray type = ARRAY
			extra_ptg = PtgPtr(new PtgExtraArray);
			break;
		case 0x0026: // PtgMemArea type = REFERENCE
		case 0x0046: // PtgMemArea type = VALUE
		case 0x0066: // PtgMemArea type = ARRAY
			extra_ptg = PtgPtr(new PtgExtraMem);
			break;
		case 0x180B: // PtgElfRadicalS
		case 0x180D: // PtgElfColS
		case 0x180F: // PtgElfColSV
			extra_ptg = PtgPtr(new PtgExtraElf);
			break;
		case 0x0023: // PtgName type = REFERENCE
		case 0x0043: // PtgName type = VALUE
		case 0x0063: // PtgName type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevNameTabid);
			}
			break;
		case 0x0039: // PtgNameX type = REFERENCE
		case 0x0059: // PtgNameX type = VALUE
		case 0x0079: // PtgNameX type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevName);
			}
			break;
		case 0x003A: // PtgRef3d type = REFERENCE
		case 0x005A: // PtgRef3d type = VALUE
		case 0x007A: // PtgRef3d type = ARRAY
		case 0x003C: // PtgRefErr3d type = REFERENCE
		case 0x005C: // PtgRefErr3d type = VALUE
		case 0x007C: // PtgRefErr3d type = ARRAY
		case 0x003B: // PtgArea3d type = REFERENCE
		case 0x005B: // PtgArea3d type = VALUE
		case 0x007B: // PtgArea3d type = ARRAY
		case 0x003D: // PtgAreaErr3d type = REFERENCE
		case 0x005D: // PtgAreaErr3d type = VALUE
		case 0x007D: // PtgAreaErr3d type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevExtern);
			}
			break;
		default:
			// we skip the records that shall not have the corresponding record in RgbExtra
			break;
	}
	if(!!extra_ptg)
	{
		extra_ptg->load(record);
		ptg_records.push(extra_ptg);
	}
}


const PtgQueue& RgbExtra::getPtgs() const
{
	return ptg_records;
}


void RgbExtra::addPtg(const PtgPtr ptg)
{
	ptg_records.push(ptg);
}


const bool RgbExtra::isEmpty() const
{
	return ptg_records.empty();
}


} // namespace XLS

