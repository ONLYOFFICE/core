
#include "FEAT.h"
#include <Logic/Biff_records/FeatHdr.h>
#include <Logic/Biff_records/Feat.h>
#include <Logic/Biff_records/ContinueFrt.h>

namespace XLS
{;


FEAT::FEAT()
{
}


FEAT::~FEAT()
{
}


class Parenthesis_FEAT_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FEAT_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FEAT_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<Feat>())
		{
			return false;
		}
		proc.repeated<ContinueFrt>(0, 0);
		return true;
	};
};



BaseObjectPtr FEAT::clone()
{
	return BaseObjectPtr(new FEAT(*this));
}


// FEAT = FeatHdr *(Feat *ContinueFrt)
const bool FEAT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory(FeatHdr(false)))
	{
		return false;
	}
	proc.repeated<Parenthesis_FEAT_1>(0, 0);

	return true;
}

} // namespace XLS

