#include "precompiled_xls.h"
#include "SXADDLHIERARCHY.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLGRPLEVEL.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLHIERARCHY::SXADDLHIERARCHY()
{
}


SXADDLHIERARCHY::~SXADDLHIERARCHY()
{
}


BaseObjectPtr SXADDLHIERARCHY::clone()
{
	return BaseObjectPtr(new SXADDLHIERARCHY(*this));
}


/*
SXADDLHIERARCHY = SXAddl_SXCHierarchy_SXDId *Continue_SxaddlSxString *SXAddl_SXCHierarchy_SXDProperty 
				*SXADDLGRPLEVEL [SXAddl_SXCHierarchy_SXDVerUpdInv] *SXAddl_SXCHierarchy_SXDFilterMember 
				[SXAddl_SXCHierarchy_SXDVerUpdInv] 
				[SXAddl_SXCHierarchy_SXDSXSetParentUnique *Continue_SxaddlSxString] [SXAddl_SXCHierarchy_SXDIconSet] 
				[SXAddl_SXCHierarchy_SXDUserCaption *Continue_SxaddlSxString] 
				*UNKNOWNFRT [SXAddl_SXCHierarchy_SXDVerUpdInv] *SXAddl_SXCHierarchy_SXDUserCaption 
				[SXAddl_SXCHierarchy_SXDVerUpdInv] [SXAddl_SXCHierarchy_SXDInfo12] 
				[SXAddl_SXCHierarchy_SXDDisplayFolder *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDMeasureGrp *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDParentKPI *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIValue *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIGoal *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIStatus *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPITrend *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIWeight *Continue_SxaddlSxString]
				[SXAddl_SXCHierarchy_SXDKPITime *Continue_SxaddlSxString] SXAddl_SXCHierarchy_SXDEnd
*/
const bool SXADDLHIERARCHY::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCHierarchy_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.repeated<SXAddl_SXCHierarchy_SXDProperty>(0, 0);
	proc.repeated<SXADDLGRPLEVEL>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();

	if(proc.optional<SXAddl_SXCHierarchy_SXDSXSetParentUnique>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.optional<SXAddl_SXCHierarchy_SXDIconSet>();
	if(proc.optional<SXAddl_SXCHierarchy_SXDUserCaption>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();
	proc.repeated<SXAddl_SXCHierarchy_SXDUserCaption>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();
	proc.optional<SXAddl_SXCHierarchy_SXDInfo12>();
	if(proc.optional<SXAddl_SXCHierarchy_SXDDisplayFolder>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDMeasureGrp>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDParentKPI>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIValue>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIGoal>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIStatus>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPITrend>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIWeight>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPITime>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	proc.mandatory<SXAddl_SXCHierarchy_SXDEnd>();

	return true;
}

} // namespace XLS

