
#include "FEAT11.h"
#include <Logic/Biff_records/FeatHdr11.h>
#include <Logic/Biff_records/Feature11.h>
#include <Logic/Biff_records/Feature12.h>
#include <Logic/Biff_records/ContinueFrt11.h>
#include <Logic/Biff_records/List12.h>
#include <Logic/Biff_records/AutoFilter12.h>
#include <Logic/Biff_records/ContinueFrt12.h>
#include <Logic/Biff_records/List12.h>
#include <Logic/Biff_unions/SORTDATA12.h>

namespace XLS
{


FEAT11::FEAT11()
{
}


FEAT11::~FEAT11()
{
}



class Parenthesis_FEAT11_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FEAT11_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FEAT11_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<Feature11>())
		{
			if(!proc.optional<Feature12>())
			{
				return false;
			}
		}
		int count = 0;
		count = proc.repeated<ContinueFrt11>(0, 0);
		count = proc.repeated<List12>(0, 0);
		
		while (proc.optional<AutoFilter12>())
		{
			count = proc.repeated<ContinueFrt12>(0, 0);
		}		
		
		count = proc.repeated<List12>(0, 0);
		proc.optional<SORTDATA12>();
		return true;
	};
};


BaseObjectPtr FEAT11::clone()
{
	return BaseObjectPtr(new FEAT11(*this));
}


/*
FEAT11 = FeatHdr11 *((Feature11 / Feature12) *ContinueFrt11 *List12 [AutoFilter12 *ContinueFrt12] *List12 [SORTDATA12])
*/
const bool FEAT11::loadContent(BinProcessor& proc)
{
	if (!proc.mandatory<FeatHdr11>())
	{
		return false;
	}
	m_FeatHdr11 = elements_.back();
	elements_.pop_back();
	
	int count = proc.repeated<Parenthesis_FEAT11_1>(0, 0);

	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeFeature11 || 
			elements_.front()->get_type() == typeFeature12 )
		{
			_data new_data;
			new_data.m_Feature = elements_.front();

			m_arFEAT.push_back(new_data);
		}

		if (elements_.front()->get_type() == typeList12)
		{
			if (m_arFEAT.back().m_AutoFilter12)
				m_arFEAT.back().m_arList12_second.push_back(elements_.front());
			else
				m_arFEAT.back().m_arList12.push_back(elements_.front());
		}
		if (elements_.front()->get_type() == typeAutoFilter12)
		{
			m_arFEAT.back().m_AutoFilter12 = elements_.front();
		}
		if (elements_.front()->get_type() == typeSORTDATA12)
		{
			m_arFEAT.back().m_SORTDATA12 = elements_.front();
		}
		elements_.pop_front();
	}
	return true;
}

} // namespace XLS

