
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
	if(!proc.mandatory<Series>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.repeated<AI>(4, 4);
	proc.repeated<SS>(0, 0);
	proc.mandatory<Parenthesis_SERIESFORMAT_1>();
	proc.repeated<Parenthesis_SERIESFORMAT_2>(0, 0);
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

