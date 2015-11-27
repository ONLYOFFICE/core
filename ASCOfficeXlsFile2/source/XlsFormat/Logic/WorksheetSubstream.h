#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_structures/CellRef.h>

namespace XLS
{;

// Logical representation of WorksheetSubstream union of records 
class WorksheetSubstream;
typedef boost::shared_ptr<WorksheetSubstream>	WorksheetSubstreamPtr;

class WorksheetSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(WorksheetSubstream)
public:
	WorksheetSubstream(const size_t ws_index);
	~WorksheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	static const ElementType	type = typeWorksheetSubstream;

	size_t						ws_index_;

	std::vector<BaseObjectPtr>	m_arMergeCells;
	std::vector<BaseObjectPtr>	m_arWINDOW;
	
	BaseObjectPtr				m_GLOBALS;
	BaseObjectPtr				m_PAGESETUP;
	BaseObjectPtr				m_BACKGROUND;
	BaseObjectPtr				m_COLUMNS;
	BaseObjectPtr				m_CELLTABLE;
	BaseObjectPtr				m_SHFMLA_SET;
	BaseObjectPtr				m_Dimensions;
	BaseObjectPtr				m_SORTANDFILTER;
	BaseObjectPtr				m_OBJECTS;
	std::vector<BaseObjectPtr>	m_arHLINK;
};

} // namespace XLS

