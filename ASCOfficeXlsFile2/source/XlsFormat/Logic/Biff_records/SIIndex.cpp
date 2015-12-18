
#include "SIIndex.h"

#include <Logic/Biff_records/Number.h>
#include <Logic/Biff_records/BoolErr.h>
#include <Logic/Biff_records/Blank.h>
#include <Logic/Biff_records/Label.h>


namespace XLS
{

SIIndex::SIIndex()
{
}


SIIndex::~SIIndex()
{
}


BaseObjectPtr SIIndex::clone()
{
	return BaseObjectPtr(new SIIndex(*this));
}


void SIIndex::writeFields(CFRecord& record)
{
	record << numIndex;
}


void SIIndex::readFields(CFRecord& record)
{
	record >> numIndex;
}

int SIIndex::serialize(std::wostream & _stream, int idx, const CellRef & in_ref)
{
	int res = 0;
	CP_XML_WRITER(_stream)    
	{	
		for (int i = 0 ; i < m_arData.size(); i++)
		{
			Number	* number	= dynamic_cast<Number*>	(m_arData[i].get());
			BoolErr * boolErr	= dynamic_cast<BoolErr*>(m_arData[i].get());
			Blank	* blank		= dynamic_cast<Blank *>	(m_arData[i].get());
			Label	* label		= dynamic_cast<Label *>	(m_arData[i].get());
			
			if (number)
			{
				if (in_ref == number->cell.getLocation())
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << number->num; }
					}
					res = 1;
				}
			}
			else if (label)
			{
				if (in_ref == label->cell.getLocation())
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << label->st.value(); }
					}
					res = 1;
				}
			}
		}
	}
	return res;
}

int SIIndex::serialize(std::wostream & _stream, ChartParsedFormula & in_ref)
{
	int idx = 0;
	CP_XML_WRITER(_stream)    
	{	
		for (int i = 0 ; i < m_arData.size(); i++)
		{
			Number	* number	= dynamic_cast<Number*>	(m_arData[i].get());
			BoolErr * boolErr	= dynamic_cast<BoolErr*>(m_arData[i].get());
			Blank	* blank		= dynamic_cast<Blank *>	(m_arData[i].get());
			Label	* label		= dynamic_cast<Label *>	(m_arData[i].get());
			
			if (number)
			{
				if (in_ref.inRange(number->cell.getLocation()))
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << number->num; }
					}
				}
			}
			else if (label)
			{
				if (in_ref.inRange(label->cell.getLocation()))
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << label->st.value(); }
					}
				}
			}
			else
			{//??
				CP_XML_NODE(L"c:pt")
				{
					CP_XML_ATTR(L"idx", idx++);
					m_arData[i]->serialize( CP_XML_STREAM());
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

