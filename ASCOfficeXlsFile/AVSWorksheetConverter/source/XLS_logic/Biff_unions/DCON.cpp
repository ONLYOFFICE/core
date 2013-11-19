#include "stdafx.h"
#include "DCON.h"
#include <XLS_logic/Biff_records/DCon.h>
#include <XLS_logic/Biff_records/DConName.h>
#include <XLS_logic/Biff_records/DConBin.h>
#include <XLS_logic/Biff_records/DConRef.h>

namespace XLS
{;


DCON::DCON()
{
}


DCON::~DCON()
{
}


class Parenthesis_DCON_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DCON_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DCON_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<DConName>() ||
				proc.optional<DConBin>() ||
				proc.optional<DConRef>();
	};
};



BaseObjectPtr DCON::clone()
{
	return BaseObjectPtr(new DCON(*this));
}


// DCON = DCon *(DConName / DConBin / DConRef)
const bool DCON::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DCon>())
	{
		return false;
	}
	proc.repeated<Parenthesis_DCON_1>(0, 0);

	return true;
}

} // namespace XLS

