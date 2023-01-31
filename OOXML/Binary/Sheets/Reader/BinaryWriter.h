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

#include "../Reader/CSVReader.h"
#include "CommonWriter.h"
#include "ChartFromToBinary.h"

#include "../Common/Common.h"
#include "../Common/BinReaderWriterDefines.h"
#include "../../Presentation/BinReaderWriterDefines.h"
#include "../../../../DesktopEditor/common/Directory.h"

#include "../../Document/DocWrapper/FontProcessor.h"

#include "../../../DocxFormat/VmlDrawing.h"

#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../XlsxFormat/WorkbookComments.h"
#include "../../../XlsxFormat/Styles/Styles.h"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinks.h"
#include "../../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../../XlsxFormat/CalcChain/CalcChain.h"
#include "../../../XlsxFormat/Table/Connections.h"
#include "../../../XlsxFormat/Table/QueryTable.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CXlsx;
		class CXlsxFlat;
		class CPerson;
		class CPersonList;
		class CThreadedComment;
		class CThreadedCommentMention;
	}
}

namespace BinXlsxRW 
{
	class BinaryTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream);
		void Write(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTableParts& oTableParts);
		void WriteTablePart(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTablePart& oTablePart);
		void WriteTable(OOX::Spreadsheet::CTableFile* pTableFile);
		void WriteAltTextTable(const OOX::Spreadsheet::CAltTextTable& oAltTextTable);
		void WriteAutoFilter(const OOX::Spreadsheet::CAutofilter& oAutofilter);
		void WriteFilterColumns(const std::vector<OOX::Spreadsheet::CFilterColumn *>& aFilterColumn);
		void WriteFilterColumn(OOX::Spreadsheet::CFilterColumn& oFilterColumn);
		void WriteFilters(const OOX::Spreadsheet::CFilters& oFilters);
		void WriteFilter(OOX::Spreadsheet::CFilter& oFilter);
		void WriteDateGroupItem(OOX::Spreadsheet::CDateGroupItem& oDateGroupItem);
		void WriteCustomFilters(const OOX::Spreadsheet::CCustomFilters& oCustomFilters);
		void WriteCustomFiltersItems(const std::vector<OOX::Spreadsheet::CCustomFilter *>& aCustomFilters);
		void WriteCustomFiltersItem(OOX::Spreadsheet::CCustomFilter& oCustomFilter);
		void WriteDynamicFilter(const OOX::Spreadsheet::CDynamicFilter& oDynamicFilter);
		void WriteColorFilter(const OOX::Spreadsheet::CColorFilter& oColorFilter);
		void WriteTop10(const OOX::Spreadsheet::CTop10& oTop10);
		void WriteSortCondition(const OOX::Spreadsheet::CSortCondition& oSortCondition);
		void WriteSortState(const OOX::Spreadsheet::CSortState& oSortState);
		void WriteTableColumn(const OOX::Spreadsheet::CTableColumn& oTableColumn);
		void WriteTableColumns(const OOX::Spreadsheet::CTableColumns& oTableColumns);
		void WriteTableStyleInfo(const OOX::Spreadsheet::CTableStyleInfo& oTableStyleInfo);
		void WriteQueryTable(const OOX::Spreadsheet::CQueryTable& oQueryTable);
		void WriteQueryTableRefresh(const OOX::Spreadsheet::CQueryTableRefresh& oQueryTableRefresh);
		void WriteQueryTableFields(const OOX::Spreadsheet::CQueryTableFields& oQueryTableFields);
		void WriteQueryTableField(const OOX::Spreadsheet::CQueryTableField& oQueryTableField);
		void WriteQueryTableDeletedFields(const OOX::Spreadsheet::CQueryTableDeletedFields& oQueryTableDeletedFields);
		void WriteQueryTableDeletedField(const OOX::Spreadsheet::CQueryTableDeletedField& oQueryTableDeletedField);
	};
	class BinaryStyleTableWriter
	{
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
	public:
		BinaryStyleTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager);
		void Write(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteStylesContent(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteBorders(const OOX::Spreadsheet::CBorders& borders, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme);
		void WriteBorder(const OOX::Spreadsheet::CBorder& border, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme);
		void WriteBorderProp(const OOX::Spreadsheet::CBorderProp& borderProp, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme);
		void WriteCellStyleXfs(const OOX::Spreadsheet::CCellStyleXfs& cellStyleXfs);
		void WriteCellXfs(const OOX::Spreadsheet::CCellXfs& cellXfs);
		void WriteXfs(const OOX::Spreadsheet::CXfs& xfs);
		void WriteAligment(const OOX::Spreadsheet::CAligment& aligment);
		void WriteProtection(const OOX::Spreadsheet::CProtection& protection);
		void WriteFills(const OOX::Spreadsheet::CFills& fills, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme);
		void WriteFill(const OOX::Spreadsheet::CFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors);
		void WritePatternFill(const OOX::Spreadsheet::CPatternFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors);
		void WriteGradientFill(const OOX::Spreadsheet::CGradientFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors);
		void WriteGradientFillStop(const OOX::Spreadsheet::CGradientStop& stop, OOX::Spreadsheet::CIndexedColors* pIndexedColors);
		void WriteFonts(const OOX::Spreadsheet::CFonts& fonts, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteFont(const OOX::Spreadsheet::CFont& font, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* theme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteNumFmts(const OOX::Spreadsheet::CNumFmts& numFmts);
		void WriteNumFmt(const OOX::Spreadsheet::CNumFmt& numFmt);
		void WriteCellStyles(const OOX::Spreadsheet::CCellStyles& oCellStyles);
		void WriteCellStyle(const OOX::Spreadsheet::CCellStyle& oCellStyle);
		void WriteDxfs(const OOX::Spreadsheet::CDxfs& oDxfs, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteDxf(const OOX::Spreadsheet::CDxf& oDxf, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteTableStyles(const OOX::Spreadsheet::CTableStyles& oTableStyles);
		void WriteTableCustomStyles(const std::vector<OOX::Spreadsheet::CTableStyle *>& aTableStyles);
		void WriteTableStyle(const OOX::Spreadsheet::CTableStyle& oTableStyle);
		void WriteTableStyleElements(const std::vector<OOX::Spreadsheet::CTableStyleElement *>& aTableStyles);
		void WriteTableStyleElement(const OOX::Spreadsheet::CTableStyleElement& oTableStyleElement);
	};
	class BinarySharedStringTableWriter
	{
		BinaryCommonWriter m_oBcw;
		NSFontCutter::CEmbeddedFontsManager* m_pEmbeddedFontsManager;
	public:
		BinarySharedStringTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager);
		void Write(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteSharedStrings(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteSharedString(OOX::Spreadsheet::CSi& si, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteRun(OOX::Spreadsheet::CRun& run, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
		void WriteRPr(const OOX::Spreadsheet::CRPr& rPr, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor);
	};
	class BinaryWorkbookTableWriter
	{
		BinaryCommonWriter		m_oBcw;
		OOX::Spreadsheet::CXlsx *m_pXlsx;
		OOX::Spreadsheet::CXlsxFlat *m_pXlsxFlat;
	public:
		BinaryWorkbookTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, OOX::Document *pDocument);
		
		void Write(OOX::Spreadsheet::CWorkbook& workbook);
		void WriteWorkbook(OOX::Spreadsheet::CWorkbook& workbook);
		void WriteWorkbookPr(const OOX::Spreadsheet::CWorkbookPr& workbookPr);
		void WriteProtection(const OOX::Spreadsheet::CWorkbookProtection& protection);
		void WriteBookViews(const OOX::Spreadsheet::CBookViews& bookViews);
		void WriteWorkbookView(const OOX::Spreadsheet::CWorkbookView& workbookView);
		void WriteDefinedNames(const OOX::Spreadsheet::CDefinedNames& definedNames);
        void WriteCalcPr(const OOX::Spreadsheet::CCalcPr& CCalcPr);
		void WriteConnections(const OOX::Spreadsheet::CConnections& connections);
		void WriteConnection(const OOX::Spreadsheet::CConnection& connection);
		void WriteConnectionDbPr(const OOX::Spreadsheet::CDbPr& dbPr);
		void WriteConnectionOlapPr(const OOX::Spreadsheet::COlapPr& olapPr);
		void WriteConnectionTextPr(const OOX::Spreadsheet::CTextPr& textPr);
		void WriteConnectionTextFields(const OOX::Spreadsheet::CTextFields& textFields);
		void WriteConnectionTextField(const OOX::Spreadsheet::CTextField& textField);
		void WriteConnectionWebPr(const OOX::Spreadsheet::CWebPr& webPr);
		void WriteConnectionRangePr(const OOX::Spreadsheet::CRangePr& rangePr);
		void WriteExternalReferences(const OOX::Spreadsheet::CExternalReferences& externalReferences, OOX::Spreadsheet::CWorkbook& workbook);
		void WriteExternalBook(const OOX::Spreadsheet::CExternalBook& externalBook, const std::wstring& sLink);
		void WriteExternalSheetNames(const OOX::Spreadsheet::CExternalSheetNames& sheetNames);
		void WriteExternalDefinedNames(const OOX::Spreadsheet::CExternalDefinedNames& definedNames);
		void WriteExternalDefinedName(const OOX::Spreadsheet::CExternalDefinedName& definedName);
		void WriteExternalSheetDataSet(const OOX::Spreadsheet::CExternalSheetDataSet& sheetDataSet);
		void WriteExternalSheetData(const OOX::Spreadsheet::CExternalSheetData& sheetData);
		void WriteExternalRow(const OOX::Spreadsheet::CExternalRow& row);
		void WriteExternalCell(const OOX::Spreadsheet::CExternalCell& cell);
		void WriteOleLink(const OOX::Spreadsheet::COleLink& oleLink, const std::wstring& sLink);
		void WriteOleItem(const OOX::Spreadsheet::COleItem& oleItem);
		void WriteDdeLink(const OOX::Spreadsheet::CDdeLink& ddeLink);
		void WriteDdeItem(const OOX::Spreadsheet::CDdeItem& ddeItem);
		void WriteDdeValues(const OOX::Spreadsheet::CDdeValues& ddeValues);
		void WriteDdeValue(const OOX::Spreadsheet::CDdeValue& ddeValue);
		void WriteDefinedName(const OOX::Spreadsheet::CDefinedName& definedName);
		void WriteSlicerCaches(OOX::Spreadsheet::CWorkbook& workbook, const OOX::Spreadsheet::CSlicerCaches& oSlicerCaches);
	};
	class BinaryPersonTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryPersonTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream);
		void Write(OOX::Spreadsheet::CPersonList& oPersonList);
		void WritePersonList(OOX::Spreadsheet::CPersonList& oPersonList);
		void WritePerson(OOX::Spreadsheet::CPerson& oPerson);
	};
	class BinaryWorksheetTableWriter
	{
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
		OOX::Spreadsheet::CIndexedColors*		m_pIndexedColors;
		PPTX::Theme*							m_pTheme;
		DocWrapper::FontProcessor&				m_oFontProcessor;
		NSBinPptxRW::CDrawingConverter*			m_pOfficeDrawingConverter;
	public:
		BinaryWorksheetTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		
		void Write(OOX::Spreadsheet::CWorkbook& workbook, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets);
		void Write(OOX::Spreadsheet::CWorkbook& workbook, std::vector<OOX::Spreadsheet::CWorksheet*>& arWorksheets);
		
		void WriteWorksheets(OOX::Spreadsheet::CWorkbook& workbook, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets);
	
		void WriteWorksheet(OOX::Spreadsheet::CSheet* pSheet, OOX::Spreadsheet::CWorksheet& oWorksheet);
		
		void WriteCellWatches(const OOX::Spreadsheet::CCellWatches& cellWatches);
		void WriteCellWatch(const OOX::Spreadsheet::CCellWatch& cellWatch);
		void WriteWorksheetProp(OOX::Spreadsheet::CSheet& oSheet);
		void WriteProtection(const OOX::Spreadsheet::CSheetProtection& protection);
		void WriteProtectedRanges(const OOX::Spreadsheet::CProtectedRanges& protectedRanges);
		void WriteProtectedRange(const OOX::Spreadsheet::CProtectedRange& protectedRanges);
		void WriteCols(const OOX::Spreadsheet::CCols& oCols);
		void WriteCol(const OOX::Spreadsheet::CCol& oCol);
		void WriteSheetViews(const OOX::Spreadsheet::CSheetViews& oSheetViews);
		void WriteSheetView(const OOX::Spreadsheet::CSheetView& oSheetView);
		void WritePane(const OOX::Spreadsheet::CPane& oPane);
		void WriteSelection(const OOX::Spreadsheet::CSelection& oSelection);
		void WriteSheetFormatPr(const OOX::Spreadsheet::CSheetFormatPr& oSheetFormatPr);
		void WritePageMargins(const OOX::Spreadsheet::CPageMargins& oPageMargins);
		void WritePageSetup(const OOX::Spreadsheet::CPageSetup& oPageSetup);
		void WritePrintOptions(const OOX::Spreadsheet::CPrintOptions& oPrintOptions);
		void WriteHyperlinks(const OOX::Spreadsheet::CHyperlinks& oHyperlinks, OOX::Spreadsheet::CWorksheet& oWorksheet);
		void WriteHyperlink(const OOX::Spreadsheet::CHyperlink& oHyperlink, OOX::Spreadsheet::CWorksheet& oWorksheet);
		void WriteMergeCells(const OOX::Spreadsheet::CMergeCells& oMergeCells);
		void WriteSheetData(const OOX::Spreadsheet::CSheetData& oSheetData);
		void WriteRow(const OOX::Spreadsheet::CRow& oRows);
		void WriteCells(const OOX::Spreadsheet::CRow& oRows);
		void WriteCell(const OOX::Spreadsheet::CCell& oCell);
		void WriteFormula(OOX::Spreadsheet::CFormula& oFormula);
        
		void WriteDrawings(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::CVmlDrawing *pVmlDrawing = NULL);
        void WriteDrawing(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::Spreadsheet::CCellAnchor* pCellAnchor, OOX::CVmlDrawing *pVmlDrawing = NULL, OOX::Spreadsheet::COleObject* pOleObject = NULL);
		void WriteCellAnchor(OOX::Spreadsheet::CCellAnchor* pCellAnchor);
		void WriteOleObjects(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::CVmlDrawing *pVmlDrawing = NULL);
		void WriteControls(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::CVmlDrawing *pVmlDrawing = NULL);
		void WriteControlPr(OOX::Spreadsheet::CControlPr* pControlPr, OOX::Spreadsheet::CFormControlPr* pFormControlPr);

		void WriteLegacyDrawingHF(const OOX::Spreadsheet::CWorksheet& oWorksheet);
		void WriteLegacyDrawingHFDrawings(OOX::CVmlDrawing* pVmlDrawing);
		void WriteLegacyDrawingHFDrawing(const OOX::CVmlDrawing::_vml_shape& oVmlShape);
		
		void WriteFromTo(const OOX::Spreadsheet::CFromTo& oFromTo);
		void WritePos(const OOX::Spreadsheet::CPos& oPos);
		void WriteExt(const OOX::Spreadsheet::CExt& oExt);
		void WriteClientData(const OOX::Spreadsheet::CClientData& oClientData);
        void WriteComments(std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>& mapComments);
		void getSavedComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aDatas);
		void WriteComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas);
		void WriteCommentData(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas);
		void WriteCommentDataContent(OOX::Spreadsheet::CCommentItem* pComment, SerializeCommon::CommentData* pCommentData, bool isThreadedComment);
		void WriteCommentReplies(std::vector<SerializeCommon::CommentData*>& aReplies, bool isThreadedComment);
		void WriteThreadedComment(OOX::Spreadsheet::CThreadedComment& oThreadedComment, bool bThreadedCommentCopy);
		void WriteThreadedCommentMention(OOX::Spreadsheet::CThreadedCommentMention& oMention);
		void WriteSheetPr(const OOX::Spreadsheet::CSheetPr& oSheetPr);
		void WritePageSetUpPr(const OOX::Spreadsheet::CPageSetUpPr& oPageSetUpPr);
		void WriteOutlinePr(const OOX::Spreadsheet::COutlinePr& oOutlinePr);
		void WritemHeaderFooter(const OOX::Spreadsheet::CHeaderFooter& oHeaderFooter);
		void WritemRowColBreaks(const OOX::Spreadsheet::CRowColBreaks& oRowColBreaks);		
		void WritemBreak(const OOX::Spreadsheet::CBreak& oBreak);
		void WriteConditionalFormattings(std::vector<OOX::Spreadsheet::CConditionalFormatting*>& arrConditionalFormatting,
			std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt);
		void WriteConditionalFormatting(const OOX::Spreadsheet::CConditionalFormatting& oConditionalFormatting,
			std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt);
		void WriteConditionalFormattingRule(const OOX::Spreadsheet::CConditionalFormattingRule& oConditionalFormattingRule,
			std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt);
		void WriteColorScale(const OOX::Spreadsheet::CColorScale& oColorScale);
		void WriteDataBar(const OOX::Spreadsheet::CDataBar& oDataBar);
		void WriteIconSet(const OOX::Spreadsheet::CIconSet& oIconSet);
		void WriteCFIcon(const OOX::Spreadsheet::CConditionalFormatIconSet& oCFIcon);
		void WriteCFVO(const OOX::Spreadsheet::CConditionalFormatValueObject& oCFVO);
        void WriteSparklineGroups(const OOX::Spreadsheet::CSparklineGroups& oSparklineGroups);
        void WriteSparklineGroup(const OOX::Spreadsheet::CSparklineGroup& oSparklineGroup);
        void WriteSparklines(const OOX::Spreadsheet::CSparklines& oSparklines);
        void WriteSparkline(const OOX::Spreadsheet::CSparkline& oSparkline);
		void WriteDataValidations(const OOX::Spreadsheet::CDataValidations& oDataValidations);
		void WriteDataValidationsContent(const OOX::Spreadsheet::CDataValidations& oDataValidations);
		void WriteDataValidation(const OOX::Spreadsheet::CDataValidation& oDataValidation);
		void WriteSlicers(OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CSlicerRefs& oSlicers);
	};
	class BinaryCustomsTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryCustomsTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream);
		void Write(OOX::IFileContainer *pContainer);
	};
	class BinaryCalcChainTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryCalcChainTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream);
		void Write(OOX::Spreadsheet::CCalcChain& pCalcChain);
		void WriteCalcChainTableContent(OOX::Spreadsheet::CCalcChain& pCalcChain);
		void WriteCalcChain(OOX::Spreadsheet::CCalcCell& oCalcCell);
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
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
		PPTX::Theme*							m_pTheme;
		NSBinPptxRW::CDrawingConverter*			m_pOfficeDrawingConverter;
	public:
		BinaryOtherTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, PPTX::Theme* pTheme, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		void Write();
		void WriteOtherTableContent();
	};
	class BinaryFileWriter 
	{
	private:
		BinaryCommonWriter* m_oBcw;
		int m_nLastFilePos;
		int m_nRealTableCount;
		int m_nMainTableStart;
		DocWrapper::FontProcessor& m_oFontProcessor;
	public:
		BinaryFileWriter(DocWrapper::FontProcessor& oFontProcessor);
		~BinaryFileWriter();
        _UINT32 Open(const std::wstring& sInputDir, const std::wstring& sFileDst, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager,
            NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bIsNoBase64);
		
		void WriteMainTableStart(NSBinPptxRW::CBinaryFileWriter &oBufferedStream);
		void WriteContent(OOX::Document *pDocument, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		void WriteMainTableEnd();

		void WriteBinaryTable(const BYTE* pBuffer, size_t size);
		//void intoBindoc(OOX::Document *pDocument, NSBinPptxRW::CBinaryFileWriter &oBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
       
		std::wstring WriteFileHeader(int nDataSize, int version);
		int GetMainTableSize();

		int m_nLastFilePosOffset;
	private:
		int WriteTableStart(BYTE type, int nStartPos = -1);
		void WriteTableEnd(int nCurPos);
	};
}
