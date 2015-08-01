#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class DXFBdr : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFBdr)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFBdr;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char dgLeft;
	unsigned char dgRight;
	unsigned char dgTop;
	unsigned char dgBottom;
	unsigned char icvLeft;
	unsigned char icvRight;
	bool bitDiagDown;
	bool bitDiagUp;

	unsigned char icvTop;
	unsigned char icvBottom;
	unsigned char icvDiag;
	unsigned char dgDiag;

};

} // namespace XLS

