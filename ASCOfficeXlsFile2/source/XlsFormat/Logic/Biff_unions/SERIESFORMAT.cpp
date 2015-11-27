
#include "SERIESFORMAT.h"
#include <Logic/Biff_records/Series.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/SerToCrt.h>
#include <Logic/Biff_records/SerParent.h>
#include <Logic/Biff_records/SerAuxTrend.h>
#include <Logic/Biff_records/SerAuxErrBar.h>
#include <Logic/Biff_records/LegendException.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/AI.h>
#include <Logic/Biff_unions/SS.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/TEXTPROPS.h>

namespace XLS
{


SERIESFORMAT::SERIESFORMAT()
{
}


SERIESFORMAT::~SERIESFORMAT()
{
}


//  (SerToCrt / (SerParent (SerAuxTrend / SerAuxErrBar))) 
class Parenthesis_SERIESFORMAT_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESFORMAT_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESFORMAT_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<SerToCrt>())
		{
			proc.mandatory<SerParent>();
			if(!proc.optional<SerAuxTrend>())
			{
				proc.mandatory<SerAuxErrBar>();
			}
		}
		return true;
	};
};


// (LegendException [Begin ATTACHEDLABEL [TEXTPROPS] End])
class Parenthesis_SERIESFORMAT_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESFORMAT_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESFORMAT_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<LegendException>())
		{
			return false;
		}
		if(proc.optional<Begin>())
		{
			proc.mandatory<ATTACHEDLABEL>();
			proc.optional<TEXTPROPS>();
			proc.mandatory<End>();
		}
		return true;
	};
};


BaseObjectPtr SERIESFORMAT::clone()
{
	return BaseObjectPtr(new SERIESFORMAT(*this));
}


/*
SERIESFORMAT = Series Begin 4AI *SS (SerToCrt / (SerParent (SerAuxTrend / SerAuxErrBar))) 
				*(LegendException [Begin ATTACHEDLABEL [TEXTPROPS] End]) End
*/
const bool SERIESFORMAT::loadContent(BinProcessor& proc)
{
	int count = 0;

	if(!proc.mandatory<Series>())
	{
		return false;
	}
	m_Series = elements_.back();
	elements_.pop_back();
	
	proc.mandatory<Begin>();								elements_.pop_back(); //skip
	
	count = proc.repeated<AI>(4, 4);
	while(count > 0)
	{
		m_arAI.insert(m_arAI.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	count = proc.repeated<SS>(0, 0);
	while(count > 0)
	{
		m_arSS.insert(m_arSS.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	//доп серии
	if (proc.mandatory<Parenthesis_SERIESFORMAT_1>())
	{
		count = elements_.size();
		while(count > 0)
		{
			m_arDopSeries.insert(m_arDopSeries.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}
	}

	count = proc.repeated<Parenthesis_SERIESFORMAT_2>(0, 0); // это типо "нормальных"

	count = elements_.size();
	while(count > 0)
	{
		_series_ex ex;

		ex.legendException = elements_.front();
		elements_.pop_front(); count--;

		while ( count > 0 ) 
		{
			if ("ATTACHEDLABEL" == elements_.front()->getClassName())
			{
				ex.attachedLABEL = elements_.front();
				elements_.pop_front(); count--;
			}
			else if ("TEXTPROPS" == elements_.front()->getClassName())
			{
				ex.textPROPS = elements_.front();
				elements_.pop_front(); count--;
			}
			else if ("LegendException" == elements_.front()->getClassName())
			{
				break;
				//next
			}
			else if (	"Begin" == elements_.front()->getClassName() ||
						"End"	== elements_.front()->getClassName())
			{
				elements_.pop_front(); count--;
			}
		}
		m_SeriesEx.push_back(ex);
	}

	proc.mandatory<End>();									elements_.pop_back(); //skip

	return true;
}

} // namespace XLS

