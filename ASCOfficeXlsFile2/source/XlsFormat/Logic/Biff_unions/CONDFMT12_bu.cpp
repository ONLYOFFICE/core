
#include "CONDFMT12.h"
#include <Logic/Biff_records/CondFmt12.h>
#include <Logic/Biff_records/CF12.h>

namespace XLS
{


CONDFMT12::CONDFMT12()
{
}


CONDFMT12::~CONDFMT12()
{
}


BaseObjectPtr CONDFMT12::clone()
{
	return BaseObjectPtr(new CONDFMT12(*this));
}


// CONDFMT12 = CondFmt12 1*CF12
const bool CONDFMT12::loadContent(BinProcessor& proc)
{
	CondFmt12 cond_fmt12;
	if(!proc.mandatory(cond_fmt12))
	{
		return false;
	}
	m_CondFmt12 = elements_.back();
	elements_.pop_back();
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt12>())
	{
		return false;
	}
	*/
	{
		CF12 cf12(cond_fmt12.getLocation());
		proc.optional(cf12);

		for(;/*infinity*/;)
		{
			CF12 cf12(cond_fmt12.getLocation());
			if(!proc.optional(cf12))
			{
				break;
			}

		}
	}
	while(!elements_.empty())
	{
		m_arCF12.push_back(elements_.front());
		elements_.pop_front();	
	}
	/*  The above is equal to:
	proc.repeated<CF12>(1, 0);
	*/
	return true;

}
int CONDFMT12::serialize(std::wostream & stream)
{
	if (!m_CondFmt12)			return 0;
	if (m_arCF12.size() < 1)	return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"conditionalFormatting")
		{
			CondFmt12 * condFmt	= dynamic_cast<CondFmt12*>(m_CondFmt12.get());

			CP_XML_ATTR(L"sqref", condFmt->mainCF.sqref.sqref);
			//condition_id
			//condition_pos
			for (int i = 0; i < m_arCF12.size(); i++)
			{
				if (m_arCF12[i] == NULL) continue;
				m_arCF12[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}
} // namespace XLS

