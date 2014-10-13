#include "precompiled_xls.h"
#include "SXADDLDBQUERY.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{;


SXADDLDBQUERY::SXADDLDBQUERY()
{
}


SXADDLDBQUERY::~SXADDLDBQUERY()
{
}


BaseObjectPtr SXADDLDBQUERY::clone()
{
	return BaseObjectPtr(new SXADDLDBQUERY(*this));
}


/*
SXADDLDBQUERY = [SXAddl_SXCQuery_SXDXMLSource *Continue_SxaddlSxString] 
				[SXAddl_SXCQuery_SXDSrcDataFile *Continue_SxaddlSxString] 
				[SXAddl_SXCQuery_SXDSrcConnFile *Continue_SxaddlSxString] 
				[SXAddl_SXCQuery_SXDReconnCond] SXAddl_SXCQuery_SXDEnd
*/
const bool SXADDLDBQUERY::loadContent(BinProcessor& proc)
{
	if(proc.optional<SXAddl_SXCQuery_SXDXMLSource>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCQuery_SXDSrcDataFile>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCQuery_SXDSrcConnFile>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.optional<SXAddl_SXCQuery_SXDReconnCond>();
	proc.mandatory<SXAddl_SXCQuery_SXDEnd>();

	return true;
}

} // namespace XLS

