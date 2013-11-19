#include "stdafx.h"
#include "SeriesList.h"

namespace XLS
{;

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

void RGISeriesListSpecial::toXML(MSXML2::IXMLDOMElementPtr xml_tag)
{
	if(series.size() != 0)
	{
		static _bstr_t tag_name(L"rgi");
		static _bstr_t attr_name(L"index");
		for(std::vector<WORD>::const_iterator it = series.begin(), itEnd = series.end(); it != itEnd; ++it)
		{
			MSXML2::IXMLDOMElementPtr rgi_tag = XMLSTUFF::makeXMLNode(tag_name, xml_tag);
			rgi_tag->setAttribute(attr_name,*it);
		}
	}
}


const bool RGISeriesListSpecial::fromXML(MSXML2::IXMLDOMElementPtr xml_tag)
{
	static _bstr_t query = L"rgi";
	static _bstr_t attr_name(L"index");
	MSXML2::IXMLDOMNodeListPtr rgi_nodes = xml_tag->selectNodes(query);
	MSXML2::IXMLDOMElementPtr rgi_node;
	while(rgi_node = rgi_nodes->nextNode())
	{
		WORD rgi = getStructAttribute(rgi_node, attr_name);
		series.push_back(rgi);
	}
	return true;
}


void RGISeriesListSpecial::load(CFRecord& record)
{
	WORD cser;
	record >> cser;
	for (size_t i = 0; i < cser; ++i)
	{
		WORD rgi;
		record >> rgi;
		series.push_back(rgi);
	}
}


void RGISeriesListSpecial::store(CFRecord& record)
{
	WORD cser = series.size();
	record >> cser >> series;
}



} // namespace XLS

