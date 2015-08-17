#pragma once

#include <Logic/CompositeObject.h>
namespace XLS
{

class CFStreamCacheReader;

// Logical representation of INTERFACE union of records 
// _T suffix is to avoid confusing with one of standard defines
class INTERFACE_T: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(INTERFACE)
public:
	INTERFACE_T();
	~INTERFACE_T();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeINTERFACE_T;
};

} // namespace XLS
