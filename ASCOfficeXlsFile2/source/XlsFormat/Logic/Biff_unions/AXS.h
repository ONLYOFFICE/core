#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of AXS union of records 
class AXS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXS)
public:
	AXS();
	~AXS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	struct _axis_line_format
	{
		BaseObjectPtr	axisLine;
		BaseObjectPtr	lineFormat;
	};
	std::vector<_axis_line_format> m_AxisLine_Format;

	BaseObjectPtr	m_IFmtRecord;
	BaseObjectPtr	m_Tick;
	BaseObjectPtr	m_FontX;
	BaseObjectPtr	m_AreaFormat;
	BaseObjectPtr	m_GELFRAME;

};

} // namespace XLS

