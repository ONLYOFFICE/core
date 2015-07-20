
#include "SXADDLCACHEITEM.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{;


SXADDLCACHEITEM::SXADDLCACHEITEM()
{
}


SXADDLCACHEITEM::~SXADDLCACHEITEM()
{
}


class Parenthesis_SXADDLCACHEITEM_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEITEM_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEITEM_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheItem_SXDSxrmitmDisp>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};


class Parenthesis_SXADDLCACHEITEM_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEITEM_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEITEM_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheItem_SXDItmMpMapCount>())
		{
			return false;
		}
		proc.mandatory<SXAddl_SXCCacheItem_SXDItmMpropMap>();
		return true;
	};
};


// SXADDLCACHEITEM = SXAddl_SXCCacheItem_SXDId [SXAddl_SXCCacheItem_SXDSxrmitmDisp *Continue_SxaddlSxString] 
BaseObjectPtr SXADDLCACHEITEM::clone()
{
	return BaseObjectPtr(new SXADDLCACHEITEM(*this));
}


//                   *(SXAddl_SXCCacheItem_SXDItmMpMapCount SXAddl_SXCCacheItem_SXDItmMpropMap)
const bool SXADDLCACHEITEM::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCCacheItem_SXDId>())
	{
		return false;
	}
	proc.optional<Parenthesis_SXADDLCACHEITEM_1>();
	proc.repeated<Parenthesis_SXADDLCACHEITEM_2>(0, 0);

	return true;
}

} // namespace XLS

