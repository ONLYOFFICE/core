#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T15:22:54
#
#-------------------------------------------------

QT       -= core gui

TARGET = DocxFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../Common/base.pri)

DEFINES +=  UNICODE _UNICODE \
            DONT_WRITE_EMBEDDED_FONTS

#BOOST
include($$PWD/../../3dParty/boost/boost.pri)

core_release {
SOURCES += \
    docxformatlib_logic.cpp
}

core_debug {
SOURCES += \
    ../Source/DocxFormat/Logic/AlternateContent.cpp \
    ../Source/DocxFormat/Logic/Annotations.cpp \
    ../Source/DocxFormat/Logic/Bdo.cpp \
    ../Source/DocxFormat/Logic/Dir.cpp \
    ../Source/DocxFormat/Logic/FldSimple.cpp \
    ../Source/DocxFormat/Logic/Hyperlink.cpp \
    ../Source/DocxFormat/Logic/Paragraph.cpp \
    ../Source/DocxFormat/Logic/ParagraphProperty.cpp \
    ../Source/DocxFormat/Logic/RunProperty.cpp \
    ../Source/DocxFormat/Logic/Sdt.cpp \
    ../Source/DocxFormat/Logic/SectionProperty.cpp \
    ../Source/DocxFormat/Logic/SmartTag.cpp \
    ../Source/DocxFormat/Logic/Table.cpp \
    ../Source/DocxFormat/Logic/TableProperty.cpp \
    ../Source/DocxFormat/Logic/Vml.cpp \
    ../Source/DocxFormat/Media/VbaProject.cpp \
    ../Source/DocxFormat/Media/JsaProject.cpp \
    ../Source/DocxFormat/Media/ActiveX.cpp \
    ../Source/DocxFormat/Math/oMath.cpp \
    ../Source/DocxFormat/Math/oMathContent.cpp \
    ../Source/DocxFormat/Math/oMathPara.cpp \
    ../Source/MathEquation/MathEquation.cpp \
    ../Source/DocxFormat/Docx.cpp \
    ../Source/DocxFormat/Comments.cpp \
    ../Source/DocxFormat/VmlDrawing.cpp \
    ../Source/XlsxFormat/Chart/ChartSerialize.cpp \
    ../Source/XlsxFormat/Chart/ChartSerializeEx.cpp \
    ../Source/XlsxFormat/Common.cpp \
    ../Source/XlsxFormat/Xlsx.cpp \
    ../Source/XlsxFormat/Worksheets/Worksheet.cpp \
    ../Source/XlsxFormat/Worksheets/SheetData.cpp \
    ../Source/XlsxFormat/Worksheets/ConditionalFormatting.cpp \
    ../Source/XlsxFormat/Worksheets/DataValidation.cpp \
    ../Source/XlsxFormat/Table/Tables.cpp \
    ../Source/XlsxFormat/Controls/Controls.cpp \
    ../Source/DocxFormat/App.cpp \
    ../Source/DocxFormat/Core.cpp \
    ../Source/DocxFormat/FileFactory.cpp \
    ../Source/DocxFormat/IFileContainer.cpp \
    ../Source/XlsxFormat/FileFactory_Spreadsheet.cpp \
    ../Source/DocxFormat/Drawing/DrawingExt.cpp \
    ../Source/Common/Align.cpp \
    ../Source/Common/Color.cpp \
    ../Source/Common/Index.cpp \
    ../Source/Common/NumFormat.cpp \
    ../Source/Common/Position.cpp \
    ../Source/Common/Wrap.cpp \
    ../Source/Common/ZIndex.cpp \
    ../Source/Common/SimpleTypes_Word.cpp \
    ../Source/SystemUtility/SystemUtility.cpp \
    ../Source/XlsxFormat/Styles/rPr.cpp \
    ../Source/XlsxFormat/SharedStrings/Si.cpp \
    ../Source/XlsxFormat/SharedStrings/Text.cpp \
    ../Source/XlsxFormat/Pivot/Pivots.cpp
}


SOURCES += docxformatlib.cpp \
    ../Source/Utility/codecvt.cpp \
    ../Source/Utility/DateTime.cpp \
    ../Source/Utility/TxtFile.cpp \
    ../Source/Base/unicode_util.cpp \
    ../../3dParty/pole/pole.cpp

HEADERS += docxformatlib.h \
    ../Source/Base/Base.h \
    ../Source/Base/Nullable.h \
    ../Source/Base/SmartPtr.h \
    ../Source/Common/AbstractConverter.h \
    ../Source/Common/Align.h \
    ../Source/Common/Color.h \
    ../Source/Common/Common.h \
    ../Source/Common/ComplexTypes.h \
    ../Source/Common/Index.h \
    ../Source/Common/NumFormat.h \
    ../Source/Common/Point.h \
    ../Source/Common/Position.h \
    ../Source/Common/SimpleTypes_Base.h \
    ../Source/Common/SimpleTypes_Drawing.h \
    ../Source/Common/SimpleTypes_OMath.h \
    ../Source/Common/SimpleTypes_Shared.h \
    ../Source/Common/SimpleTypes_Vml.h \
    ../Source/Common/SimpleTypes_Word.h \
    ../Source/Common/Size.h \
    ../Source/Common/Unit.h \
    ../Source/Common/Wrap.h \
    ../Source/Common/ZIndex.h \
    ../Source/DocxFormat/Drawing/Drawing.h \
    ../Source/DocxFormat/Drawing/DrawingBody.h \
    ../Source/DocxFormat/Drawing/DrawingExt.h \
    ../Source/DocxFormat/External/External.h \
    ../Source/DocxFormat/External/HyperLink.h \
    ../Source/DocxFormat/Logic/AlternateContent.h \
    ../Source/DocxFormat/Logic/Annotations.h \
    ../Source/DocxFormat/Logic/BaseLogicSettings.h \
    ../Source/DocxFormat/Logic/Bdo.h \
    ../Source/DocxFormat/Logic/Dir.h \
    ../Source/DocxFormat/Logic/FldChar.h \
    ../Source/DocxFormat/Logic/FldSimple.h \
    ../Source/DocxFormat/Logic/Hyperlink.h \
    ../Source/DocxFormat/Logic/Paragraph.h \
    ../Source/DocxFormat/Logic/ParagraphProperty.h \
    ../Source/DocxFormat/Logic/Pict.h \
    ../Source/DocxFormat/Logic/Run.h \
    ../Source/DocxFormat/Logic/RunContent.h \
    ../Source/DocxFormat/Logic/RunProperty.h \
    ../Source/DocxFormat/Logic/Sdt.h \
    ../Source/DocxFormat/Logic/SectionProperty.h \
    ../Source/DocxFormat/Logic/Shape.h \
    ../Source/DocxFormat/Logic/SmartTag.h \
    ../Source/DocxFormat/Logic/Table.h \
    ../Source/DocxFormat/Logic/TableProperty.h \
    ../Source/DocxFormat/Logic/Vml.h \
    ../Source/DocxFormat/Logic/VmlOfficeDrawing.h \
    ../Source/DocxFormat/Logic/VmlWord.h \
    ../Source/DocxFormat/Math/OMath.h \
    ../Source/DocxFormat/Math/oMathBottomNodes.h \
    ../Source/DocxFormat/Math/oMathContent.h \
    ../Source/DocxFormat/Math/oMathPara.h \
    ../Source/DocxFormat/Media/Audio.h \
    ../Source/DocxFormat/Media/Image.h \
    ../Source/DocxFormat/Media/Media.h \
    ../Source/DocxFormat/Media/OleObject.h \
    ../Source/DocxFormat/Media/Video.h \
    ../Source/DocxFormat/Media/JsaProject.h \
    ../Source/DocxFormat/Settings/Settings.h \
    ../Source/DocxFormat/Settings/WebSettings.h \
    ../Source/DocxFormat/App.h \
    ../Source/DocxFormat/Bibliography.h \
    ../Source/DocxFormat/Comments.h \
    ../Source/DocxFormat/ContentTypes.h \
    ../Source/DocxFormat/Core.h \
    ../Source/DocxFormat/CustomXml.h \
    ../Source/DocxFormat/Document.h \
    ../Source/DocxFormat/Docx.h \
    ../Source/DocxFormat/Endnote.h \
    ../Source/DocxFormat/File.h \
    ../Source/DocxFormat/FileFactory.h \
    ../Source/DocxFormat/FileType.h \
    ../Source/DocxFormat/FileTypes.h \
    ../Source/DocxFormat/Font.h \
    ../Source/DocxFormat/FontTable.h \
    ../Source/DocxFormat/Footnote.h \
    ../Source/DocxFormat/FtnEdn.h \
    ../Source/DocxFormat/HeaderFooter.h \
    ../Source/DocxFormat/IFileBuilder.h \
    ../Source/DocxFormat/IFileContainer.h \
    ../Source/DocxFormat/NamespaceOwn.h \
    ../Source/DocxFormat/Namespaces.h \
    ../Source/DocxFormat/Numbering.h \
    ../Source/DocxFormat/Rels.h \
    ../Source/DocxFormat/RId.h \
    ../Source/DocxFormat/Styles.h \
    ../Source/DocxFormat/Unit.h \
    ../Source/DocxFormat/UnknowTypeFile.h \
    ../Source/DocxFormat/WritingElement.h \
    ../Source/SystemUtility/SystemUtility.h \
    ../Source/Utility/codecvt.h \
    ../Source/Utility/DateTime.h \
    ../Source/Utility/TxtFile.h \
    ../Source/Utility/Unit.h \
    ../Source/Utility/Utility.h \
    ../Source/XlsxFormat/CalcChain/CalcChain.h \
    ../Source/XlsxFormat/Chart/Chart.h \
    ../Source/XlsxFormat/Chart/ChartSerialize.h \
    ../Source/XlsxFormat/Chart/ChartSerializeEx.h \
    ../Source/XlsxFormat/Comments/Comments.h \
	../Source/XlsxFormat/Comments/ThreadedComments.h \
    ../Source/XlsxFormat/Drawing/CellAnchor.h \
    ../Source/XlsxFormat/Drawing/Drawing.h \
    ../Source/XlsxFormat/Drawing/FromTo.h \
    ../Source/XlsxFormat/Drawing/Pos.h \
    ../Source/XlsxFormat/SharedStrings/PhoneticPr.h \
    ../Source/XlsxFormat/SharedStrings/rPr.h \
    ../Source/XlsxFormat/SharedStrings/Run.h \
    ../Source/XlsxFormat/SharedStrings/SharedStrings.h \
    ../Source/XlsxFormat/SharedStrings/Si.h \
    ../Source/XlsxFormat/SharedStrings/Text.h \
    ../Source/XlsxFormat/Styles/Borders.h \
    ../Source/XlsxFormat/Styles/CellStyles.h \
    ../Source/XlsxFormat/Styles/Colors.h \
    ../Source/XlsxFormat/Styles/dxf.h \
    ../Source/XlsxFormat/Styles/Fills.h \
    ../Source/XlsxFormat/Styles/Fonts.h \
    ../Source/XlsxFormat/Styles/NumFmts.h \
    ../Source/XlsxFormat/Styles/rPr.h \
    ../Source/XlsxFormat/Styles/Styles.h \
    ../Source/XlsxFormat/Styles/TableStyles.h \
    ../Source/XlsxFormat/Styles/Xfs.h \
    ../Source/XlsxFormat/Table/Autofilter.h \
    ../Source/XlsxFormat/Table/Table.h \
    ../Source/XlsxFormat/Workbook/BookViews.h \
	../Source/XlsxFormat/Workbook/CalcPr.h \
    ../Source/XlsxFormat/Workbook/DefinedNames.h \
    ../Source/XlsxFormat/Workbook/Sheets.h \
    ../Source/XlsxFormat/Workbook/Workbook.h \
    ../Source/XlsxFormat/Workbook/WorkbookPr.h \
    ../Source/XlsxFormat/Worksheets/Cols.h \
    ../Source/XlsxFormat/Worksheets/ConditionalFormatting.h \
    ../Source/XlsxFormat/Worksheets/DataValidation.h \
    ../Source/XlsxFormat/Worksheets/Hyperlinks.h \
    ../Source/XlsxFormat/Worksheets/MergeCells.h \
    ../Source/XlsxFormat/Worksheets/SheetData.h \
    ../Source/XlsxFormat/Worksheets/Worksheet.h \
    ../Source/XlsxFormat/Worksheets/WorksheetChildOther.h \
	../Source/XlsxFormat/Controls/Controls.h \
    ../Source/XlsxFormat/Common.h \
    ../Source/XlsxFormat/CommonInclude.h \
    ../Source/XlsxFormat/ComplexTypes_Spreadsheet.h \
    ../Source/XlsxFormat/FileFactory_Spreadsheet.h \
    ../Source/XlsxFormat/FileTypes_Spreadsheet.h \
    ../Source/XlsxFormat/SimpleTypes_Spreadsheet.h \
    ../Source/XlsxFormat/Workbook.h \
    ../Source/XlsxFormat/WritingElement.h \
    ../Source/DocxFormat/WorkbookComments.h \
    ../Source/XlsxFormat/Xlsx.h \
    ../Source/XML/Utils.h \
    ../Source/XML/XmlSimple.h \
    ../Source/MathEquation/LEStream.h \
    ../Source/MathEquation/MathEquation.h \
    ../Source/MathEquation/OutputDev.h \
    ../Source/MathEquation/String.h \
    ../Source/MathEquation/StringOutput.h \
    ../Source/MathEquation/Types.h \
    ../Source/Base/unicode_util.h \
    ../Source/XlsxFormat/ExternalLinks/ExternalLinkPath.h \
    ../Source/XlsxFormat/ExternalLinks/ExternalLinks.h \
    ../Source/XlsxFormat/Worksheets/Sparkline.h \
    ../Source/XlsxFormat/Ole/OleObjects.h \
    ../Source/DocxFormat/Diagram/DiagramData.h \
    ../Source/DocxFormat/Diagram/DiagramDrawing.h \
    ../Source/XlsxFormat/Pivot/PivotTable.h \
    ../Source/XlsxFormat/Pivot/PivotCacheDefinition.h \
    ../Source/XlsxFormat/Pivot/PivotCacheRecords.h
