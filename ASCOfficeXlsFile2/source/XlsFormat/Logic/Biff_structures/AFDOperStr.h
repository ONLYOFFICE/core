#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>


namespace XLS
{

class AFDOperStr : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOperStr)
public:
	BiffStructurePtr clone();

	AFDOperStr();
	AFDOperStr(bool bAutoFilter);
	~AFDOperStr();

	static const ElementType	type = typeAFDOperStr;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char	cch;
	unsigned char	fCompare;	

	bool m_bAutoFilter;
};

} // namespace XLS
