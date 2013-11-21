#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class XTI : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XTI)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD iSupBook;
	short itabFirst;
	short itabLast;
};

typedef boost::shared_ptr<XTI> XTIPtr;


} // namespace XLS
