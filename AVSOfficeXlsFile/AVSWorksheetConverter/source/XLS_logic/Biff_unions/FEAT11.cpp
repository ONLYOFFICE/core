#include "stdafx.h"
#include "FEAT11.h"
#include <XLS_logic/Biff_records/FeatHdr11.h>
#include <XLS_logic/Biff_records/Feature11.h>
#include <XLS_logic/Biff_records/Feature12.h>
#include <XLS_logic/Biff_records/ContinueFrt11.h>
#include <XLS_logic/Biff_records/List12.h>
#include <XLS_logic/Biff_records/AutoFilter12.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>
#include <XLS_logic/Biff_records/List12.h>
#include <XLS_logic/Biff_unions/SORTDATA12.h>

namespace XLS
{;


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
		proc.repeated<ContinueFrt11>(0, 0);
		proc.repeated<List12>(0, 0);
		
		while (proc.optional<AutoFilter12>())
		{
			proc.repeated<ContinueFrt12>(0, 0);
		}		
		
		proc.repeated<List12>(0, 0);
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
	proc.repeated<Parenthesis_FEAT11_1>(0, 0);
	return true;
}

} // namespace XLS

