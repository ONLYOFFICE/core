
#include "SXADDLSXMG.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLSXMG::SXADDLSXMG()
{
}


SXADDLSXMG::~SXADDLSXMG()
{
}


class Parenthesis_SXADDLSXMG_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLSXMG_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLSXMG_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCSXMg_SXDUserCaption>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};



// SXADDLSXMG = SXAddl_SXCSXMg_SXDId *Continue_SxaddlSxString 
BaseObjectPtr SXADDLSXMG::clone()
{
	return BaseObjectPtr(new SXADDLSXMG(*this));
}


//              [SXAddl_SXCSXMg_SXDUserCaption *Continue_SxaddlSxString] *UNKNOWNFRT SXAddl_SXCSXMg_SXDEnd
const bool SXADDLSXMG::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXMg_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<Parenthesis_SXADDLSXMG_1>();
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCSXMg_SXDEnd>();
	return true;
}

} // namespace XLS

