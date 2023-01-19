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
#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/Xnum.h"

namespace XLS
{

class XLUnicodeStringSegmentedSXADDL : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XLUnicodeStringSegmentedSXADDL)
public:

	XLUnicodeStringSegmentedSXADDL(){}
	~XLUnicodeStringSegmentedSXADDL(){}

	BiffStructurePtr clone();
	
	virtual void load(CFRecord& record);

	static const ElementType type = typeStringSegmentedSXADDL;

	_UINT32				cchTotal;
	XLUnicodeString		string;
};

class SXAddl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXAddl)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXAddl)
public:
	SXAddl();
	~SXAddl();
	
	virtual BaseObjectPtr clone();
	virtual void readFields(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	bool				bStartElement;
	bool				bEndElement;

	_UINT32				frtHeaderOld;
	unsigned char		sxc;
	unsigned char		sxd;
	BiffStructurePtr	content;
private:
	BiffStructurePtr createSxcView		(CFRecord& record);
	BiffStructurePtr createSxcField		(CFRecord& record);
	BiffStructurePtr createSxcHierarchy	(CFRecord& record);
	BiffStructurePtr createSxcCache		(CFRecord& record);
	BiffStructurePtr createSxcCacheField(CFRecord& record);
	BiffStructurePtr createSxcQsi		(CFRecord& record);	
	BiffStructurePtr createSxcQuery		(CFRecord& record);
	BiffStructurePtr createSxcGrpLevel	(CFRecord& record);
	BiffStructurePtr createSxcGroup		(CFRecord& record);
	BiffStructurePtr createSxcCacheItem	(CFRecord& record);
	BiffStructurePtr createSxcSXRule	(CFRecord& record);
	BiffStructurePtr createSxcSXFilt	(CFRecord& record);
	BiffStructurePtr createSxcSXDH		(CFRecord& record);
	BiffStructurePtr createSxcAutoSort	(CFRecord& record);
	BiffStructurePtr createSxcSXMgs		(CFRecord& record);
	BiffStructurePtr createSxcSXMg		(CFRecord& record);
	BiffStructurePtr createSxcField12	(CFRecord& record);
	BiffStructurePtr createSxcSXCondFmts(CFRecord& record);
	BiffStructurePtr createSxcSXCondFmt	(CFRecord& record);
	BiffStructurePtr createSxcSXFilters12(CFRecord& record);
	BiffStructurePtr createSxcSXFilter12(CFRecord& record);
};

class Continue_SxaddlSxString : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Continue_SxaddlSxString)
public:
	BiffStructurePtr clone();

	Continue_SxaddlSxString(){}
	~Continue_SxaddlSxString(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stContinue;	
};
class SXAddl_SXCField_SXDId	: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField_SXDId(){}
	~SXAddl_SXCField_SXDId(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stName;	
};
class SXAddl_Universal_End : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_Universal_End)
public:
	BiffStructurePtr clone();

	SXAddl_Universal_End(){}
	~SXAddl_Universal_End(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);
};
class SXAddl_SXCField_SXDVer10Info	: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField_SXDVer10Info)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField_SXDVer10Info(){}
	~SXAddl_SXCField_SXDVer10Info(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);

	bool fHideDD;	
};

class SXAddl_SXCView_SXDId	: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCView_SXDId(){}
	~SXAddl_SXCView_SXDId(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stName;
};
class SXAddl_SXCView_SXDVerUpdInv	: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDVerUpdInv)
public:
	BiffStructurePtr clone();

	SXAddl_SXCView_SXDVerUpdInv(){}
	~SXAddl_SXCView_SXDVerUpdInv(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);

	unsigned char dwVersionInvalidates;
};
class SXAddl_SXCView_SXDTableStyleClient : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDTableStyleClient)
public:
	BiffStructurePtr clone();

	SXAddl_SXCView_SXDTableStyleClient(){}
	~SXAddl_SXCView_SXDTableStyleClient(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	bool fLastColumn;
	bool fRowStrips;
	bool fColumnStrips;
	bool fRowHeaders;
	bool fColumnHeaders;
	bool fDefaultStyle;

	LPWideString stName;
};

class SXAddl_SXCAutoSort_SXDId  : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCAutoSort_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCAutoSort_SXDId(){}
	~SXAddl_SXCAutoSort_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	bool fAscendSort;
};

class SXAddl_SXCCache_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDId(){}
	~SXAddl_SXCCache_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
	
	_UINT32 idCache;
};

class SXAddl_SXCCache_SXDInfo12  : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDInfo12)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDInfo12(){}
	~SXAddl_SXCCache_SXDInfo12(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	bool fSheetData;
	bool fSrvSupportAttribDrill;
	bool fSrvSupportSubQuery;
};

class SXAddl_SXCCache_SXDInvRefreshReal : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDInvRefreshReal)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDInvRefreshReal(){}
	~SXAddl_SXCCache_SXDInvRefreshReal(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	bool fEnableRefresh;
	bool fInvalid;
};

class SXAddl_SXCCache_SXDVer10Info : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDVer10Info)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDVer10Info(){}
	~SXAddl_SXCCache_SXDVer10Info(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	_INT32 citmGhostMax;
	unsigned char bVerCacheLastRefresh;
	unsigned char bVerCacheRefreshableMin;

	unsigned char numDateCopy[8];
};

class SXAddl_SXCCache_SXDVerSXMacro : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDVerSXMacro)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDVerSXMacro(){}
	~SXAddl_SXCCache_SXDVerSXMacro(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	unsigned char dwVer;
};

class SXAddl_SXCCache_SXDVerUpdInv : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDVerUpdInv)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDVerUpdInv(){}
	~SXAddl_SXCCache_SXDVerUpdInv(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	unsigned char dwVersionInvalidates;
};

class SXAddl_SXCCacheField_SXDCaption : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDCaption)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDCaption(){}
	~SXAddl_SXCCacheField_SXDCaption(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stCaption;
};

class SXAddl_SXCCacheField_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDId(){}
	~SXAddl_SXCCacheField_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
	
	XLUnicodeStringSegmentedSXADDL stSourceName;
};

class SXAddl_SXCCacheField_SXDIfdbMempropMap : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDIfdbMempropMap)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDIfdbMempropMap(){}
	~SXAddl_SXCCacheField_SXDIfdbMempropMap(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	std::vector<unsigned short>	rgMap;
};
class SXAddl_SXCCacheField_SXDIfdbMpMapCount : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDIfdbMpMapCount)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDIfdbMpMapCount(){}
	~SXAddl_SXCCacheField_SXDIfdbMpMapCount(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	_UINT32 ifdbMemProp;
};
class SXAddl_SXCCacheField_SXDProperty : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDProperty)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDProperty(){}
	~SXAddl_SXCCacheField_SXDProperty(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	_UINT32 ihdb;
};
class SXAddl_SXCCacheField_SXDPropName : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDPropName)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDPropName(){}
	~SXAddl_SXCCacheField_SXDPropName(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stPropName;
};
class SXAddl_SXCCacheField_SXDSxrmitmCount : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDSxrmitmCount)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDSxrmitmCount(){}
	~SXAddl_SXCCacheField_SXDSxrmitmCount(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

	_UINT32 citm;
};

class SXAddl_SXCCacheItem_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheItem_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheItem_SXDId(){}
	~SXAddl_SXCCacheItem_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
	
	_UINT32 dwItem;
};
class SXAddl_SXCView_SXDVer10Info: public BiffStructure
{
    BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDVer10Info)
public:
    BiffStructurePtr clone();

    SXAddl_SXCView_SXDVer10Info(){}
    ~SXAddl_SXCView_SXDVer10Info(){}

    virtual void load(CFRecord& record);

    static const ElementType	type = typeSXAddl;

    unsigned char   bVerSxMacro;
    bool            fDisplayImmediateItems;
    bool            fEnableDataEd;
    bool            fDisableFList;
    bool            fReenterOnLoadOnce;
    bool            fNotViewCalculatedMembers;
    bool            fNotVisualTotals;
    bool            fPageMultipleItemLabel;
    bool            fTensorFillCv;
    bool            fHideDDData;
};
class SXAddl_SXCView_SXDVer12Info: public BiffStructure
{
    BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDVer12Info)
public:
    BiffStructurePtr clone();

    SXAddl_SXCView_SXDVer12Info(){}
    ~SXAddl_SXCView_SXDVer12Info(){}

    virtual void load(CFRecord& record);

    static const ElementType	type = typeSXAddl;

    bool    fDefaultCompact;
    bool    fDefaultOutline;
    bool    fOutlineData;
    bool    fCompactData;
    bool    fNewDropZones;
    bool    fPublished;
    bool    fTurnOffImmersive;
    bool    fSingleFilterPerField;
    bool    fNonDefaultSortInFlist;
    bool    fDontUseCustomLists;
    bool    fHideDrillIndicators;
    bool    fPrintDrillIndicators;
    bool    fMemPropsInTips;
    bool    fNoPivotTips;
    unsigned char    cIndentInc;
    bool    fNoHeaders;
};

class SXAddl_SXCQsi_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCQsi_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCQsi_SXDId(){}
	~SXAddl_SXCQsi_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stName;	
};
class SXAddl_SXCField12_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDId(){}
	~SXAddl_SXCField12_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stName;	
};
class SXAddl_SXCField12_SXDISXTH : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDISXTH)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDISXTH(){}
	~SXAddl_SXCField12_SXDISXTH(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 isxth;
};
class SXAddl_SXCField12_SXDVer12Info : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDVer12Info)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDVer12Info(){}
	~SXAddl_SXCField12_SXDVer12Info(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	bool fHiddenLvl;
	bool fUseMemPropCaption;
	bool fCompact;
	bool fNotAutoSortDft;
	bool fFilterInclusive;
};
class SXAddl_SXCField12_SXDVerUpdInv : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDVerUpdInv)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDVerUpdInv(){}
	~SXAddl_SXCField12_SXDVerUpdInv(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	unsigned char dwVersionInvalidates;
};
class SXAddl_SXCField12_SXDMemberCaption : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDMemberCaption)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDMemberCaption(){}
	~SXAddl_SXCField12_SXDMemberCaption(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stMemberPropertyCaptionUnique;
};
class SXAddl_SXCField12_SXDAutoshow : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField12_SXDAutoshow)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField12_SXDAutoshow(){}
	~SXAddl_SXCField12_SXDAutoshow(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 citmAutoShow;

};
class SXAddl_SXCCacheItem_SXDSxrmitmDisp : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheItem_SXDSxrmitmDisp)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheItem_SXDSxrmitmDisp(){}
	~SXAddl_SXCCacheItem_SXDSxrmitmDisp(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stDisplay;
};
class SXAddl_SXCCacheItem_SXDItmMpropMap : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheItem_SXDItmMpropMap)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheItem_SXDItmMpropMap(){}
	~SXAddl_SXCCacheItem_SXDItmMpropMap(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	std::vector<_INT32> rgMemProps;
};
class SxAddl_SXCCacheItem_SXDItmMpMapCount : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SxAddl_SXCCacheItem_SXDItmMpMapCount)
public:
	BiffStructurePtr clone();

	SxAddl_SXCCacheItem_SXDItmMpMapCount(){}
	~SxAddl_SXCCacheItem_SXDItmMpMapCount(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 cMemProps;
};
class SXAddl_SXCSXDH_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXDH_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXDH_SXDId(){}
	~SXAddl_SXCSXDH_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 dwCount;
};
class SXAddl_SXCSXDH_SXDSxdh : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXDH_SXDSxdh)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXDH_SXDSxdh(){}
	~SXAddl_SXCSXDH_SXDSxdh(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_INT32 isxth;
	unsigned short cchDimensionName;
	unsigned short cchDimensionUnique;
	unsigned short cchDimensionCaption;

	XLUnicodeStringNoCch stDimensionName;
	XLUnicodeStringNoCch stDimensionUnique;
	XLUnicodeStringNoCch stDimensionCaption;
};
class SXAddl_SXCSXMg_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXMg_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXMg_SXDId(){}
	~SXAddl_SXCSXMg_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stName;
};
class SXAddl_SXCSXMg_SXDUserCaption : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXMg_SXDUserCaption)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXMg_SXDUserCaption(){}
	~SXAddl_SXCSXMg_SXDUserCaption(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stUserCaption;
};
class SXAddl_SXCSXMgs_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXMgs_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXMgs_SXDId(){}
	~SXAddl_SXCSXMgs_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 cmgs;
	_UINT32 cmaps;
};
class SXAddl_SXCSXMgs_SXDMGrpSXDHMap : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXMgs_SXDMGrpSXDHMap)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXMgs_SXDMGrpSXDHMap(){}
	~SXAddl_SXCSXMgs_SXDMGrpSXDHMap(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 iKey;
	_UINT32 iVal;
};
class SXAddl_SXCHierarchy_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDId(){}
	~SXAddl_SXCHierarchy_SXDId(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stHierUnq;
};
class SXAddl_SXCHierarchy_SXDVerUpdInv : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDVerUpdInv)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDVerUpdInv(){}
	~SXAddl_SXCHierarchy_SXDVerUpdInv(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	unsigned char dwVersionInvalidates;
};
class SXAddl_SXCHierarchy_SXDProperty : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDProperty)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDProperty(){}
	~SXAddl_SXCHierarchy_SXDProperty(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	bool	fDisplayInReport;
	bool	fDisplayInTip;
	bool	fDisplayInCaption;

	unsigned short			cchProperty;
	XLUnicodeStringNoCch	stProperty;

	unsigned short			cchLevelUnq;
	unsigned short			ichPropName;
	unsigned short			cchPropName;
	short					isxtl;
};
class SXAddl_SXCHierarchy_SXDFilterMember : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDFilterMember)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDFilterMember(){}
	~SXAddl_SXCHierarchy_SXDFilterMember(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	bool							fMultFiltHavePlex;
	unsigned short					cItems;
	std::vector<std::wstring>		rgStMembers;
};
class SXAddl_SXCHierarchy_SXDFilterMember12 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDFilterMember12)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDFilterMember12(){}
	~SXAddl_SXCHierarchy_SXDFilterMember12(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32							isxtl;
	unsigned short					cItems;
	std::vector<std::wstring>		rgStMembers;
};
class SXAddl_SXCHierarchy_SXDSXSetParentUnique : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDSXSetParentUnique)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDSXSetParentUnique(){}
	~SXAddl_SXCHierarchy_SXDSXSetParentUnique(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stHierUnique;
};
class SXAddl_SXCHierarchy_SXDUserCaption : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDUserCaption)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDUserCaption(){}
	~SXAddl_SXCHierarchy_SXDUserCaption(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stCaption;
};
class SXAddl_SXCHierarchy_SXDMeasureGrp : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDMeasureGrp)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDMeasureGrp(){}
	~SXAddl_SXCHierarchy_SXDMeasureGrp(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stMeasureGroup;
};
class SXAddl_SXCHierarchy_SXDDisplayFolder : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDDisplayFolder)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDDisplayFolder(){}
	~SXAddl_SXCHierarchy_SXDDisplayFolder(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stDisplayFolder;
};
class SXAddl_SXCHierarchy_SXDParentKPI : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDParentKPI)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDParentKPI(){}
	~SXAddl_SXCHierarchy_SXDParentKPI(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stParentKPI;
};
class SXAddl_SXCHierarchy_SXDKPIValue : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPIValue)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPIValue(){}
	~SXAddl_SXCHierarchy_SXDKPIValue(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPIValue;
};
class SXAddl_SXCHierarchy_SXDKPIGoal : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPIGoal)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPIGoal(){}
	~SXAddl_SXCHierarchy_SXDKPIGoal(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPIGoal;
};
class SXAddl_SXCHierarchy_SXDKPIStatus : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPIStatus)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPIStatus(){}
	~SXAddl_SXCHierarchy_SXDKPIStatus(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPIStatus;
};
class SXAddl_SXCHierarchy_SXDKPITrend : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPITrend)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPITrend(){}
	~SXAddl_SXCHierarchy_SXDKPITrend(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPITrend;
};
class SXAddl_SXCHierarchy_SXDKPIWeight : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPIWeight)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPIWeight(){}
	~SXAddl_SXCHierarchy_SXDKPIWeight(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPIWeight;
};
class SXAddl_SXCHierarchy_SXDKPITime : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDKPITime)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDKPITime(){}
	~SXAddl_SXCHierarchy_SXDKPITime(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	XLUnicodeStringSegmentedSXADDL stKPITime;
};

class SXAddl_SXCHierarchy_SXDInfo12 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCHierarchy_SXDInfo12)
public:
	BiffStructurePtr clone();

	SXAddl_SXCHierarchy_SXDInfo12(){}
	~SXAddl_SXCHierarchy_SXDInfo12(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	bool fUnbalancedRealKnown;
	bool fUnbalancedReal;
	bool fUnbalancedGroupKnown;
	bool fUnbalancedGroup;
	bool fHidden;
};

class SXAddl_SXCSXCondFmt_SXDSXCondFmt : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXCondFmt_SXDSXCondFmt)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXCondFmt_SXDSXCondFmt(){}
	~SXAddl_SXCSXCondFmt_SXDSXCondFmt(){}

	static const ElementType type = typeSXAddl;

	virtual void load(CFRecord& record);

	_UINT32 sxcondfmtScope;
	_UINT32 sxcondfmtType;
	_UINT32 ipriority;
	_UINT32 csxrule;
};

class SXAddl_SXCSXCondFmts_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXCondFmts_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXCondFmts_SXDId(){}
	~SXAddl_SXCSXCondFmts_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;
	
	_INT32 cSxcondfmt;
};

class SXAddl_SXCSXrule_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXrule_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXrule_SXDId(){}
	~SXAddl_SXCSXrule_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;
};

class SXAddl_SXCSXrule_SXDSXrule : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXrule_SXDSXrule)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXrule_SXDSXrule(){}
	~SXAddl_SXCSXrule_SXDSXrule(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	BYTE sxrtype;
	bool fPart;
	bool fDataOnly;
	bool fLabelOnly;
	bool fGrandRw;
	bool fGrandCol;
	bool fGrandRwSav;
	bool fGrandColSav;
	bool fFuzzy;
	bool fLineMode;
	bool fDrillOnly;

	BYTE irwFirst;
	BYTE irwLast;
	BYTE icolFirst;
	BYTE icolLast;

	_UINT32 csxfilt;
	_INT32 iDim;
	_INT32 isxvd;
};

class SXAddl_SXCSXfilt_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXfilt_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXfilt_SXDId(){}
	~SXAddl_SXCSXfilt_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;
};

class SXAddl_SXCSXfilt_SXDSXfilt : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXfilt_SXDSXfilt)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXfilt_SXDSXfilt(){}
	~SXAddl_SXCSXfilt_SXDSXfilt(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	bool sxaxisRw;
	bool sxaxisCol;
	bool sxaxisData;
	bool fSelected;

	short grbitSbt;
	_INT32 iDim;
	_INT32 isxvd;
	_UINT32 cisxvi;
};

class SXAddl_SXCSXfilt_SXDSXItm : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXfilt_SXDSXItm)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXfilt_SXDSXItm(){}
	~SXAddl_SXCSXfilt_SXDSXItm(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	std::vector<unsigned short> rgIsxvi;
};

class SXAddl_SXCSXFilters12_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilters12_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilters12_SXDId(){}
	~SXAddl_SXCSXFilters12_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	_UINT32 cSxfilter12;
};

class SXAddl_SXCSXFilter12_SXDId : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDId)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDId(){}
	~SXAddl_SXCSXFilter12_SXDId(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	_UINT32 dwFilterid;
};

class SXAddl_SXCSXFilter12_SXDCaption : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDCaption)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDCaption(){}
	~SXAddl_SXCSXFilter12_SXDCaption(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stName;
};

class SXAddl_SXCSXFilter12_SXDSXFilter : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDSXFilter)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDSXFilter(){}
	~SXAddl_SXCSXFilter12_SXDSXFilter(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	_UINT32 isxvd;
	_INT32 isxvdMProp;
	_INT32 sxft; //SxFT enum
	_INT32 isxdiMeasure;
	_INT32 isxthMeasure;
};
class SXAddl_SXCSXFilter12_SXDSXFilterDesc : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDSXFilterDesc)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDSXFilterDesc(){}
	~SXAddl_SXCSXFilter12_SXDSXFilterDesc(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stDescription;
};
class SXAddl_SXCSXFilter12_SXDSXFilterValue1 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDSXFilterDesc)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDSXFilterValue1(){}
	~SXAddl_SXCSXFilter12_SXDSXFilterValue1(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stValue;
};
class SXAddl_SXCSXFilter12_SXDSXFilterValue2 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDSXFilterValue2)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDSXFilterValue2(){}
	~SXAddl_SXCSXFilter12_SXDSXFilterValue2(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stValue;
};

class SXAddl_SXCSXFilter12_SXDXlsFilter : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDXlsFilter)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDXlsFilter(){}
	~SXAddl_SXCSXFilter12_SXDXlsFilter(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	_UINT32 cft; //CFT enum
	_INT32 ccriteria;

	BiffStructurePtr data; //XlsFilter_Top10 or XlsFilter_Criteria
};

class SXAddl_SXCSXFilter12_SXDXlsFilterValue1 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDXlsFilterValue1)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDXlsFilterValue1(){}
	~SXAddl_SXCSXFilter12_SXDXlsFilterValue1(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stValue;
};
class SXAddl_SXCSXFilter12_SXDXlsFilterValue2 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCSXFilter12_SXDXlsFilterValue2)
public:
	BiffStructurePtr clone();

	SXAddl_SXCSXFilter12_SXDXlsFilterValue2(){}
	~SXAddl_SXCSXFilter12_SXDXlsFilterValue2(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	XLUnicodeStringSegmentedSXADDL stValue;
};

class XlsFilter_Top10 : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XlsFilter_Top10)
public:
	BiffStructurePtr clone();

	XlsFilter_Top10(){}
	~XlsFilter_Top10(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeXlsFilter_Top10;

	_UINT32 top10ft; //Top10FT enum
	bool fTop;

	Xnum numTopN;
};


class SXEZDoper : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXEZDoper)
public:
	BiffStructurePtr clone();

	static const ElementType type = typeSXEZDoper;
	
	virtual void load(CFRecord& record);

	BYTE vts;
	BYTE grbitSign;
	_UINT32 vtValue;
};

class XlsFilter_Criteria : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XlsFilter_Criteria)
public:
	BiffStructurePtr clone();

	XlsFilter_Criteria(){}
	~XlsFilter_Criteria(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeXlsFilter_Criteria;

	SXEZDoper ezdoper1;
	SXEZDoper ezdoper2;

	_UINT32 djoin1; //DJoin enum
};

class SXAddl_SXCView_SXDSXPIIvmb : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDSXPIIvmb)
public:
	BiffStructurePtr clone();

	SXAddl_SXCView_SXDSXPIIvmb(){}
	~SXAddl_SXCView_SXDSXPIIvmb(){}

	virtual void load(CFRecord& record);

	static const ElementType type = typeSXAddl;

	_UINT32 isxpi; 
	_UINT32 ivmb;
};
} // namespace XLS


