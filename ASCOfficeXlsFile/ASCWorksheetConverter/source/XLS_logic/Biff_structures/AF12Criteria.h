#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include "AFDOper.h"

namespace XLS
{;

class AF12Criteria: public BiffStructTagged
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(AF12Criteria)
public:
	BiffStructurePtr clone();

	AF12Criteria();
	~AF12Criteria();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	AFDOper doper;
	XLUnicodeStringNoCch str;
	BIFF_BSTR _str;
};

typedef boost::shared_ptr<AF12Criteria> AF12CriteriaPtr;

} // namespace XLS