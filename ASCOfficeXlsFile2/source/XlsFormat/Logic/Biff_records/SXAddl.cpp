/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

SXAddl::SXAddl()
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

}
BiffStructurePtr SXAddl::createSxcView(CFRecord& record)
{
	BiffStructurePtr result;
	
	switch(sxd)
	{
		case 0x00: result = BiffStructurePtr(new SXAddl_SXCView_SXDId());				break;
		case 0x1E: result = BiffStructurePtr(new SXAddl_SXCView_SXDTableStyleClient()); break;
		case 0xff: result = BiffStructurePtr(new SXAddl_SXCView_SXDEnd());				break;
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
		case 0xff: result = BiffStructurePtr(new SXAddl_SXCField_SXDEnd());			break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcHierarchy(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}

	if (result)
		result->load(record);

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
		case 0xff: result = BiffStructurePtr(new SXAddl_SXCCache_SXDEnd());				break;
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
		case 0xff: result = BiffStructurePtr(new SXAddl_SXCCacheField_SXDEnd());			break;
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
		case 0xff: break;
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
		case 0xff: break;
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
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcSXMg(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
	}
	return result;
}
BiffStructurePtr SXAddl::createSxcField12(CFRecord& record)
{
	BiffStructurePtr result;	
	switch(sxd)
	{
		case 0xff: break;
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
BiffStructurePtr SXAddl_SXCField_SXDEnd::clone()
{
	return BiffStructurePtr(new SXAddl_SXCField_SXDEnd(*this));
}

void SXAddl_SXCField_SXDEnd::load(CFRecord& record)
{
	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;
}
//----------------------------------------------------------------------------
BiffStructurePtr SXAddl_SXCView_SXDEnd::clone()
{
	return BiffStructurePtr(new SXAddl_SXCView_SXDEnd(*this));
}

void SXAddl_SXCView_SXDEnd::load(CFRecord& record)
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
BiffStructurePtr SXAddl_SXCCache_SXDEnd::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCache_SXDEnd(*this));
}

void SXAddl_SXCCache_SXDEnd::load(CFRecord& record)
{
	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;
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
BiffStructurePtr SXAddl_SXCCacheField_SXDEnd::clone()
{
	return BiffStructurePtr(new SXAddl_SXCCacheField_SXDEnd(*this));
}

void SXAddl_SXCCacheField_SXDEnd::load(CFRecord& record)
{
	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;
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
} // namespace XLS

