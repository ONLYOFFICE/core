
#include "SXTBL.h"
#include <Logic/Biff_records/SXTbl.h>
#include <Logic/Biff_unions/DREF.h>
#include <Logic/Biff_records/SxTbpg.h>
#include <Logic/Biff_records/SXTBRGIITM.h>
#include <Logic/Biff_records/SXString.h>

namespace XLS
{


SXTBL::SXTBL()
{
}


SXTBL::~SXTBL()
{
}

class Parenthesis_SXTBL_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXTBL_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXTBL_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXTBRGIITM>())
		{
			return false;
		}
		proc.repeated<SXString>(0, 0);
		return true;
	};
};


BaseObjectPtr SXTBL::clone()
{
	return BaseObjectPtr(new SXTBL(*this));
}


// SXTBL = SXTbl *DREF *SxTbpg *(SXTBRGIITM *SXString)
const bool SXTBL::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<SXTbl>())
	{
		return false;
	}
	proc.repeated<DREF>(0, 0);
	proc.repeated<SxTbpg>(0, 0);
	proc.repeated<Parenthesis_SXTBL_1>(0, 0);

	return true;
}

} // namespace XLS

