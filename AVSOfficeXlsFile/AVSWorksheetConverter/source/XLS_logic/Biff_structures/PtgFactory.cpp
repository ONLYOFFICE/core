#include "stdafx.h"
#include "PtgFactory.h"
#include <XLS_bin/CFRecord.h>
#include "PtgExp.h"
#include "PtgTbl.h"
#include "PtgRef3d.h"
#include "OperatorPtgs.h"
#include <XLS_logic/Biff_structures/PtgParen.h>
#include <XLS_logic/Biff_structures/PtgMissArg.h>
#include "PtgStr.h"
#include "PtgErr.h"
#include "PtgBool.h"
#include "PtgInt.h"
#include "PtgNum.h"
#include "PtgArray.h"
#include "PtgFunc.h"
#include "PtgFuncVar.h"
#include "PtgName.h"
#include "PtgRef.h"
#include "PtgArea.h"
#include "PtgMemArea.h"
#include "PtgMemErr.h"
#include "PtgMemNoMem.h"
#include "PtgMemFunc.h"
#include "PtgRefErr.h"
#include "PtgAreaErr.h"
#include "PtgRefN.h"
#include "PtgAreaN.h"
#include "PtgNameX.h"
#include "PtgRef3d.h"
#include "PtgArea3d.h"
#include "PtgRefErr3d.h"
#include "PtgAreaErr3d.h"
#include "PtgElfCol.h"
#include "PtgElfRwV.h"
#include "PtgElfColV.h"
#include "PtgElfRadical.h"
#include "PtgElfRadicalS.h"
#include "PtgElfColS.h"
#include "PtgElfColSV.h"
#include "PtgElfRadicalLel.h"
#include "PtgSxName.h"
#include "PtgAttrSemi.h"
#include "PtgAttrIf.h"
#include "PtgAttrChoose.h"
#include "PtgAttrGoto.h"
#include "PtgAttrSum.h"
#include "PtgAttrBaxcel.h"
#include "PtgAttrSpace.h"
#include "PtgAttrSpaceSemi.h"
#include "PtgElfLel.h"
#include "PtgElfRw.h"
#include "PtgMemAreaN.h"
#include "PtgMemNoMemN.h"

namespace XLS
{;

// Loads a Ptg record // static
PtgPtr PtgFactory::loadPtg(CFRecord& record, const CellRef& cell_base_ref)
{
	BYTE rec_type = *record.getCurData<BYTE>();
	WORD full_type = rec_type;
	if(0x18 == rec_type || 0x19 == rec_type)
	{
		full_type = *record.getCurData<WORD>();
	}

	PtgPtr ptg = createPtg(full_type, cell_base_ref);
	if(!ptg)
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported type of Ptg.", record.getTypeString());
	}
	ptg->load(record);
	return ptg;
}


// static
PtgPtr PtgFactory::createPtg(const WORD type, const CellRef& cell_base_ref)
{
	switch(type)
	{
		case 0x0001:
			return PtgPtr(new PtgExp); // *

		case 0x0002:
			return PtgPtr(new PtgTbl); // -

		case 0x0003:
			return PtgPtr(new PtgAdd); // *

		case 0x0004:
			return PtgPtr(new PtgSub); // *

		case 0x0005:
			return PtgPtr(new PtgMul); // *

		case 0x0006:
			return PtgPtr(new PtgDiv); // *

		case 0x0007:
			return PtgPtr(new PtgPower); // *

		case 0x0008:
			return PtgPtr(new PtgConcat); // *

		case 0x0009:
			return PtgPtr(new PtgLt); // *

		case 0x000A:
			return PtgPtr(new PtgLe); // *

		case 0x000B:
			return PtgPtr(new PtgEq); // *

		case 0x000C:
			return PtgPtr(new PtgGe); // *

		case 0x000D:
			return PtgPtr(new PtgGt); // *

		case 0x000E:
			return PtgPtr(new PtgNe); // *

		case 0x000F:
			return PtgPtr(new PtgIsect); // *

		case 0x0010:
			return PtgPtr(new PtgUnion); // *

		case 0x0011:
			return PtgPtr(new PtgRange); // *

		case 0x0012:
			return PtgPtr(new PtgUplus); // *

		case 0x0013:
			return PtgPtr(new PtgUminus); // *

		case 0x0014:
			return PtgPtr(new PtgPercent); // *

		case PtgParen::fixed_id:
			return PtgPtr(new PtgParen); // *

		case PtgMissArg::fixed_id:
			return PtgPtr(new PtgMissArg); // *

		case PtgStr::fixed_id:
			return PtgPtr(new PtgStr); // *

		case PtgErr::fixed_id:
			return PtgPtr(new PtgErr); // *

		case PtgBool::fixed_id:
			return PtgPtr(new PtgBool); // *

		case PtgInt::fixed_id:
			return PtgPtr(new PtgInt); // *

		case PtgNum::fixed_id:
			return PtgPtr(new PtgNum); // *

		case 0x0020: // type = REFERENCE
		case 0x0040: // type = VALUE
		case 0x0060: // type = ARRAY
			return PtgPtr(new PtgArray); // *

		case 0x0021:
		case 0x0041:
		case 0x0061:
			return PtgPtr(new PtgFunc); // *

		case 0x0022:
		case 0x0042:
		case 0x0062:
			return PtgPtr(new PtgFuncVar); // *

		case 0x0023:
		case 0x0043:
		case 0x0063:
			return PtgPtr(new PtgName); // *

		case 0x0024:
		case 0x0044:
		case 0x0064:
			return PtgPtr(new PtgRef); // *

		case 0x0025:
		case 0x0045:
		case 0x0065:
			return PtgPtr(new PtgArea); // *

		case 0x0026:
		case 0x0046:
		case 0x0066:
			return PtgPtr(new PtgMemArea); // *

		case 0x0027: // type = REFERENCE
		case 0x0047: // type = VALUE
		case 0x0067: // type = ARRAY
			return PtgPtr(new PtgMemErr); // *

		case 0x0028:
		case 0x0048:
		case 0x0068:
			return PtgPtr(new PtgMemNoMem); // *

		case 0x0029:
		case 0x0049:
		case 0x0069:
			return PtgPtr(new PtgMemFunc); // *

		case 0x002A:
		case 0x004A:
		case 0x006A:
			return PtgPtr(new PtgRefErr); // *

		case 0x002B:
		case 0x004B:
		case 0x006B:
			return PtgPtr(new PtgAreaErr); // *

		case 0x002C:
		case 0x004C:
		case 0x006C:
			return PtgPtr(new PtgRefN(cell_base_ref)); // *

		case 0x002D:
		case 0x004D:
		case 0x006D:
			return PtgPtr(new PtgAreaN(cell_base_ref)); // *

/*
		case 0x002E:
		case 0x004E:
		case 0x006E:
			return PtgPtr(new PtgMemAreaN); // *

		case 0x002F:
		case 0x004F:
		case 0x006F:
			return PtgPtr(new PtgMemNoMemN); // *
*/

		case 0x0039:
		case 0x0059:
		case 0x0079:
			return PtgPtr(new PtgNameX); // -   // Points to Extern Workbook

		case 0x003A: // type = REFERENCE
		case 0x005A: // type = VALUE
		case 0x007A: // type = ARRAY
			return PtgPtr(new PtgRef3d(cell_base_ref)); // *

		case 0x003B:
		case 0x005B:
		case 0x007B:
			return PtgPtr(new PtgArea3d(cell_base_ref)); // *

		case 0x003C:
		case 0x005C:
		case 0x007C:
			return PtgPtr(new PtgRefErr3d); // *

		case 0x003D:
		case 0x005D:
		case 0x007D:
			return PtgPtr(new PtgAreaErr3d); // *



		case 0x0118:
			return PtgPtr(new PtgElfLel); // *

		case 0x0218:
			return PtgPtr(new PtgElfRw); // *

		case 0x0318:
			return PtgPtr(new PtgElfCol); // *

/*
		case 0x0418:
			return PtgPtr(new PtgStubElfRef); // *

*/
		case 0x0618:
			return PtgPtr(new PtgElfRwV); // *

		case 0x0718:
			return PtgPtr(new PtgElfColV); // *

		case 0x0A18:
			return PtgPtr(new PtgElfRadical); // *

		case 0x0B18:
			return PtgPtr(new PtgElfRadicalS); // *

		case 0x0D18:
			return PtgPtr(new PtgElfColS); // *

		case 0x0F18:
			return PtgPtr(new PtgElfColSV); // *

		case 0x1018:
			return PtgPtr(new PtgElfRadicalLel); // *

		case 0x1D18:
			return PtgPtr(new PtgSxName); // - index of an SxName following an SxFmla



		case 0x0119:
			return PtgPtr(new PtgAttrSemi); // *

		case 0x0219:
			return PtgPtr(new PtgAttrIf); // *

		case 0x0419:
			return PtgPtr(new PtgAttrChoose); // *

		//case 0x0019: // This value is possibly "PtgAttrGoto" if bitGoto is not set
		case 0x0819:
			return PtgPtr(new PtgAttrGoto); // *

		case PtgAttrSum::fixed_id:
			return PtgPtr(new PtgAttrSum); // *

		case 0x2019:
		case 0x2119:
			return PtgPtr(new PtgAttrBaxcel); // *

		case 0x4019:
			return PtgPtr(new PtgAttrSpace); // *

		case 0x4119:
			return PtgPtr(new PtgAttrSpaceSemi); // *

		default:
			return PtgPtr();
	}
}

} // namespace XLS

