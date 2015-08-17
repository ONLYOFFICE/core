
#include "CONDFMTS.h"
#include <Logic/Biff_unions/CONDFMT.h>
#include <Logic/Biff_unions/CONDFMT12.h>
#include <Logic/Biff_records/CFEx.h>
#include <Logic/Biff_records/CF12.h>

namespace XLS
{


CONDFMTS::CONDFMTS()
{
}


CONDFMTS::~CONDFMTS()
{
}


class Parenthesis_CONDFMTS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CONDFMTS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CONDFMTS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return proc.optional<CONDFMT>() ||
				proc.optional<CONDFMT12>();
	};
};

class Parenthesis_CONDFMTS_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CONDFMTS_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CONDFMTS_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<CFEx>())
		{
			return false;
		}
		//proc.optional<CF12>(); // TODO: uncomment this and pass a base cell reference to the constructor
		return true;
	};
};



BaseObjectPtr CONDFMTS::clone()
{
	return BaseObjectPtr(new CONDFMTS(*this));
}


// CONDFMTS = *(CONDFMT / CONDFMT12) *(CFEx [CF12])
const bool CONDFMTS::loadContent(BinProcessor& proc)
{
	bool res1 = proc.repeated<Parenthesis_CONDFMTS_1>(0, 0);
	bool res2 = proc.repeated<Parenthesis_CONDFMTS_2>(0, 0);
	return res1 || res2;
}

} // namespace XLS

