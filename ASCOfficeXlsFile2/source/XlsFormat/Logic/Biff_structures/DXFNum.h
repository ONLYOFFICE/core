#pragma once

#include "BiffStructure.h"
#include "DXFNumUsr.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class DXFNum : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFNum)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFNum;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setIsUserDefined(const bool is_user_defined); 
	const bool getIsUserDefined() const; 

	_CP_OPT(bool)	is_user_defined_;
	DXFNumIFmt		fmt_id;
	DXFNumUsr		user_defined;

};

} // namespace XLS

