
#include "FRAME.h"
#include <Logic/Biff_records/Frame.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/AreaFormat.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/GELFRAME.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>

#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_records/EndObject.h>

#include <utils.h>

namespace XLS
{


FRAME::FRAME()
{
}


FRAME::~FRAME()
{
}


BaseObjectPtr FRAME::clone()
{
	return BaseObjectPtr(new FRAME(*this));
}


// FRAME = Frame Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool FRAME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Frame>())
	{
		return false;
	}
	m_Frame = elements_.back();
	elements_.pop_back();

	if (proc.mandatory<Begin>())			elements_.pop_back();

	if (proc.mandatory<LineFormat>())
	{
		m_LineFormat = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<StartObject>())
	{
		elements_.pop_back();
		if (proc.mandatory<EndObject>())	elements_.pop_back();
	}
	if (proc.mandatory<AreaFormat>())
	{
		m_AreaFormat = elements_.back();	elements_.pop_back();
	}

	if (proc.optional<GELFRAME>())
	{
		m_GELFRAME = elements_.back();		elements_.pop_back();
	}

	if (proc.optional<SHAPEPROPS>())
	{
		m_SHAPEPROPS = elements_.back();	elements_.pop_back();
	}

	if(proc.mandatory<End>())				elements_.pop_back();

	return true;
}

int FRAME::serialize(std::wostream & _stream)
{
	AreaFormat * area = dynamic_cast<AreaFormat*>(m_AreaFormat.get());
	LineFormat * line = dynamic_cast<LineFormat*>(m_LineFormat.get());

	bool bArea = m_GELFRAME ? true : false;
	if (!bArea && (area) && (area->fAuto == false)) bArea = true;

	bool bLine = false;
	if ((line) && (line->fAuto == false)) bLine = true;

	if (!bArea && !bLine) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:spPr")
		{
			if (bArea)
			{
				if (m_GELFRAME)
					m_GELFRAME->serialize(CP_XML_STREAM());
				else if (m_AreaFormat) m_AreaFormat->serialize(CP_XML_STREAM());
			}
			
			if (m_LineFormat && bLine) m_LineFormat->serialize(CP_XML_STREAM());
		}
	}
	return 0;
}
} // namespace XLS

