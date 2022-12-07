#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = DocxFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

DEFINES +=  UNICODE _UNICODE \
            DONT_WRITE_EMBEDDED_FONTS

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

!disable_precompiled_header:CONFIG += precompile_header
precompile_header {
    PRECOMPILED_HEADER = precompiled.h
    HEADERS += precompiled.h
}

core_release {
SOURCES += \
	docx_format_logic.cpp
}

core_debug {
SOURCES += \
	../../../DocxFormat/Logic/AlternateContent.cpp \
	../../../DocxFormat/Logic/Annotations.cpp \
	../../../DocxFormat/Logic/Bdo.cpp \
	../../../DocxFormat/Logic/Dir.cpp \
	../../../DocxFormat/Logic/FldSimple.cpp \
	../../../DocxFormat/Logic/Hyperlink.cpp \
	../../../DocxFormat/Logic/Paragraph.cpp \
	../../../DocxFormat/Logic/ParagraphProperty.cpp \
	../../../DocxFormat/Logic/RunProperty.cpp \
	../../../DocxFormat/Logic/Run.cpp \
	../../../DocxFormat/Logic/Sdt.cpp \
	../../../DocxFormat/Logic/SectionProperty.cpp \
	../../../DocxFormat/Logic/SmartTag.cpp \
	../../../DocxFormat/Logic/Table.cpp \
	../../../DocxFormat/Logic/TableProperty.cpp \
	../../../DocxFormat/Logic/Vml.cpp \
	../../../DocxFormat/Logic/DocParts.cpp \
	../../../DocxFormat/Logic/Pict.cpp \
	../../../DocxFormat/Logic/FldChar.cpp \
	../../../DocxFormat/Logic/RunContent.cpp \
	../../../DocxFormat/Logic/VmlOfficeDrawing.cpp \
	../../../DocxFormat/Logic/VmlWord.cpp \
	../../../DocxFormat/Logic/Shape.cpp \
	../../../DocxFormat/External/ExternalHyperLink.cpp \
	../../../DocxFormat/External/External.cpp \
	../../../DocxFormat/Media/Media.cpp \
	../../../DocxFormat/Media/VbaProject.cpp \
	../../../DocxFormat/Media/JsaProject.cpp \
	../../../DocxFormat/Media/ActiveX.cpp \
	../../../DocxFormat/Media/Audio.cpp \
	../../../DocxFormat/Media/Video.cpp \
	../../../DocxFormat/Media/Image.cpp \
	../../../DocxFormat/Media/OleObject.cpp \
	../../../DocxFormat/Math/oMath.cpp \
	../../../DocxFormat/Math/oMathContent.cpp \
	../../../DocxFormat/Math/oMathPara.cpp \
	../../../Binary/MathEquation/MathEquation.cpp \
	../../../DocxFormat/Docx.cpp \
	../../../DocxFormat/DocxFlat.cpp \
	../../../DocxFormat/Comments.cpp \
	../../../DocxFormat/Document.cpp \
	../../../DocxFormat/VmlDrawing.cpp \
	../../../DocxFormat/Namespaces.cpp \
	../../../XlsxFormat/CalcChain/CalcChain.cpp \
	../../../XlsxFormat/Chart/Chart.cpp \
	../../../XlsxFormat/Chart/ChartColors.cpp \
	../../../XlsxFormat/Chart/ChartDrawing.cpp \
	../../../XlsxFormat/Chart/ChartStyle.cpp \
	../../../XlsxFormat/Chart/ChartSerialize.cpp \
	../../../XlsxFormat/Chart/ChartSerializeEx.cpp \
	../../../XlsxFormat/Common.cpp \
	../../../XlsxFormat/Xlsx.cpp \
	../../../XlsxFormat/XlsxFlat.cpp \
	../../../XlsxFormat/FileTypes_Spreadsheet.cpp \
	../../../XlsxFormat/Worksheets/Worksheet.cpp \
	../../../XlsxFormat/Worksheets/SheetData.cpp \
	../../../XlsxFormat/Worksheets/ConditionalFormatting.cpp \
	../../../XlsxFormat/Worksheets/DataValidation.cpp \
	../../../XlsxFormat/Worksheets/Cols.cpp \
	../../../XlsxFormat/Worksheets/XlsxHyperlinks.cpp \
	../../../XlsxFormat/Worksheets/MergeCells.cpp \
	../../../XlsxFormat/Worksheets/Sparkline.cpp \
	../../../XlsxFormat/Worksheets/WorksheetChildOther.cpp \
	../../../XlsxFormat/Table/Tables.cpp \
	../../../XlsxFormat/Table/Autofilter.cpp \
	../../../XlsxFormat/Table/Connections.cpp \
	../../../XlsxFormat/Controls/Controls.cpp \
	../../../DocxFormat/Settings/Settings.cpp \
	../../../DocxFormat/Settings/WebSettings.cpp \
	../../../DocxFormat/App.cpp \
	../../../DocxFormat/Core.cpp \
	../../../DocxFormat/Bibliography.cpp \
	../../../DocxFormat/ContentTypes.cpp \
	../../../DocxFormat/CustomXml.cpp \
	../../../DocxFormat/File.cpp \
	../../../DocxFormat/FileType.cpp \
	../../../DocxFormat/Font.cpp \
	../../../DocxFormat/FontTable.cpp \
	../../../DocxFormat/Footnote.cpp \
	../../../DocxFormat/FtnEdn.cpp \
	../../../DocxFormat/FileFactory.cpp \
	../../../DocxFormat/FileTypes.cpp \
	../../../DocxFormat/Numbering.cpp \
	../../../DocxFormat/Rels.cpp \
	../../../DocxFormat/RId.cpp \
	../../../DocxFormat/Styles.cpp \
	../../../DocxFormat/UnknowTypeFile.cpp \
	../../../DocxFormat/IFileContainer.cpp \
	../../../DocxFormat/Document.cpp \
	../../../XlsxFormat/FileFactory_Spreadsheet.cpp \
	../../../XlsxFormat/ComplexTypes_Spreadsheet.cpp \
	../../../DocxFormat/Drawing/Drawing.cpp \
	../../../DocxFormat/Drawing/DrawingExt.cpp \
	../../../DocxFormat/Diagram/DiagramColors.cpp \
	../../../DocxFormat/Diagram/DiagramData.cpp \
	../../../DocxFormat/Diagram/DiagramDrawing.cpp \
	../../../DocxFormat/Diagram/DiagramLayout.cpp \
	../../../DocxFormat/Diagram/DiagramQuickStyle.cpp \
	../../../DocxFormat/WritingElement.cpp \
	../../../DocxFormat/HeaderFooter.cpp \
	../../../DocxFormat/Endnote.cpp \
	../../../Common/SimpleTypes_Base.cpp \
	../../../Common/SimpleTypes_Word.cpp \
	../../../Common/SimpleTypes_Drawing.cpp \
	../../../Common/SimpleTypes_OMath.cpp \
	../../../Common/SimpleTypes_Rtf.cpp \
	../../../Common/SimpleTypes_Shared.cpp \
	../../../Common/SimpleTypes_Spreadsheet.cpp \
	../../../Common/SimpleTypes_Vml.cpp \
	../../../Common/ComplexTypes.cpp \
	../../../SystemUtility/SystemUtility.cpp \
	../../../SystemUtility/FileUtils.cpp \
	../../../XML/XmlSimple.cpp \
	../../../XlsxFormat/Styles/XlsxStyles.cpp \
	../../../XlsxFormat/Styles/rPr.cpp \
	../../../XlsxFormat/Styles/Borders.cpp \
	../../../XlsxFormat/Styles/CellStyles.cpp \
	../../../XlsxFormat/Styles/Colors.cpp \
	../../../XlsxFormat/Styles/dxf.cpp \
	../../../XlsxFormat/Styles/Fills.cpp \
	../../../XlsxFormat/Styles/Fonts.cpp \
	../../../XlsxFormat/Styles/NumFmts.cpp \
	../../../XlsxFormat/Styles/TableStyles.cpp \
	../../../XlsxFormat/Styles/Xfs.cpp \
	../../../XlsxFormat/SharedStrings/Si.cpp \
	../../../XlsxFormat/SharedStrings/Text.cpp \
	../../../XlsxFormat/SharedStrings/PhoneticPr.cpp \
	../../../XlsxFormat/SharedStrings/XlsxRun.cpp \
	../../../XlsxFormat/SharedStrings/SharedStrings.cpp \
	../../../XlsxFormat/Pivot/Pivots.cpp \
	../../../XlsxFormat/Slicer/SlicerCache.cpp \
	../../../XlsxFormat/Slicer/SlicerCacheExt.cpp \
	../../../XlsxFormat/Slicer/Slicer.cpp \
	../../../XlsxFormat/ExternalLinks/ExternalLinks.cpp \
	../../../XlsxFormat/NamedSheetViews/NamedSheetViews.cpp \
	../../../XlsxFormat/Pivot/PivotCacheDefinitionExt.cpp \
	../../../XlsxFormat/Workbook/Workbook.cpp \
	../../../XlsxFormat/Workbook/BookViews.cpp \
	../../../XlsxFormat/Workbook/CalcPr.cpp \
	../../../XlsxFormat/Workbook/DefinedNames.cpp \
	../../../XlsxFormat/Workbook/ExternalReferences.cpp \
	../../../XlsxFormat/Workbook/Sheets.cpp \
	../../../XlsxFormat/Workbook/WorkbookPr.cpp \
	../../../XlsxFormat/Comments/XlsxComments.cpp \
	../../../XlsxFormat/Comments/ThreadedComments.cpp \
	../../../XlsxFormat/Drawing/CellAnchor.cpp \
	../../../XlsxFormat/Drawing/XlsxDrawing.cpp \
	../../../XlsxFormat/Drawing/FromTo.cpp \
	../../../XlsxFormat/Drawing/Pos.cpp \
	../../../XlsxFormat/ExternalLinks/ExternalLinkPath.cpp \
	../../../XlsxFormat/ExternalLinks/ExternalLinks.cpp \
	../../../XlsxFormat/Ole/OleObjects.cpp
}


SOURCES += \
	../../../Base/codecvt.cpp \
	../../../Base/DateTime.cpp \
	../../../Base/unicode_util.cpp \
	../../../Base/Unit.cpp \
	../../../Base/WinColor.cpp \
	../../../../Common/3dParty/pole/pole.cpp \
	docx_format.cpp


HEADERS += \
	../../../Base/Base.h \
	../../../Base/Nullable.h \
	../../../Base/SmartPtr.h \
	../../../Common/ComplexTypes.h \
	../../../Common/SimpleTypes_Base.h \
	../../../Common/SimpleTypes_Drawing.h \
	../../../Common/SimpleTypes_OMath.h \
	../../../Common/SimpleTypes_Shared.h \
	../../../Common/SimpleTypes_Vml.h \
	../../../Common/SimpleTypes_Word.h \
	../../../Common/Size.h \
	../../../Common/Unit.h \
	../../../Common/Wrap.h \
	../../../Common/ZIndex.h \
	../../../DocxFormat/Drawing/Drawing.h \
	../../../DocxFormat/Drawing/DrawingExt.h \
	../../../DocxFormat/External/External.h \
	../../../DocxFormat/External/HyperLink.h \
	../../../DocxFormat/Logic/AlternateContent.h \
	../../../DocxFormat/Logic/Annotations.h \
	../../../DocxFormat/Logic/BaseLogicSettings.h \
	../../../DocxFormat/Logic/Bdo.h \
	../../../DocxFormat/Logic/Dir.h \
	../../../DocxFormat/Logic/FldChar.h \
	../../../DocxFormat/Logic/FldSimple.h \
	../../../DocxFormat/Logic/Hyperlink.h \
	../../../DocxFormat/Logic/Paragraph.h \
	../../../DocxFormat/Logic/ParagraphProperty.h \
	../../../DocxFormat/Logic/Pict.h \
	../../../DocxFormat/Logic/Run.h \
	../../../DocxFormat/Logic/RunContent.h \
	../../../DocxFormat/Logic/RunProperty.h \
	../../../DocxFormat/Logic/Sdt.h \
	../../../DocxFormat/Logic/SectionProperty.h \
	../../../DocxFormat/Logic/Shape.h \
	../../../DocxFormat/Logic/SmartTag.h \
	../../../DocxFormat/Logic/Table.h \
	../../../DocxFormat/Logic/TableProperty.h \
	../../../DocxFormat/Logic/Vml.h \
	../../../DocxFormat/Logic/VmlOfficeDrawing.h \
	../../../DocxFormat/Logic/VmlWord.h \
	../../../DocxFormat/Logic/DocParts.h \
	../../../DocxFormat/Math/OMath.h \
	../../../DocxFormat/Math/oMathBottomNodes.h \
	../../../DocxFormat/Math/oMathContent.h \
	../../../DocxFormat/Math/oMathPara.h \
	../../../DocxFormat/Media/Audio.h \
	../../../DocxFormat/Media/Image.h \
	../../../DocxFormat/Media/Media.h \
	../../../DocxFormat/Media/OleObject.h \
	../../../DocxFormat/Media/Video.h \
	../../../DocxFormat/Media/JsaProject.h \
	../../../DocxFormat/Settings/Settings.h \
	../../../DocxFormat/Settings/WebSettings.h \
	../../../DocxFormat/App.h \
	../../../DocxFormat/Bibliography.h \
	../../../DocxFormat/Comments.h \
	../../../DocxFormat/ContentTypes.h \
	../../../DocxFormat/Core.h \
	../../../DocxFormat/CustomXml.h \
	../../../DocxFormat/Document.h \
	../../../DocxFormat/Docx.h \
	../../../DocxFormat/DocxFlat.h \
	../../../DocxFormat/Endnote.h \
	../../../DocxFormat/File.h \
	../../../DocxFormat/FileFactory.h \
	../../../DocxFormat/FileType.h \
	../../../DocxFormat/FileTypes.h \
	../../../DocxFormat/Font.h \
	../../../DocxFormat/FontTable.h \
	../../../DocxFormat/Footnote.h \
	../../../DocxFormat/FtnEdn.h \
	../../../DocxFormat/HeaderFooter.h \
	../../../DocxFormat/IFileBuilder.h \
	../../../DocxFormat/IFileContainer.h \
	../../../DocxFormat/NamespaceOwn.h \
	../../../DocxFormat/Namespaces.h \
	../../../DocxFormat/Numbering.h \
	../../../DocxFormat/Rels.h \
	../../../DocxFormat/RId.h \
	../../../DocxFormat/Styles.h \
	../../../DocxFormat/Unit.h \
	../../../DocxFormat/UnknowTypeFile.h \
	../../../DocxFormat/WritingElement.h \
	../../../SystemUtility/SystemUtility.h \
	../../../Base/codecvt.h \
	../../../Base/DateTime.h \
	../../../Utility/TxtFile.h \
	../../../Base/Unit.h \
	../../../Utility/Utility.h \
	../../../XlsxFormat/CalcChain/CalcChain.h \
	../../../XlsxFormat/Chart/Chart.h \
	../../../XlsxFormat/Chart/ChartSerialize.h \
	../../../XlsxFormat/Chart/ChartSerializeEx.h \
	../../../XlsxFormat/Chart/ChartColors.h \
	../../../XlsxFormat/Chart/ChartDrawing.h \
	../../../XlsxFormat/Chart/ChartStyle.h \
	../../../XlsxFormat/Comments/Comments.h \
	../../../XlsxFormat/Comments/ThreadedComments.h \
	../../../XlsxFormat/Drawing/CellAnchor.h \
	../../../XlsxFormat/Drawing/Drawing.h \
	../../../XlsxFormat/Drawing/FromTo.h \
	../../../XlsxFormat/Drawing/Pos.h \
	../../../XlsxFormat/SharedStrings/PhoneticPr.h \
	../../../XlsxFormat/SharedStrings/Run.h \
	../../../XlsxFormat/SharedStrings/SharedStrings.h \
	../../../XlsxFormat/SharedStrings/Si.h \
	../../../XlsxFormat/SharedStrings/Text.h \
	../../../XlsxFormat/Styles/Borders.h \
	../../../XlsxFormat/Styles/CellStyles.h \
	../../../XlsxFormat/Styles/Colors.h \
	../../../XlsxFormat/Styles/dxf.h \
	../../../XlsxFormat/Styles/Fills.h \
	../../../XlsxFormat/Styles/Fonts.h \
	../../../XlsxFormat/Styles/NumFmts.h \
	../../../XlsxFormat/Styles/rPr.h \
	../../../XlsxFormat/Styles/Styles.h \
	../../../XlsxFormat/Styles/TableStyles.h \
	../../../XlsxFormat/Styles/Xfs.h \
	../../../XlsxFormat/Table/Autofilter.h \
	../../../XlsxFormat/Table/Table.h \
	../../../XlsxFormat/Table/Autofilter.h \
	../../../XlsxFormat/Table/Connections.h \
	../../../XlsxFormat/Table/QueryTable.h \
	../../../XlsxFormat/Workbook/BookViews.h \
	../../../XlsxFormat/Workbook/CalcPr.h \
	../../../XlsxFormat/Workbook/DefinedNames.h \
	../../../XlsxFormat/Workbook/Sheets.h \
	../../../XlsxFormat/Workbook/Workbook.h \
	../../../XlsxFormat/Workbook/WorkbookPr.h \
	../../../XlsxFormat/Workbook/ExternalReferences.h \
	../../../XlsxFormat/Worksheets/Cols.h \
	../../../XlsxFormat/Worksheets/ConditionalFormatting.h \
	../../../XlsxFormat/Worksheets/DataValidation.h \
	../../../XlsxFormat/Worksheets/Hyperlinks.h \
	../../../XlsxFormat/Worksheets/MergeCells.h \
	../../../XlsxFormat/Worksheets/SheetData.h \
	../../../XlsxFormat/Worksheets/Worksheet.h \
	../../../XlsxFormat/Worksheets/WorksheetChildOther.h \
	../../../XlsxFormat/Controls/Controls.h \
	../../../XlsxFormat/Common.h \
	../../../XlsxFormat/CommonInclude.h \
	../../../XlsxFormat/ComplexTypes_Spreadsheet.h \
	../../../XlsxFormat/FileFactory_Spreadsheet.h \
	../../../XlsxFormat/FileTypes_Spreadsheet.h \
	../../../XlsxFormat/SimpleTypes_Spreadsheet.h \
	../../../XlsxFormat/WritingElement.h \
	../../../DocxFormat/WorkbookComments.h \
	../../../XlsxFormat/Xlsx.h \
	../../../XlsxFormat/XlsxFlat.h \
	../../../XML/Utils.h \
	../../../XML/XmlSimple.h \
	../../../MathEquation/LEStream.h \
	../../../MathEquation/MathEquation.h \
	../../../MathEquation/OutputDev.h \
	../../../MathEquation/String.h \
	../../../MathEquation/StringOutput.h \
	../../../MathEquation/Types.h \
	../../../Base/unicode_util.h \
	../../../XlsxFormat/ExternalLinks/ExternalLinkPath.h \
	../../../XlsxFormat/ExternalLinks/ExternalLinks.h \
	../../../XlsxFormat/Worksheets/Sparkline.h \
	../../../XlsxFormat/Ole/OleObjects.h \
	../../../DocxFormat/ Diagram/DiagramColors.h \
	../../../DocxFormat/Diagram/DiagramData.h \
	../../../DocxFormat/Diagram/DiagramDrawing.h \
	../../../DocxFormat/Diagram/DiagramLayout.h \
	../../../DocxFormat/Diagram/DiagramQuickStyle.h \
	../../../XlsxFormat/Pivot/PivotTable.h \
	../../../XlsxFormat/Pivot/PivotCacheDefinition.h \
	../../../XlsxFormat/Pivot/PivotCacheDefinitionExt.h \
	../../../XlsxFormat/Pivot/PivotCacheRecords.h \
	../../../XlsxFormat/Slicer/SlicerCache.h \
	../../../XlsxFormat/Slicer/SlicerCacheExt.h \
	../../../XlsxFormat/Slicer/Slicer.h \
	../../../XlsxFormat/NamedSheetViews/NamedSheetViews.h \
	docx_format.h
