
#include "ExtRst.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExtRst::clone()
{
	return BiffStructurePtr(new ExtRst(*this));
}


//void ExtRst::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cb", cb);
//	phs.toXML(xml_tag);
//	rphssub.toXML(xml_tag);
//	std::for_each(rgphruns.begin(), rgphruns.end(), boost::bind(&PhRuns::toXML, _1, xml_tag));
//}

//void ExtRst::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	cb = getStructAttribute(xml_tag, L"cb");
//	phs.fromXML(xml_tag);
//	rphssub.fromXML(xml_tag);
//
//	PhRuns runs;
//	size_t number = 0;
//	while(runs.fromXML(xml_tag, ++number, false))
//	{
//		rgphruns.push_back(runs);
//	}
//}
//

void ExtRst::store(CFRecord& record)
{
	unsigned short reserved = 1;
	record  << reserved << cb << phs << rphssub << rgphruns;
}


void ExtRst::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cb;
	size_t data_start = record.getRdPtr();
	record >> phs >> rphssub;
	for(unsigned short i = 0; i < rphssub.getRunsNumber(); ++i)
	{
		PhRuns run;
		record >> run;
		rgphruns.push_back(run);
	}
	size_t data_end = record.getRdPtr();
	if(data_end - data_start < cb)
	{
		record.skipNunBytes(cb - (data_end - data_start)); // trash for unknown reason
	}

}


const size_t ExtRst::getSize() const
{
	return 2/*reserved*/ + sizeof(unsigned short) /*cb*/ + sizeof(unsigned short) * 2/*phs*/ + rphssub.getSize() + (rgphruns.size() ? rgphruns.size() * sizeof(short) * 3 : 0);
}


} // namespace XLS
