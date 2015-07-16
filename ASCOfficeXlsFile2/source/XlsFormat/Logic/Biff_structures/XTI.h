#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class XTI : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XTI)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned short iSupBook;
	short itabFirst;
	short itabLast;
};

typedef boost::shared_ptr<XTI> XTIPtr;


} // namespace XLS
