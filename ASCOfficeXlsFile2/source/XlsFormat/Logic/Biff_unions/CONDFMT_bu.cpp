
#include "CONDFMT.h"
#include <Logic/Biff_records/CondFmt.h>
#include <Logic/Biff_records/CF.h>

namespace XLS
{


CONDFMT::CONDFMT()
{
}


CONDFMT::~CONDFMT()
{
}


BaseObjectPtr CONDFMT::clone()
{
	return BaseObjectPtr(new CONDFMT(*this));
}


// CONDFMT = CondFmt 1*3CF
const bool CONDFMT::loadContent(BinProcessor& proc)
{
	CondFmt cond_fmt;
	if(!proc.mandatory(cond_fmt))
	{
		return false;
	}
	m_CondFmt = elements_.back();
	elements_.pop_back();
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt>())
	{
		return false;
	}
	*/
	{
		CF cf1(cond_fmt.getLocation());
		if (proc.optional(cf1))
		{
			m_arCF.push_back( elements_.back());
			elements_.pop_back();
		}

		CF cf2(cond_fmt.getLocation());
		if(proc.optional(cf2))
		{
			m_arCF.push_back( elements_.back());
			elements_.pop_back();

			CF cf3(cond_fmt.getLocation());
			if (proc.optional(cf3))
			{
				m_arCF.push_back( elements_.back());
				elements_.pop_back();
			}
		}
	}
	/*  The above is equal to:
		proc.repeated<CF>(1, 3);
	*/
	return true;
}
int CONDFMT::serialize(std::wostream & stream)
{
	if (!m_CondFmt) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"conditionalFormatting")
		{
			CondFmt * condFmt	= dynamic_cast<CondFmt*>(m_CondFmt.get());

			CP_XML_ATTR(L"sqref", condFmt->sqref.sqref);
			//condition_id
			//condition_pos

			for (int i = 0; i < m_arCF.size(); i++)
			{
				if (m_arCF[i] == NULL) continue;
				m_arCF[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}
} // namespace XLS

