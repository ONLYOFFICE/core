#pragma once

#include "BiffStructure.h"
#include "BitMarkedStructs.h"

namespace XLS
{
class CFRecord;
class DXFN;

class DXFPat : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFPat)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFPat;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int serialize(std::wostream & stream);

	FillPattern fls;
	
	unsigned char icvForeground;
	unsigned char icvBackground;
//------------------------------------------------
	DXFN *parent;
};

} // namespace XLS

