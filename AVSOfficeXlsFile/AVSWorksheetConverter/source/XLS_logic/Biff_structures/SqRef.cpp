#include "stdafx.h"
#include "SqRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr SqRef::clone()
{
	return BiffStructurePtr(new SqRef(*this));
}


void SqRef::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	std::for_each(rgrefs.begin(), rgrefs.end(), boost::bind(&Ref8U::toXML, _1, xml_tag));
}


void SqRef::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	Ref8U ref1;
	size_t counter = 0;
	while(ref1.fromXML(xml_tag, ++counter))
	{
		rgrefs.push_back(ref1);
	}
}


void SqRef::store(CFRecord& record)
{
	WORD cref = rgrefs.size();
	record << cref;
	for(std::vector<Ref8U>::iterator it = rgrefs.begin(), itEnd = rgrefs.end(); it != itEnd; ++it)
	{
		it->store(record);
	}
}


void SqRef::load(CFRecord& record)
{
	WORD cref;
	record >> cref;
	for(int i = 0; i < cref; ++i)
	{
		Ref8U refu;
		record >> refu;
		rgrefs.push_back(refu);
	}
}


} // namespace XLS

