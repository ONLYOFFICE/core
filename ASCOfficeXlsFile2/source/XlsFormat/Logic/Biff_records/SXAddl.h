#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SXAddl record in BIFF8
class SXAddl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXAddl)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXAddl)
public:
	SXAddl();
	~SXAddl();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXAddl;


//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

// The following classes are not implemented and most probably won't be.
// So we save some compilation and link time and resulting dll size
class Continue_SxaddlSxString : public SXAddl {};
class SXAddl_SXCAutoSort_SXDEnd : public SXAddl {};
class SXAddl_SXCAutoSort_SXDId : public SXAddl {};
class SXAddl_SXCCache_SXDEnd : public SXAddl {};
class SXAddl_SXCCache_SXDId : public SXAddl {};
class SXAddl_SXCCache_SXDInfo12 : public SXAddl {};
class SXAddl_SXCCache_SXDInvRefreshReal : public SXAddl {};
class SXAddl_SXCCache_SXDVer10Info : public SXAddl {};
class SXAddl_SXCCache_SXDVerSXMacro : public SXAddl {};
class SXAddl_SXCCache_SXDVerUpdInv : public SXAddl {};
class SXAddl_SXCCacheField_SXDCaption : public SXAddl {};
class SXAddl_SXCCacheField_SXDEnd : public SXAddl {};
class SXAddl_SXCCacheField_SXDId : public SXAddl {};
class SXAddl_SXCCacheField_SXDIfdbMempropMap : public SXAddl {};
class SXAddl_SXCCacheField_SXDIfdbMpMapCount : public SXAddl {};
class SXAddl_SXCCacheField_SXDProperty : public SXAddl {};
class SXAddl_SXCCacheField_SXDPropName : public SXAddl {};
class SXAddl_SXCCacheField_SXDSxrmitmCount : public SXAddl {};
class SXAddl_SXCCacheItem_SXDEnd : public SXAddl {};
class SXAddl_SXCCacheItem_SXDId : public SXAddl {};
class SXAddl_SXCCacheItem_SXDItmMpMapCount : public SXAddl {};
class SXAddl_SXCCacheItem_SXDItmMpropMap : public SXAddl {};
class SXAddl_SXCCacheItem_SXDSxrmitmDisp : public SXAddl {};
class SXAddl_SXCField12_SXDAutoshow : public SXAddl {};
class SXAddl_SXCField12_SXDEnd : public SXAddl {};
class SXAddl_SXCField12_SXDId : public SXAddl {};
class SXAddl_SXCField12_SXDISXTH : public SXAddl {};
class SXAddl_SXCField12_SXDMemberCaption : public SXAddl {};
class SXAddl_SXCField12_SXDVer12Info : public SXAddl {};
class SXAddl_SXCField12_SXDVerUpdInv : public SXAddl {};
class SXAddl_SXCField_SXDEnd : public SXAddl {};
class SXAddl_SXCField_SXDId : public SXAddl {};
class SXAddl_SXCField_SXDVer10Info : public SXAddl {};
class SXAddl_SXCGroup_SXDEnd : public SXAddl {};
class SXAddl_SXCGroup_SXDGrpInfo : public SXAddl {};
class SXAddl_SXCGroup_SXDId : public SXAddl {};
class SXAddl_SXCGroup_SXDMember : public SXAddl {};
class SXAddl_SXCGrpLevel_SXDEnd : public SXAddl {};
class SXAddl_SXCGrpLevel_SXDGrpLevelInfo : public SXAddl {};
class SXAddl_SXCGrpLevel_SXDId : public SXAddl {};
class SXAddl_SXCHierarchy_SXDDisplayFolder : public SXAddl {};
class SXAddl_SXCHierarchy_SXDEnd : public SXAddl {};
class SXAddl_SXCHierarchy_SXDFilterMember : public SXAddl {};
class SXAddl_SXCHierarchy_SXDFilterMember12 : public SXAddl {};
class SXAddl_SXCHierarchy_SXDIconSet : public SXAddl {};
class SXAddl_SXCHierarchy_SXDId : public SXAddl {};
class SXAddl_SXCHierarchy_SXDInfo12 : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPIGoal : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPIStatus : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPITime : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPITrend : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPIValue : public SXAddl {};
class SXAddl_SXCHierarchy_SXDKPIWeight : public SXAddl {};
class SXAddl_SXCHierarchy_SXDMeasureGrp : public SXAddl {};
class SXAddl_SXCHierarchy_SXDParentKPI : public SXAddl {};
class SXAddl_SXCHierarchy_SXDProperty : public SXAddl {};
class SXAddl_SXCHierarchy_SXDSXSetParentUnique : public SXAddl {};
class SXAddl_SXCHierarchy_SXDUserCaption : public SXAddl {};
class SXAddl_SXCHierarchy_SXDVerUpdInv : public SXAddl {};
class SXAddl_SXCQsi_SXDEnd : public SXAddl {};
class SXAddl_SXCQsi_SXDId : public SXAddl {};
class SXAddl_SXCQuery_SXDEnd : public SXAddl {};
class SXAddl_SXCQuery_SXDReconnCond : public SXAddl {};
class SXAddl_SXCQuery_SXDSrcConnFile : public SXAddl {};
class SXAddl_SXCQuery_SXDSrcDataFile : public SXAddl {};
class SXAddl_SXCQuery_SXDXMLSource : public SXAddl {};
class SXAddl_SXCSXCondFmt_SXDEnd : public SXAddl {};
class SXAddl_SXCSXCondFmt_SXDSXCondFmt : public SXAddl {};
class SXAddl_SXCSXCondFmts_SXDEnd : public SXAddl {};
class SXAddl_SXCSXCondFmts_SXDId : public SXAddl {};
class SXAddl_SXCSXDH_SXDEnd : public SXAddl {};
class SXAddl_SXCSXDH_SXDId : public SXAddl {};
class SXAddl_SXCSXDH_SXDSxdh : public SXAddl {};
class SXAddl_SXCSXfilt_SXDEnd : public SXAddl {};
class SXAddl_SXCSXfilt_SXDId : public SXAddl {};
class SXAddl_SXCSXfilt_SXDSXfilt : public SXAddl {};
class SXAddl_SXCSXfilt_SXDSXItm : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDCaption : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDEnd : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDId : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDSXFilter : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDSXFilterDesc : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDSXFilterValue1 : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDSXFilterValue2 : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDXlsFilter : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDXlsFilterValue1 : public SXAddl {};
class SXAddl_SXCSXFilter12_SXDXlsFilterValue2 : public SXAddl {};
class SXAddl_SXCSXFilters12_SXDEnd : public SXAddl {};
class SXAddl_SXCSXFilters12_SXDId : public SXAddl {};
class SXAddl_SXCSXMg_SXDEnd : public SXAddl {};
class SXAddl_SXCSXMg_SXDId : public SXAddl {};
class SXAddl_SXCSXMg_SXDUserCaption : public SXAddl {};
class SXAddl_SXCSXMgs_SXDEnd : public SXAddl {};
class SXAddl_SXCSXMgs_SXDId : public SXAddl {};
class SXAddl_SXCSXMgs_SXDMGrpSXDHMap : public SXAddl {};
class SXAddl_SXCSXrule_SXDEnd : public SXAddl {};
class SXAddl_SXCSXrule_SXDId : public SXAddl {};
class SXAddl_SXCSXrule_SXDSXrule : public SXAddl {};
class SXAddl_SXCView_SXDCalcMember : public SXAddl {};
class SXAddl_SXCView_SXDCalcMemString : public SXAddl {};
class SXAddl_SXCView_SXDCompactColHdr : public SXAddl {};
class SXAddl_SXCView_SXDCompactRwHdr : public SXAddl {};
class SXAddl_SXCView_SXDEnd : public SXAddl {};
class SXAddl_SXCView_SXDId : public SXAddl {};
class SXAddl_SXCView_SXDSXPIIvmb : public SXAddl {};
class SXAddl_SXCView_SXDTableStyleClient : public SXAddl {};
class SXAddl_SXCView_SXDVer10Info : public SXAddl {};
class SXAddl_SXCView_SXDVer12Info : public SXAddl {};
class SXAddl_SXCView_SXDVerUpdInv : public SXAddl {};

} // namespace XLS

