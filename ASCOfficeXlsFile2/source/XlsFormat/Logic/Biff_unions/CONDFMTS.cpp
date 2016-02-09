
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
	bool res = false;
	
	int count = proc.repeated<Parenthesis_CONDFMTS_1>(0, 0);
	if (count > 0) res = true;
	while(count > 0)
	{
		m_arCONDFMT.insert(m_arCONDFMT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	count = proc.repeated<Parenthesis_CONDFMTS_2>(0, 0);
	if (count > 0) res = true;
	while(count > 0)
	{
		m_arCFEx.insert(m_arCFEx.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	return res;
}


int CONDFMTS::serialize(std::wostream & stream)
{
	if (m_arCONDFMT.empty()) return 0;

	for (int i = 0 ; i < m_arCONDFMT.size(); i++)
	{
		if (!m_arCONDFMT[i]) continue;
		m_arCONDFMT[i]->serialize(stream);
	}

	return 0;
}
} // namespace XLS

