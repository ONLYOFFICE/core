#include "stdafx.h"
#include "OfficeArtFRITContainer.h"
#include <XLS_bin/CFRecord.h>

namespace ODRAW
{;


OfficeArtFRITContainer::OfficeArtFRITContainer()
:	OfficeArtRecord(0, FRITContainer)
{
}


XLS::BiffStructurePtr OfficeArtFRITContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFRITContainer(*this));
}


void OfficeArtFRITContainer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	std::for_each(rgfrit.begin(), rgfrit.end(), boost::bind(&OfficeArtFRIT::toXML, _1, xml_tag));
}


void OfficeArtFRITContainer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	OfficeArtFRIT frit;
	size_t number = 0;
	while(frit.fromXML(xml_tag, ++number, false))
	{
		rgfrit.push_back(frit);
	}
}


void OfficeArtFRITContainer::storeFields(XLS::CFRecord& record)
{
	record << rgfrit;
}


void OfficeArtFRITContainer::loadFields(XLS::CFRecord& record)
{
	size_t frits_start = record.getRdPtr();
	while(record.getRdPtr() < frits_start + rh_own.recLen)
	{
		OfficeArtFRIT frit;
		record >> frit;
		rgfrit.push_back(frit);
	}
}

const WORD OfficeArtFRITContainer::GetInstanceToStore()
{
	return rgfrit.size();
}

} // namespace XLS
