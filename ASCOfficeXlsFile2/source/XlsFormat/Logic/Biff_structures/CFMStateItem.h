#pragma once

#include "BiffStructure.h"
#include "Boolean.h"
#include "CFVO.h"

namespace XLS
{;

class CFRecord;

class CFMStateItem : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFMStateItem)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeCFMStateItem;

	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	CFVO cfvo;
	Boolean<unsigned char> fEqual;
};

typedef boost::shared_ptr<CFMStateItem> CFMStateItemPtr;

} // namespace XLS

