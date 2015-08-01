#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

template<class Type>
class IMsoArray : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IMsoArray)
public:
	XLS::BiffStructurePtr clone(){return XLS::BiffStructurePtr(new IMsoArray(*this));}

	static const XLS::ElementType	type = XLS::typeIMsoArray;

	virtual void load(XLS::CFRecord& record)
	{
		unsigned short nElems, nElemsAlloc;
		unsigned short cbElem;
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
