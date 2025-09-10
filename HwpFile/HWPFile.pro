QT       -= core gui

VERSION = 0.0.0.1
TARGET = HWPFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics)

DEFINES += HWPFILE_USE_DYNAMIC_LIBRARY \
           CRYPTOPP_DISABLE_ASM 


SOURCES += \
    HWPFile.cpp \
    HwpDoc/Common/XMLReader.cpp \
    HwpDoc/Common/WriterContext.cpp \
    HwpDoc/Conversion/Converter2OOXML.cpp \
    HwpDoc/Conversion/FootnoteConverter.cpp \
    HwpDoc/Conversion/NumberingConverter.cpp \
    HwpDoc/Conversion/OleConverter.cpp \
    HwpDoc/Conversion/StyleConverter.cpp \
    HwpDoc/HWPDocInfo.cpp \
    HwpDoc/HWPElements/HWPRecord.cpp \
    HwpDoc/HWPElements/HWPRecordBinData.cpp \
    HwpDoc/HWPElements/HWPRecordBorderFill.cpp \
    HwpDoc/HWPElements/HWPRecordBullet.cpp \
    HwpDoc/HWPElements/HWPRecordCharShape.cpp \
    HwpDoc/HWPElements/HWPRecordCtrlData.cpp \
    HwpDoc/HWPElements/HWPRecordCtrlHeader.cpp \
    HwpDoc/HWPElements/HWPRecordDocumentProperties.cpp \
    HwpDoc/HWPElements/HWPRecordFaceName.cpp \
    HwpDoc/HWPElements/HWPRecordFormObject.cpp \
    HwpDoc/HWPElements/HWPRecordIDMaping.cpp \
    HwpDoc/HWPElements/HWPRecordListHeader.cpp \
    HwpDoc/HWPElements/HWPRecordNumbering.cpp \
    HwpDoc/HWPElements/HWPRecordParaRangeTag.cpp \
    HwpDoc/HWPElements/HWPRecordParaShape.cpp \
    HwpDoc/HWPElements/HWPRecordParaText.cpp \
    HwpDoc/HWPElements/HWPRecordStyle.cpp \
    HwpDoc/HWPElements/HwpRecordTabDef.cpp \
    HwpDoc/HWPFile.cpp \
    HwpDoc/HWPMLFile.cpp \
    HwpDoc/HWPSection.cpp \
    HwpDoc/HWPStream.cpp \
    HwpDoc/HWPXFile.cpp \
    HwpDoc/HwpFileHeader.cpp \
    HwpDoc/OLEdoc/CompoundFile.cpp \
    HwpDoc/OLEdoc/DirectoryEntry.cpp \
    HwpDoc/Paragraph/CapParagraph.cpp \
    HwpDoc/Paragraph/CellParagraph.cpp \
    HwpDoc/Paragraph/CharShape.cpp \
    HwpDoc/Paragraph/Ctrl.cpp \
    HwpDoc/Paragraph/CtrlAutoNumber.cpp \
    HwpDoc/Paragraph/CtrlCharacter.cpp \
    HwpDoc/Paragraph/CtrlClick.cpp \
    HwpDoc/Paragraph/CtrlColumnDef.cpp \
    HwpDoc/Paragraph/CtrlCommon.cpp \
    HwpDoc/Paragraph/CtrlContainer.cpp \
    HwpDoc/Paragraph/CtrlEmpty.cpp \
    HwpDoc/Paragraph/CtrlEqEdit.cpp \
    HwpDoc/Paragraph/CtrlField.cpp \
    HwpDoc/Paragraph/CtrlForm.cpp \
    HwpDoc/Paragraph/CtrlGeneralShape.cpp \
    HwpDoc/Paragraph/CtrlHeadFoot.cpp \
    HwpDoc/Paragraph/CtrlNewNumber.cpp \
    HwpDoc/Paragraph/CtrlNote.cpp \
    HwpDoc/Paragraph/CtrlObjElement.cpp \
    HwpDoc/Paragraph/CtrlPageNumPos.cpp \
    HwpDoc/Paragraph/CtrlSectionDef.cpp \
    HwpDoc/Paragraph/CtrlShapeArc.cpp \
    HwpDoc/Paragraph/CtrlShapeConnectLine.cpp \
    HwpDoc/Paragraph/CtrlShapeCurve.cpp \
    HwpDoc/Paragraph/CtrlShapeEllipse.cpp \
    HwpDoc/Paragraph/CtrlShapeLine.cpp \
    HwpDoc/Paragraph/CtrlShapeOle.cpp \
    HwpDoc/Paragraph/CtrlShapePic.cpp \
    HwpDoc/Paragraph/CtrlShapePolygon.cpp \
    HwpDoc/Paragraph/CtrlShapeRect.cpp \
    HwpDoc/Paragraph/CtrlShapeTextArt.cpp \
    HwpDoc/Paragraph/CtrlShapeVideo.cpp \
    HwpDoc/Paragraph/CtrlTable.cpp \
    HwpDoc/Paragraph/HWPPargraph.cpp \
    HwpDoc/Paragraph/LineSeg.cpp \
    HwpDoc/Paragraph/ParaText.cpp \
    HwpDoc/Paragraph/TblCell.cpp \
    HwpDoc/Section/NoteShape.cpp \
    HwpDoc/Section/Page.cpp \
    HwpDoc/Section/PageBorderFill.cpp

HEADERS += \
    HWPFile.h \
    HwpDoc/Common/Common.h \
    HwpDoc/Common/NodeNames.h \
    HwpDoc/Common/WriterContext.h \
    HwpDoc/Common/XMLReader.h \
    HwpDoc/Conversion/ConversionState.h \
    HwpDoc/Conversion/Converter2OOXML.h \
    HwpDoc/Conversion/FootnoteConverter.h \
    HwpDoc/Conversion/NumberingConverter.h \
    HwpDoc/Conversion/OleConverter.h \
    HwpDoc/Conversion/StyleConverter.h \
    HwpDoc/Conversion/Transform.h \
    HwpDoc/Conversion/Types.h \
    HwpDoc/Errors.h \
    HwpDoc/HWPDocInfo.h \
    HwpDoc/HWPElements/HWPRecord.h \
    HwpDoc/HWPElements/HWPRecordBinData.h \
    HwpDoc/HWPElements/HWPRecordBorderFill.h \
    HwpDoc/HWPElements/HWPRecordBullet.h \
    HwpDoc/HWPElements/HWPRecordCharShape.h \
    HwpDoc/HWPElements/HWPRecordCtrlData.h \
    HwpDoc/HWPElements/HWPRecordCtrlHeader.h \
    HwpDoc/HWPElements/HWPRecordDocumentProperties.h \
    HwpDoc/HWPElements/HWPRecordFaceName.h \
    HwpDoc/HWPElements/HWPRecordFormObject.h \
    HwpDoc/HWPElements/HWPRecordIDMaping.h \
    HwpDoc/HWPElements/HWPRecordListHeader.h \
    HwpDoc/HWPElements/HWPRecordNumbering.h \
    HwpDoc/HWPElements/HWPRecordParaRangeTag.h \
    HwpDoc/HWPElements/HWPRecordParaShape.h \
    HwpDoc/HWPElements/HWPRecordParaText.h \
    HwpDoc/HWPElements/HWPRecordStyle.h \
    HwpDoc/HWPElements/HWPTag.h \
    HwpDoc/HWPElements/HWPType.h \
    HwpDoc/HWPElements/HwpRecordTabDef.h \
    HwpDoc/HWPElements/HwpRecordTypes.h \
    HwpDoc/HWPFile.h \
    HwpDoc/HWPMLFile.h \
    HwpDoc/HWPSection.h \
    HwpDoc/HWPStream.h \
    HwpDoc/HWPXFile.h \
    HwpDoc/HanType.h \
    HwpDoc/HwpFileHeader.h \
    HwpDoc/OLEdoc/CompoundFile.h \
    HwpDoc/OLEdoc/DirectoryEntry.h \
    HwpDoc/OLEdoc/Sector.h \
    HwpDoc/OLEdoc/SectorType.h \
    HwpDoc/Paragraph/CapParagraph.h \
    HwpDoc/Paragraph/CellParagraph.h \
    HwpDoc/Paragraph/CharShape.h \
    HwpDoc/Paragraph/CommonObj.h \
    HwpDoc/Paragraph/Ctrl.h \
    HwpDoc/Paragraph/CtrlAutoNumber.h \
    HwpDoc/Paragraph/CtrlCharacter.h \
    HwpDoc/Paragraph/CtrlClick.h \
    HwpDoc/Paragraph/CtrlColumnDef.h \
    HwpDoc/Paragraph/CtrlCommon.h \
    HwpDoc/Paragraph/CtrlContainer.h \
    HwpDoc/Paragraph/CtrlEmpty.h \
    HwpDoc/Paragraph/CtrlEqEdit.h \
    HwpDoc/Paragraph/CtrlField.h \
    HwpDoc/Paragraph/CtrlForm.h \
    HwpDoc/Paragraph/CtrlGeneralShape.h \
    HwpDoc/Paragraph/CtrlHeadFoot.h \
    HwpDoc/Paragraph/CtrlNewNumber.h \
    HwpDoc/Paragraph/CtrlNote.h \
    HwpDoc/Paragraph/CtrlObjElement.h \
    HwpDoc/Paragraph/CtrlPageNumPos.h \
    HwpDoc/Paragraph/CtrlSectionDef.h \
    HwpDoc/Paragraph/CtrlShapeArc.h \
    HwpDoc/Paragraph/CtrlShapeConnectLine.h \
    HwpDoc/Paragraph/CtrlShapeCurve.h \
    HwpDoc/Paragraph/CtrlShapeEllipse.h \
    HwpDoc/Paragraph/CtrlShapeLine.h \
    HwpDoc/Paragraph/CtrlShapeOle.h \
    HwpDoc/Paragraph/CtrlShapePic.h \
    HwpDoc/Paragraph/CtrlShapePolygon.h \
    HwpDoc/Paragraph/CtrlShapeRect.h \
    HwpDoc/Paragraph/CtrlShapeTextArt.h \
    HwpDoc/Paragraph/CtrlShapeVideo.h \
    HwpDoc/Paragraph/CtrlTable.h \
    HwpDoc/Paragraph/HWPPargraph.h \
    HwpDoc/Paragraph/LineSeg.h \
    HwpDoc/Paragraph/ParaText.h \
    HwpDoc/Paragraph/Point.h \
    HwpDoc/Paragraph/RangeTag.h \
    HwpDoc/Paragraph/TblCell.h \
    HwpDoc/Section/NoteShape.h \
    HwpDoc/Section/Page.h \
    HwpDoc/Section/PageBorderFill.h

