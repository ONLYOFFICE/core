
// auto inserted precompiled begin
#include "precompiled_common.h"
// auto inserted precompiled end

#include "ZIndex.h"


namespace Common
{

	ZIndex::ZIndex()
		: Index(0)			
	{
	}


	ZIndex::ZIndex(const int& index)
		: Index(index)
	{
	}


	void ZIndex::fromBase(const ZIndex& zIndex)
	{
		Index	= zIndex.Index;
	}

} // namespace Common