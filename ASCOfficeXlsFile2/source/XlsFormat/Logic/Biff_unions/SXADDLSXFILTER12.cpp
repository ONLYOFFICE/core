
#include "SXADDLSXFILTER12.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{


SXADDLSXFILTER12::SXADDLSXFILTER12()
{
}


SXADDLSXFILTER12::~SXADDLSXFILTER12()
{
}


BaseObjectPtr SXADDLSXFILTER12::clone()
{
	return BaseObjectPtr(new SXADDLSXFILTER12(*this));
}


/*
SXADDLSXFILTER12 = SXAddl_SXCSXFilter12_SXDId SXAddl_SXCSXFilter12_SXDSXFilter 
					[SXAddl_SXCSXFilter12_SXDCaption *Continue_SxaddlSxString] 
					[SXAddl_SXCSXFilter12_SXDSXFilterDesc *Continue_SxaddlSxString] 
					[SXAddl_SXCSXFilter12_SXDSXFilterValue1 *Continue_SxaddlSxString] 
					[SXAddl_SXCSXFilter12_SXDSXFilterValue2 *Continue_SxaddlSxString] 
					SXAddl_SXCSXFilter12_SXDXlsFilter 
					[SXAddl_SXCSXFilter12_SXDXlsFilterValue1 *Continue_SxaddlSxString] 
					[SXAddl_SXCSXFilter12_SXDXlsFilterValue2 *Continue_SxaddlSxString] SXAddl_SXCSXFilter12_SXDEnd
*/
const bool SXADDLSXFILTER12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXFilter12_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCSXFilter12_SXDSXFilter>();

	if(proc.optional<SXAddl_SXCSXFilter12_SXDCaption>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCSXFilter12_SXDSXFilterDesc>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCSXFilter12_SXDSXFilterValue1>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCSXFilter12_SXDSXFilterValue2>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.mandatory<SXAddl_SXCSXFilter12_SXDXlsFilter>();
	if(proc.optional<SXAddl_SXCSXFilter12_SXDXlsFilterValue1>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCSXFilter12_SXDXlsFilterValue2>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.mandatory<SXAddl_SXCSXFilter12_SXDEnd>();

	return true;
}

} // namespace XLS

