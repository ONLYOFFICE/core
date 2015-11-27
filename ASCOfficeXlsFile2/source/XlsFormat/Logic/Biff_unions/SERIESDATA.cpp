
#include "SERIESDATA.h"
#include <Logic/Biff_records/Dimensions.h>
#include <Logic/Biff_records/SIIndex.h>
#include <Logic/Biff_records/Number.h>
#include <Logic/Biff_records/BoolErr.h>
#include <Logic/Biff_records/Blank.h>
#include <Logic/Biff_records/Label.h>

namespace XLS
{


SERIESDATA::SERIESDATA()
{
}


SERIESDATA::~SERIESDATA()
{
}


//  (Number / BoolErr / Blank / Label)
class Parenthesis_SERIESDATA_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESDATA_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESDATA_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<Number>() ||
				proc.optional<BoolErr>() ||
				proc.optional<Blank>() ||
				proc.optional<Label>();
	};
};


//  (SIIndex *(Number / BoolErr / Blank / Label))
class Parenthesis_SERIESDATA_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESDATA_1)
public:
	std::vector<BaseObjectPtr>	m_arSERIESDATA;
	BaseObjectPtr				m_SIIndex;

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESDATA_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SIIndex>())
		{
			return false;
		}

		int count = proc.repeated<Parenthesis_SERIESDATA_2>(0, 0);
		return true;
	};
};



BaseObjectPtr SERIESDATA::clone()
{
	return BaseObjectPtr(new SERIESDATA(*this));
}


// SERIESDATA = Dimensions 3(SIIndex *(Number / BoolErr / Blank / Label))
const bool SERIESDATA::loadContent(BinProcessor& proc)
{
	if(!proc.optional<Dimensions>())
	{
		return false;
	}

	if (elements_.size() > 0)
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();

	}

	int count = proc.repeated<Parenthesis_SERIESDATA_1>(3, 3);

	int count1= elements_.size();
	while(count1 > 0)
	{
		if ("SIIndex" == elements_.front()->getClassName())
		{
			m_arSIIndex.push_back(elements_.front()); count--;
		}
		else
		{
			SIIndex * si_in = dynamic_cast<SIIndex *>(m_arSIIndex.back().get());
			if (si_in)
				si_in->m_arData.push_back(elements_.front());
		}
		elements_.pop_front(); count1--;
	}

	return true;
}

} // namespace XLS

