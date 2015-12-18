
#include "AI.h"
#include <Logic/Biff_records/BRAI.h>
#include <Logic/Biff_records/SeriesText.h>

//#include <Logic/Biff_unions/PAGESETUP.h>

namespace XLS
{


AI::AI()
{
}


AI::~AI()
{
}


BaseObjectPtr AI::clone()
{
	return BaseObjectPtr(new AI(*this));
}


// AI = BRAI [SeriesText]
const bool AI::loadContent(BinProcessor& proc)
{
	// fix
	if(!proc.optional<BRAI>())
	{
		return false;
	}
	m_BRAI = elements_.back();
	elements_.pop_back();

	if (proc.optional<SeriesText>())
	{
		m_SeriesText = elements_.back();
		elements_.pop_back();
	}

	return true;
}

int AI::serialize(std::wostream & _stream)
{
	BRAI* brai	= dynamic_cast<BRAI*>(m_BRAI.get());

	CP_XML_WRITER(_stream)    
	{
		if ((brai) && (brai->formula.getAssembledFormula().empty() == false))
		{
			CP_XML_NODE(L"c:tx")
			{
				CP_XML_NODE(L"c:strRef") 
				{
					CP_XML_NODE(L"c:f")
					{
						CP_XML_STREAM() << brai->formula.getAssembledFormula();
					}
				}
			}
		}
		else
		{
			SeriesText * text = dynamic_cast<SeriesText *>(m_SeriesText.get());
			if ((text) && (text->stText.value().empty() == false))
			{
				CP_XML_NODE(L"c:tx")
				{
					CP_XML_NODE(L"c:v") 
					{
						CP_XML_STREAM() << text->stText.value();
					}
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

