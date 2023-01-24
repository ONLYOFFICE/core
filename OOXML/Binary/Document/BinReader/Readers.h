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

#include "FileWriter.h"
#include "ReaderClasses.h"

#include "../../../../DesktopEditor/common/StringBuilder.h"

namespace OOX
{
	class CSettingsCustom;
	class CSettings;
}
namespace BinDocxRW {


class Binary_CommonReader
{
protected:
	NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
public:
	Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream);
};
class Binary_CommonReader2 : public Binary_CommonReader
{
public:
	Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream);
	docRGB ReadColor();
	void ReadHexColor(SimpleTypes::CHexColor *pColor);
	void ReadThemeColor(int length, CThemeColor& oCThemeColor);
	int ReadThemeColorContent(BYTE type, long length, void* poResult);
	template<typename T> int ReadTrackRevision(long length, T* poResult);
	template<typename T> int ReadTrackRevision2(long length, T* poResult);
	int ReadShdComplexType(BYTE type, long length, void* poResult);
private:
	template<typename T> int ReadTrackRevisionInner(BYTE type, long length, T* poResult);
	template<typename T> int ReadTrackRevisionInner2(BYTE type, long length, T* poResult);
};
class Binary_HdrFtrTableReader : public Binary_CommonReader
{
	Writers::FileWriter&	m_oFileWriter;

	int						nCurType;
	int						nCurHeaderType;
public:
	Writers::HeaderFooterWriter& m_oHeaderFooterWriter;
	
	Binary_HdrFtrTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
    int Read();
    int ReadHdrFtrContent	(BYTE type, long length, void* poResult);
    int ReadHdrFtrFEO		(BYTE type, long length, void* poResult);
    int ReadHdrFtrItem		(BYTE type, long length, void* poResult);
	int ReadHdrFtrItemContent(BYTE type, long length, void* poResult);
};
class Binary_rPrReader : public Binary_CommonReader
{
protected:
	Binary_CommonReader2 oBinary_CommonReader2;
	Writers::FileWriter& m_oFileWriter;
	boost::unordered_map<std::wstring, char>& m_mapFonts;
public:
	Binary_rPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read(long stLen, void* poResult);
	int ReadContent(BYTE type, long length, void* poResult);
	int ReadrPrChange2(BYTE type, long length, void* poResult);
};
class Binary_pPrReader : public Binary_CommonReader
{
private:
	Writers::FontTableWriter& m_oFontTableWriter;
public:
	Binary_CommonReader2		oBinary_CommonReader2;
	Binary_rPrReader			oBinary_rPrReader;
	Binary_HdrFtrTableReader	oBinary_HdrFtrTableReader;

	Writers::FileWriter&		m_oFileWriter;
	bool						bDoNotWriteNullProp;
	long						m_nCurNumId;
	long						m_nCurLvl;

	Binary_pPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read(long stLen, void* poResult);
	int ReadContent( BYTE type, long length, void* poResult);
	int ReadPPrChange(BYTE type, long length, void* poResult);
	int ReadInd(BYTE type, long length, void* poResult);
	int ReadSpacing(BYTE type, long length, void* poResult);
	int ReadTabs(BYTE type, long length, void* poResult);
	int ReadTabItem(BYTE type, long length, void* poResult);
	int ReadNumPr(BYTE type, long length, void* poResult);
	int ReadBorders(BYTE type, long length, void* poResult);
	int ReadBorder(BYTE type, long length, void* poResult);
	int ReadBorder2(BYTE type, long length, void* poResult);
	int ReadFramePr(BYTE type, long length, void* poResult);
    int Read_SecPr(BYTE type, long length, void* poResult);
	int ReadFootnotePr(BYTE type, long length, void* poResult);
	int ReadEndnotePr(BYTE type, long length, void* poResult);
	int ReadNumFmt(BYTE type, long length, void* poResult);
	int ReadSectPrChange(BYTE type, long length, void* poResult);
	int Read_pgSz(BYTE type, long length, void* poResult);
	int Read_pgMar(BYTE type, long length, void* poResult);
	int Read_pgSetting(BYTE type, long length, void* poResult);
	int Read_pgHeader(BYTE type, long length, void* poResult);
	int Read_pgFooter(BYTE type, long length, void* poResult);
	int Read_pageNumType(BYTE type, long length, void* poResult);
	int Read_lineNumType(BYTE type, long length, void* poResult);
	int ReadCols(BYTE type, long length, void* poResult);
	int ReadCol(BYTE type, long length, void* poResult);
	int ReadPageBorders(BYTE type, long length, void* poResult);
	int ReadPageBorder(BYTE type, long length, void* poResult);
};
class Binary_tblPrReader : public Binary_CommonReader
{
protected:
	Binary_pPrReader oBinary_pPrReader;
	Binary_CommonReader2 oBinary_CommonReader2;
public:
	std::vector<double> m_aCurTblGrid;
public:
	Binary_tblPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read_tblPr(BYTE type, long length, void* poResult);
	int ReadW(BYTE type, long length, void* poResult);
	int ReadCellMargins(BYTE type, long length, void* poResult);
	int Read_tblpPr(BYTE type, long length, void* poResult);
	int Read_tblpPr2(BYTE type, long length, void* poResult);
	int Read_RowPr(BYTE type, long length, void* poResult);
	int ReadAfter(BYTE type, long length, void* poResult);
	int ReadBefore(BYTE type, long length, void* poResult);
	int ReadHeight(BYTE type, long length, void* poResult);
	int Read_CellPr(BYTE type, long length, void* poResult);
	int ReadPaddings(BYTE type, long length, void* poResult);
	int ReadPaddings2(BYTE type, long length, void* poResult);
	int ReadCellMerge(BYTE type, long length, void* poResult);
	int ReadTblPrChange(BYTE type, long length, void* poResult);
	int ReadTrPrChange(BYTE type, long length, void* poResult);
	int ReadTcPrChange(BYTE type, long length, void* poResult);
};
class Binary_NumberingTableReader : public Binary_CommonReader
{
	Binary_pPrReader oBinary_pPrReader;
	Binary_rPrReader oBinary_rPrReader;
	Writers::NumberingWriter& oNumberingWriters;
	Writers::FontTableWriter& m_oFontTableWriter;
	std::vector<docNum*> m_aDocNums;
	std::vector<docANum*> m_aDocANums;
	std::map<int, int> m_mapANumToNum;
public:
	Binary_NumberingTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read();
	int ReadNumberingContent(BYTE type, long length, void* poResult);
	int ReadNums(BYTE type, long length, void* poResult);
	int ReadNum(BYTE type, long length, void* poResult);
	int ReadLvlOverride(BYTE type, long length, void* poResult);
	int ReadAbstractNums(BYTE type, long length, void* poResult);
	int ReadAbstractNum(BYTE type, long length, void* poResult);
	int ReadLevels(BYTE type, long length, void* poResult);
	int ReadLevel(BYTE type, long length, void* poResult);
	int ReadLvlLegacy(BYTE type, long length, void* poResult);
	int ReadLevelText(BYTE type, long length, void* poResult);
	int ReadLevelTextItem(BYTE type, long length, void* poResult);
};
class BinaryStyleTableReader : public Binary_CommonReader
{
	Binary_pPrReader			oBinary_pPrReader;
	Binary_rPrReader			oBinary_rPrReader;
	Binary_tblPrReader			oBinary_tblPrReader;

	Writers::StylesWriter&		m_oStylesWriter;
	Writers::FontTableWriter&	m_oFontTableWriter;
public:
	BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read();
	int ReadStyleTableContent(BYTE type, long length, void* poResult);
	int ReadStyle(BYTE type, long length, void* poResult);
	int ReadStyleContent(BYTE type, long length, void* poResult);
	int ReadTblStylePr(BYTE type, long length, void* poResult);
	int ReadTblStyleProperty(BYTE type, long length, void* poResult);
};
class Binary_OtherTableReader : public Binary_CommonReader
{
	Writers::FileWriter& m_oFileWriter;
    std::wstring m_sFileInDir;
public:
    Binary_OtherTableReader(std::wstring sFileInDir, NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read();
	int ReadOtherContent(BYTE type, long length, void* poResult);
//	int ReadImageMapContent(BYTE type, long length, void* poResult); not using now
};
class Binary_CustomsTableReader : public Binary_CommonReader
{
	Writers::FileWriter& m_oFileWriter;
public:
	Binary_CustomsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read();
	int ReadCustom(BYTE type, long length, void* poResult);
	int ReadCustomContent(BYTE type, long length, void* poResult);
};
class Binary_CommentsTableReader : public Binary_CommonReader
{
public:
	CComments				m_oComments;
	Writers::FileWriter&	m_oFileWriter;

	Binary_CommentsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter);
	int Read();
	int ReadComments(BYTE type, long length, void* poResult);
	int ReadCommentContent(BYTE type, long length, void* poResult);
	int ReadCommentContentExt(BYTE type, long length, void* poResult);
	int ReadReplies(BYTE type, long length, void* poResult);
};
class Binary_SettingsTableReader : public Binary_CommonReader
{
	Binary_pPrReader m_oBinary_pPrReader;
	Binary_rPrReader m_oBinary_rPrReader;
	Writers::SettingWriter& m_oSettingWriter;
	Writers::FileWriter& m_oFileWriter;
	
	OOX::CSettingsCustom* m_pSettingsCustom;
public:
	Binary_SettingsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, OOX::CSettingsCustom* pSettingsCustom);
	int Read();
	int ReadDocProtect(BYTE type, long length, void* poResult);
	int ReadWriteProtect(BYTE type, long length, void* poResult);
	int ReadSettings(BYTE type, long length, void* poResult);
	int ReadCompat(BYTE type, long length, void* poResult);
	int ReadCompatSetting(BYTE type, long length, void* poResult);
	int ReadFootnotePr(BYTE type, long length, void* poResult);
	int ReadEndnotePr(BYTE type, long length, void* poResult);
	int ReadMathPr(BYTE type, long length, void* poResult);
	int ReadMathBrkBin(BYTE type, long length, void* poResult);
	int ReadMathBrkBinSub(BYTE type, long length, void* poResult);
	int ReadMathDefJc(BYTE type, long length, void* poResult);
	int ReadMathDispDef(BYTE type, long length, void* poResult);
	int ReadMathInterSp(BYTE type, long length, void* poResult);
	int ReadMathIntLim(BYTE type, long length, void* poResult);
	int ReadMathIntraSp(BYTE type, long length, void* poResult);
	int ReadMathLMargin(BYTE type, long length, void* poResult);
	int ReadMathMathFont(BYTE type, long length, void* poResult);
	
	int ReadMathNaryLim(BYTE type, long length, void* poResult);
	int ReadMathPostSp(BYTE type, long length, void* poResult);
	int ReadMathPreSp(BYTE type, long length, void* poResult);
	int ReadMathRMargin(BYTE type, long length, void* poResult);
	int ReadMathSmallFrac(BYTE type, long length, void* poResult);
	int ReadMathWrapIndent(BYTE type, long length, void* poResult);
	int ReadMathWrapRight(BYTE type, long length, void* poResult);
	int ReadClrSchemeMapping(BYTE type, long length, void* poResult);
};
class Binary_DocumentTableReader : public Binary_CommonReader
{
private:
    Binary_CommonReader2            oBinary_CommonReader2;

    Writers::FileWriter&            m_oFileWriter;
    Writers::FontTableWriter&       m_oFontTableWriter;
    Binary_pPrReader                oBinary_pPrReader;
    Binary_rPrReader                oBinary_rPrReader;
    Binary_tblPrReader              oBinary_tblPrReader;
	NSStringUtils::CStringBuilder*	m_pCurWriter;
    OOX::Logic::CRunProperty		m_oCur_rPr;
	OOX::Logic::CRunProperty		m_oMath_rPr;
	NSStringUtils::CStringBuilder	m_oCur_pPr;
    BYTE                            m_byteLastElemType;
public:
    Writers::ContentWriter&         m_oDocumentWriter;
    Writers::MediaWriter&           m_oMediaWriter;

	bool							m_bUsedParaIdCounter;
	bool							m_bOFormRead;

    Binary_DocumentTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, Writers::ContentWriter& oDocumentWriter, bool bOFormRead);
	~Binary_DocumentTableReader();
	
	int Read();
	NSStringUtils::CStringBuilder& GetRunStringWriter();
	int ReadDocumentContentOut(long length);
	int ReadDocumentContent(BYTE type, long length, void* poResult);
	int ReadParagraph(BYTE type, long length, void* poResult);
	int ReadParagraphContent(BYTE type, long length, void* poResult);
	int ReadDelIns(BYTE type, long length, void* poResult);
	int ReadMoveFromRangeStart(BYTE type, long length, void* poResult);
	int ReadMoveToRangeStart(BYTE type, long length, void* poResult);
	int ReadMoveFromRangeEnd(BYTE type, long length, void* poResult);
	int ReadMoveToRangeEnd(BYTE type, long length, void* poResult);
	int ReadFldChar(BYTE type, long length, void* poResult);
	int ReadFldSimple(BYTE type, long length, void* poResult);
	int ReadFFData(BYTE type, long length, void* poResult);
	int ReadFFCheckBox(BYTE type, long length, void* poResult);
	int ReadDDList(BYTE type, long length, void* poResult);
	int ReadFFHelpText(BYTE type, long length, void* poResult);
	int ReadFFStatusText(BYTE type, long length, void* poResult);
	int ReadTextInput(BYTE type, long length, void* poResult);
	int ReadBookmarkStart(BYTE type, long length, void* poResult);
	int ReadBookmarkEnd(BYTE type, long length, void* poResult);
	int ReadPermStart(BYTE type, long length, void* poResult);
	int ReadPermEnd(BYTE type, long length, void* poResult);	
	int ReadHyperlink(BYTE type, long length, void* poResult);
	int ReadMathLong(BYTE type, long length, void* poResult);
	int ReadMathArgOut(long length);
	int ReadMathArg(BYTE type, long length, void* poResult);
	int ReadMathAcc(BYTE type, long length, void* poResult);
	int ReadMathAccPr(BYTE type, long length, void* poResult);
	int ReadMathAln(BYTE type, long length, void* poResult);
	int ReadMathAlnScr(BYTE type, long length, void* poResult);
	int ReadMathArgPr(BYTE type, long length, void* poResult);
	int ReadMathArgSz(BYTE type, long length, void* poResult);
	int ReadMathBar(BYTE type, long length, void* poResult);
	int ReadMathBarPr(BYTE type, long length, void* poResult);
	int ReadMathBaseJc(BYTE type, long length, void* poResult);
	int ReadMathBegChr(BYTE type, long length, void* poResult);
	int ReadMathBorderBox(BYTE type, long length, void* poResult);
	int ReadMathBorderBoxPr(BYTE type, long length, void* poResult);
	int ReadMathBox(BYTE type, long length, void* poResult);
	int ReadMathBoxPr(BYTE type, long length, void* poResult);
	int ReadMathBrk(BYTE type, long length, void* poResult);
	int ReadMathCGp(BYTE type, long length, void* poResult);
	int ReadMathCGpRule(BYTE type, long length, void* poResult);
	int ReadMathChr(BYTE type, long length, void* poResult);
	int ReadMathCount(BYTE type, long length, void* poResult);
	int ReadMathCSp(BYTE type, long length, void* poResult);
	int ReadMathCtrlPr(BYTE type, long length, void* poResult);
	int ReadMathCtrlPrDelIns(BYTE type, long length, void* poResult);
	int ReadMathDelimiter(BYTE type, long length, void* poResult);
	int ReadMathDegHide(BYTE type, long length, void* poResult);
	int ReadMathDiff(BYTE type, long length, void* poResult);
	int ReadMathDelimiterPr(BYTE type, long length, void* poResult);
	int ReadMathEndChr(BYTE type, long length, void* poResult);
	int ReadMathEqArr(BYTE type, long length, void* poResult);
	int ReadMathEqArrPr(BYTE type, long length, void* poResult);
	int ReadMathFraction(BYTE type, long length, void* poResult);
	int ReadMathFPr(BYTE type, long length, void* poResult);
	int ReadMathFunc(BYTE type, long length, void* poResult);
	int ReadMathFuncPr(BYTE type, long length, void* poResult);
	int ReadMathGroupChr(BYTE type, long length, void* poResult);
	int ReadMathGroupChrPr(BYTE type, long length, void* poResult);
	int ReadMathGrow(BYTE type, long length, void* poResult);
	int ReadMathHideBot(BYTE type, long length, void* poResult);
	int ReadMathHideLeft(BYTE type, long length, void* poResult);
	int ReadMathHideRight(BYTE type, long length, void* poResult);
	int ReadMathHideTop(BYTE type, long length, void* poResult);
	int ReadMathMJc(BYTE type, long length, void* poResult);
	int ReadMathLimLoc(BYTE type, long length, void* poResult);
	int ReadMathLimLow(BYTE type, long length, void* poResult);
	int ReadMathLimLowPr(BYTE type, long length, void* poResult);
	int ReadMathLimUpp(BYTE type, long length, void* poResult);
	int ReadMathLimUppPr(BYTE type, long length, void* poResult);
	int ReadMathLit(BYTE type, long length, void* poResult);
	int ReadMathMatrix(BYTE type, long length, void* poResult);
	int ReadMathMaxDist(BYTE type, long length, void* poResult);
	int ReadMathMc(BYTE type, long length, void* poResult);
	int ReadMathMcJc(BYTE type, long length, void* poResult);
	int ReadMathMcPr(BYTE type, long length, void* poResult);
	int ReadMathMcs(BYTE type, long length, void* poResult);
	int ReadMathMPr(BYTE type, long length, void* poResult);
	int ReadMathMr(BYTE type, long length, void* poResult);
	int ReadMathNary(BYTE type, long length, void* poResult);
	int ReadMathNaryPr(BYTE type, long length, void* poResult);
	int ReadMathNoBreak(BYTE type, long length, void* poResult);
	int ReadMathNor(BYTE type, long length, void* poResult);
	int ReadMathObjDist(BYTE type, long length, void* poResult);
	int ReadMathOMathParaOut(long length);
	int ReadMathOMathPara(BYTE type, long length, void* poResult);
	int ReadMathOMathParaPr(BYTE type, long length, void* poResult);
	int ReadMathOpEmu(BYTE type, long length, void* poResult);
	int ReadMathPhant(BYTE type, long length, void* poResult);
	int ReadMathPhantPr(BYTE type, long length, void* poResult);
	int ReadMathPlcHide(BYTE type, long length, void* poResult);
	int ReadMathPos(BYTE type, long length, void* poResult);
    std::wstring GetMathText (long length);
	int ReadMathText(BYTE type, long length, void* poResult);
	int ReadMathMRun(BYTE type, long length, void* poResult);
	int ReadMathInsDel(BYTE type, long length, void* poResult);
	int ReadMathRad(BYTE type, long length, void* poResult);
	int ReadMathRadPr(BYTE type, long length, void* poResult);
	int ReadMathMRPr(BYTE type, long length, void* poResult);
	int ReadMathRSp(BYTE type, long length, void* poResult);
	int ReadMathRSpRule(BYTE type, long length, void* poResult);
	int ReadMathScr(BYTE type, long length, void* poResult);
	int ReadMathSepChr(BYTE type, long length, void* poResult);
	int ReadMathShow(BYTE type, long length, void* poResult);
	int ReadMathShp(BYTE type, long length, void* poResult);
	int ReadMathSPre(BYTE type, long length, void* poResult);
	int ReadMathSPrePr(BYTE type, long length, void* poResult);
	int ReadMathSSub(BYTE type, long length, void* poResult);
	int ReadMathSSubPr(BYTE type, long length, void* poResult);
	int ReadMathSSubSup(BYTE type, long length, void* poResult);
	int ReadMathSSubSupPr(BYTE type, long length, void* poResult);
	int ReadMathSSup(BYTE type, long length, void* poResult);
	int ReadMathSSupPr(BYTE type, long length, void* poResult);
	int ReadMathStrikeBLTR(BYTE type, long length, void* poResult);
	int ReadMathStrikeH(BYTE type, long length, void* poResult);
	int ReadMathStrikeTLBR(BYTE type, long length, void* poResult);
	int ReadMathStrikeV(BYTE type, long length, void* poResult);
	int ReadMathSty(BYTE type, long length, void* poResult);
	int ReadMathSubHide(BYTE type, long length, void* poResult);
	int ReadMathSupHide(BYTE type, long length, void* poResult);
	int ReadMathTransp(BYTE type, long length, void* poResult);
	int ReadMathType(BYTE type, long length, void* poResult);
	int ReadMathVertJc(BYTE type, long length, void* poResult);
	int ReadMathZeroAsc(BYTE type, long length, void* poResult);
	int ReadMathZeroDesc(BYTE type, long length, void* poResult);
	int ReadMathZeroWid(BYTE type, long length, void* poResult);
	int ReadRun(BYTE type, long length, void* poResult);
	int ReadRunContent(BYTE type, long length, void* poResult);
	int ReadFootnoteRef(BYTE type, long length, void* poResult);
	int ReadEndnoteRef(BYTE type, long length, void* poResult);
	void ReadDrawing(CDrawingProperty &oCDrawingProperty);
	int ReadObject(BYTE type, long length, void* poResult);
	int ReadComment(BYTE type, long length, void* poResult);
	int ReadDocTable(BYTE type, long length, void* poResult);
	int Read_tblGrid(BYTE type, long length, void* poResult);
	int Read_tblGridChange(BYTE type, long length, void* poResult);
	int Read_TableContent(BYTE type, long length, void* poResult);
	int Read_Row(BYTE type, long length, void* poResult);
	int ReadRowContent(BYTE type, long length, void* poResult);
	int ReadCell(BYTE type, long length, void* poResult);
	int ReadCellContent(BYTE type, long length, void* poResult);
	int ReadImage(BYTE type, long length, void* poResult);
    int Read_Background(BYTE type, long length, void* poResult);
	int ReadPptxDrawing(BYTE type, long length, void* poResult);
	int ReadEmbedded(BYTE type, long length, void* poResult);
	int ReadCNvGraphicFramePr(BYTE type, long length, void* poResult);
	int ReadDocPr(BYTE type, long length, void* poResult);
	int ReadEffectExtent(BYTE type, long length, void* poResult);
	int ReadExtent(BYTE type, long length, void* poResult);
	int ReadPositionH(BYTE type, long length, void* poResult);
	int ReadPositionV(BYTE type, long length, void* poResult);
	int ReadSimplePos(BYTE type, long length, void* poResult);
	int ReadSizeRelH(BYTE type, long length, void* poResult);
	int ReadSizeRelV(BYTE type, long length, void* poResult);
	int ReadWrapThroughTight(BYTE type, long length, void* poResult);
	int ReadWrapPolygon(BYTE type, long length, void* poResult);
	int ReadLineTo(BYTE type, long length, void* poResult);
	int ReadPolygonPoint(BYTE type, long length, void* poResult);
	int ReadEmptyWrap(BYTE type, long length, void* poResult);
	int Read_SecPr(BYTE type, long length, void* poResult);
	int ReadSdt(BYTE type, long length, void* poResult);
	int ReadSdtPr(BYTE type, long length, void* poResult);
	int ReadSdtCheckBox(BYTE type, long length, void* poResult);
	int ReadSdtComboBox(BYTE type, long length, void* poResult);
	int ReadSdtListItem(BYTE type, long length, void* poResult);
	int ReadSdtPrDataBinding(BYTE type, long length, void* poResult);
	int ReadSdtPrDate(BYTE type, long length, void* poResult);
	int ReadDocPartList(BYTE type, long length, void* poResult);
	int ReadDropDownList(BYTE type, long length, void* poResult);

	int ReadDocParts(BYTE type, long length, void* poResult);
	int ReadDocPart(BYTE type, long length, void* poResult);
	int ReadDocPartPr(BYTE type, long length, void* poResult);
	int ReadDocPartBehaviors(BYTE type, long length, void* poResult);
	int ReadDocPartTypes(BYTE type, long length, void* poResult);

	int ReadSdtFormPr(BYTE type, long length, void* poResult);
	int ReadSdtTextFormPr(BYTE type, long length, void* poResult);
	int ReadSdtTextFormPrComb(BYTE type, long length, void* poResult);
	int ReadSdtTextFormPrFormat(BYTE type, long length, void* poResult);
	int ReadSdtPicture(BYTE type, long length, void* poResult);
	int ReadSdtComplexFormPr(BYTE type, long length, void* poResult);
};
class Binary_NotesTableReader : public Binary_CommonReader
{
	Writers::FileWriter&					m_oFileWriter;
	bool									m_bIsFootnote;
	nullable<SimpleTypes::CDecimalNumber>	m_oId;
	nullable<SimpleTypes::CFtnEdn>		m_oType;
public:
	Binary_NotesTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, bool bIsFootnote);
	int Read();
	int ReadNotes(BYTE type, long length, void* poResult);
	int ReadNote(BYTE type, long length, void* poResult);
	int ReadNoteContent(BYTE type, long length, void* poResult);
};
class BinaryFileReader
{
private:
	NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
	Writers::FileWriter&			m_oFileWriter;
    std::wstring					m_sFileInDir;
	bool							m_bMacro = false;
	bool							m_bMacroRead = false;
	bool							m_bOForm = false;
public:
	BinaryFileReader(std::wstring& sFileInDir, NSBinPptxRW::CBinaryFileReader& oBufferedStream, Writers::FileWriter& oFileWriter, bool bMacro = false, bool bOForm = false);
	int ReadFile();
	int ReadMainTable();
};
}
