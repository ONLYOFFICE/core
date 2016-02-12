
#include "DROPBAR.h"
#include <Logic/Biff_records/DropBar.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/AreaFormat.h>
#include <Logic/Biff_unions/GELFRAME.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>
#include <Logic/Biff_records/End.h>
#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_records/EndObject.h>

namespace XLS
{


DROPBAR::DROPBAR()
{
}


DROPBAR::~DROPBAR()
{
}


BaseObjectPtr DROPBAR::clone()
{
	return BaseObjectPtr(new DROPBAR(*this));
}


// DROPBAR = DropBar Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool DROPBAR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DropBar>())
	{
		return false;
	}
	m_DropBar = elements_.back();			elements_.pop_back();
	
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
	if (proc.mandatory<End>())				elements_.pop_back();

	return true;
}

int DROPBAR::serialize(std::wostream & _stream)
{
	DropBar *dropBar = dynamic_cast<DropBar*>(m_DropBar.get());
	if (!dropBar) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:spPr")
		{
			if (m_GELFRAME)
				m_GELFRAME->serialize(CP_XML_STREAM());
			else if (m_AreaFormat)
				m_AreaFormat->serialize(CP_XML_STREAM());

			if (m_LineFormat)
				m_LineFormat->serialize(CP_XML_STREAM());
		}
	}
	return 0;
}

} // namespace XLS

