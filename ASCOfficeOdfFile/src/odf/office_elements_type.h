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

namespace cpdoccore { 
namespace odf_reader {

enum ElementType
{
    typeNone = 0,
    type_AbstractXml,

    typeTextText = 1000,
    typeTextS,
    typeTextTab,
    typeTextLineBreak,
    typeTextBookmark,
    typeTextBookmarkStart,
    typeTextBookmarkEnd,
    typeTextReferenceMark,
    typeTextReferenceMarkStart,
    typeTextReferenceMarkEnd,
    typeTextSpan,
    typeTextA,
    typeTextNote,
    typeTextNoteCitation,
    typeTextNoteBody,
    typeTextRuby,
    typeTextRubyBase,
    typeTextRubyText,
    typeTextOfficeAnnotation,
    typeTextTitle,
	typeTextChapter,
	typeTextSubject,
    typeTextPlaceholder,
    typeTextPageNumber,
    typeTextPageCount,
    typeTextDate,
    typeTextModificationDate,
    typeTextTime,
    typeTextModificationTime,
    typeTextFileName,
    typeTextSequence,
    typeTextSheetName,

	typePresentationFooter,
	typePresentationDateTime,

	typeDcCreator,
	typeDcDate,

    typeTextH,
    typeTextP,

    typeTextList,
    typeTextListItem,
    typeTextListHeader,
    typeTextNumber,
    typeTextSoftPageBreak,
    typeTextSection,
    typeTextSectionSource,
    typeTextIndexTitle,
    typeTextIndexBody,

    typeTextNumberedParagraph,    
   
    typeDrawA,

    typeTextTableOfContent,
    typeTextIllustrationIndex,
    typeTextTableIndex,
    typeTextObjectIndex,
    typeTextUserIndex,
    typeTextAlphabeticalIndex,
    typeTextBibliography,

	typeTextBibliographyMark,
  	
    typeTextTableOfContentSource,
    typeTextIllustrationIndexSource,
    typeTextTableIndexSource,
    typeTextObjectIndexSource,
    typeTextUserIndexSource,
    typeTextAlphabeticalIndexSource,
    typeTextBibliographySource,

	typeTextTrackedChanges,
	typeTextChangedRegion,
	typeTextChangeStart,
	typeTextChangeEnd,
	typeTextChange,
	typeOfficeChangeInfo,
	
	typeTextUnknownChange,
	typeTextDeletion,
	typeTextFormatChange,
	typeTextInsertion,

	typeShape,
    typeChangeMarks,

    typeOfficeAutomaticStyles,
    typeOfficeMasterStyles,
    typeStyleMasterPage,
    typeOfficeStyles,
    typeStyleDefaultStyle,
    typeStyleStyle,
    typeTextListStyle,
    typeTextListLevelStyleNumber,
    typeTextListLevelStyleBullet,
	typeTextListLevelStyleImage,
	typeStyleListLevelProperties,
    typeStyleListLevelLabelAlignment,

    typeStyleTabStop,
    typeStyleTabStops,
    typeStyleDropCap,
    typeStyleBackgroundImage,
    typeStyleTextProperties,
    typeStyleParagraphProperties,
    typeStyleTableProperties,
    typeStyleTableColumnProperties,
    typeStyleTableRowProperties,
    typeStyleTableCellProperties,
    typeStyleHeader,
    typeStyleFooter,
    typeStyleHeaderFirst,
    typeStyleFooterFirst,
	typeStyleHeaderLeft,
    typeStyleFooterLeft,
    typeStyleFootnoteSep,

    typeStyleSectionProperties,
    typeStyleHeaderStyle,
    typeStyleFooterStyle,
	typeStyleHeaderFooterProperties,

	typeStylePresentationSound,
	typeStylePresentationPageLayout,
	typeStylePresentationPlaceholder,
	typeStyleDrawingPageProperties,

    typeStyleColumns,
    typeStyleColumn,
    typeStyleColumnSep,

    typeStylePageLayout,
    typeStyleMap,

	typeTableTemplate,
	typeTableTemplateElement,
	typeTableBodyTemplate,
	typeTableFirstRowTemplate,
	typeTableLastRowTemplate,
	typeTableFirstColumnTemplate,
	typeTableLastColumnTemplate,
	typeTableOddColumnsTemplate,
	typeTableOddRowsTemplate,

    typeTextNotesConfiguration,

    typeStyleFontFace,

    typeSvgFontFaceUri,
    typeSvgFontFaceFormat,
    typeSvgFontFaceName,
    typeSvgDefinitionSrc,
    typeOfficeFontFaceDecls,

    typeOfficeBinaryData,

    typeTableNamedExpressions,
    typeTableNamedExpression,
    typeTableNamedRange,
	typeTableDatabaseRanges,
	typeTableDatabaseRange,
	typeTableSort,
	typeTableSortBy,

	typeTableFilter,
	typeTableFilterAnd,
	typeTableFilterOr,
	typeTableFilterCondition,
	typeTableFilterSetItem,

	typeTableTable,
    typeTableTableColumn,
    typeTableTableColumns,
    typeTableTableHeaderColumns,
    typeTableTableColumnGroup,
    typeTableTableRow,
    typeTableTableCell,
    typeTableCoveredTableCell,
    typeTableTableRows,
    typeTableTableHeaderRows,
    typeTableTableRowGroup,
	typeTableTableRowNoGroup,
    typeTableTableSource,

	typeTableDataPilotTables,
	typeTableDataPilotTable,
	typeTableDataPilotField,
	typeTableDataPilotFieldReference,
	typeTableDatabaseSourceTable,
	typeTableDatabaseSourceQuery,
	typeTableDatabaseSourceSql,
	typeTableSourceCellRange,
	typeTableSourceService,
	typeTableDataPilotSubtotals,
	typeTableDataPilotSubtotal,
	typeTableDataPilotGroupMember,
	typeTableDataPilotMember,
	typeTableDataPilotMembers,
	typeTableDataPilotGroup,
	typeTableDataPilotGroups,
	typeTableDataPilotLevel,
	typeTableDataSortInfo,
	typeTableDataDisplayInfo,
	typeTableDataLayoutInfo,
        
    typeOfficeBody,
    typeOfficeText,
    typeOfficeDocument,
    typeOfficeDocumentContent,
    typeOfficeDocumentStyles,
    typeOfficeDocumentMeta,
    typeOfficeDocumentSettings,

	typeMathElement,
	typeMath,
	typeMathSemantics,
	typeMathAnnotation,
	typeMathAnnotationXml,
	typeMN,
	typeMI,
	typeMS,
	typeMSpace,
	typeMO,
	typeMText,
	typeMGlyph,
	typeMSqrt,
	typeMRoot,
	typeMFrac,
	typeMRow,
	typeMStyle,
	typeMEnClose,
	typeMFenced,
	typeMPadded,
	typeMError,
	typeMPhantom,
	typeMTable,
	typeMAlignMark,
	typeMAlignGroup,
	typeMTd,
	typeMLabelEdTr,
	typeMTr,
	typeMSub,
	typeMSup,
	typeMSubSup,
	typeMNone,
	typeMPreScripts,
	typeMMultiScripts,
	typeMUnderOver,
	typeMOver,
	typeMUnder,
	typeMStack,
	typeMSRow,
	typeMSLine,
	typeMSCarry,
	typeMSCarries,
	typeMSGroup,
	typeMSLongDiv,

	typeManifest,
	typeManifestEntry,
	typeManifestEncryptionData,

	typeOfficeSettings,
	typeOfficeSettingsConfigItemSet,
	typeOfficeSettingsConfigItem,
	typeOfficeSettingsConfigItemMapIndexed,
	typeOfficeSettingsConfigItemMapNamed,
  	typeOfficeSettingsConfigItemMapEntry,

 	typeDrawG,
	typeDrawFrame,
    typeDrawImage,
    typeDrawTextBox,
    typeDrawObject,
    typeDrawObjectOle,
 	typeDrawChart,
 	typeDrawParam,
 	typeDrawPlugin,

	typeDrawShape,

	typeDrawRect,
	typeDrawEllipse,
	typeDrawLine,
	typeDrawPolygon,
	typeDrawContourPolygon,
	typeDrawPolyline,
	typeDrawRegularPolyline,
	typeDrawPath,
	typeDrawCircle,
	typeDrawg,
	typeDrawPageThumbnail,
	typeDrawMeasure,
	typeDrawCaption,
	typeDrawConnector,
	typeDrawControl,
	typeDraw3dScene,
	typeDrawCustomShape,
	typeDrawHandle,
	typeDrawEquation,

	typeDrawPage,    
	typePresentationFooterDecl,
	typePresentationDateTimeDecl,
	typePresentationNotes,

	typeAnimPar,
	typeAnimSeq,
	typeAnimTransitionFilter,
	typeAnimAudio,
	typeAnimCommand,
	typeAnimIterate,

	typeStyleGraphicPropertis,
	typeStyleDrawGradient,
	typeStyleDrawFillImage,
	typeStyleDrawHatch,
	typeStyleDrawMarker,

	typeOfficeSpreadsheet,
	typeOfficeAnnotation,
	typeStyleRegionLeft,
	typeOfficeScripts,
	typeOfficeScript,
	typeOfficePresentation,
	typeOfficeChart,
	typeOfficeEventListeners,
  
	typePresentationEventListener,	

	typeScriptEventListener,

	typeTableCalculationSettings,
    typeTableNullDate,

    typeNumberNumberStyle,
    typeNumberDataStyle,
    typeNumberText,
    typeNumberNumber,
    typeNumberEmbeddedText,
    typeNumberScientificNumber,
    typeNumberFraction,
    typeNumberCurrencyStyle,
    typeNumberCurrencySymbol,
    typeNumberTextContent,
    typeNumberTextStyle,
    typeNumberPercentageStyle,
    typeNumberTimeStyle,
    typeNumberDay,
    typeNumberMonth,
    typeNumberYear,
    typeNumberDayOfWeek,
    typeNumberQuarter,
    typeNumberHours,
    typeNumberMinutes,
    typeNumberSeconds,
    typeNumberAmPm,
    typeTableShapes,

    typeChartChart,
    typeChartTitle,
    typeChartSubtitle,
    typeChartFooter,
    typeChartLegend,
    typeChartPlotArea,
    typeChartWall,
    typeChartFloor,
    typeChartAxis,
    typeChartGrid,
    typeChartCategories,
    typeChartSeries,
    typeChartDomain,
    typeChartDataPoint,
    typeChartErrorIndicator,
    typeChartMeanValue,
    typeChartStockGainMarker,
    typeChartStockLossMarker,
    typeChartStockRangeLine,
	typeChartRegressionCurve,
	typeChartEquation,

	typeStyleRegionCenter,
    typeStyleRegionRight,
	typeStyleChartProperties,

	typeCalcextColorScaleEntry,
	typeCalcextFormattingEntry,
	typeCalcextIconSet,
	typeCalcextDataBar,
	typeCalcextColorScale,
	typeCalcextDateIs,
	typeCalcextCondition,
	typeCalcextConditionalFormat,
	typeCalcextConditionalFormats

};


}
}

