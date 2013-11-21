#include "stdafx.h"
#include "MulBlank.h"

namespace XLS
{;

MulBlank::MulBlank()
{
}


MulBlank::~MulBlank()
{
}


BaseObjectPtr MulBlank::clone()
{
	return BaseObjectPtr(new MulBlank(*this));
}


void MulBlank::writeFields(CFRecord& record)
{
#pragma message("####################### MulBlank record is not implemented")
	Log::error("MulBlank record is not implemented.");
	//record << some_value;
}


void MulBlank::readFields(CFRecord& record)
{
	// A little hack to extract colLast before it is used
	record.skipNBytes(record.getDataSize() - sizeof(WORD));
	record >> colLast;
	record.resetPointerToBegin();
	//------------------
	record >> rw >> colFirst;

	rgixfe.load(record, colLast - colFirst + 1);

	record.skipNBytes(sizeof(WORD));
}

const long MulBlank::GetRow() const
{
	return static_cast<WORD>(rw);
}




BiffStructurePtr IXFCellMulBlankSpecial::clone()
{
	return BiffStructurePtr(new IXFCellMulBlankSpecial(*this));
}


void IXFCellMulBlankSpecial::toXML(MSXML2::IXMLDOMElementPtr xml_tag)
{
	if(rgixfe.size() != 0)
	{
		static _bstr_t IXFCell_tag_name(L"IXFCell");
		for(std::vector<WORD>::const_iterator it = rgixfe.begin(), itEnd = rgixfe.end(); it != itEnd; ++it)
		{
			MSXML2::IXMLDOMElementPtr IXFCell_tag = XMLSTUFF::makeXMLNode(IXFCell_tag_name, xml_tag);
			IXFCell_tag->Puttext(STR::int2str(*it, 10).c_str());
		}
	}
	else
	{
		xml_tag->setAttribute(L"common_ixfe", common_ixfe);
	}
}


void IXFCellMulBlankSpecial::load(CFRecord& record, const size_t num_cells)
{
	WORD ixfe;
	for(size_t i = 0; i < num_cells; ++i)
	{
		record >> ixfe;
		rgixfe.push_back(ixfe);
	}
	if(rgixfe.size() != 0)
	{
		common_ixfe = rgixfe[0];
		size_t j = 0;
		for(; j < rgixfe.size(); ++j)
		{
			if(rgixfe[j] != common_ixfe) break;
		}
		if(j == rgixfe.size())
		{
			rgixfe.clear();
		}
	}
	else
	{
		common_ixfe = 0; // just to avoid warnings
	}
}



} // namespace XLS

