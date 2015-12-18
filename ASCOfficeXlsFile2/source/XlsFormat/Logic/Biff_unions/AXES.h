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

	int serialize(std::wostream & _stream);
	int serialize(std::wostream & _stream, bool secondary);

	std::vector<BaseObjectPtr >	m_arAxes;
	std::vector<BaseObjectPtr >	m_arATTACHEDLABEL;

//--------------------------------------------------------------------
	std::vector<std::pair<int, int>>	m_arAxesId;
	
	BaseObjectPtr						m_PlotArea_FRAME;
	BaseObjectPtr						m_Wall_FRAME;
	BaseObjectPtr						m_Floor_FRAME;
};

} // namespace XLS

