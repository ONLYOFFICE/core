
#include "BopPopCustom.h"

namespace XLS
{

BopPopCustom::BopPopCustom()
{
}


BopPopCustom::~BopPopCustom()
{
}


BaseObjectPtr BopPopCustom::clone()
{
	return BaseObjectPtr(new BopPopCustom(*this));
}


void BopPopCustom::writeFields(CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void BopPopCustom::readFields(CFRecord& record)
{
	record >> rggrbit;
}

BiffStructurePtr BopPopCustomPiesIndices::clone()
{
	return BiffStructurePtr(new BopPopCustomPiesIndices(*this));
}


void BopPopCustomPiesIndices::load(CFRecord& record)
{
	unsigned short cxi;
	record >> cxi;

	const unsigned short padding = 8 - cxi % 8;

	unsigned char bit_data = 0;

	for(unsigned short i = padding; i < cxi - 1 + padding; i++)
	{
		if(0 == i % 8 || padding == i)
		{
			record >> bit_data;
		}

		unsigned char mask = 1 << (7 - i % 8);

		if(0 != (bit_data & mask))
		{
			pie_indices.push_back(i - padding);
		}
	}
}

//
//void BopPopCustomPiesIndices::toXML(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(pie_indices.size() != 0)
//	{
//		static std::wstring  tag_name(L"pie");
//		static std::wstring  attr_name(L"index");
//		for(std::vector<unsigned short>::const_iterator it = pie_indices.begin(), itEnd = pie_indices.end(); it != itEnd; ++it)
//		{
//			BiffStructurePtr rgi_tag = XMLSTUFF::createElement(tag_name, xml_tag);
//			rgi_tag->setAttribute(attr_name, *it);
//		}
//	}
//}




} // namespace XLS

