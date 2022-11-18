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
#ifndef _CPDOCCORE_ODF_OFFCIE_ELEMENTS_TYPE_H_
#define _CPDOCCORE_ODF_OFFCIE_ELEMENTS_TYPE_H_

namespace cpdoccore { 
namespace odf_writer {

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
    typeTextPlaceholder,
    typeTextPageNumber,
    typeTextPageCount,
    typeTextDate,
    typeTextTime,
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

    typeTextNumberedParagraph,    
   
    typeDrawA,

    typeTextTableOfContent,
    typeTextIllustrationIndex,
    typeTextTableIndex,
    typeTextObjectIndex,
    typeTextUserIndex,
    typeTextAlphabeticalIndex,
    typeTextBibliography,
    typeShape,
    typeChangeMarks,

    typeOfficeAutomaticStyles,
    typeOfficeMasterStyles,
    typeStyleMasterPage,
    typeStyleHandoutMasterPage,
	typeStyleDrawLayerSet,
	typeStyleDrawLayer,
    typeOfficeStyles,
    typeStyleDefaultStyle,
    typeStyleStyle,
    typeTextListStyle,
	typeTextListLevelStyleImage,
    typeTextListLevelStyleNumber,
    typeTextListLevelStyleBullet,
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

	typeTableBodyTemplate,
	typeTableTemplate,

    typeTextNotesConfiguration,

    typeStyleFontFace,

    typeSvgFontFaceUri,
    typeSvgFontFaceFormat,
    typeSvgFontFaceName,
    typeSvgDefinitionSrc,
    typeOfficeFontFaceDecls,

    typeOfficeBinaryData,
    typeOfficeForms,

    typeTableTableSource,
    typeTableNamedExpressions,
    typeTableNamedExpression,
    typeTableNamedRange,
	typeTableDatabaseRanges,
	typeTableDatabaseRange,
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
        
    typeTableShapes,
	typeTableCalculationSettings,
    typeTableNullDate,	
	
	typeTableTrackedChanges,
	typeTableChangeTrackTableCell,
	typeTableCellContentChange,
	typeTableDeletion,
	typeTablePrevious,

	typeOfficeBody,
    typeOfficeText,
    typeOfficeDocument,
    typeOfficeDocumentContent,
    typeOfficeDocumentStyles,
    typeOfficeDocumentMeta,
    typeOfficeDocumentSettings,

	typeManifest,
	typeManifestEntry,
	typeManifestEncryptionData,

	typeManifestAlgorithm,
	typeManifestStartKeyGeneration,
	typeManifestKeyDerivation,

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
	typeDrawPlugin,

	typeDrawBase,
	typeDrawShape,

	typeDrawRect,
	typeDrawEllipse,
	typeDrawLine,
	typeDrawPolygon,
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
	typeDrawEnhancedGeometry,

	typeDrawPage,    
	typePresentationFooterDecl,
	typePresentationDateTimeDecl,
	typePresentationNotes,

	typeAnimPar,
	typeAnimSeq,
	typeAnimTransitionFilter,
	typeAnimAudio,
	typeAnimCommand,
	typeAnimIterator,

	typeStyleGraphicPropertis,
	typeStyleDrawGradient,
	typeStyleDrawFillImage,
	typeStyleDrawHatch,
	typeStyleDrawMarker,
	typeStyleDrawOpacity,
	typeStyleDrawStrokeDash,

	typeOfficeSpreadsheet,
	typeOfficeAnnotation,
	typeOfficeAnnotationEnd,
	typeStyleRegionLeft,
	typeOfficeScripts,
	typeOfficeScript,
	typeOfficePresentation,
	typeOfficeChart,
	typeOfficeEventListeners,

	typeCalcextConditionalFormats,
	typeCalcextConditionalFormat,
	typeCalcextIconSet,
	typeCalcextDataBar,
	typeCalcextColorScale,
	typeCalcextCondition,
	typeCalcextDateIs,
	typeCalcextFormattingEntry,
	typeCalcextColorScaleEntry,
  
	typePresentationEventListener,	

	typeScriptEventListener,



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
	typeStyleChartProperties

};


}
}

#endif // #ifndef _CPDOCCORE_ODF_OFFCIE_ELEMENTS_TYPE_H_
