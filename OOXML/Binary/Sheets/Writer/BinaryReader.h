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
#include <unordered_map>

#include "../../../../DesktopEditor/common/StreamWriter.h"

#include "../Common/BinReaderWriterDefines.h"
#include "../Common/Common.h"
#include "../Reader/ChartFromToBinary.h"

#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../XlsxFormat/Table/QueryTable.h"

#include "../../../XlsxFormat/Pivot/PivotTable.h"
#include "../../../XlsxFormat/Pivot/PivotCacheDefinition.h"
#include "../../../XlsxFormat/Pivot/PivotCacheRecords.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPersonList;
		class CPerson;
		class CThreadedComment;
		class CDxf;
		class CSortCondition;
		class CFilterColumn;
	}
}

namespace BinXlsxRW 
{
	class ImageObject
	{
	public:
        std::wstring sPath;
		int nIndex;
        std::map<OOX::Spreadsheet::CDrawing*, std::wstring> mapDrawings;

        ImageObject()
		{
		}
        ImageObject(std::wstring& _sPath, int _nIndex)
		{
			sPath = _sPath;
			nIndex = _nIndex;
		}
	};
	class PivotCachesTemp
	{
	public:
		long nId;
		BYTE* pDefinitionData;
		long nDefinitionLength;
		OOX::Spreadsheet::CPivotCacheRecordsFile* pRecords;
		long nCacheId;
		OOX::Spreadsheet::CPivotTableFile* pTable;

        PivotCachesTemp()
		{
			nId = -1;
			pDefinitionData = NULL;
			nDefinitionLength = 0;
			pRecords = NULL;
			nCacheId = -1;
			pTable = NULL;
		}
	};

	class Binary_CommonReader2
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_poBufferedStream;
	public:
		Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream);
		int ReadColor(BYTE type, long length, void* poResult);
	};
	class BinaryTableReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CWorksheet* m_pCurWorksheet;
		public:
		BinaryTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorksheet* pCurWorksheet);
		int Read(long length, OOX::Spreadsheet::CTableParts* pTableParts);
		int ReadQueryTable(long length, OOX::Spreadsheet::CQueryTable* pQueryTable);
		
		int ReadTablePart(BYTE type, long length, void* poResult);
		int ReadTable(BYTE type, long length, void* poResult);
		int ReadAltTextTable(BYTE type, long length, void* poResult);
		int ReadAutoFilter(BYTE type, long length, void* poResult);
		int ReadFilterColumns(BYTE type, long length, void* poResult);
		int ReadFilterColumn(BYTE type, long length, void* poResult);
		int ReadFilterColumnExternal(OOX::Spreadsheet::CFilterColumn* pFilterColumn);
		int ReadFilterFilters(BYTE type, long length, void* poResult);
		int ReadFilterFilter(BYTE type, long length, void* poResult);
		int ReadDateGroupItem(BYTE type, long length, void* poResult);
		int ReadCustomFilters(BYTE type, long length, void* poResult);
		int ReadCustomFilter(BYTE type, long length, void* poResult);
		int ReadCustomFiltersItem(BYTE type, long length, void* poResult);
		int ReadDynamicFilter(BYTE type, long length, void* poResult);
		int ReadColorFilter(BYTE type, long length, void* poResult);
		int ReadTop10(BYTE type, long length, void* poResult);
		int ReadSortState(BYTE type, long length, void* poResult);
		int ReadSortConditions(BYTE type, long length, void* poResult);
		int ReadSortCondition(BYTE type, long length, void* poResult);
		int ReadSortConditionExternal(OOX::Spreadsheet::CSortCondition* pSortCondition);
		int ReadTableColumns(BYTE type, long length, void* poResult);
		int ReadTableColumn(BYTE type, long length, void* poResult);
		int ReadTableStyleInfo(BYTE type, long length, void* poResult);
		int ReadQueryTableContent(BYTE type, long length, void* poResult);
		int ReadQueryTableRefresh(BYTE type, long length, void* poResult);
		int ReadQueryTableFields(BYTE type, long length, void* poResult);
		int ReadQueryTableField(BYTE type, long length, void* poResult);
		int ReadQueryTableDeletedFields(BYTE type, long length, void* poResult);
		int ReadQueryTableDeletedField(BYTE type, long length, void* poResult);
	};
	class BinarySharedStringTableReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CSharedStrings& m_oSharedStrings;
		Binary_CommonReader2 m_oBcr;
	public:
		BinarySharedStringTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CSharedStrings& oSharedStrings);
		int Read();
		int ReadSharedStringTableContent(BYTE type, long length, void* poResult);
		int ReadSi(BYTE type, long length, void* poResult);
		int ReadRun(BYTE type, long length, void* poResult);
		int ReadRPr(BYTE type, long length, void* poResult);
		int ReadColor(BYTE type, long length, void* poResult);
	};
	class BinaryStyleTableReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CStyles& m_oStyles;
		Binary_CommonReader2 m_oBcr;
	public:
		BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CStyles& oStyles);
		int Read();
		int ReadStyleTableContent(BYTE type, long length, void* poResult);
		int ReadBorders(BYTE type, long length, void* poResult);
		int ReadBorder(BYTE type, long length, void* poResult);
		int ReadBorderProp(BYTE type, long length, void* poResult);
		int ReadColor(BYTE type, long length, void* poResult);
		int ReadFills(BYTE type, long length, void* poResult);
		int ReadFill(BYTE type, long length, void* poResult);
		int ReadPatternFill(BYTE type, long length, void* poResult);
		int ReadGradientFill(BYTE type, long length, void* poResult);
		int ReadGradientFillStop(BYTE type, long length, void* poResult);
		int ReadFonts(BYTE type, long length, void* poResult);
		int ReadFont(BYTE type, long length, void* poResult);
		int ReadNumFmts(BYTE type, long length, void* poResult);
		int ReadNumFmt(BYTE type, long length, void* poResult);
		int ReadCellStyleXfs(BYTE type, long length, void* poResult);
		int ReadCellXfs(BYTE type, long length, void* poResult);
		int ReadXfs(BYTE type, long length, void* poResult);
		int ReadAligment(BYTE type, long length, void* poResult);
		int ReadProtection(BYTE type, long length, void* poResult);
		int ReadDxfs(BYTE type, long length, void* poResult);
		int ReadDxf(BYTE type, long length, void* poResult);
		int ReadDxfExternal(OOX::Spreadsheet::CDxf* pDxf);
		int ReadCellStyles(BYTE type, long length, void* poResult);
		int ReadCellStyle(BYTE type, long length, void* poResult);
		int ReadTableStyles(BYTE type, long length, void* poResult);
		int ReadTableCustomStyles(BYTE type, long length, void* poResult);
		int ReadTableCustomStyle(BYTE type, long length, void* poResult);
		int ReadTableCustomStyleElements(BYTE type, long length, void* poResult);
		int ReadTableCustomStyleElement(BYTE type, long length, void* poResult);
	};
	class BinaryWorkbookTableReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CWorkbook									& m_oWorkbook;
        boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>	& m_mapPivotCacheDefinitions;
        const std::wstring											& m_sDestinationDir;
		
		NSBinPptxRW::CDrawingConverter *m_pOfficeDrawingConverter = NULL;
		bool m_bMacroRead = false;
	public:
        BinaryWorkbookTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook, boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>& mapPivotCacheDefinitions, const std::wstring& sDestinationDir, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		int Read();
		int ReadWorkbookTableContent(BYTE type, long length, void* poResult);
		int ReadWorkbookPr(BYTE type, long length, void* poResult);
		int ReadProtection(BYTE type, long length, void* poResult);
		int ReadBookViews(BYTE type, long length, void* poResult);
		int ReadWorkbookView(BYTE type, long length, void* poResult);
		int ReadExternalReferences(BYTE type, long length, void* poResult);
		int ReadExternalReference(BYTE type, long length, void* poResult);
		int ReadDefinedNames(BYTE type, long length, void* poResult);
		int ReadDefinedName(BYTE type, long length, void* poResult);
		int ReadCalcPr(BYTE type, long length, void* poResult);
		int ReadExternalBook(BYTE type, long length, void* poResult);
		int ReadExternalSheetNames(BYTE type, long length, void* poResult);
		int ReadExternalDefinedNames(BYTE type, long length, void* poResult);
		int ReadExternalDefinedName(BYTE type, long length, void* poResult);
		int ReadExternalSheetDataSet(BYTE type, long length, void* poResult);
		int ReadExternalSheetData(BYTE type, long length, void* poResult);
		int ReadExternalRow(BYTE type, long length, void* poResult);
		int ReadExternalCell(BYTE type, long length, void* poResult);
		int ReadOleLink(BYTE type, long length, void* poResult);
		int ReadOleItem(BYTE type, long length, void* poResult);
		int ReadDdeLink(BYTE type, long length, void* poResult);
		int ReadDdeItem(BYTE type, long length, void* poResult);
		int ReadDdeValues(BYTE type, long length, void* poResult);
		int ReadDdeValue(BYTE type, long length, void* poResult);
		int ReadPivotCaches(BYTE type, long length, void* poResult);
		int ReadPivotCache(BYTE type, long length, void* poResult);
		int ReadConnections(BYTE type, long length, void* poResult);
		int ReadConnection(BYTE type, long length, void* poResult);
		int ReadConnectionDbPr(BYTE type, long length, void* poResult);
		int ReadConnectionOlapPr(BYTE type, long length, void* poResult);
		int ReadConnectionTextPr(BYTE type, long length, void* poResult);
		int ReadConnectionWebPr(BYTE type, long length, void* poResult);
		int ReadConnectionRangePr(BYTE type, long length, void* poResult);
		int ReadConnectionTextFields(BYTE type, long length, void* poResult);
		int ReadConnectionTextField(BYTE type, long length, void* poResult);
		int ReadSlicerCaches(BYTE type, long length, void* poResult);
	};
	class BinaryCommentReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CWorksheet* m_pCurWorksheet;
	public:
		BinaryCommentReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorksheet* pCurWorksheet);
		int Read(long length, void* poResult);
		int ReadExternal(long length, void* poResult);
		int ReadCommentDatasExternal(BYTE type, long length, void* poResult);
		int ReadComments(BYTE type, long length, void* poResult);
		int ReadComment(BYTE type, long length, void* poResult);
		int ReadCommentDatas(BYTE type, long length, void* poResult);
		int ReadCommentData(BYTE type, long length, void* poResult);
		int ReadCommentReplies(BYTE type, long length, void* poResult);
		int ReadThreadedComment(BYTE type, long length, void* poResult);
		int ReadThreadedCommentMention(BYTE type, long length, void* poResult);
		void parseCommentData(SerializeCommon::CommentData* pCommentData, OOX::Spreadsheet::CSi& oSi);
		static int addCommentRun(OOX::Spreadsheet::CSi& oSi, const std::wstring& text, bool isBold, int nLimit);
		static void addThreadedComment(OOX::Spreadsheet::CSi& oSi, OOX::Spreadsheet::CThreadedComment* pThreadedComment, nullable<std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>>& mapPersonList);
		static std::wstring getThreadedCommentAuthor(nullable<std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>>& mapPersonList, nullable<SimpleTypes::CGuid>& personId, const std::wstring& sDefault);
	};
	class BinaryWorksheetsTableReader : public Binary_CommonReader
	{
		Binary_CommonReader2				m_oBcr2;
		NSFile::CStreamWriter*		m_pCurStreamWriter;
		NSBinPptxRW::CDrawingConverter*		m_pOfficeDrawingConverter;

        OOX::Spreadsheet::CWorkbook&					m_oWorkbook;
        OOX::Spreadsheet::CSharedStrings*				m_pSharedStrings;
        boost::unordered_map<long, ImageObject*>&		m_mapMedia;
       
		NSCommon::smart_ptr<OOX::Spreadsheet::CSheet>		m_pCurSheet;
        NSCommon::smart_ptr<OOX::Spreadsheet::CWorksheet>	m_pCurWorksheet;
        NSCommon::smart_ptr<OOX::Spreadsheet::CDrawing>		m_pCurDrawing;
		NSCommon::smart_ptr<OOX::Spreadsheet::COleObjects>	m_pCurOleObjects;
        NSCommon::smart_ptr<OOX::CVmlDrawing>				m_pCurVmlDrawing;
		
        unsigned int						m_lObjectIdVML;

        const std::wstring&					m_sDestinationDir;
        const std::wstring&					m_sMediaDir;
		SaveParams&							m_oSaveParams;
		int									m_nNextObjectId;

		std::vector<OOX::Spreadsheet::CWorksheet*>&					m_arWorksheets;
		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>&		m_mapWorksheets; // for fast find 

        boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>&	m_mapPivotCacheDefinitions;
	public:
		BinaryWorksheetsTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook,
			OOX::Spreadsheet::CSharedStrings* pSharedStrings, std::vector<OOX::Spreadsheet::CWorksheet*>& arWorksheets, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets,
            boost::unordered_map<long, ImageObject*>& mapMedia, const std::wstring& sDestinationDir, const std::wstring& sMediaDir, SaveParams& oSaveParams,
            NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>& mapPivotCacheDefinitions);
		int Read();
		int ReadWorksheetsTableContent(BYTE type, long length, void* poResult);
		int ReadWorksheetSeekPositions(BYTE type, long length, void* poResult);
		int ReadWorksheet(boost::unordered_map<BYTE, std::vector<unsigned int>>& mapPos, NSFile::CStreamWriter& oStreamWriter, void* poResult);
		int ReadPivotTable(BYTE type, long length, void* poResult);
		int ReadWorksheetProp(BYTE type, long length, void* poResult);
		int ReadWorksheetCols(BYTE type, long length, void* poResult);
		int ReadWorksheetCol(BYTE type, long length, void* poResult);
		int ReadProtectedRanges(BYTE type, long length, void* poResult);
		int ReadProtectedRange(BYTE type, long length, void* poResult);
		int ReadSheetViews(BYTE type, long length, void* poResult);
		int ReadSheetView(BYTE type, long length, void* poResult);
		int ReadPane(BYTE type, long length, void* poResult);
		int ReadSelection(BYTE type, long length, void* poResult);
		int ReadSheetPr(BYTE type, long length, void* poResult);
		int ReadOutlinePr(BYTE type, long length, void* poResult);
		int ReadPageSetUpPr(BYTE type, long length, void* poResult);
		int ReadColor(BYTE type, long length, void* poResult);
		int ReadSheetFormatPr(BYTE type, long length, void* poResult);
		int ReadPageMargins(BYTE type, long length, void* poResult);
		int ReadPageSetup(BYTE type, long length, void* poResult);
		int ReadHeaderFooter(BYTE type, long length, void* poResult);
		int ReadProtection(BYTE type, long length, void* poResult);
		int ReadRowColBreaks(BYTE type, long length, void* poResult);
		int ReadBreak(BYTE type, long length, void* poResult);
		int ReadCellWatches(BYTE type, long length, void* poResult);
		int ReadCellWatch(BYTE type, long length, void* poResult);
		int ReadPrintOptions(BYTE type, long length, void* poResult);
		int ReadHyperlinks(BYTE type, long length, void* poResult);
		int ReadHyperlink(BYTE type, long length, void* poResult);
		int ReadMergeCells(BYTE type, long length, void* poResult);
		int ReadDrawings(BYTE type, long length, void* poResult);
		int ReadDrawing(BYTE type, long length, void* poResult);
		int ReadCellAnchor(BYTE type, long length, void* poResult);
		int ReadLegacyDrawingHF(BYTE type, long length, void* poResult);
		int ReadLegacyDrawingHFDrawings(BYTE type, long length, void* poResult);
		int ReadLegacyDrawingHFDrawing(BYTE type, long length, void* poResult);
		int ReadControls(BYTE type, long length, void* poResult);
		int ReadControl(BYTE type, long length, void* poResult);
		int ReadControlItems(BYTE type, long length, void* poResult);
		int ReadFromTo(BYTE type, long length, void* poResult);
		int ReadExt(BYTE type, long length, void* poResult);
		int ReadPos(BYTE type, long length, void* poResult);
		int ReadClientData(BYTE type, long length, void* poResult);
		int ReadSheetData(BYTE type, long length, void* poResult);
		int ReadRow(BYTE type, long length, void* poResult);
		int ReadCells(BYTE type, long length, void* poResult);
		int ReadCell(BYTE type, long length, void* poResult);
		int ReadFormula(BYTE type, long length, void* poResult);
		int ReadConditionalFormatting(BYTE type, long length, void* poResult);
		int ReadConditionalFormattingRule(BYTE type, long length, void* poResult);
		int ReadColorScale(BYTE type, long length, void* poResult);
		int ReadDataBar(BYTE type, long length, void* poResult);
		int ReadIconSet(BYTE type, long length, void* poResult);
		int ReadCFIcon(BYTE type, long length, void* poResult);
		int ReadCFVO(BYTE type, long length, void* poResult);
		int ReadSparklineGroups(BYTE type, long length, void* poResult);
        int ReadSparklineGroup(BYTE type, long length, void* poResult);
        int ReadSparklines(BYTE type, long length, void* poResult);
        int ReadSparkline(BYTE type, long length, void* poResult);
        int ReadDataValidations(BYTE type, long length, void* poResult);
		int ReadDataValidationsContent(BYTE type, long length, void* poResult);
        int ReadDataValidation(BYTE type, long length, void* poResult);
		int ReadSlicers(BYTE type, long length, void* poResult);
		
		void WriteComments();
		void AddLineBreak(OOX::Spreadsheet::CSi& oSi);
		std::wstring GetControlVmlShape(void* pControl);
	};
	class BinaryOtherTableReader : public Binary_CommonReader
	{
        boost::unordered_map<long, ImageObject*>&	m_mapMedia;
        const std::wstring&                         m_sFileInDir;
        long                                        m_nCurId;
        std::wstring                                m_sCurSrc;
        long                                        m_nCurIndex;
        SaveParams&                                 m_oSaveParams;
        NSBinPptxRW::CDrawingConverter*             m_pOfficeDrawingConverter;
        const std::wstring&                         m_sMediaDir;
	public:
        BinaryOtherTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, boost::unordered_map<long, ImageObject*>& mapMedia,
                               const std::wstring& sFileInDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter,
                               const std::wstring& sMediaDir);
		int Read();
		int ReadOtherTableContent(BYTE type, long length, void* poResult);
		int ReadMediaContent(BYTE type, long length, void* poResult);
		int ReadMediaItem(BYTE type, long length, void* poResult);
        std::wstring ReadMediaItemSaveFileGetNewPath(const std::wstring& sTempPath);
		void ReadMediaItemSaveFileFILE(FILE* pFile);
        void ReadMediaItemSaveFilePath(const std::wstring& sTempPath);
	};
	class BinaryPersonReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CWorkbook& m_oWorkbook;
	public:
		BinaryPersonReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook);
		int Read();
		int ReadPersonList(BYTE type, long length, void* poResult);
		int ReadPerson(BYTE type, long length, void* poResult);
	};
	class BinaryCustomsReader : public Binary_CommonReader
	{
		OOX::Spreadsheet::CWorkbook* m_pWorkbook;
	public:
		BinaryCustomsReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook* pWorkbook);
		int Read();
		int ReadCustom(BYTE type, long length, void* poResult);
		int ReadCustomContent(BYTE type, long length, void* poResult);
	};
	class BinaryFileReader
	{
	public: 
		BinaryFileReader();
        int ReadFile(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bMacro = false);
        int ReadMainTable(OOX::Spreadsheet::CXlsx& oXlsx, NSBinPptxRW::CBinaryFileReader& oBufferedStream, const std::wstring& sFileInDir, const std::wstring& sOutDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		void initWorkbook(OOX::Spreadsheet::CWorkbook* pWorkbook);
		
		int Xml2Xlsx(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bMacro = false);
	};
}
