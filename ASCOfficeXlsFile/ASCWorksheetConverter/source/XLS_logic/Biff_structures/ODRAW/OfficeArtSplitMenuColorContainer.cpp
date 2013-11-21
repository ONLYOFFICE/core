#include "stdafx.h"
#include "OfficeArtColorMRUContainer.h"
#include <XLS_bin/CFRecord.h>

namespace ODRAW
{;


OfficeArtColorMRUContainer::OfficeArtColorMRUContainer()
:	OfficeArtRecord(0, ColorMRUContainer)
{
}


XLS::BiffStructurePtr OfficeArtColorMRUContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtColorMRUContainer(*this));
}


void OfficeArtColorMRUContainer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	std::for_each(rgmsocr.begin(), rgmsocr.end(), boost::bind(&MSOCR::toXML, _1, xml_tag));
}


void OfficeArtColorMRUContainer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	MSOCR color;
	size_t number = 0;
	while(color.fromXML(xml_tag, ++number, false))
	{
		rgmsocr.push_back(color);
	}
}


void OfficeArtColorMRUContainer::storeFields(XLS::CFRecord& record)
{
	record << rgmsocr;
}


void OfficeArtColorMRUContainer::loadFields(XLS::CFRecord& record)
{
	for(size_t i = 0; i < rh_own.recInstance; ++i)
	{
		MSOCR color;
		record >> color;
		rgmsocr.push_back(color);
	}
}

const WORD OfficeArtColorMRUContainer::GetInstanceToStore()
{
	return rgmsocr.size();
}

} // namespace XLS
