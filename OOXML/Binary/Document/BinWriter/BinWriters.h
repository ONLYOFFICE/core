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

#include "BinReaderWriterDefines.h"

#include "../../../DocxFormat/DocxFlat.h"
#include "../../../DocxFormat/Docx.h"
#include "../../../Binary/MathEquation/MathEquation.h"

#include "../../../DocxFormat/Document.h"
#include "../../../DocxFormat/FontTable.h"
#include "../../../DocxFormat/Numbering.h"
#include "../../../DocxFormat/Styles.h"
#include "../../../DocxFormat/Comments.h"
#include "../../../DocxFormat/Settings/Settings.h"
#include "../../../DocxFormat/External/HyperLink.h"
#include "../../../DocxFormat/Media/VbaProject.h"
#include "../../../DocxFormat/Media/JsaProject.h"
#include "../../../DocxFormat/HeaderFooter.h"
#include "../../../DocxFormat/App.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/Footnote.h"
#include "../../../DocxFormat/Endnote.h"
#include "../../../DocxFormat/Math/OMath.h"
#include "../../../DocxFormat/Math/oMathPara.h"
#include "../../../DocxFormat/Logic/Sdt.h"
#include "../../../DocxFormat/Logic/Table.h"
#include "../../../DocxFormat/Logic/Paragraph.h"
#include "../../../DocxFormat/Logic/Annotations.h"
#include "../../../DocxFormat/Logic/Hyperlink.h"
#include "../../../DocxFormat/Logic/FldSimple.h"

namespace NSBinPptxRW
{
	class CDrawingConverter;
	class CBinaryFileWriter;
}
namespace DocWrapper
{
	class FontProcessor;
}
namespace NSFontCutter
{
	class CEmbeddedFontsManager;
}
namespace BinDocxRW
{
	class ParamsWriter
	{
	public:
		NSBinPptxRW::CBinaryFileWriter*			m_pCBufferedStream;
		DocWrapper::FontProcessor*				m_pFontProcessor;
		NSBinPptxRW::CDrawingConverter*			m_pOfficeDrawingConverter;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;

		OOX::Document*			m_pMain;
		OOX::CSettings*			m_pSettings;
		PPTX::Theme*			m_pTheme;
		
		OOX::CStyles*			m_pStyles;
		OOX::CNumbering*		m_pNumbering;

		bool m_bLocalStyles;
		bool m_bLocalNumbering;

		OOX::CStyles*			m_pEmbeddedStyles;
		OOX::CNumbering*		m_pEmbeddedNumbering;

		OOX::IFileContainer*	m_pCurRels;
		std::map<int, bool>		m_mapIgnoreComments;

		ParamsWriter(NSBinPptxRW::CBinaryFileWriter* pCBufferedStream,
					 DocWrapper::FontProcessor* pFontProcessor,
					 NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter,
					 NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager);

		std::wstring AddEmbeddedStyle(const std::wstring & styleId);
	};
	class ParamsDocumentWriter
	{
	public:
		OOX::IFileContainer* m_pRels;

		ParamsDocumentWriter(OOX::IFileContainer* pRels) : m_pRels(pRels)
		{
		}
	};
	class BinaryCommonWriter
	{
	public:
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
		NSBinPptxRW::CBinaryFileWriter&			m_oStream;

		BinaryCommonWriter(ParamsWriter& oParamsWriter);
		int WriteItemStart(BYTE type);
		void WriteItemEnd(int nStart);
		int WriteItemWithLengthStart();
		void WriteItemWithLengthEnd(int nStart);
		void WriteBorder(const BYTE & type, const ComplexTypes::Word::CBorder& border);
		void WriteBorder(const ComplexTypes::Word::CBorder& border);
		void WriteTblBorders(const OOX::Logic::CTblBorders& Borders);
		void WriteTcBorders(const OOX::Logic::CTcBorders& Borders);
		void WritePBorders(const OOX::Logic::CPBdr& Borders);
		void WriteColor(BYTE type, const SimpleTypes::CHexColor& color);
		void WriteThemeColor(BYTE type, const nullable<SimpleTypes::CHexColor>& oHexColor, const nullable<SimpleTypes::CThemeColor>& oThemeColor,
			const nullable<SimpleTypes::CUcharHexNumber>& oThemeTint, const nullable<SimpleTypes::CUcharHexNumber>& oThemeShade);
		void WriteShd(const ComplexTypes::Word::CShading& Shd);
		void WritePaddings(const nullable<SimpleTypes::CTwipsMeasure>& left, const nullable<SimpleTypes::CTwipsMeasure>& top,
			const nullable<SimpleTypes::CTwipsMeasure>& right, const nullable<SimpleTypes::CTwipsMeasure>& bottom);
		void WriteFont(std::wstring sFontName, BYTE bType, DocWrapper::FontProcessor& m_oFontProcessor);
		void WriteBytesArray(BYTE* pData, long nDataSize);
		template<typename T> void WriteTrackRevision(const T& elem);
	};

	class BinaryHeaderFooterTableWriter
	{
		BinaryCommonWriter				m_oBcw;

		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		std::map<int, bool>*			m_mapIgnoreComments;
	public:
		ParamsWriter&								m_oParamsWriter;
		OOX::IFileContainer*						m_oDocumentRelsWriter;

		std::vector<OOX::CHdrFtr*>					m_aHeaders;
		std::vector<SimpleTypes::EHdrFtr>			m_aHeaderTypes;
		std::vector<OOX::Logic::CSectionProperty*>	m_aHeaderSectPrs;
		std::vector<OOX::CHdrFtr*>					m_aFooters;
		std::vector<SimpleTypes::EHdrFtr>			m_aFooterTypes;
		std::vector<OOX::Logic::CSectionProperty*>	m_aFooterSectPrs;

		BinaryHeaderFooterTableWriter(ParamsWriter& oParamsWriter, OOX::IFileContainer* oDocumentRel, std::map<int, bool>* mapIgnoreComments);
		void Write();
		void WriteHdrFtrContent(std::vector<OOX::CHdrFtr*>& aHdrFtrs, std::vector<SimpleTypes::EHdrFtr>& aHdrFtrTypes, std::vector<OOX::Logic::CSectionProperty*>& aHdrSectPrs, bool bHdr);
		void WriteHdrFtrItem(OOX::Logic::CSectionProperty* pSectPr, OOX::CHdrFtr* pHdrFtr, bool bHdr);
	};
	class BinarySigTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinarySigTableWriter(ParamsWriter& oParamsWriter);
		void Write();
	};
	class Binary_rPrWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		ParamsWriter& m_oParamsWriter;

		Binary_rPrWriter(ParamsWriter& oParamsWriter);
		void Write_rPr(OOX::Logic::CRunProperty* rPr);
		void Write_rPrChange(const OOX::Logic::CRPrChange& rPrChange);
	};
	class Binary_pPrWriter
	{
		BinaryCommonWriter m_oBcw;
		Binary_rPrWriter brPrs;
		
		ParamsWriter& m_oParamsWriter;
	public:
		BinaryHeaderFooterTableWriter* m_oBinaryHeaderFooterTableWriter;

		Binary_pPrWriter(ParamsWriter& oParamsWriter, BinaryHeaderFooterTableWriter* oBinaryHeaderFooterTableWriter);
		void Write_pPr(const OOX::Logic::CParagraphProperty& pPr);
		void WritePPrChange(const OOX::Logic::CPPrChange& pPrChange);
		void WriteInd(const ComplexTypes::Word::CInd& Ind);
		void WriteSpacing(const ComplexTypes::Word::CSpacing& Spacing);
		void WriteTabs(const OOX::Logic::CTabs& Tab, const nullable<ComplexTypes::Word::CInd>& oInd);
		void WriteTabItem(const ComplexTypes::Word::CTabStop& TabItem, const nullable<ComplexTypes::Word::CInd>& oInd);
		void WriteNumPr(const OOX::Logic::CNumPr& numPr, const OOX::Logic::CParagraphProperty& pPr);
		void WriteFramePr(const ComplexTypes::Word::CFramePr& oFramePr);
		void WriteSectPr (OOX::Logic::CSectionProperty* pSectPr);
		void WritePageSettings(OOX::Logic::CSectionProperty* pSectPr);
		void WritePageSize(OOX::Logic::CSectionProperty* pSectPr);
		void WritePageMargin(OOX::Logic::CSectionProperty* pSectPr);
		void WriteHeaderFooter(OOX::Logic::CSectionProperty* pSectPr, std::vector<ComplexTypes::Word::CHdrFtrRef*>& aRefs, bool bHdr);
		void WritePageNumType(const ComplexTypes::Word::CPageNumber& pPageNumber);
		void WriteLineNumType(const ComplexTypes::Word::CLineNumber& pLineNumber);
		void WriteSectPrChange(const OOX::Logic::CSectPrChange& sectPrChange);
		void WriteColumns(const OOX::Logic::CColumns& columns);
		void WriteColumn(const ComplexTypes::Word::CColumn& column);
		void WritePageBorders(const OOX::Logic::CPageBorders& PageBorders);
		void WritePageBorder(const ComplexTypes::Word::CPageBorder& pageBorder);
		void WriteNotePr(const nullable<ComplexTypes::Word::CNumFmt>& numFmt, const nullable<ComplexTypes::Word::CNumRestart>& numRestart,
							 const nullable<ComplexTypes::Word::CDecimalNumber>& numStart, nullable<ComplexTypes::Word::CFtnPos>* ftnPos,
							 nullable<ComplexTypes::Word::CEdnPos>* endPos, std::vector<OOX::CFtnEdnSepRef*>* refs);
		void WriteNumFmt(const ComplexTypes::Word::CNumFmt& oNumFmt);
	};
	class Binary_tblPrWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		Binary_tblPrWriter(ParamsWriter& oParamsWriter);
		void WriteTblPr(OOX::Logic::CTableProperty* p_tblPr);
		void WriteTblMar(const OOX::Logic::CTblCellMar& cellMar);
		void WriteCellMar(const OOX::Logic::CTcMar& cellMar);
		void Write_tblpPr(const ComplexTypes::Word::CTblPPr& pr);
		void WriteRowPr(const OOX::Logic::CTableRowProperties& rowPr);
		void WriteAfter(const nullable<ComplexTypes::Word::CDecimalNumber>& GridAfter, const nullable<ComplexTypes::Word::CTblWidth>& WAfter);
		void WriteBefore(const nullable<ComplexTypes::Word::CDecimalNumber>& GridBefore, const nullable<ComplexTypes::Word::CTblWidth>& WBefore);
		void WriteRowHeight(const ComplexTypes::Word::CHeight& rowHeight);
		void WriteW(int nType, const ComplexTypes::Word::CTblWidth& tableW, bool bWrite2 = false);
		void WriteCellPr(OOX::Logic::CTableCellProperties* cellPr);
		void WriteCellMerge(const ComplexTypes::Word::CCellMergeTrackChange& cellMerge);
		void WriteTblPrChange(const OOX::Logic::CTblPrChange& tblPrChange);
		void WriteTcPrChange(const OOX::Logic::CTcPrChange& tcPrChange);
		void WriteTrPrChange(const OOX::Logic::CTrPrChange& trPrChange);
	};
	class BinaryStyleTableWriter
	{
		BinaryCommonWriter m_oBcw;
		Binary_pPrWriter bpPrs;
		Binary_rPrWriter brPrs;
		Binary_tblPrWriter btblPrs;
		int m_nReaderGenName;
	public:
		BinaryStyleTableWriter(ParamsWriter& oParamsWriter);
		void Write(OOX::CStyles& styles);
		void WriteStylesContent(OOX::CStyles& styles);
		void WriteStyles(std::vector<OOX::CStyle*>& styles);
		void WriteStyle(const OOX::CStyle& style);
		void WriteTblStylePr(const std::vector<OOX::Logic::CTableStyleProperties*>& aProperties);
		void WriteTblStyleProperties(const OOX::Logic::CTableStyleProperties& oProperty);
	};
	class BinaryNumberingTableWriter
	{
		BinaryCommonWriter m_oBcw;
		Binary_pPrWriter bpPrs;
		Binary_rPrWriter brPrs;
	public:
		BinaryNumberingTableWriter(ParamsWriter& oParamsWriter);
		void Write(const OOX::CNumbering& numbering);
		void WriteNumberingContent(const OOX::CNumbering& numbering);
		void WriteNums(const OOX::CNumbering& numbering);
		void WriteNum(const OOX::Numbering::CNum& num);
		void WriteLvlOverride(const OOX::Numbering::CNumLvl& numLvl);
		void WriteAbstractNums(const OOX::CNumbering& numbering);
		void WriteAbstractNum(const OOX::Numbering::CAbstractNum& num, int nIndex, const std::vector<OOX::Numbering::CNum*>& aNums);
		void WriteLevels(const std::vector<OOX::Numbering::CLvl*>& lvls, int nAId, const std::vector<OOX::Numbering::CNum*>& aNums);
		void WriteLevel(const OOX::Numbering::CLvl& lvl);
		void WriteLvlLegacy(const ComplexTypes::Word::CLvlLegacy& lvlLegacy);
		void WriteLevelText(const std::wstring& text);
	};
	class BinaryOtherTableWriter
	{
		class EmbeddedBinaryWriter
		{
		private: NSBinPptxRW::CBinaryFileWriter &m_oStream;
		public: EmbeddedBinaryWriter(NSBinPptxRW::CBinaryFileWriter &oStream):m_oStream(oStream)
			{
			}
		public: void WriteBYTE(BYTE btVal)
			{
				m_oStream.WriteBYTE(btVal);
			}
		public: void WriteString(std::wstring& sVal)
			{
				m_oStream.WriteStringW2(sVal);
			}
		public: void WriteULONG(long nVal)
			{
				m_oStream.WriteLONG(nVal);
			}
		};
		BinaryCommonWriter	m_oBcw;
		PPTX::Theme*		m_pTheme;

		ParamsWriter&		m_oParamsWriter;
	public:
		BinaryOtherTableWriter(ParamsWriter& oParamsWriter, PPTX::Theme* pTheme);
		void Write();
		void WriteOtherContent();
	};
	class BinaryDocumentTableWriter
	{
	private:
		ParamsWriter&			m_oParamsWriter;
		
		ParamsDocumentWriter&	m_oParamsDocumentWriter;
		BinaryCommonWriter		m_oBcw;
		Binary_pPrWriter		bpPrs;
		Binary_rPrWriter		brPrs;
		std::wstring			m_sCurParStyle;

		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		std::map<int, bool>*			m_mapIgnoreComments;
	public:
		Binary_tblPrWriter				btblPrs;
		OOX::Logic::CSectionProperty*	pSectPr;
		OOX::WritingElement*			pBackground;
		OOX::CDocument*					poDocument;
		OOX::JsaProject*				pJsaProject;

		bool							m_bWriteSectPr;//Записывать ли свойства верхнего уровня в данном экземпляре BinaryOtherTableWriter
//---------------------------------
		BinaryDocumentTableWriter(ParamsWriter& oParamsWriter, ParamsDocumentWriter& oParamsDocumentWriter, std::map<int, bool>* mapIgnoreComments, BinaryHeaderFooterTableWriter* oBinaryHeaderFooterTableWriter);
	
		void Write(OOX::Logic::CDocPartPr* pDocPartPr);
		void Write(OOX::Logic::CDocParts* pDocParts);
		void Write(OOX::Logic::CDocPartTypes* pDocPartTypes);
		void Write(OOX::Logic::CDocPartBehaviors* pDocPartBehaviors);

		void WriteAltChunk(OOX::Media& oAltChunk, OOX::CStyles* styles);

		void Write(std::vector<OOX::WritingElement*> & aElems);
		void WriteDocumentContent(const std::vector<OOX::WritingElement*> & aElems);
		void WriteBackground (OOX::WritingElement* pBackground);
		void WriteParapraph(OOX::Logic::CParagraph& par, OOX::Logic::CParagraphProperty* pPr);
		void WriteParagraphContent(const std::vector<OOX::WritingElement*> & content, bool bHyperlink = false);
		void WriteDel(const OOX::Logic::CDel& oDel);
		void WriteIns(const OOX::Logic::CIns& oIns);
		void WriteMoveFrom(const OOX::Logic::CMoveFrom& oMoveFrom);
		void WriteMoveTo(const OOX::Logic::CMoveTo& oMoveTo);
		template<typename T> void WriteMoveRangeStart(const T& elem);
		template<typename T> void WriteMoveRangeEnd(const T& elem);
		void WriteComment(OOX::EElementType eType, nullable<SimpleTypes::CDecimalNumber>& oId);
		void WriteFldChar(OOX::Logic::CFldChar* pFldChar);
		void WritePermission(OOX::Logic::CPermStart* pPerm);
		void WritePermission(OOX::Logic::CPermEnd* pPerm);
		void WriteFldSimple(OOX::Logic::CFldSimple* pFldSimple);
		void WriteFldSimpleContent(OOX::Logic::CFldSimple* pFldSimple);
		void WriteFFData(const OOX::Logic::CFFData& oFFData);
		void WriteFFCheckBox(const OOX::Logic::CFFCheckBox& oCheckBox);
		void WriteDDList(const OOX::Logic::CFFDDList& oDDList);
		void WriteFFHelpText(const ComplexTypes::Word::CFFHelpText& oHelpText);
		void WriteFFStatusText(const ComplexTypes::Word::CFFStatusText& oStatusText);
		void WriteTextInput(const OOX::Logic::CFFTextInput& oTextInput);
		void WriteBookmarkStart(const OOX::Logic::CBookmarkStart& oBookmarkStart);
		void WriteBookmarkEnd(const OOX::Logic::CBookmarkEnd& oBookmarkEnd);
		void WriteHyperlink(OOX::Logic::CHyperlink* pHyperlink);
		void WriteHyperlinkContent(std::wstring& sLink, OOX::Logic::CHyperlink* pHyperlink);
		void WriteRun(OOX::Logic::CRun* pRun, bool bHyperlink = false, bool bMathRun = false);
		void WriteMathArgNodes(const std::vector<OOX::WritingElement*>& arrItems);
		void WriteMathRunContent(OOX::Logic::CMRun* pMRun);
		void WriteMathAccPr(const OOX::Logic::CAccPr &pAccPr);
		void WriteMathAln(const OOX::Logic::CAln &pAln);
		void WriteMathAlnScr(const OOX::Logic::CAlnScr &pAlnScr);
		void WriteMathArgPr(const OOX::Logic::CArgPr &pArgPr);
		void WriteMathArgSz(const OOX::Logic::CArgSz &pArgSz);
		void WriteMathBarPr(const OOX::Logic::CBarPr &pBarPr);
		void WriteMathBaseJc(const OOX::Logic::CBaseJc &pBaseJc);
		void WriteMathBegChr(const OOX::Logic::CBegChr &pBegChr);
		void WriteMathBorderBoxPr(const OOX::Logic::CBorderBoxPr &pBorderBoxPr);
		void WriteMathBoxPr(const OOX::Logic::CBoxPr &pBoxPr);
		void WriteMathBrk(const OOX::Logic::CBrk &pBrk);
		void WriteMathCGp(const OOX::Logic::CCGp &pCGp);
		void WriteMathCGpRule(const OOX::Logic::CCGpRule &pCGpRule);
		void WriteMathChr(const OOX::Logic::CChr &pChr);
		void WriteMathCount(const OOX::Logic::CCount &pCount);
		void WriteMathCSp(const OOX::Logic::CCSp &pCSp);
		void WriteMathCtrlPr(const OOX::Logic::CCtrlPr &pCtrlPr, BYTE type = c_oSer_OMathBottomNodesType::CtrlPr);
		void WriteMathDelimiter(const std::vector<OOX::WritingElement*> & arrItems, LONG &lColumn);
		void WriteMathDeg(const OOX::Logic::CDeg &pDeg);
		void WriteMathDegHide(const OOX::Logic::CDegHide &pDegHide);
		void WriteMathDen(const OOX::Logic::CDen &pDen);
		void WriteMathDiff(const OOX::Logic::CDiff &pDiff);
		void WriteMathDelimiterPr(const OOX::Logic::CDelimiterPr &pDelimiterPr, long &lColumn);
		void WriteMathElement(const OOX::Logic::CElement &pElement);
		void WriteMathEndChr(const OOX::Logic::CEndChr &pEndChr);
		void WriteMathEqArr(const std::vector<OOX::WritingElement*> & arrItems, LONG& lRow);
		void WriteMathFName(const OOX::Logic::CFName &pFName);
		void WriteMathFPr(const OOX::Logic::CFPr &pFPr);
		void WriteMathFuncPr(const OOX::Logic::CFuncPr &pFuncPr);
		void WriteMathGroupChrPr(const OOX::Logic::CGroupChrPr &pGroupChrPr);
		void WriteMathGrow(const OOX::Logic::CGrow &pGrow);
		void WriteMathHideBot(const OOX::Logic::CHideBot &pHideBot);
		void WriteMathHideLeft(const OOX::Logic::CHideLeft &pHideLeft);
		void WriteMathHideRight(const OOX::Logic::CHideRight &pHideRight);
		void WriteMathHideTop(const OOX::Logic::CHideTop &pHideTop);
		void WriteMathMJc(const OOX::Logic::CMJc &pMJc);
		void WriteMathLim(const OOX::Logic::CLim &pLim);
		void WriteMathLimLoc(const OOX::Logic::CLimLoc &pLimLoc);
		void WriteMathColumn(const LONG &lColumn);
		void WriteMathRow(const LONG &lRow);
		void WriteMathLimLowPr(const OOX::Logic::CLimLowPr &pLimLowPr);
		void WriteMathLimUppPr(const OOX::Logic::CLimUppPr &pLimUppPr);
		void WriteMathLit(const OOX::Logic::CLit &pLit);
		void WriteMathMatrix(const std::vector<OOX::WritingElement*> & arrItems, LONG &lRow, LONG &lCol);
		void WriteMathMaxDist(const OOX::Logic::CMaxDist &pMaxDist);
		void WriteMathMc(const OOX::Logic::CMc &pMc);
		void WriteMathMcJc(const OOX::Logic::CMcJc &pMcJc);
		void WriteMathMcPr(const OOX::Logic::CMcPr &pMcPr);
		void WriteMathMcs(const OOX::Logic::CMcs &pMcs);
		void WriteMathMcs ( LONG lColumn );
		void WriteMathMPr(const OOX::Logic::CMPr &pMPr);
		void WriteMathMr(const std::vector<OOX::WritingElement*> & arrItems);
		void WriteMathNaryPr(const OOX::Logic::CNaryPr &pNaryPr);
		void WriteMathNoBreak(const OOX::Logic::CNoBreak &pNoBreak);
		void WriteMathNor(const OOX::Logic::CNor &pNor);
		void WriteMathNum(const OOX::Logic::CNum &pNum);
		void WriteMathObjDist(const OOX::Logic::CObjDist &pObjDist);
		void WriteMathOMath(const OOX::Logic::COMath &pOMath);
		void WriteMathOMathPara(const std::vector<OOX::WritingElement*> & arrItems);
		void WriteMathOMathParaPr(const OOX::Logic::COMathParaPr &pOMathParaPr);
		void WriteMathOpEmu(const OOX::Logic::COpEmu &pOpEmu);
		void WriteMathPhantPr(const OOX::Logic::CPhantPr &pPhantPr);
		void WriteMathPlcHide(const OOX::Logic::CPlcHide &pPlcHide);
		void WriteMathPos(const OOX::Logic::CPos &pPos);
		void WriteMathRadPr(const OOX::Logic::CRadPr &pRadPr);
		void WriteMathMRPr(const OOX::Logic::CMRPr &pMRPr);
		void WriteMathRSp(const OOX::Logic::CRSp &pRSp);
		void WriteMathRSpRule(const OOX::Logic::CRSpRule &pRSpRule);
		void WriteMathScr(const OOX::Logic::CScr &pScr);
		void WriteMathSepChr(const OOX::Logic::CSepChr &pSepChr);
		void WriteMathShow(const OOX::Logic::CShow &pShow);
		void WriteMathShp(const OOX::Logic::CShp &pShp);
		void WriteMathSPrePr(const OOX::Logic::CSPrePr &pSPrePr);
		void WriteMathSSubPr(const OOX::Logic::CSSubPr &pSSubPr);
		void WriteMathSSubSupPr(const OOX::Logic::CSSubSupPr &pSSubSupPr);
		void WriteMathSSupPr(const OOX::Logic::CSSupPr &pSSupPr);
		void WriteMathStrikeBLTR(const OOX::Logic::CStrikeBLTR &pStrikeBLTR);
		void WriteMathStrikeH(const OOX::Logic::CStrikeH &pStrikeH);
		void WriteMathStrikeTLBR(const OOX::Logic::CStrikeTLBR &pStrikeTLBR);
		void WriteMathStrikeV(const OOX::Logic::CStrikeV &pStrikeV);
		void WriteMathSty(const OOX::Logic::CSty &pSty);
		void WriteMathSub(const OOX::Logic::CSub &pSub);
		void WriteMathSubHide(const OOX::Logic::CSubHide &pSubHide);
		void WriteMathSup(const OOX::Logic::CSup &pSup);
		void WriteMathSupHide(const OOX::Logic::CSupHide &pSupHide);
		void WriteMathDel(const OOX::Logic::CMDel &pMDel);
		void WriteMathIns(const OOX::Logic::CMIns &pMIns);
		void WriteMathText(const OOX::Logic::CMText &pMText);
		void WriteMathTransp(const OOX::Logic::CTransp &pTransp);
		void WriteMathType(const OOX::Logic::CType &pType);
		void WriteMathVertJc(const OOX::Logic::CVertJc &pVertJc);
		void WriteMathZeroAsc(const OOX::Logic::CZeroAsc &pZeroAsc);
		void WriteMathZeroDesc(const OOX::Logic::CZeroDesc &pZeroDesc);
		void WriteMathZeroWid(const OOX::Logic::CZeroWid &pZeroWid);
		void WritePreparedRun(OOX::Logic::CRun *pRun, bool bHyperlink, std::vector<OOX::WritingElement*>::iterator &start, std::vector<OOX::WritingElement*>::iterator &end);
		void WriteRunContent(std::vector<OOX::WritingElement*>::iterator &start, std::vector<OOX::WritingElement*>::iterator &end, bool bHyperlink = false);
		void WriteNoteRef(const nullable<SimpleTypes::COnOff>& oCustomMarkFollows, const nullable<SimpleTypes::CDecimalNumber>& oId);
		void WriteText(const std::wstring& text, BYTE type);
		bool WriteDrawingPptx(OOX::WritingElement* item);
		void WriteDrawing(std::wstring* pXml, OOX::Logic::CDrawing* pDrawing, PPTX::Logic::GraphicFrame *pGraphic);
		void WriteNvGraphicFramePr(const PPTX::Logic::NvGraphicFramePr& oGraphicFramePr);
		void WriteDocPr(const PPTX::Logic::CNvPr& oDocPr);
		void WriteEffectExtent(const OOX::Drawing::CEffectExtent& oEffectExtent);
		void WriteExtent(const ComplexTypes::Drawing::CPositiveSize2D& oExtent);
		void WritePositionH(const OOX::Drawing::CPosH& oPosH);
		void WritePositionV(const OOX::Drawing::CPosV& oPosV);
		void WriteSizeRelH(const OOX::Drawing::CSizeRelH& oSizeRelH);
		void WriteSizeRelV(const OOX::Drawing::CSizeRelV& oSizeRelV);
		void WriteSimplePos(const ComplexTypes::Drawing::CPoint2D& oSimplePos);
		void WriteWrapSquare(const OOX::Drawing::CWrapSquare& oWrapSquare);
		void WriteWrapThrough(const OOX::Drawing::CWrapThrough& oWrapThrough);
		void WriteWrapTight(const OOX::Drawing::CWrapTight& oWrapTight);
		void WriteWrapTopBottom(const OOX::Drawing::CWrapTopBottom& oWrapTopBottom);
		void WriteWrapPolygon(const OOX::Drawing::CWrapPath& oWrapPath);
		void WriteLineTo(const std::vector<ComplexTypes::Drawing::CPoint2D*>& aLineTo);
		void WritePoint2D(const ComplexTypes::Drawing::CPoint2D& oPoint2D);
		void WriteDocTable(OOX::Logic::CTbl* tbl);
		bool ValidateRow(const std::vector<OOX::WritingElement *> & arrItems);
		int GetColsCount(const std::vector<OOX::WritingElement *>& arrItems);
		void WriteTblGrid(const OOX::Logic::CTblGrid& grid);
		void WriteTblGridChange(const OOX::Logic::CTblGridChange& tblGridChange);
		void WriteTableContent(std::vector<OOX::WritingElement *>& content, OOX::Logic::CTableProperty* pTblPr);
		void WriteRow(const OOX::Logic::CTr& Row, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex);
		void WriteRowContent(const std::vector<OOX::WritingElement *> & content, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex);
		void WriteCell(OOX::Logic::CTc& tc, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex, int nCurColIndex);
		void WriteSdt(OOX::Logic::CSdt* pStd, int type, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex);
		void WriteSdtPr(const OOX::Logic::CSdtPr& oStdPr);
		void WriteSdtCheckBox(const OOX::Logic::CSdtCheckBox& oSdtCheckBox);
		void WriteSdtComboBox(const OOX::Logic::CSdtComboBox& oSdtComboBox);
		void WriteSdtListItem(const ComplexTypes::Word::CSdtListItem& oSdtListItem);
		void WriteSdtPrDataBinding(const ComplexTypes::Word::CDataBinding& oDataBinding);
		void WriteSdtPrDate(const OOX::Logic::CDate& oDate);
		void WriteDocPartList(const OOX::Logic::CSdtDocPart& oSdtDocPart);
		void WriteDropDownList(const OOX::Logic::CSdtDropDownList& oDropDownList);
		void WriteSdtFormPr(const OOX::Logic::CFormPr& oFormPr);
		void WriteSdtTextFormPr(const OOX::Logic::CTextFormPr& oTextFormPr);
		void WriteSdtTextFormPrComb(const ComplexTypes::Word::CComb& oComb);
		void WriteSdtTextFormPrFormat(const ComplexTypes::Word::CTextFormFormat& oFormat);
		void WriteSdtPicture(const OOX::Logic::CSdtPicture& oSdtPicture);
		void WriteSdtComplexFormPr(const OOX::Logic::CComplexFormPr& oComplexFormPr);
	};
	class BinaryCustomsTableWriter
	{
	private:
		ParamsWriter&			m_oParamsWriter;
		BinaryCommonWriter		m_oBcw;
	public:
		BinaryCustomsTableWriter(ParamsWriter& oParamsWriter);
		void Write(OOX::CDocument* poDocument);
	};
	class BinaryCommentsTableWriter
	{
		struct CCommentWriteTemp
		{
			OOX::CComment* pComment;
			nullable<bool> bDone;
			nullable<std::wstring> sUserId;
			nullable<std::wstring> sProviderId;
			nullable<SimpleTypes::CLongHexNumber> nDurableId;
			nullable<std::wstring> sDateUtc;
			nullable<std::wstring> sUserData;
			std::vector<CCommentWriteTemp*> aReplies;
		};
		BinaryCommonWriter m_oBcw;
		ParamsWriter& m_oParamsWriter;
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
	public:
		BinaryCommentsTableWriter(ParamsWriter& oParamsWriter);
		void Write(OOX::CComments& oComments, OOX::CCommentsExt* pCommentsExt, OOX::CCommentsExtensible* pCommentsExtensible, OOX::CCommentsUserData* pCommentsUserData, OOX::CPeople* pPeople, OOX::CCommentsIds* pCommentsIds, std::map<int, bool>& mapIgnoreComments);
		void WriteCommentsContent(OOX::CComments& oComments, OOX::CCommentsExt* pCommentsExt, OOX::CCommentsExtensible* pCommentsExtensible, OOX::CCommentsUserData* pCommentsUserData, OOX::CPeople* pPeople, OOX::CCommentsIds* pCommentsIds, std::map<int, bool>& mapIgnoreComments, ParamsDocumentWriter& oParamsDocumentWriter);
		void WriteComment(CCommentWriteTemp& oComment, BinaryDocumentTableWriter & oBinaryDocumentTableWriter);
		void WriteReplies(std::vector<CCommentWriteTemp*>& aCommentWriteTemp, BinaryDocumentTableWriter & oBinaryDocumentTableWriter);
	};
	class BinarySettingsTableWriter
	{
		BinaryCommonWriter m_oBcw;
		Binary_pPrWriter bpPrs;
		Binary_rPrWriter brPrs;
	public:
		BinarySettingsTableWriter(ParamsWriter& oParamsWriter);
		void Write(OOX::CSettings& oSettings, OOX::CSettingsCustom& oSettingsCustom);
		void WriteSettingsContent(OOX::CSettings& oSettings, OOX::CSettingsCustom& oSettingsCustom);
		void WriteMathPr(const OOX::Logic::CMathPr &pMathPr);
		void WriteColorSchemeMapping(const PPTX::Logic::ClrMap& oColorSchemeMapping);
		void WriteCompat(const OOX::Settings::CCompat& oCompat);
		void WriteCompatSetting(const OOX::Settings::CCompatSetting& oCompatSetting);
		void WriteDocumentProtection(const OOX::Settings::CDocProtect& oDocProtect);
		void WriteWriteProtection(const OOX::Settings::CWriteProtection& oWriteProtection);
	};
	class BinaryNotesTableWriter
	{
		BinaryCommonWriter m_oBcw;
		ParamsWriter& m_oParamsWriter;
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
	public:
		BinaryNotesTableWriter(ParamsWriter& oParamsWriter);
		void WriteFootnotes(OOX::CFootnotes& oFootnotes);
		void WriteEndnotes(OOX::CEndnotes& oEndnotes);
		void WriteNotes(const std::vector<OOX::CFtnEdn*>& arrNotes, ParamsDocumentWriter& oParamsDocumentWriter);
		void WriteNote(const OOX::CFtnEdn& oFtnEdn, BinaryDocumentTableWriter & oBinaryDocumentTableWriter);
	};
	class BinaryFileWriter
	{
		BinaryCommonWriter	m_oBcw;
		int					m_nLastFilePos;
		int					m_nRealTableCount;
		int					m_nMainTableStart;
	public:
		ParamsWriter&		m_oParamsWriter;

		BinaryFileWriter(ParamsWriter& oParamsWriter);
		static std::wstring WriteFileHeader(long nDataSize, int version);
		void WriteMainTableStart(bool bSigTable = true);
		void WriteMainTableEnd();
		int WriteTableStart(BYTE type, int nStartPos = -1);
		void WriteTableEnd(int nCurPos);
		void intoBindoc(const std::wstring& sDir);
		void ParagraphAddBreak(OOX::Logic::CParagraph* pParagraph);
	};

}
