
#include "OfficeArtRGFOPTE.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtRGFOPTE::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtRGFOPTE(*this));
}

//
//void OfficeArtRGFOPTE::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(rgfopte.begin(), rgfopte.end(), boost::bind(&OfficeArtFOPTE::toXML, _1, xml_tag));
//}
//
//void OfficeArtRGFOPTE::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	// We don't support xlsx to xls conversion
//}


void OfficeArtRGFOPTE::store(XLS::CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void OfficeArtRGFOPTE::load(XLS::CFRecord& record)
{
// 	size_t end = record.getRdPtr() + data_size;
	for(int i = 0; i < options_num; ++i)
	{
		OfficeArtFOPTEPtr fopte = OfficeArtFOPTE::load_and_create(record);
		rgfopte.push_back(fopte);
	}

	for(std::vector<OfficeArtFOPTEPtr>::iterator it = rgfopte.begin(), itEnd = rgfopte.end(); it != itEnd; ++it)
	{
		if((*it)->fComplex && (*it)->op > 0)
		{
			(*it)->ReadComplexData(record);
		}
	}
}

void OfficeArtRGFOPTE::SetOptionsNumber(const unsigned short number)
{
	options_num = number;
}


const unsigned short OfficeArtRGFOPTE::GetOptionsNumber()
{
	return options_num;
}

} // namespace XLS
