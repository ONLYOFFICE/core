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

	static const ElementType	type = typeXTI;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short iSupBook;
	short itabFirst;
	short itabLast;
};

typedef boost::shared_ptr<XTI> XTIPtr;


} // namespace XLS
