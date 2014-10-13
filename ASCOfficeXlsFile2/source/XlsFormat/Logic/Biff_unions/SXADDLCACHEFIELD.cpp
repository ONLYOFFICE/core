#include "precompiled_xls.h"
#include "SXADDLCACHEFIELD.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCACHEITEM.h>

namespace XLS
{;


SXADDLCACHEFIELD::SXADDLCACHEFIELD()
{
}


SXADDLCACHEFIELD::~SXADDLCACHEFIELD()
{
}


class Parenthesis_SXADDLCACHEFIELD_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDCaption>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};



class Parenthesis_SXADDLCACHEFIELD_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDPropName>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDProperty>())
		{
			return false;
		}
		proc.optional<Parenthesis_SXADDLCACHEFIELD_3>();
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_4: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_4)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_4(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDIfdbMpMapCount>())
		{
			return false;
		}
		proc.mandatory<SXAddl_SXCCacheField_SXDIfdbMempropMap>();
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_5: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_5)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_5(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDSxrmitmCount>())
		{
			return false;
		}
		proc.repeated<SXADDLCACHEITEM>(0, 0);
		proc.mandatory<SXAddl_SXCCacheItem_SXDEnd>();
		return true;
	};
};




// SXADDLCACHEFIELD = SXAddl_SXCCacheField_SXDId *Continue_SxaddlSxString 
//                    [SXAddl_SXCCacheField_SXDCaption *Continue_SxaddlSxString] 
//                    [SXAddl_SXCCacheField_SXDProperty [SXAddl_SXCCacheField_SXDPropName *Continue_SxaddlSxString]] 
//                    [SXAddl_SXCCacheField_SXDIfdbMpMapCount SXAddl_SXCCacheField_SXDIfdbMempropMap] 
//                    [SXAddl_SXCCacheField_SXDSxrmitmCount *SXADDLCACHEITEM SXAddl_SXCCacheItem_SXDEnd] 
BaseObjectPtr SXADDLCACHEFIELD::clone()
{
	return BaseObjectPtr(new SXADDLCACHEFIELD(*this));
}


//                    SXAddl_SXCCacheField_SXDEnd
const bool SXADDLCACHEFIELD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCCacheField_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<Parenthesis_SXADDLCACHEFIELD_1>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_2>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_4>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_5>();
	proc.mandatory<SXAddl_SXCCacheField_SXDEnd>();
	return true;
}

} // namespace XLS

