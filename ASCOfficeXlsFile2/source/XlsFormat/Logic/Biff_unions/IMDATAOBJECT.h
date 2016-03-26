#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{
// Logical representation of IMDATAOBJECT union of records 
class IMDATAOBJECT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(IMDATAOBJECT)
public:
	IMDATAOBJECT()
	{
	}
	~IMDATAOBJECT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeIMDATAOBJECT;
	
	BaseObjectPtr		m_IMDATA;
};

} // namespace XLS

