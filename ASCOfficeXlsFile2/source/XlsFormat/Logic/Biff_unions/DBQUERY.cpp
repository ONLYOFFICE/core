
#include "DBQUERY.h"
#include <Logic/Biff_records/DbOrParamQry.h>
#include <Logic/Biff_records/SXString.h>

namespace XLS
{


DBQUERY::DBQUERY()
{
}


DBQUERY::~DBQUERY()
{
}


class Parenthesis_DBQUERY_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXString>())
		{
			return false;
		}
		proc.mandatory<DbOrParamQry>();
		return true;
	};
};


class Parenthesis_DBQUERY_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<DbOrParamQry>())
		{
			return false;
		}
		proc.repeated<Parenthesis_DBQUERY_3>(0, 0);
		return true;
	};
};


class Parenthesis_DBQUERY_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERY_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERY_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXString>())
		{
			return false;
		}
		proc.repeated<SXString>(0, 0);
		proc.optional<Parenthesis_DBQUERY_2>();
		return true;
	};
};


BaseObjectPtr DBQUERY::clone()
{
	return BaseObjectPtr(new DBQUERY(*this));
}


// DBQUERY = DbOrParamQry [1*SXString [DbOrParamQry *(SXString DbOrParamQry)]] *SXString
const bool DBQUERY::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<DbOrParamQry>())
	{
		return false;
	}
	proc.optional<Parenthesis_DBQUERY_1>();
	proc.repeated<SXString>(0, 0);

	return true;
}

} // namespace XLS

