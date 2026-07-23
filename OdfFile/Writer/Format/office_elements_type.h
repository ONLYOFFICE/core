/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
