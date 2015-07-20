
#include "PIVOTADDL.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCALCMEMBER.h>
#include <Logic/Biff_unions/SXADDLHIERARCHY.h>
#include <Logic/Biff_unions/SXADDLFIELD.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>
#include <Logic/Biff_unions/SXADDLCONDFMTS.h>
#include <Logic/Biff_unions/SXADDLSXFILTERS12.h>

namespace XLS
{;


PIVOTADDL::PIVOTADDL()
{
}


PIVOTADDL::~PIVOTADDL()
{
}
/*
int b = 5;
boost::shared_ptr<int> by;

void raw_ptr(const int* ptr) // указатель на константный объект.
{
	ptr = &b;  // компилится, потому что указатель не константный
	*ptr = 4;  // не компилится, потому что объект константный
}
// то же самое для shared_ptr
void raw_ptr(boost::shared_ptr<const int> ptr) // указатель на константный объект.
{
	ptr = by;  // компилится, потому что указатель не константный
	*ptr = 4;  // не компилится, потому что объект константный
}
// ===================== теперь наоборот
void raw_ptr2(int* const ptr) // константный указатель на неконстантный объект.
{
	ptr = &b;  // не компилится, потому что указатель - константный
	*ptr = 4;  // компилится, потому что объект не константный
}
// то же самое для shared_ptr
void raw_ptr2(const boost::shared_ptr<int> ptr) // константный указатель на не константный объект.
{
	ptr = by;  // не компилится, потому что указатель - константный
	*ptr = 4;  // компилится, потому что объект не константный
}
*/


BaseObjectPtr PIVOTADDL::clone()
{
	return BaseObjectPtr(new PIVOTADDL(*this));
}


/*
PIVOTADDL = SXAddl_SXCView_SXDId *Continue_SxaddlSxString [SXAddl_SXCView_SXDVer10Info] 
			[SXAddl_SXCView_SXDVer12Info] *SXADDLCALCMEMBER *SXADDLHIERARCHY *SXADDLFIELD *UNKNOWNFRT 
			[SXAddl_SXCView_SXDTableStyleClient] [SXAddl_SXCView_SXDCompactRwHdr *Continue_SxaddlSxString] 
			[SXAddl_SXCView_SXDCompactColHdr *Continue_SxaddlSxString] [SXAddl_SXCView_SXDVerUpdInv] 
			[SXADDLCONDFMTS] [SXADDLSXFILTERS12] *SXAddl_SXCView_SXDVerUpdInv *SXAddl_SXCView_SXDSXPIIvmb 
			[SXAddl_SXCView_SXDVerUpdInv] SXAddl_SXCView_SXDEnd
*/
const bool PIVOTADDL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCView_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVer10Info>();
	proc.optional<SXAddl_SXCView_SXDVer12Info>();
	proc.repeated<SXADDLCALCMEMBER>(0, 0);
	proc.repeated<SXADDLHIERARCHY>(0, 0);
	proc.repeated<SXADDLFIELD>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.optional<SXAddl_SXCView_SXDTableStyleClient>();

	if(proc.optional<SXAddl_SXCView_SXDCompactRwHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCView_SXDCompactColHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.optional<SXADDLCONDFMTS>();
	proc.optional<SXADDLSXFILTERS12>();
	proc.repeated<SXAddl_SXCView_SXDVerUpdInv>(0, 0);
	proc.repeated<SXAddl_SXCView_SXDSXPIIvmb>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.mandatory<SXAddl_SXCView_SXDEnd>();
	return true;
}

} // namespace XLS

