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
#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"

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
class SXAddl_SXCField_SXDEnd : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCField_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCField_SXDEnd(){}
	~SXAddl_SXCField_SXDEnd(){}

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
class SXAddl_SXCView_SXDEnd	: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCView_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCView_SXDEnd(){}
	~SXAddl_SXCView_SXDEnd(){}

	static const ElementType	type = typeSXAddl;

	virtual void load(CFRecord& record);
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
class SXAddl_SXCAutoSort_SXDEnd : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCAutoSort_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCAutoSort_SXDEnd(){}
	~SXAddl_SXCAutoSort_SXDEnd(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
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
class SXAddl_SXCCache_SXDEnd  : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCache_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCache_SXDEnd(){}
	~SXAddl_SXCCache_SXDEnd(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
	SXAddl_SXCCache_SXDEnd(CFRecord& record);
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
class SXAddl_SXCCacheField_SXDEnd : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheField_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheField_SXDEnd(){}
	~SXAddl_SXCCacheField_SXDEnd(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;
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

class SXAddl_SXCCacheItem_SXDEnd : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SXAddl_SXCCacheItem_SXDEnd)
public:
	BiffStructurePtr clone();

	SXAddl_SXCCacheItem_SXDEnd(){}
	~SXAddl_SXCCacheItem_SXDEnd(){}

	virtual void load(CFRecord& record);

	static const ElementType	type = typeSXAddl;

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

} // namespace XLS

