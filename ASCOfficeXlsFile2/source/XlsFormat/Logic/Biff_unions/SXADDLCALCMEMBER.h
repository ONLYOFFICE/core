#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLCALCMEMBER union of records 
class SXADDLCALCMEMBER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCALCMEMBER)
public:
	SXADDLCALCMEMBER();
	~SXADDLCALCMEMBER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLCALCMEMBER;
	
};

} // namespace XLS

