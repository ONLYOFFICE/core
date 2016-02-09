#pragma once

#include "BiffStructure.h"

namespace XLS
{
class CFRecord;
class DXFN;

class DXFProt : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFProt)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFProt;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int serialize(std::wostream & stream);

	bool fLocked;
	bool fHidden;
//------------------------------------------------
	DXFN *parent;
};

} // namespace XLS

