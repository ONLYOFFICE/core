#pragma once

#include "BiffStructure.h"
namespace XLS
{

class CFRecord;

//#pragma pack(1)
class BuiltInStyle : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BuiltInStyle)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeBuiltInStyle;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char istyBuiltIn;
	unsigned char iLevel;
};
//#pragma pack() // restore

} // namespace XLS
