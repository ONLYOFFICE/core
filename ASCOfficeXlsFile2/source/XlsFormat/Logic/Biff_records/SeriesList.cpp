
#include "SeriesList.h"

namespace XLS
{

SeriesList::SeriesList()
{
}


SeriesList::~SeriesList()
{
}


BaseObjectPtr SeriesList::clone()
{
	return BaseObjectPtr(new SeriesList(*this));
}


void SeriesList::writeFields(CFRecord& record)
{
	rgiser.store(record);
}


void SeriesList::readFields(CFRecord& record)
{
	rgiser.load(record);
}


BiffStructurePtr RGISeriesListSpecial::clone()
{
	return BiffStructurePtr(new RGISeriesListSpecial(*this));
}

//void RGISeriesListSpecial::toXML(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(series.size() != 0)
//	{
//		static std::wstring  tag_name(L"rgi");
//		static std::wstring  attr_name(L"index");
//		for(std::vector<unsigned short>::const_iterator it = series.begin(), itEnd = series.end(); it != itEnd; ++it)
//		{
//			MSXML2::IXMLDOMElementPtr rgi_tag = XMLSTUFF::createElement(tag_name, xml_tag);
//			rgi_tag->setAttribute(attr_name,*it);
//		}
//	}
//}
//
//
//const bool RGISeriesListSpecial::fromXML(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	static std::wstring  query = L"rgi";
//	static std::wstring  attr_name(L"index");
//	MSXML2::IXMLDOMNodeListPtr rgi_nodes = xml_tag->selectNodes(query);
//	MSXML2::IXMLDOMElementPtr rgi_node;
//	while(rgi_node = rgi_nodes->nextNode())
//	{
//		unsigned short rgi = getStructAttribute(rgi_node, attr_name);
//		series.push_back(rgi);
//	}
//	return true;
//}


void RGISeriesListSpecial::load(CFRecord& record)
{
	unsigned short cser;
	record >> cser;
	for (size_t i = 0; i < cser; ++i)
	{
		unsigned short rgi;
		record >> rgi;
		series.push_back(rgi);
	}
}


void RGISeriesListSpecial::store(CFRecord& record)
{
	unsigned short cser = series.size();
	record >> cser >> series;
}



} // namespace XLS

