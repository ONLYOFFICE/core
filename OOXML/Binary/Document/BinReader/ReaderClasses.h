﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "HeaderFooterWriter.h"

#include "../../../Base/Unit.h"
#include "../../../DocxFormat/Logic/RunProperty.h"
#include "../../../DocxFormat/Logic/ParagraphProperty.h"
#include "../BinWriter/BinReaderWriterDefines.h"

#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>

namespace BinDocxRW {

class docRGB
{
public:
	BYTE R;
	BYTE G;
	BYTE B;

	docRGB();
	std::wstring ToString();
};
class CThemeColor{
public:
	bool Auto;
	BYTE Color;
	BYTE Tint;
	BYTE Shade;

	bool bShade;
	bool bTint;
	bool bColor;

	CThemeColor();

	void Reset();
	bool IsNoEmpty();

	std::wstring ToStringColor();
	std::wstring ToStringTint();
	std::wstring ToStringShade();
	void ToCThemeColor( nullable<SimpleTypes::CHexColor>& oColor,
												nullable<SimpleTypes::CThemeColor>& oThemeColor,
												nullable<SimpleTypes::CUcharHexNumber>& oThemeTint,
												nullable<SimpleTypes::CUcharHexNumber>& oThemeShade);
};
class Background
{
public:
    docRGB      Color;
    CThemeColor ThemeColor;

    std::wstring     sObject;

    bool bColor;
    bool bThemeColor;

	Background();

	std::wstring Write();
};

class docStyle
{
public:
    std::wstring Name;
    std::wstring Id;
	BYTE byteType;
	bool Default;
	bool Custom;
	std::wstring Aliases;
    std::wstring BasedOn;
    std::wstring NextId;
	std::wstring Link;
	bool qFormat;
	long uiPriority;
	bool hidden;
	bool semiHidden;
	bool unhideWhenUsed;
	bool autoRedefine;
	bool locked;
	bool personal;
	bool personalCompose;
	bool personalReply;
    std::wstring TextPr;
    std::wstring ParaPr;
    std::wstring TablePr;
    std::wstring RowPr;
    std::wstring CellPr;
    std::vector<std::wstring> TblStylePr;

	bool bDefault;
	bool bCustom;
	bool bqFormat;
	bool buiPriority;
	bool bhidden;
	bool bsemiHidden;
	bool bunhideWhenUsed;
	bool bautoRedefine;
	bool blocked;
	bool bpersonal;
	bool bpersonalCompose;
	bool bpersonalReply;

public:
	docStyle();

	void Write(NSStringUtils::CStringBuilder*  pCStringWriter);
};
class tblStylePr
{
public:
	NSStringUtils::CStringBuilder Writer;
	BYTE Type;
	bool bType;

public:
	tblStylePr();
};
class PaddingsToWrite
{
public:
	long Left;
	long Top;
	long Right;
	long Bottom;

	bool bLeft;
	bool bTop;
	bool bRight;
	bool bBottom;

public:
	PaddingsToWrite();
};
class PaddingsToWriteMM
{
public:
	double Left;
	double Top;
	double Right;
	double Bottom;

	bool bLeft;
	bool bTop;
	bool bRight;
	bool bBottom;

public:
	PaddingsToWriteMM();
};
class docImg
{
public:
	long MediaId;
	BYTE Type;
	double X;
	double Y;
	double Width;
	double Height;
	PaddingsToWriteMM Paddings;
	_INT32 m_nDocPr;

	bool bMediaId;
	bool bType;
	bool bX;
	bool bY;
	bool bWidth;
	bool bHeight;
	bool bPaddings;
    std::wstring srId;

	docImg(_INT32 nDocPr);

	void Write(NSStringUtils::CStringBuilder*  pCStringWriter);
};
class docW
{
public:
	BYTE Type;
	double W;
	long WDocx;

	bool bType;
	bool bW;
	bool bWDocx;

	docW();

	void Write(NSStringUtils::CStringBuilder& pCStringWriter, const std::wstring& sName);
	std::wstring Write(const std::wstring& sName);
};
class rowPrAfterBefore
{
public:
    std::wstring sName;
	long nGridAfter;
	docW oAfterWidth;
	bool bGridAfter;

	rowPrAfterBefore(std::wstring name);
	void Write(NSStringUtils::CStringBuilder& writer);
};
class WriteHyperlink
{
public:
    std::wstring rId;
    std::wstring href;
    std::wstring anchor;
    std::wstring tooltip;
	NSStringUtils::CStringBuilder writer;

	static WriteHyperlink* Parse(std::wstring fld);
	void Write(NSStringUtils::CStringBuilder& wr);
};
class IdCounter
{
private:
	_INT32 m_nId;

public:
	IdCounter(_INT32 nStart = 0);

	_INT32 getNextId(_INT32 nCount = 1);
	_INT32 getCurrentId();
};
class CComment
{
private:
	IdCounter& m_oParaIdCounter;
	IdCounter& m_oFormatIdCounter;

public:
	void *pBinary_DocumentTableReader;

	_INT32 IdOpen;
	_INT32 IdFormat;
    std::wstring UserName;
	std::wstring Initials;
    std::wstring UserId;
	std::wstring ProviderId;
    std::wstring Date;
	std::wstring OOData;
	std::wstring DateUtc;
	std::wstring UserData;
	bool Solved;
	_UINT32 DurableId;
    std::wstring Text;
	std::wstring sContent;
    
	std::wstring sParaId;
    std::wstring sParaIdParent;
	std::vector<CComment*> replies;

	bool bIdOpen;
	bool bIdFormat;
	bool bSolved;
	bool bDurableId;

public:
	CComment(IdCounter& oParaIdCounter, IdCounter& oFormatIdCounter);
	~CComment();

	_INT32 getCount();

	void setFormatStart(_INT32 IdFormatStart);

	std::wstring writeRef(const std::wstring& sBefore, const std::wstring& sRef, const std::wstring& sAfter);
	static std::wstring writeRef(CComment* pComment, const std::wstring& sBefore, const std::wstring& sRef, const std::wstring& sAfter);

	static void writeContentWritePart(CComment* pComment, std::wstring& sText, _INT32 nPrevIndex, _INT32 nCurIndex, std::wstring& sRes);
	static std::wstring writeContent(CComment* pComment);
	static std::wstring writeContentExt(CComment* pComment);
	static std::wstring writeContentExtensible(CComment* pComment);
	static std::wstring writeContentUserData(CComment* pComment);
	static std::wstring writeContentsIds(CComment* pComment);
	static std::wstring writePeople(CComment* pComment);
};
class CComments
{
    boost::unordered_map<_INT32, CComment*> m_mapComments;
    boost::unordered_map<std::wstring, CComment*> m_mapAuthors;

public:
	IdCounter m_oFormatIdCounter;
	IdCounter m_oParaIdCounter;

	CComments();
	~CComments();

	void add(CComment* pComment);
	void addAuthor(CComment* pComment);

	CComment* get(_INT32 nInd);
	_INT32 getNextId(_INT32 nCount = 1);

	std::wstring writeContent();
	std::wstring writeContentExt();
	std::wstring writeContentExtensible();
	std::wstring writeContentUserData();
	std::wstring writeContentsIds();
	std::wstring writePeople();
};

class CDrawingPropertyWrapPoint
{
public:
	_INT64 X;
	_INT64 Y;

	bool bX;
	bool bY;

	CDrawingPropertyWrapPoint();
};
class CDrawingPropertyWrap
{
public:
	BYTE WrappingType;
	bool Edited;
	CDrawingPropertyWrapPoint Start;
	std::vector<CDrawingPropertyWrapPoint*> Points;

	bool bWrappingType;
	bool bEdited;
	bool bStart;
	
	CDrawingPropertyWrap();
	~CDrawingPropertyWrap();
};
class CDrawingProperty
{
public:
	bool			bObject;
    std::wstring	sObjectProgram;	
	_UINT32			nObjectId;
    BYTE			nObjectType;
	
	_INT32 DataPos;
	_INT32 DataLength;
    BYTE Type;
    bool BehindDoc;
	_INT64 DistL;
	_INT64 DistT;
	_INT64 DistR;
	_INT64 DistB;
    bool LayoutInCell;
	_UINT32 RelativeHeight;
    bool BSimplePos;
	_INT64 EffectExtentL;
	_INT64 EffectExtentT;
	_INT64 EffectExtentR;
	_INT64 EffectExtentB;
	_INT64 Width;
	_INT64 Height;
    BYTE PositionHRelativeFrom;
    BYTE PositionHAlign;
	_INT64 PositionHPosOffset;
	double PositionHPctOffset;
    BYTE PositionVRelativeFrom;
    BYTE PositionVAlign;
	_INT64 PositionVPosOffset;
    double PositionVPctOffset;
	_INT64 SimplePosX;
	_INT64 SimplePosY;
    std::wstring sSizeRelH;
    std::wstring sSizeRelV;
	_INT32 m_nDocPr;
    std::wstring sGraphicFramePr;
    std::wstring sDocPr;
   
    std::wstring sGraphicFrameContent;

	CDrawingPropertyWrap DrawingPropertyWrap;

    bool bDataPos;
	bool bDataLength;
	bool bType;
	bool bBehindDoc;
	bool bDistL;
	bool bDistT;
	bool bDistR;
	bool bDistB;
	bool bLayoutInCell;
	bool bRelativeHeight;
	bool bBSimplePos;
	bool bEffectExtentL;
	bool bEffectExtentT;
	bool bEffectExtentR;
	bool bEffectExtentB;
	bool bWidth;
	bool bHeight;
	bool bPositionHRelativeFrom;
	bool bPositionHAlign;
	bool bPositionHPosOffset;
	bool bPositionHPctOffset;
	bool bPositionVRelativeFrom;
	bool bPositionVAlign;
	bool bPositionVPosOffset;
	bool bPositionVPctOffset;
	bool bSimplePosX;
	bool bSimplePosY;
	bool bDrawingPropertyWrap;

	CDrawingProperty(_INT32 nDocPr);

	bool IsGraphicFrameContent();
	std::wstring Write();
};
class CWiterTblPr
{
public:
    std::wstring Jc;
    std::wstring TableInd;
    std::wstring TableW;
    std::wstring TableCellMar;
    std::wstring TableBorders;
    std::wstring Shd;
    std::wstring tblpPr;
    std::wstring Style;
    std::wstring RowBandSize;
    std::wstring ColBandSize;
    std::wstring Look;
    std::wstring Layout;
    std::wstring tblPrChange;
    std::wstring TableCellSpacing;
	std::wstring Caption;
	std::wstring Description;
	std::wstring Overlap;

	bool IsEmpty();
	std::wstring Write();
};
class CHyperlink{
public:
    std::wstring    rId;
    std::wstring    sLink;
    std::wstring    sAnchor;
    std::wstring    sTooltip;
    bool            History;
    std::wstring    sDocLocation;
    std::wstring    sTgtFrame;

	NSStringUtils::CStringBuilder writer;

	bool bHistory;

public:
	CHyperlink();

	void Write(NSStringUtils::CStringBuilder& wr);
};
class CFldSimple{
public:
    std::wstring sInstr;
	NSStringUtils::CStringBuilder writer;

public:
	CFldSimple();

	void Write(NSStringUtils::CStringBuilder& wr);
};

class TrackRevision
{
public:
    std::wstring Author;
    std::wstring Date;
	_INT32* Id;
    std::wstring UserId;
	_INT32* vMerge;
	_INT32* vMergeOrigin;

	OOX::Logic::CRunProperty* RPr;

	CWiterTblPr* tblPr;
	NSStringUtils::CStringBuilder* tblGridChange;
	NSStringUtils::CStringBuilder* trPr;
	NSStringUtils::CStringBuilder* tcPr;
	NSStringUtils::CStringBuilder* content;
	NSStringUtils::CStringBuilder* contentRun;

	TrackRevision();
	~TrackRevision();

	bool IsNoEmpty();
	std::wstring ToString(std::wstring sName);
	void Write(NSStringUtils::CStringBuilder*  pCStringWriter, std::wstring sName);
};
}
