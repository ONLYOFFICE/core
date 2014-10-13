#include "precompiled_xls.h"
#include "DBQUERYEXT.h"
#include <Logic/Biff_records/DBQueryExt.h>
#include <Logic/Biff_records/ExtString.h>
#include <Logic/Biff_records/OleDbConn.h>
#include <Logic/Biff_records/TxtQry.h>

namespace XLS
{;


DBQUERYEXT::DBQUERYEXT()
{
}


DBQUERYEXT::~DBQUERYEXT()
{
}


class Parenthesis_DBQUERYEXT_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERYEXT_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERYEXT_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<OleDbConn>())
		{
			return false;
		}
		proc.repeated<ExtString>(0, 0);
		return true;
	};
};


BaseObjectPtr DBQUERYEXT::clone()
{
	return BaseObjectPtr(new DBQUERYEXT(*this));
}


// DBQUERYEXT = DBQueryExt [ExtString] *4[OleDbConn *ExtString] [TxtQry *ExtString]
const bool DBQUERYEXT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DBQueryExt>())
	{
		return false;
	}
	proc.optional<ExtString>();
	proc.repeated<Parenthesis_DBQUERYEXT_1>(0, 4);
	if(proc.optional<TxtQry>())
	{
		proc.repeated<ExtString>(0, 0);
	}

	return true;
}

} // namespace XLS

