#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of GELFRAME union of records 
class GELFRAME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GELFRAME)
public:
	GELFRAME();
	~GELFRAME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeGELFRAME;

	int serialize(std::wostream & stream_);

	GlobalWorkbookInfoPtr global_info;

	BaseObjectPtr	m_GelFrame;
	BaseObjectPtr	m_PICF;
};

} // namespace XLS

