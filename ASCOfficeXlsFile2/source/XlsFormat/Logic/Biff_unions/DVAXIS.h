#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of DVAXIS union of records 
class DVAXIS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DVAXIS)
public:
	DVAXIS();
	~DVAXIS();

	BaseObjectPtr clone();

	virtual const bool loadContentRead(BinReaderProcessor& proc);
	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDVAXIS;

	unsigned int id;

	BaseObjectPtr	m_Axis;
	BaseObjectPtr	m_AXS;

	BaseObjectPtr	m_ValueRange;
	BaseObjectPtr	m_AXM;

	BaseObjectPtr	m_CatSerRange;
	BaseObjectPtr	m_Tick;
	BaseObjectPtr	m_FontX;
	
	BaseObjectPtr	m_AxisLine1;
	BaseObjectPtr	m_LineFormat1;
	BaseObjectPtr	m_AxisLine2;
	BaseObjectPtr	m_LineFormat2;


};

} // namespace XLS

