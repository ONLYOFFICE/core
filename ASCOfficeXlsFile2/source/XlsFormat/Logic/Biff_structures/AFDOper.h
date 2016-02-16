#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>

#include "AFDOperStr.h"
#include "AFDOperRk.h"
#include "AFDOperBoolErr.h"
#include "AFDOperXNum.h"

namespace XLS
{

class AFDOper: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOper)
public:
	BiffStructurePtr clone();

	AFDOper();
	AFDOper(bool bAutoFilter);
	~AFDOper();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeAFDOper;

	unsigned char	vt;
	unsigned char	grbitSign;
	
	AFDOperStr		vtValueStr;
	AFDOperBoolErr	vtValueBool;
	AFDOperRk		vtValueRk;
	AFDOperXNum		vtValueNum;	//64-bit binary floating-point number

	bool m_bAutoFilter;
};

} // namespace XLS
