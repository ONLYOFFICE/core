#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of QSIR union of records 
class QSIR: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(QSIR)
public:
	QSIR();
	~QSIR();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

