#pragma once

#include "../BiffStructTagged.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

template<class Type>
class IMsoArray : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(IMsoArray)
public:
	XLS::BiffStructurePtr clone(){return XLS::BiffStructurePtr(new IMsoArray(*this));}

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	{
		for (std::vector<Type>::iterator it = data.begin(), itEnd = data.end(); it != itEnd; ++it)
		{
			it->toXML(xml_tag);
		}
	}
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag) {}
	virtual void load(XLS::CFRecord& record)
	{
		WORD nElems, nElemsAlloc;
		WORD cbElem;
		record >> nElems >> nElemsAlloc >> cbElem;
		while(nElems--)
		{
			Type element;
			record >> element;
			data.push_back(element);
		}
	}
	virtual void store(XLS::CFRecord& record){}

public:
	std::vector<Type> data;
};



} // namespace XLS
