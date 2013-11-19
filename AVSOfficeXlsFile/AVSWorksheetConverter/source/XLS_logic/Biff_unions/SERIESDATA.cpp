#include "stdafx.h"
#include "SERIESDATA.h"
#include <XLS_logic/Biff_records/Dimensions.h>
#include <XLS_logic/Biff_records/SIIndex.h>
#include <XLS_logic/Biff_records/Number.h>
#include <XLS_logic/Biff_records/BoolErr.h>
#include <XLS_logic/Biff_records/Blank.h>
#include <XLS_logic/Biff_records/Label.h>

namespace XLS
{;


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
		proc.repeated<Parenthesis_SERIESDATA_2>(0, 0);
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
	/*if(!proc.mandatory<Dimensions>())
	{
		return false;
	}*/

	// fix
	if(!proc.optional<Dimensions>())
	{
		return false;
	}

	proc.repeated<Parenthesis_SERIESDATA_1>(3, 3);

	return true;
}

} // namespace XLS

