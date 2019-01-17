/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "SXAddl.h"

namespace XLS
{

BiffStructurePtr XLUnicodeStringSegmentedSXADDL::clone()
{
	return BiffStructurePtr(new XLUnicodeStringSegmentedSXADDL(*this));
}
void XLUnicodeStringSegmentedSXADDL::load(CFRecord& record)
{
	unsigned short reserved;
	record >> cchTotal >> reserved;

	record >> string;
}
//-----------------------------------------

SXAddl::SXAddl() : bEndElement(false), bStartElement(false)
{
}


SXAddl::~SXAddl()
{
}


BaseObjectPtr SXAddl::clone()
{
	return BaseObjectPtr(new SXAddl(*this));
}

void SXAddl::readFields(CFRecord& record)
{
	record >> frtHeaderOld >> sxc >> sxd;
	switch(sxc)
	{
		case 0x00:	content	= createSxcView		(record);	break;
		case 0x01:	content	= createSxcField	(record);	break;
		case 0x02:	content = createSxcHierarchy(record);	break;
		case 0x03:	content = createSxcCache	(record);	break;
		case 0x04:	content = createSxcCacheField(record);	break;
		case 0x05:	content = createSxcQsi		(record);	break;
		case 0x06:	content = createSxcQuery	(record);	break;
		case 0x07:	content = createSxcGrpLevel	(record);	break;
		case 0x08:	content = createSxcGroup	(record);	break;
		case 0x09:	content = createSxcCacheItem(record);	break;
		case 0x0C:	content = createSxcSXRule	(record);	break;
		case 0x0D:	content = createSxcSXFilt	(record);	break;
		case 0x10:	content = createSxcSXDH		(record);	break;
		case 0x12:	content = createSxcAutoSort	(record);	break;
		case 0x13:	content = createSxcSXMgs	(record);	break;
		case 0x14:	content = createSxcSXMg		(record);	break;
		case 0x17:	content = createSxcField12	(record);	break;
		case 0x1A:	content = createSxcSXCondFmts(record);	break;
		case 0x1B:	content = createSxcSXCondFmt(record);	break;
		case 0x1C:	content = createSxcSXFilters12(record);	break;
		case 0x1D:	content = createSxcSXFilter12(record);	break;
	}
	if (content)
		content->load(record);

	if (sxd == 0x00)	bStartElement	= true;
	if (sxd == 0xff)
	{
		BiffStructurePtr end = BiffStructurePtr(new SXAddl_Universal_End());
		end->load(record);

		bEndElement		= true;
	}

}
BiffStructurePtr SXAddl::createSxcView(CFRecord& record)
{
	BiffStructurePtr result;
	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCView_SXDId());				break;
 		case 0x01: result = BiffStructurePtr(new SXAddl_SXCView_SXDVerUpdInv());		break;
		case 0x02: result = BiffStructurePtr(new SXAddl_SXCView_SXDVer10Info());    	break;
		//case 0x03: result = BiffStructurePtr(new SXAddl_SXCView_SXDCalcMember());    	break;
		//case 0x0A: result = BiffStructurePtr(new SXAddl_SXCView_SXDCalcMemString());  break;
		case 0x19: result = BiffStructurePtr(new SXAddl_SXCView_SXDVer12Info());    	break;
        case 0x1E: result = BiffStructurePtr(new SXAddl_SXCView_SXDTableStyleClient()); break;
        //case 0x21: result = BiffStructurePtr(new SXAddl_SXCView_SXDCompactRwHdr());	break;
        //case 0x22: result = BiffStructurePtr(new SXAddl_SXCView_SXDCompactColHdr());	break;
        //case 0x26: result = BiffStructurePtr(new SXAddl_SXCView_SXDSXPIIvmb());		break;
	}
	return result;
}

BiffStructurePtr SXAddl::createSxcField(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCField_SXDId());			break;
		case 0x02: result = BiffStructurePtr(new SXAddl_SXCField_SXDVer10Info());	break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcHierarchy(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDId());				break;
		case 0x01: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDVerUpdInv());		break;
		case 0x05: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDProperty());		break;
		case 0x09: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDFilterMember());	break;
		case 0x1d: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDSXSetParentUnique());break;
		case 0x1f: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDUserCaption());		break;
		case 0x20: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDId());				break;
		case 0x24: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDMeasureGrp());		break;
		case 0x25: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDDisplayFolder());	break;
		case 0x26: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDParentKPI());		break;
		case 0x27: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIValue());		break;
		case 0x28: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIGoal());			break;
		case 0x29: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIStatus());		break;
		case 0x2a: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPITrend());		break;
		case 0x2b: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIWeight());		break;
		case 0x2c: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPITime());			break;
		case 0x3f: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDFilterMember12());	break;
		case 0x41: result = BiffStructurePtr(new SXAddl_SXCHierarchy_SXDInfo12());			break;
	}

	return result;
}
BiffStructurePtr SXAddl::createSxcCache	(CFRecord& record)
{
	BiffStructurePtr result;
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCCache_SXDId());				break;
		case 0x01: result = BiffStructurePtr(new SXAddl_SXCCache_SXDVerUpdInv());		break;
		case 0x02: result = BiffStructurePtr(new SXAddl_SXCCache_SXDVer10Info());		break;
		case 0x18: result = BiffStructurePtr(new SXAddl_SXCCache_SXDVerSXMacro());		break;
		case 0x34: result = BiffStructurePtr(new SXAddl_SXCCache_SXDInvRefreshReal());	break;
		case 0x41: result = BiffStructurePtr(new SXAddl_SXCCache_SXDInfo12());			break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcCacheField(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDId());				break;
		case 0x05: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDProperty());		break;
		case 0x2d: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDSxrmitmCount());	break;
		case 0x2f: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDCaption());		break;
		case 0x30: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDIfdbMempropMap());	break;
		case 0x31: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDIfdbMpMapCount());	break;
		case 0x40: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDPropName());		break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcField12(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCField12_SXDId());			break;
		case 0x01: result = BiffStructurePtr(new SXAddl_SXCField12_SXDVerUpdInv());		break;
		case 0x11: result = BiffStructurePtr(new SXAddl_SXCField12_SXDMemberCaption());	break;
		case 0x19: result = BiffStructurePtr(new SXAddl_SXCField12_SXDVer12Info());		break;
		case 0x1c: result = BiffStructurePtr(new SXAddl_SXCField12_SXDISXTH());			break;
		case 0x37: result = BiffStructurePtr(new SXAddl_SXCField12_SXDAutoshow());		break;

	}
	return result;
}
BiffStructurePtr SXAddl::createSxcQsi(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}	
BiffStructurePtr SXAddl::createSxcQuery(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcGrpLevel(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcGroup(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcCacheItem(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCCacheItem_SXDId());			break;
		case 0x2e: result = BiffStructurePtr(new SXAddl_SXCCacheItem_SXDSxrmitmDisp());	break;
		case 0x32: result = BiffStructurePtr(new SXAddl_SXCCacheItem_SXDItmMpropMap());	break;
		case 0x33: result = BiffStructurePtr(new SxAddl_SXCCacheItem_SXDItmMpMapCount());break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXRule(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXFilt(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXDH(CFRecord& record)
{
	BiffStructurePtr result;
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCSXDH_SXDId()); break;
		case 0x1A: result = BiffStructurePtr(new SXAddl_SXCSXDH_SXDSxdh()); break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcAutoSort(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXMgs(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCSXMgs_SXDId());			break;
		case 0x23: result = BiffStructurePtr(new SXAddl_SXCSXMgs_SXDMGrpSXDHMap());	break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXMg(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCSXMg_SXDId());			break;
		case 0x1F: result = BiffStructurePtr(new SXAddl_SXCSXMg_SXDUserCaption());	break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXCondFmts(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXCondFmt(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXFilters12(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXFilter12(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField_SXDId(*this));
}

void SXAddl_SXCField_SXDId::load(CFRecord& record)
{
	record >> stName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCView_SXDId(*this));
}

void SXAddl_SXCView_SXDId::load(CFRecord& record)
{
	record >> stName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCQsi_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCQsi_SXDId(*this));
}

void SXAddl_SXCQsi_SXDId::load(CFRecord& record)
{
	record >> stName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDVer10Info::clone()
{
    return BiffStructurePtr(new SXAddl_SXCView_SXDVer10Info(*this));
}

void SXAddl_SXCView_SXDVer10Info::load(CFRecord& record)
{
    unsigned short flags, reserved2;
    unsigned char unused;

    record >> bVerSxMacro >> flags >> unused >> reserved2;

    fDisplayImmediateItems  = GETBIT(flags, 0);
    fEnableDataEd           = GETBIT(flags, 1);
    fDisableFList           = GETBIT(flags, 2);
    fReenterOnLoadOnce      = GETBIT(flags, 3);
    fNotViewCalculatedMembers  = GETBIT(flags, 4);
    fNotVisualTotals        = GETBIT(flags, 5);
    fPageMultipleItemLabel  = GETBIT(flags, 6);
    fTensorFillCv           = GETBIT(flags, 7);
    fHideDDData             = GETBIT(flags, 8);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDVer12Info::clone()
{
    return BiffStructurePtr(new SXAddl_SXCView_SXDVer12Info(*this));
}

void SXAddl_SXCView_SXDVer12Info::load(CFRecord& record)
{
    _UINT32 flags;
    unsigned short reserved3;

    record >> flags >> reserved3;

    fDefaultCompact     = GETBIT(flags, 0);
    fDefaultOutline     = GETBIT(flags, 1);
    fOutlineData        = GETBIT(flags, 2);
    fCompactData        = GETBIT(flags, 3);
    fNewDropZones       = GETBIT(flags, 4);
    fPublished          = GETBIT(flags, 5);
    fTurnOffImmersive   = GETBIT(flags, 6);
    fSingleFilterPerField   = GETBIT(flags, 7);
    fNonDefaultSortInFlist  = GETBIT(flags, 8);

    fDontUseCustomLists = GETBIT(flags, 10);

    fHideDrillIndicators    = GETBIT(flags, 12);
    fPrintDrillIndicators   = GETBIT(flags, 13);
    fMemPropsInTips         = GETBIT(flags, 14);
    fNoPivotTips            = GETBIT(flags, 15);
    cIndentInc              = GETBITS(flags, 16, 22);
    fNoHeaders              = GETBIT(flags, 23);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_Universal_End::clone()
{
	return BiffStructurePtr(new SXAddl_Universal_End(*this));
}

void SXAddl_Universal_End::load(CFRecord& record)
{
	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;
}
//-------------------------------------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheItem_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheItem_SXDId(*this));
}

void SXAddl_SXCCacheItem_SXDId::load(CFRecord& record)
{
	short reserved;
	record >> dwItem >> reserved;
}
//-------------------------------------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDId(*this));
}

void SXAddl_SXCCacheField_SXDId::load(CFRecord& record)
{
	record >> stSourceName;
}
//-------------------------------------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCache_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDId(*this));
}

void SXAddl_SXCCache_SXDId::load(CFRecord& record)
{
	short reserved;
	record >> idCache >> reserved;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField_SXDVer10Info::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField_SXDVer10Info(*this));
}

void SXAddl_SXCField_SXDVer10Info::load(CFRecord& record)
{
	_UINT32	flags;
	short	reserved2;	
	record >> flags >> reserved2;

	fHideDD = GETBIT(flags, 0);
}
//----------------------------------------------------------------------------
BiffStructurePtr Continue_SxaddlSxString::clone()
{
	return BiffStructurePtr(new Continue_SxaddlSxString(*this));
}

void Continue_SxaddlSxString::load(CFRecord& record)
{
	int sz = record.getDataSize() - record.getRdPtr();

	if (sz > 0)
		record >> stContinue;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCache_SXDVer10Info::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDVer10Info(*this));
}

void SXAddl_SXCCache_SXDVer10Info::load(CFRecord& record)
{
	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;

	record >> citmGhostMax >> bVerCacheLastRefresh >> bVerCacheRefreshableMin;

	for (int i = 0; i < 8; i++)
		record >> numDateCopy[i];

	record >> reserved2;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCache_SXDVerUpdInv::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDVerUpdInv(*this));
}

void SXAddl_SXCCache_SXDVerUpdInv::load(CFRecord& record)
{
	record >> dwVersionInvalidates;

	record.skipNunBytes(5);	
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDVerUpdInv::clone()
{
	return BiffStructurePtr(new SXAddl_SXCView_SXDVerUpdInv(*this));
}

void SXAddl_SXCView_SXDVerUpdInv::load(CFRecord& record)
{
	record >> dwVersionInvalidates;

	record.skipNunBytes(5);	
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCache_SXDInfo12::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDInfo12(*this));
}

void SXAddl_SXCCache_SXDInfo12::load(CFRecord& record)
{
	_UINT32	flags;
	short	reserved;	
	
	record >> flags >> reserved;
	
	fSheetData				= GETBIT(flags, 0);
	fSrvSupportAttribDrill	= GETBIT(flags, 1);
	fSrvSupportSubQuery		= GETBIT(flags, 2);
}
//----------------------------------------------------------------------------

BiffStructurePtr SXAddl_SXCCache_SXDVerSXMacro::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDVerSXMacro(*this));
}

void SXAddl_SXCCache_SXDVerSXMacro::load(CFRecord& record)
{
	unsigned char	reserved1;
	short			reserved2, reserved3;	
	
	record >> dwVer >> reserved1 >> reserved2 >> reserved3;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDIfdbMempropMap::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDIfdbMempropMap(*this));
}

void SXAddl_SXCCacheField_SXDIfdbMempropMap::load(CFRecord& record)
{
	record.skipNunBytes(6);
	
	int sz = record.getDataSize() - record.getRdPtr();

	for (int i = 0; i < sz/2; i++)
	{
		unsigned short val;
		record >> val;

		rgMap.push_back(val);
	}
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDTableStyleClient::clone()
{
	return BiffStructurePtr(new SXAddl_SXCView_SXDTableStyleClient(*this));
}

void SXAddl_SXCView_SXDTableStyleClient::load(CFRecord& record)
{
	record.skipNunBytes(6);
	
	unsigned short	flags;
	
	record >> flags >> stName;

	fLastColumn		= GETBIT(flags, 1);
	fRowStrips		= GETBIT(flags, 2);
	fColumnStrips	= GETBIT(flags, 3);
	fRowHeaders		= GETBIT(flags, 4);
	fColumnHeaders	= GETBIT(flags, 5);
	fDefaultStyle	= GETBIT(flags, 6);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDSxrmitmCount::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDSxrmitmCount(*this));
}

void SXAddl_SXCCacheField_SXDSxrmitmCount::load(CFRecord& record)
{
	short	reserved2;	
	record >> citm >> reserved2;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDPropName::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDPropName(*this));
}

void SXAddl_SXCCacheField_SXDPropName::load(CFRecord& record)
{
	record >> stPropName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDProperty::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDProperty(*this));
}

void SXAddl_SXCCacheField_SXDProperty::load(CFRecord& record)
{
	short	reserved2;	
	record >> ihdb >> reserved2;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDIfdbMpMapCount::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDIfdbMpMapCount(*this));
}

void SXAddl_SXCCacheField_SXDIfdbMpMapCount::load(CFRecord& record)
{
	short	reserved2;	
	record >> ifdbMemProp >> reserved2;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheField_SXDCaption::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDCaption(*this));
}

void SXAddl_SXCCacheField_SXDCaption::load(CFRecord& record)
{
	record >> stCaption;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCache_SXDInvRefreshReal::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDInvRefreshReal(*this));
}

void SXAddl_SXCCache_SXDInvRefreshReal::load(CFRecord& record)
{
	_UINT32	flags;
	short	reserved2;	
	record >> flags >> reserved2;
	
	fEnableRefresh	= GETBIT(flags, 0);
	fInvalid		= GETBIT(flags, 1);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDId(*this));
}

void SXAddl_SXCField12_SXDId::load(CFRecord& record)
{
	record >> stName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDISXTH::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDISXTH(*this));
}

void SXAddl_SXCField12_SXDISXTH::load(CFRecord& record)
{
	short reserved2;
	record >> isxth >> reserved2;;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDVer12Info::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDVer12Info(*this));
}

void SXAddl_SXCField12_SXDVer12Info::load(CFRecord& record)
{
	short flags, reserved2, reserved3;
	record >> flags >> reserved2 >> reserved3;

	fHiddenLvl			= GETBIT(flags, 1);
	fUseMemPropCaption	= GETBIT(flags, 2);
	fCompact			= GETBIT(flags, 3);
	fNotAutoSortDft		= GETBIT(flags, 4);
	fFilterInclusive	= GETBIT(flags, 5);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDMemberCaption::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDMemberCaption(*this));
}

void SXAddl_SXCField12_SXDMemberCaption::load(CFRecord& record)
{
	record >> stMemberPropertyCaptionUnique;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDVerUpdInv::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDVerUpdInv(*this));
}

void SXAddl_SXCField12_SXDVerUpdInv::load(CFRecord& record)
{
	record >> dwVersionInvalidates;

	record.skipNunBytes(5);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCField12_SXDAutoshow::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField12_SXDAutoshow(*this));
}

void SXAddl_SXCField12_SXDAutoshow::load(CFRecord& record)
{
	short reserved;
	record >> citmAutoShow >> reserved;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheItem_SXDSxrmitmDisp::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheItem_SXDSxrmitmDisp(*this));
}

void SXAddl_SXCCacheItem_SXDSxrmitmDisp::load(CFRecord& record)
{
	record >> stDisplay;
}
//----------------------------------------------------------------------------
BiffStructurePtr SxAddl_SXCCacheItem_SXDItmMpMapCount::clone()
{
	return BiffStructurePtr(new SxAddl_SXCCacheItem_SXDItmMpMapCount(*this));
}

void SxAddl_SXCCacheItem_SXDItmMpMapCount::load(CFRecord& record)
{
	short reserved;
	record >> cMemProps >> reserved;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCCacheItem_SXDItmMpropMap::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheItem_SXDItmMpropMap(*this));
}

void SXAddl_SXCCacheItem_SXDItmMpropMap::load(CFRecord& record)
{
	int sz = (record.getDataSize() - record.getRdPtr()) / 4;
	for (int i = 0; i < sz; i++)
	{
		_INT32 val;
		record >> val;
		rgMemProps.push_back(val);
	}
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXDH_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXDH_SXDId(*this));
}

void SXAddl_SXCSXDH_SXDId::load(CFRecord& record)
{
	short reserved;
	record >> dwCount >> reserved;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXDH_SXDSxdh::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXDH_SXDSxdh(*this));
}

void SXAddl_SXCSXDH_SXDSxdh::load(CFRecord& record)
{
	_UINT32 reserved1;
	short reserved2;
	record >> reserved1 >> reserved2 >> isxth >> cchDimensionName >> cchDimensionUnique >> cchDimensionCaption;

	if (cchDimensionName > 0)
	{
		stDimensionName.setSize(cchDimensionName);
		record >> stDimensionName;
	}
	if (cchDimensionUnique > 0)
	{
		stDimensionUnique.setSize(cchDimensionUnique);
		record >> stDimensionUnique;
	}
	if (cchDimensionCaption > 0)
	{
		stDimensionCaption.setSize(cchDimensionCaption);
		record >> stDimensionCaption;
	}
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXMg_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXMg_SXDId(*this));
}

void SXAddl_SXCSXMg_SXDId::load(CFRecord& record)
{
	record >> stName;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXMg_SXDUserCaption::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXMg_SXDUserCaption(*this));
}

void SXAddl_SXCSXMg_SXDUserCaption::load(CFRecord& record)
{
	record >> stUserCaption;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXMgs_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXMgs_SXDId(*this));
}

void SXAddl_SXCSXMgs_SXDId::load(CFRecord& record)
{
	short reserved;
	record >> cmgs >> reserved >> cmaps;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCSXMgs_SXDMGrpSXDHMap::clone()
{
	return BiffStructurePtr(new SXAddl_SXCSXMgs_SXDMGrpSXDHMap(*this));
}

void SXAddl_SXCSXMgs_SXDMGrpSXDHMap::load(CFRecord& record)
{
	_UINT32 reserved1;
	short reserved2;
	record >> reserved1 >> reserved2 >> iKey >> iVal;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDId::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDId(*this));
}

void SXAddl_SXCHierarchy_SXDId::load(CFRecord& record)
{
	record >> stHierUnq;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDVerUpdInv::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDVerUpdInv(*this));
}

void SXAddl_SXCHierarchy_SXDVerUpdInv::load(CFRecord& record)
{
	record >> dwVersionInvalidates;
	record.skipNunBytes(5);
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDProperty::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDProperty(*this));
}

void SXAddl_SXCHierarchy_SXDProperty::load(CFRecord& record)
{
	_UINT32 flags;
	short reserved2;

	record >> flags >> reserved2 >> cchProperty;

	fDisplayInReport	= GETBIT(flags, 0);
	fDisplayInTip		= GETBIT(flags, 1);
	fDisplayInCaption	= GETBIT(flags, 2);

	if (cchProperty > 0)
	{
		stProperty.setSize(cchProperty);
		record >> stProperty;
	}
	record >> cchLevelUnq >> ichPropName >> cchPropName >> isxtl;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDFilterMember::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDFilterMember(*this));
}

void SXAddl_SXCHierarchy_SXDFilterMember::load(CFRecord& record)
{
	_UINT32 flags;
	short reserved2;

	record >> flags >> reserved2 >> cItems;
	fMultFiltHavePlex = GETBIT(flags, 1);

	for (unsigned short i = 0; i < cItems; i++)
	{
		XLUnicodeString val;
		record >> val;
		rgStMembers.push_back(val.value());
	}
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDFilterMember12::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDFilterMember12(*this));
}

void SXAddl_SXCHierarchy_SXDFilterMember12::load(CFRecord& record)
{
	short reserved2;

	record >> isxtl >> reserved2 >> cItems;

	for (unsigned short i = 0; i < cItems; i++)
	{
		XLUnicodeString val;
		record >> val;
		rgStMembers.push_back(val.value());
	}
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDSXSetParentUnique::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDSXSetParentUnique(*this));
}

void SXAddl_SXCHierarchy_SXDSXSetParentUnique::load(CFRecord& record)
{
	record >> stHierUnique;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDUserCaption::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDUserCaption(*this));
}

void SXAddl_SXCHierarchy_SXDUserCaption::load(CFRecord& record)
{
	record >> stCaption;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDMeasureGrp::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDMeasureGrp(*this));
}

void SXAddl_SXCHierarchy_SXDMeasureGrp::load(CFRecord& record)
{
	record >> stMeasureGroup;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDDisplayFolder::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDDisplayFolder(*this));
}

void SXAddl_SXCHierarchy_SXDDisplayFolder::load(CFRecord& record)
{
	record >> stDisplayFolder;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDParentKPI::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDParentKPI(*this));
}

void SXAddl_SXCHierarchy_SXDParentKPI::load(CFRecord& record)
{
	record >> stParentKPI;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPIValue::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIValue(*this));
}

void SXAddl_SXCHierarchy_SXDKPIValue::load(CFRecord& record)
{
	record >> stKPIValue;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPIGoal::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIGoal(*this));
}

void SXAddl_SXCHierarchy_SXDKPIGoal::load(CFRecord& record)
{
	record >> stKPIGoal;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPIStatus::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIStatus(*this));
}

void SXAddl_SXCHierarchy_SXDKPIStatus::load(CFRecord& record)
{
	record >> stKPIStatus;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPITime::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPITime(*this));
}

void SXAddl_SXCHierarchy_SXDKPITime::load(CFRecord& record)
{
	record >> stKPITime;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPIWeight::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPIWeight(*this));
}

void SXAddl_SXCHierarchy_SXDKPIWeight::load(CFRecord& record)
{
	record >> stKPIWeight;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDKPITrend::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDKPITrend(*this));
}

void SXAddl_SXCHierarchy_SXDKPITrend::load(CFRecord& record)
{
	record >> stKPITrend;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCHierarchy_SXDInfo12::clone()
{
	return BiffStructurePtr(new SXAddl_SXCHierarchy_SXDInfo12(*this));
}

void SXAddl_SXCHierarchy_SXDInfo12::load(CFRecord& record)
{
	_UINT32 flags;
	short reserved2;

	record >> flags >> reserved2;
	
	fUnbalancedRealKnown	= GETBIT(flags, 0);
	fUnbalancedReal			= GETBIT(flags, 1);
	fUnbalancedGroupKnown	= GETBIT(flags, 2);
	fUnbalancedGroup		= GETBIT(flags, 3);
	fHidden					= GETBIT(flags, 4);

}

} // namespace XLS

