#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of AXES union of records 
class AXES: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXES)
public:
	AXES();
	~AXES();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	std::vector<BaseObjectPtr >	m_arAxes;
	std::vector<BaseObjectPtr >	m_arATTACHEDLABEL;

	BaseObjectPtr				m_PlotArea;
	BaseObjectPtr				m_FRAME;
};

} // namespace XLS

