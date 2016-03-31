#pragma once

#include "BiffStructure.h"
#include "CFVO.h"
#include "CFColor.h"

namespace XLS
{

class CFRecord;

class CFDatabar : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFDatabar)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeCFMultistate;//todoooo
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char iPercentMin;
	unsigned char iPercentMax;

	bool fShowValue;
	bool fRightToLeft;

	CFColor color;

	CFVO cfvoDB1;
	CFVO cfvoDB2;
};

typedef boost::shared_ptr<CFDatabar> CFDatabarPtr;

} // namespace XLS

